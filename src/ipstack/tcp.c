#include "tcp.h"
#include "ipv4.h"
#include "uart.h"

#define TCP_MAX_LISTEN_PORTS 4
#define TCP_MAX_CONNECTIONS 16

typedef void (*TcpConnectedHandler_t) (void* connectionHandle);

typedef struct TcpPacketHeader_s
{
    UI16_t portSource;
    UI16_t portDestination;
    UI32_t sequenceNumber;
    UI32_t acknowledgement;
    UI16_t flags;
    UI16_t length;
    UI16_t crc;
    UI16_t urgPointer;
    
} TcpPacketHeader_t;

typedef struct TcpPacket_s
{
    EthernetIpv4_t ipv4;
    TcpPacketHeader_t tcp;
} TcpPacket_t;

typedef struct TcpListener_s
{
    bool_t InUse;
    UI16_t localPort;
    TcpConnectedHandler_t connectionHandler;
} TcpListener_t;

// http://userpages.umbc.edu/~jeehye/cmsc491b/lectures/tcpstate/sld001.htm
typedef enum TcpState_e
{
            // server: listen
            // client: syn tx
    TcpClosed,

            // wait for new connection
            // goto synRx
    TcpListen,

            // received sync, send sync+ack, wait for it's ack
            // goto established
    TcpSynRx,

            // establishing connection, send syn, wait for syn+ack, send ack
            // goto established
    TcpSynTx,

            // data pumping mode
    TcpEstablished,

            // sent close() , wait for others fin
    TcpFinWait1,

            // received fin, send ack
    TcpFinWait2,

            // Send ack, wait for ack, goto timewait
    TcpClosing,
    TcpTimeWait,

    TcpCloseWait,
    TcpLastAck
} TcpState_t;

typedef struct TcpConnection_s
{
    bool_t InUse;
    TcpState_t state;
    UI16_t remotePort;
    UI08_t remoteIp[4];
    TcpListener_t* listener;

    
} TcpConnection_t;

TcpListener_t tcpListeners[TCP_MAX_LISTEN_PORTS];
TcpConnection_t tcpConnections[TCP_MAX_CONNECTIONS];

void tcpPacketHandler(EthernetIpv4_t* ipv4, bool_t* done);


void tcpInit()
{
    ipv4RegisterHandler(tcpPacketHandler);
}

void tcpListen(UI16_t port, TcpConnectedHandler_t connectHandler)
{
    
}

void tcpPacketHandler(EthernetIpv4_t* ipv4, bool_t* done)
{
    TcpPacket_t* packet = (TcpPacket_t*) ipv4;
    
    if(ipv4->header.protocol == Ipv4TCP)
    {
        packet->tcp.portSource          = htons(packet->tcp.portSource);
        packet->tcp.portDestination     = htons(packet->tcp.portDestination);

#ifdef DEBUG_CONSOLE
        sprintf(debugBuffer, "[tcp] RX packet @ port %d/%d, crc %04X length %02X\r\n", packet->tcp.portSource, packet->tcp.portDestination, packet->tcp.crc, packet->tcp.length);
        uartTxString(debugBuffer);
#endif
    }
}