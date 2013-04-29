#ifndef ETHDEFS_H
#define ETHDEFS_H

#include "stddefs.h"

// http://en.wikipedia.org/wiki/Ethernet_frame
typedef struct EthernetFrame_s
{
    UI08_t dstMac[6];
    UI08_t srcMac[6];
    UI16_t type;
} EthernetFrame_t;

typedef enum EthernetProtocol_e
{
    ProtocolIPv4 = 0x0800,
    ProtocolARP  = 0x0806,

} EthernetProtocol_t;

typedef void (*EthernetPacketHandler_t) (EthernetFrame_t* frame, bool_t* handled);

#define htonl(x) ( ((x & 0xFF000000) >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | ((x & 0x000000FF) << 24) )
#define htons(x) ( (( x & 0xFF00) >> 8) | ((x & 0xFF) << 8))

#endif