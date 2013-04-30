#include "icpm.h"
#include "ipv4.h"
#include "spi.h"
#include "uart.h"
#include "ethdefs.h"

// ICPM is for relaying and ping commands
void icmpHandlePacket (EthernetIpv4_t* header, bool_t* handled);

void icmpInit()
{
    ipv4RegisterHandler(icmpHandlePacket);
}
void icmpPing(UI08_t* target)
{
}

void icmpTxPacket(ICMPPacket_t* packet, UI08_t* ip)
{
    // create new packet, with ip
}

void icmpHandlePacket (EthernetIpv4_t* header, bool_t* handled)
{
    ICMPPacket_t* packet;
    if (header->header.protocol == 1)
    {
        *handled = TRUE;

        packet = (ICMPPacket_t*) header;

        if (packet->icmp.type == 8 && packet->icmp.code == 0) // echo request
        {
#ifdef DEBUG_CONSOLE
            sprintf(debugBuffer, "[icmp] Ping pong, sizeof: %02X\r\n", sizeof(ICMPPacket_t));
            uartTxString(debugBuffer);
#endif
            // it requests us a echo reply.
            packet->icmp.type = 0;

            SPI_SetDebug(1);

            // recalculate CRC
            packet->icmp.crc = 0;
            packet->icmp.crc = htons( ipv4Crc( (UI08_t*) (&packet->icmp), header->header.length - sizeof(EthernetIpv4Header_t)) );

            // reply
            ipv4TxReplyPacket((EthernetIpv4_t*) packet, header->header.length - sizeof(EthernetIpv4Header_t) );

            SPI_SetDebug(0);
        }
        
    }


}