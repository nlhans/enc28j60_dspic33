/* 
 * File:   arp.h
 * Author: Hans
 *
 * Created on March 10, 2013, 9:06 AM
 */

#ifndef ARP_H
#define	ARP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "types.h"
#include "enc28j60.h"

#pragma pack(1)
typedef struct ArpPacket_s
{
    EthernetFrame_t frame;
    
    UI16_t htype;
    UI16_t ptype;
    UI08_t hlen;
    UI08_t plen;
    UI16_t oper;
    UI08_t sha[6]; // mac
    UI08_t spa[4]; // ip
    UI08_t tha[6]; // mac
    UI08_t tpa[4]; // ip
} ArpPacket_t;

void arpAnnounce(UI08_t* myMac, UI08_t* myIp, UI08_t* gateway);
void arpInit();
UI08_t* arpResolve(UI08_t* ip);

extern  UI08_t thisIp[4];
extern  UI08_t thisMac[6];
extern const UI08_t onesMac[6]; // broadcast mac

#ifdef	__cplusplus
}
#endif

#endif	/* ARP_H */

