/* 
 * File:   tcp.h
 * Author: Hans
 *
 * Created on March 12, 2013, 7:35 PM
 */

#ifndef TCP_H
#define	TCP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "stddefs.h"
#include "ipv4.h"

#define TCP_MAX_LISTEN_PORTS 4
#define TCP_MAX_CONNECTIONS 32

typedef union TcpFlags_u
{
    struct
    {
        UI08_t fin:1;
        UI08_t syn:1;
        UI08_t rst:1;
        UI08_t psh:1;
        UI08_t ack:1;
        UI08_t urg:1;
        UI08_t ece:1;
        UI08_t cwr:1;
        UI08_t ns:1;
        UI08_t resvr:3;
        UI08_t dataOffset:4;
    }bits;
    UI16_t data;
} TcpFlags_t;
typedef struct TcpPacketHeader_s
{
    UI16_t portSource;
    UI16_t portDestination;
    UI32_t sequenceNumber;
    UI32_t acknowledgement;
    TcpFlags_t flags;
    UI16_t length;
    UI16_t crc;
    UI16_t urgPointer;

} TcpPacketHeader_t;

typedef struct TcpPacket_s
{
    EthernetIpv4_t ipv4;
    TcpPacketHeader_t tcp;
} TcpPacket_t;

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

typedef bool_t (*TcpConnectedHandler_t) (void* connectionHandle);
typedef void (*TcpRxDataHandler_t) (void* connectionHandle, bool_t push, UI08_t* data, UI16_t length);

typedef struct TcpListener_s
{
    bool_t InUse;
    UI08_t maxConnections;
    UI16_t localPort;
    TcpConnectedHandler_t connectionHandler;
} TcpListener_t;

typedef struct TcpConnection_s
{
    UI32_t lastSequenceNumber;
    UI32_t lastAcknowledgeNumber;
    TcpListener_t* listener;
    TcpState_t state;
    UI08_t remoteIp[4];
    UI08_t remoteMac[6];
    UI16_t remotePort;
    UI16_t timer;
    TcpRxDataHandler_t rxData;
} TcpConnection_t;


void tcpInit();
bool_t tcpListen(UI16_t port, UI08_t maxConnections, TcpConnectedHandler_t connectHandler);
void tcpTxReplyPacket(UI16_t dataSize, TcpFlags_t flags, TcpPacket_t* packet, TcpConnection_t* connection);
void tcpTxPacket(UI08_t* data, UI16_t dataSize, TcpFlags_t flags, TcpConnection_t* connection);


#ifdef	__cplusplus
}
#endif

#endif	/* TCP_H */

