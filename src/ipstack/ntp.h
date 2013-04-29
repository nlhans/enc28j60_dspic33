/* 
 * File:   ntp.h
 * Author: Hans
 *
 * Created on March 11, 2013, 9:30 PM
 */

#ifndef NTP_H
#define	NTP_H

#include "stddefs.h"
#include "udp.h"

typedef struct NTPPacketContent_s
{
    UI08_t flags;
    UI08_t stratum;
    UI08_t interval;
    UI08_t precision;
    UI32_t rootdelay;
    UI32_t rootdispersion;
    UI32_t id;

    UI32_t stampRef[2];
    UI32_t stampOrigin[2];
    UI32_t stampReceive[2];
    UI32_t stampTransmit[2];

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

#endif	/* NTP_H */

