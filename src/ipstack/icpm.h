/* 
 * File:   icpm.h
 * Author: Hans
 *
 * Created on March 10, 2013, 6:14 PM
 */

#ifndef ICPM_H
#define	ICPM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "types.h"
#include "ipv4.h"

typedef struct ICMPacketHeader_s
{
    UI08_t type;
    UI08_t code;
    volatile UI16_t crc;
    UI16_t rest;
} ICMPacketHeader_t;

typedef struct ICMPPacket_s
{
    EthernetIpv4_t ipv4;
    ICMPacketHeader_t icmp;
} ICMPPacket_t;

void icmpInit();
void icmpPing(UI08_t* target);




#ifdef	__cplusplus
}
#endif

#endif	/* ICPM_H */

