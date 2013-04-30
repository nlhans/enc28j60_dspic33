#include "ethdefs.h"
#include "ipv4.h"
#include "uart.h"
#include "arp.h"
#include "uart.h"
#include "insight.h"

typedef struct Ipv4PacketHandlerInfo_s
{
    bool_t used;
    Ipv4PacketHandler_t handler;
} Ipv4PacketHandlerInfo_t;

Ipv4PacketHandlerInfo_t Ipv4Handlers[IPV4_MAXIMUM_PROTOCOL_HANDLERS];

void ipv4Init()
{
    UI08_t i = 0 ;

    for (i = 0; i < IPV4_MAXIMUM_PROTOCOL_HANDLERS; i++)
    {
        Ipv4Handlers[i].used = FALSE;
    }
}

void ipv4RegisterHandler(Ipv4PacketHandler_t myHandler)
{
    UI08_t i = 0;

    while (i < IPV4_MAXIMUM_PROTOCOL_HANDLERS)
    {
        if (Ipv4Handlers[i].used == FALSE)
        {
            Ipv4Handlers[i].used = TRUE;
            Ipv4Handlers[i].handler = myHandler;
            break;
        }
        i++;
    }
}

void ipv4UnregisterHandler(Ipv4PacketHandler_t myHandler)
{
    UI08_t i = 0;

    while (i < IPV4_MAXIMUM_PROTOCOL_HANDLERS)
    {
        if (Ipv4Handlers[i].handler == myHandler)
        {
            Ipv4Handlers[i].used = FALSE;
        }
        i++;
    }
}


void ipv4FireHandlers(EthernetIpv4_t* frame)
{
    UI08_t i = 0;
    bool_t done = FALSE;

    while (i < IPV4_MAXIMUM_PROTOCOL_HANDLERS && done == FALSE)
    {
        if (Ipv4Handlers[i].used == TRUE)
        {
            Ipv4Handlers[i].handler(frame, &done);
        }
        i++;
    }
}

void ipv4HandlePacket(EthernetFrame_t* frame, bool_t* handled)
{
    EthernetIpv4_t*         ipv4Header;
    UI08_t                  headerSize;
    
    if (frame->type == 0x0800)
    {
        *handled = TRUE;
        
        ipv4Header = (EthernetIpv4_t*) frame;
        ipv4Header->header.length = htons(ipv4Header->header.length);

        headerSize = 4 * ipv4Header->header.ihl;

        INSIGHT(IPV4_RX, ipv4Header->header.length, ipv4Header->header.protocol, htons(ipv4Header->header.crc),
        ipv4Header->header.sourceIp[0],ipv4Header->header.sourceIp[1],ipv4Header->header.sourceIp[2],ipv4Header->header.sourceIp[3]);

        
        ipv4FireHandlers(ipv4Header);
        
    }
    
}

UI16_t ipv4Crc(UI08_t* data, UI16_t size)
{
    volatile UI16_t b = 0;
    volatile UI16_t* dataUI16 = (UI16_t*) (data);
    volatile UI32_t crc = 0;
    volatile UI32_t sum = 0;
    UI08_t counts = 0;

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

void ipv4TxReplyPacket(EthernetIpv4_t* ipv4Packet, UI08_t totalSize)
{
    UI08_t ipTmp[4];
    // Swap source/destination
    memcpy(ipTmp,                               ipv4Packet->header.destinationIp, 4);
    memcpy(ipv4Packet->header.destinationIp,    ipv4Packet->header.sourceIp     , 4);
    memcpy(ipv4Packet->header.sourceIp,         ipTmp                           , 4);

    // Recalculate crc
    ipv4Packet->header.crc          = 0;
    ipv4Packet->frame.type          = htons(ipv4Packet->frame.type);
    ipv4Packet->header.length       = htons(ipv4Packet->header.length);
    ipv4Packet->header.crc          = htons(ipv4Crc((UI08_t*)(&ipv4Packet->header), 4*ipv4Packet->header.ihl ) );

    INSIGHT(IPV4_TX_REPLY, totalSize, ipv4Packet->header.protocol, htons(ipv4Packet->header.crc),
    ipv4Packet->header.destinationIp[0],ipv4Packet->header.destinationIp[1],ipv4Packet->header.destinationIp[2],ipv4Packet->header.destinationIp[3]);
    
    // Push 1 layer further down
    macTxReplyFrame((EthernetFrame_t*)ipv4Packet, sizeof(EthernetIpv4Header_t) + totalSize);
}

UI08_t gw[6] = {0xB0, 0x48, 0x7A, 0xDB, 0x5B, 0xEA };
void ipv4TxPacket(UI08_t* dstIp, UI08_t protocol, EthernetIpv4_t *ipv4Packet, UI16_t size)
{
    size += sizeof(EthernetIpv4Header_t);

    //memcpy(ipv4Packet.frame.dstMac, arpResolve(dstIp), 6);
    memcpy(ipv4Packet->frame.dstMac, gw, 6);
    memcpy(ipv4Packet->frame.srcMac, thisMac, 6);
    ipv4Packet->frame.type = htons(ProtocolIPv4);

    memcpy(ipv4Packet->header.destinationIp,    dstIp,  4);
    memcpy(ipv4Packet->header.sourceIp,         thisIp, 4);

    ipv4Packet->header.version      = 4;
    ipv4Packet->header.ihl          = 5;
    ipv4Packet->header.DSCP         = 0;
    ipv4Packet->header.length       = htons(size);
    ipv4Packet->header.ID           = htons(0x1234);
    ipv4Packet->header.flags        = 0;
    ipv4Packet->header.timeToLive   = 0x80;
    ipv4Packet->header.protocol     = protocol;
    ipv4Packet->header.crc          = 0;
    ipv4Packet->header.crc          = htons(ipv4Crc((UI08_t*)(&ipv4Packet->header), 4*ipv4Packet->header.ihl ) );

    INSIGHT(IPV4_TX, size, ipv4Packet->header.protocol, htons(ipv4Packet->header.crc),
    ipv4Packet->header.destinationIp[0],ipv4Packet->header.destinationIp[1],ipv4Packet->header.destinationIp[2],ipv4Packet->header.destinationIp[3]);

    macTxFrame((EthernetFrame_t*) ipv4Packet, sizeof(EthernetFrame_t) + size);
}