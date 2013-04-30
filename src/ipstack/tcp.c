#include "tcp.h"
#include "ipv4.h"
#include "uart.h"
#include "insight.h"

TcpListener_t tcpListeners[TCP_MAX_LISTEN_PORTS];
TcpConnection_t tcpConnections[TCP_MAX_CONNECTIONS];

TcpConnection_t* tcpPickFreeConnection();
TcpConnection_t* tcpMatchConnection(UI08_t* ip, UI16_t remotePort);

void tcpPacketHandler(EthernetIpv4_t* ipv4, bool_t* done);

void tcpInit()
{
    UI08_t i = 0;

    ipv4RegisterHandler(tcpPacketHandler);
    for(i = 0; i < TCP_MAX_CONNECTIONS; i++)
    {
        tcpConnections[i].state = TcpClosed;
    }
    for(i = 0; i < TCP_MAX_LISTEN_PORTS; i++)
    {
        tcpListeners[i].InUse = FALSE;
    }
}

TcpConnection_t* tcpPickFreeConnection()
{
    UI08_t i;
    for(i = 0; i < TCP_MAX_CONNECTIONS; i++)
    {
        if(tcpConnections[i].state == TcpClosed)
        {
            INSIGHT(TCP_PICKED, i);
            return &(tcpConnections[i]);
        }
    }
    return NULL;
}
TcpConnection_t* tcpMatchConnection(UI08_t* ip, UI16_t remotePort)
{
    UI08_t i;
    for(i = 0; i < TCP_MAX_CONNECTIONS; i++)
    {
        if(tcpConnections[i].state != TcpClosed
        && tcpConnections[i].remotePort == remotePort
        && memcmp(ip, tcpConnections[i].remoteIp, 4) == 0)
        {
            INSIGHT(TCP_MATCHED, "connection", i);
            return &(tcpConnections[i]);
        }
    }
    return NULL;
}
TcpConnection_t* tcpMatchPort(UI16_t localPort)
{
    UI08_t i;
    for(i = 0; i < TCP_MAX_CONNECTIONS; i++)
    {
        if(tcpConnections[i].state == TcpListen
        && tcpConnections[i].listener->localPort == localPort)
        {
            INSIGHT(TCP_MATCHED, "port", i);
            return &(tcpConnections[i]);
        }
    }
    return NULL;
}

bool_t tcpListen(UI16_t port, TcpConnectedHandler_t connectHandler)
{
    UI08_t i;
    TcpConnection_t* connection;

    for(i = 0; i < TCP_MAX_LISTEN_PORTS; i++)
    {
        if(tcpListeners[i].localPort == port) 
        {
            return FALSE; // error!
        }
    }
    for(i = 0; i < TCP_MAX_LISTEN_PORTS; i++)
    {
        if(tcpListeners[i].InUse == FALSE)
        {
            // Pick up 1 connection that listens at this port
            connection = tcpPickFreeConnection();
            if(connection == NULL)
            {
                return FALSE;
            }
            tcpListeners[i].InUse = TRUE;
            tcpListeners[i].localPort = port;
            tcpListeners[i].connectionHandler = connectHandler;

            connection->listener = &(tcpListeners[i]);
            connection->state = TcpListen; // this is a server
            INSIGHT(TCP_LISTEN, port);
            return TRUE;
        }
    }
    return FALSE;
}

void tcpPacketHandler(EthernetIpv4_t* ipv4, bool_t* done)
{
    TcpPacket_t* packet = (TcpPacket_t*) ipv4;
    TcpConnection_t* connection;
    TcpFlags_t flags;

    if(ipv4->header.protocol == Ipv4TCP)
    {
        packet->tcp.portSource          = htons(packet->tcp.portSource);
        packet->tcp.portDestination     = htons(packet->tcp.portDestination);
        packet->tcp.flags.data          = htons(packet->tcp.flags.data);
        packet->tcp.length              = htons(packet->tcp.length);
        packet->tcp.sequenceNumber      = htonl(packet->tcp.sequenceNumber);
        packet->tcp.acknowledgement     = htonl(packet->tcp.acknowledgement);
        
        INSIGHT(TCP_RX, packet->tcp.portSource, packet->tcp.portDestination, packet->tcp.flags.data, packet->tcp.length, packet->tcp.acknowledgement);
        INSIGHT(TCP_RX_FLAGS, packet->tcp.flags.bits.syn, packet->tcp.flags.bits.ack, packet->tcp.flags.bits.rst, packet->tcp.flags.bits.fin);

        // Determine corresponding connection slot, otherwise listener.
        connection = tcpMatchConnection(packet->ipv4.header.sourceIp, packet->tcp.portSource);

        if (connection != NULL)
        {
            INSIGHT(TCP_RX_CONNECTION, (UI08_t)(connection - tcpConnections), connection->state);
        }
        else
        {
            connection = tcpMatchPort(packet->tcp.portDestination);

            if (connection != NULL)
            {
                memcpy(connection->remoteIp, packet->ipv4.header.sourceIp, 4);
                memcpy(connection->remoteMac, packet->ipv4.frame.srcMac, 6);
                connection->remotePort = packet->tcp.portSource;
                INSIGHT(TCP_RX_RESERVING);
                INSIGHT(TCP_RX_CONNECTION, (UI08_t)(connection - tcpConnections), connection->state);
            }
            else
            {
                INSIGHT(TCP_NO_CONNECTION);
                return;
            }
        }

        switch(connection->state)
        {
            case TcpClosed:
                //WTF?
                break;

            case TcpListen:
                // Opens connection on this slot.
                // Conditions to be met for next state:
                if (packet->tcp.flags.bits.syn == 1)
                {
                    // Reply with SYN-ACK
                    flags.bits.syn = 1;
                    flags.bits.ack = 1;
                    flags.bits.dataOffset = 5; // 4x7=28
                    //packet->tcp.sequenceNumber++;
                    packet->tcp.acknowledgement = packet->tcp.sequenceNumber+1;
                    packet->tcp.sequenceNumber = 0xAA55AA55;
                    
                    tcpTxPacket(0, flags, packet, connection);

                    // Switch state to TcpSynRcvd
                    connection->state = TcpSynRx;
                }
                break;

            default:
                connection->state = TcpListen;//TcpClosed;
                break;
        }
    }
}

void tcpTxPacket(UI16_t dataSize, TcpFlags_t flags, TcpPacket_t* packet, TcpConnection_t* connection)
{
    dataSize += packet->tcp.flags.bits.dataOffset*4;
    
    packet->tcp.portDestination = htons(connection->remotePort);
    packet->tcp.portSource = htons(connection->listener->localPort);
    packet->tcp.sequenceNumber = htonl(packet->tcp.sequenceNumber);
    packet->tcp.acknowledgement = htonl(packet->tcp.acknowledgement);
    packet->tcp.flags.data = htons(flags.data);
    //packet->tcp.length = htons(128);
    packet->tcp.length = htons(packet->tcp.length);

    packet->tcp.crc = 0;
    packet->tcp.crc = htons(ipv4Crc((UI08_t*)packet, dataSize));
    
    ipv4TxReplyPacket((EthernetIpv4_t*)packet, dataSize);
}