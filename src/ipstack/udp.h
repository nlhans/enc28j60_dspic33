/* 
 * File:   udp.h
 * Author: Hans
 *
 * Created on March 11, 2013, 9:10 PM
 */

#ifndef UDP_H
#define	UDP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "ipv4.h"

#define UDP_MAX_HANDLERS 8

typedef struct UDPPacketHeader_s
{
    UI16_t portSource;
    UI16_t portDestination;

    UI16_t length;
    UI16_t crc;
} UDPPacketHeader_t;

typedef struct UDPPacket_s
{
    EthernetIpv4_t ipv4;
    UDPPacketHeader_t udp;
} UDPPacket_t;

typedef void (*UDPSocketHandler_t) (UDPPacket_t* udp, bool_t* handled);

void udpInit();
void udpRegisterHandler(UDPSocketHandler_t handler, UI16_t port);
void udpTxPacket(UDPPacket_t* packet, UI16_t size, UI08_t* ip, UI16_t port);

#ifdef	__cplusplus
}
#endif

#endif	/* UDP_H */

