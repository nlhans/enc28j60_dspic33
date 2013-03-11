/* 
 * File:   ntp.h
 * Author: Hans
 *
 * Created on March 11, 2013, 9:30 PM
 */

#ifndef NTP_H
#define	NTP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "udp.h"

typedef struct NTPPacketContent_s
{
    UI08_t flags;
    UI08_t stratum;
    UI08_t interval;
    UI08_t precision;
    UI32_t rootdelay;
    UI32_t rootdispersion;
    UI08_t id;

    UI32_t stampRef;
    UI32_t stampOrigin;
    UI32_t stampReceive;
    UI32_t stampTransmit;

} NTPPacketContent_t;

typedef struct NTPPacket_s
{
    UDPPacket_t udp;
    NTPPacketContent_t ntp;
} NTPPacket_t;

extern UI32_t ntpTimestamp;

void ntpInit();
void ntpRequest(UI08_t* ip);
void ntpHandle (UDPPacket_t* packet);

#ifdef	__cplusplus
}
#endif

#endif	/* NTP_H */

