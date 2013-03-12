/* 
 * File:   ipv4.h
 * Author: Hans
 *
 * Created on March 10, 2013, 5:29 PM
 */

#ifndef IPV4_H
#define	IPV4_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "types.h"
#include "enc28j60.h"

#define IPV4_MAXIMUM_PROTOCOL_HANDLERS 3 // UDP, TCP, ICMP

// http://en.wikipedia.org/wiki/List_of_IP_protocol_numbers
typedef enum EthernetIpv4Protocol_e
{
    Ipv4ICMP = 0x01,
    Ipv4TCP = 0x06,
    Ipv4UDP = 0x11
} EthernetIpv4Protocol_t;

typedef struct EthernetIpv4Header_s
{
    UI08_t ihl:4;
    UI08_t version:4;
    UI08_t DSCP;
    UI16_t length;

    UI16_t ID;
    UI16_t flags;
    UI08_t timeToLive;
    UI08_t protocol;
    volatile UI16_t crc;

    UI08_t sourceIp[4];
    UI08_t destinationIp[4];

} EthernetIpv4Header_t;

typedef struct EthernetIpv4_s
{
    EthernetFrame_t frame;
    EthernetIpv4Header_t header;
} EthernetIpv4_t;

typedef void (*Ipv4PacketHandler_t) (EthernetIpv4_t* header, bool_t* handled);


void ipv4Init();
void ipv4RegisterHandler(Ipv4PacketHandler_t myHandler);
void ipv4UnregisterHandler(Ipv4PacketHandler_t myHandler);
void ipvFireHandlers(EthernetIpv4_t* frame, UI08_t* data, UI16_t length);
void ipv4TxPacket(UI08_t* dstIp, UI08_t protocol, EthernetIpv4_t *data, UI16_t size);
void ipv4TxReplyPacket(EthernetIpv4_t* ipv4Packet, UI08_t totalSize);
UI16_t ipv4Crc(UI08_t* data, UI16_t size);

#ifdef	__cplusplus
}
#endif

#endif	/* IPV4_H */

