#include "udp.h"
#include "ipv4.h"
#include "uart.h"

typedef struct UDPSocketHandlerInfo_s
{
    UI16_t port;
    UDPSocketHandler_t handler;
}UDPSocketHandlerInfo_t;

UDPSocketHandlerInfo_t handlers[UDP_MAX_HANDLERS];

void udpHandlePacket (EthernetIpv4_t* ipv4, bool_t* handled);

void udpInit()
{
    UI08_t i = 0;
    ipv4RegisterHandler(udpHandlePacket);

    while (i < UDP_MAX_HANDLERS)
    {
        handlers[i].port = 0xFFFF; // not used
        i++;
    }
}

void udpFirePacket(UDPPacket_t* packet)
{
    UI08_t i =0 ;
    bool_t done= FALSE;
#ifndef DEBUG_CONSOLE
    uartTxByte('.');
#endif
    while (i < UDP_MAX_HANDLERS && !done)
    {
        if(handlers[i].port == packet->udp.portDestination) // it matches our port
        {
            handlers[i].handler(packet, &done);
        }
        i++;
    }
}

void udpRegisterHandler(UDPSocketHandler_t handler, UI16_t port)
{
    UI08_t i =0 ;
    while (i < UDP_MAX_HANDLERS)
    {
        if(handlers[i].port == 0xFFFF) // it matches our port
        {
            handlers[i].port = port;
            handlers[i].handler = handler;
            break;
        }
        i++;
    }
}

void udpHandlePacket (EthernetIpv4_t* ipv4, bool_t* handled)
{
    UDPPacket_t* packet = (UDPPacket_t*)ipv4;

    if (ipv4->header.protocol == Ipv4UDP)
    {
        *handled = TRUE;

        packet->udp.portSource = htons(packet->udp.portSource);
        packet->udp.portDestination = htons(packet->udp.portDestination);
        packet->udp.length = htons(packet->udp.length);

#ifdef DEBUG_CONSOLE
        sprintf(debugBuffer, "[udp] RX packet @ port %d/%d, crc %04X length %02X\r\n", packet->udp.portSource, packet->udp.portDestination, packet->udp.crc, packet->udp.length);
        uartTxString(debugBuffer);
#endif
        udpFirePacket(packet);
    }
    
}
void udpTxPacket(UDPPacket_t* packet, UI16_t size, UI08_t* ip, UI16_t port)
{
    size += sizeof(UDPPacketHeader_t);
    
    packet->udp.length = htons(size);
    packet->udp.portDestination = htons( port );
    packet->udp.portSource =  htons( port );
    packet->udp.crc = 0;

#ifdef DEBUG_CONSOLE
        sprintf(debugBuffer, "[udp] TX packet @ port %d/%d, crc %04X length %02X/%02X\r\n", htons(packet->udp.portSource), htons(packet->udp.portDestination), packet->udp.crc, htons(packet->udp.length), size);
        uartTxString(debugBuffer);
#endif
    ipv4TxPacket(ip, Ipv4UDP, (EthernetIpv4_t*)packet, size);
}