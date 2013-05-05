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

bool_t tcpListenMore(TcpListener_t* listener)
{
    UI08_t i = 0, c = 0;
    TcpConnection_t* connection;
    
    for(; i < TCP_MAX_LISTEN_PORTS; i++)
    {
        if(tcpConnections[i].listener == listener)
        {
            c++;
        }
    }

    if (c != listener->maxConnections)
    {
        connection = tcpPickFreeConnection();
        if(connection == NULL)
        {
            return FALSE;
        }
        connection->listener = listener;
        connection->state = TcpListen; // this is a server
        INSIGHT(TCP_LISTEN, listener->localPort);

        return TRUE;
    }

    return FALSE;
}

bool_t tcpListen(UI16_t port, UI08_t maxConnections, TcpConnectedHandler_t connectHandler)
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
            tcpListeners[i].maxConnections = maxConnections;
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

    UI32_t sequenceNumber, acknowledgeNumber;

    if(ipv4->header.protocol == Ipv4TCP)
    {
        packet->tcp.portSource          = htons(packet->tcp.portSource);
        packet->tcp.portDestination     = htons(packet->tcp.portDestination);
        packet->tcp.flags.data          = htons(packet->tcp.flags.data);
        packet->tcp.length              = htons(packet->tcp.length);
        sequenceNumber                  = htonl(packet->tcp.sequenceNumber);
        acknowledgeNumber               = htonl(packet->tcp.acknowledgement);
        
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

        flags.bits.dataOffset = 5;
        flags.bits.syn = 0;
        flags.bits.ack = 0;
        flags.bits.rst = 0;

        packet->tcp.length = 1400;
        packet->tcp.acknowledgement = connection->lastAcknowledgeNumber;
        packet->tcp.sequenceNumber = connection->lastSequenceNumber;
        
        switch(connection->state)
        {
            case TcpClosed:
                if (packet->tcp.flags.bits.fin == 1)
                {
                    flags.bits.ack = 1;
                    tcpTxPacket(0, flags, packet, connection);
                        
                    connection->state = TcpCloseWait;
                }
                break;

            case TcpListen:
                // Opens connection on this slot.
                // Conditions to be met for next state:
                if (packet->tcp.flags.bits.syn == 1)
                {
                    // Reply 
                    flags.bits.dataOffset = 6; // 4x7=28
                    //packet->tcp.sequenceNumber++;
                    packet->tcp.acknowledgement = sequenceNumber+1;
                    packet->tcp.sequenceNumber = 0xAA55AA55;

                    // Execute handler; if TRUE send ACK
                    // Otherwise send RST
                    if (connection->listener->connectionHandler((void*)connection))
                    {
                        flags.bits.syn = 1;
                        flags.bits.ack = 1;

                        // Switch state to TcpSynRcvd
                        connection->state = TcpSynRx;

                        tcpListenMore(connection->listener);
                    }
                    else
                    {
                        flags.bits.rst = 1;

                        // Switch state to TcpListen
                        // TODO: reset connection state
                        connection->state = TcpListen;
                        connection->listener = NULL;
                    }

                    tcpTxPacket(0, flags, packet, connection);
                        
                }
                break;

            case TcpSynRx:
                // We received syn, and are awaiting for ACK
                if (packet->tcp.flags.bits.ack == 1 &&
                    packet->tcp.flags.bits.syn == 0)
                {
                    // Huuray!
                    connection->state = TcpEstablished;
                }
                else if (packet->tcp.flags.bits.rst == 1)
                {
                    connection->state = TcpListen;
                }
                break;

            case TcpEstablished:
                if (packet->tcp.flags.bits.fin == 1)
                {
                    flags.bits.ack = 1;
                    tcpTxPacket(0, flags, packet, connection);
                    
                    connection->state = TcpCloseWait;
                }
                else
                {
                    // Record data.
                    UI08_t headerOffset = 4*packet->tcp.flags.bits.dataOffset;
                    INSIGHT(DATASIZE, headerOffset);
                    UI16_t payloadSize = ipv4->header.length - sizeof(EthernetIpv4Header_t) - headerOffset;
                    INSIGHT(DATASIZE, payloadSize);
                    UI32_t ackNumber = payloadSize + sequenceNumber;
                    INSIGHT(TCP_RX_DATA, 0);
                    connection->rxData(connection, ((UI08_t*)(&packet->tcp)) + headerOffset, payloadSize);

                    flags.bits.ack = 1;

                    packet->tcp.sequenceNumber = acknowledgeNumber;
                    packet->tcp.acknowledgement = ackNumber;

                    tcpTxPacket(0, flags, packet, connection);
                }
                break;

            case TcpFinWait1:
                if (packet->tcp.flags.bits.ack == 1)
                {
                    if ( packet->tcp.flags.bits.fin == 0)
                    {
                        connection->state = TcpFinWait2;
                    }
                    else
                    {
                        flags.bits.ack = 1;
                        tcpTxPacket(0, flags, packet, connection);
                        
                        connection->state = TcpTimeWait;
                    }
                }
                else if ( packet->tcp.flags.bits.fin == 1)
                {
                    flags.bits.ack = 1;
                    packet->tcp.acknowledgement = packet->tcp.sequenceNumber+1;
                    packet->tcp.sequenceNumber = 0xAA55AA55;
                    tcpTxPacket(0, flags, packet, connection);

                    connection->state = TcpTimeWait;
                }
                break;

            case TcpFinWait2:
                if ( packet->tcp.flags.bits.fin == 1)
                {
                    flags.bits.ack = 1;
                    tcpTxPacket(0, flags, packet, connection);

                    connection->state = TcpTimeWait;
                }
                break;

            case TcpTimeWait:
                connection->state = TcpClosed;
                break;

            case TcpCloseWait:
                flags.bits.rst = 1;
                packet->tcp.acknowledgement = 0;
                tcpTxPacket(0, flags, packet, connection);

                connection->state = TcpClosed;
                break;

            case TcpLastAck:
                if ( packet->tcp.flags.bits.ack == 1 || 1)
                {
                    flags.bits.ack = 1;
                    tcpTxPacket(0, flags, packet, connection);
                    connection->state = TcpClosed;
                }
                break;

            default:
                connection->state = TcpListen;//TcpClosed;
                break;
        }
        INSIGHT(TCP_RX_CONNECTION, (UI08_t)(connection - tcpConnections), connection->state);
    }
}

UI16_t tcpCrc(TcpPacket_t* packet, UI08_t* data, UI16_t size)
{
    UI08_t counts = 0;
    volatile UI16_t b = 0;
    volatile UI16_t* dataUI16 = (UI16_t*) (data);
    volatile UI32_t crc = 0;
    volatile UI32_t sum = 0;

    while (b < size/2)
    {
        sum += htons(dataUI16[b]);
        b += 1;
        counts++;

    }

    b = sum >> 16;
    sum = sum & 0xFFFF;
    sum += b;
    crc = ~sum;

    return (UI16_t) crc;
}

void tcpTxPacket(UI16_t dataSize, TcpFlags_t flags, TcpPacket_t* packet, TcpConnection_t* connection)
{
    dataSize += 4 * flags.bits.dataOffset;

    connection->lastAcknowledgeNumber = packet->tcp.acknowledgement;
    connection->lastSequenceNumber = packet->tcp.sequenceNumber;
    
    packet->tcp.portDestination = htons(connection->remotePort);
    packet->tcp.portSource = htons(connection->listener->localPort);
    packet->tcp.sequenceNumber = htonl(packet->tcp.sequenceNumber);
    packet->tcp.acknowledgement = htonl(packet->tcp.acknowledgement);
    packet->tcp.length = htons(packet->tcp.length);
    packet->tcp.flags.data = htons(flags.data);
    packet->tcp.crc = 0;
    packet->tcp.crc = ipv4Crc((UI08_t*)&(packet->ipv4.header.sourceIp), dataSize + 8) - dataSize - 6;
    packet->tcp.crc = htons(packet->tcp.crc);

    // hack ipv4 id
    packet->ipv4.header.ID = 0;

    ipv4TxReplyPacket((EthernetIpv4_t*)packet, dataSize);
}