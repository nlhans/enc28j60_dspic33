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
        }
        i++;
    }
}

void udpHandlePacket (EthernetIpv4_t* ipv4, bool_t* handled)
{
    if (ipv4->header.protocol == Ipv4UDP)
    {
        *handled = TRUE;

        uartTxString("UDP Packet");
    }
    
}
void udpTxPacket(UDPPacket_t* packet, UI16_t size, UI08_t* ip, UI16_t port)
{
    packet->udp.length = size;
    packet->udp.portDestination = port;
    packet->udp.portSource = port;
    packet->udp.crc = 0;

    ipv4TxPacket(ip, Ipv4UDP, (UI08_t*)packet, size);
}