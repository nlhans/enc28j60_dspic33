#include "ntp.h"
#include "uart.h"
#include "insight.h"
void ntpHandlePacket (UDPPacket_t* udp, bool_t* handled);

UI32_t ntpTimestamp = 0;

NTPPacket_t ntpRequestPacket;

void ntpInit()
{
    udpRegisterHandler(ntpHandlePacket, 123);
}
void ntpRequest(UI08_t* ip)
{
    memset(&ntpRequestPacket, 0, sizeof(NTPPacket_t));
    
    ntpRequestPacket.ntp.flags = 0b11100011; // LI 3 unsynchronised; version 4; node
    ntpRequestPacket.ntp.interval = 10;
    ntpRequestPacket.ntp.precision = 0xEC;
    
    INSIGHT(NTP_REQUEST, ip[0], ip[1], ip[2], ip[3]);
    udpTxPacket((UDPPacket_t*) &ntpRequestPacket, sizeof(NTPPacketContent_t), ip, 123);

}

void ntpHandlePacket(UDPPacket_t* udp, bool_t* handled)
{
    *handled = TRUE;

    if (udp->udp.portDestination == 123)
    {
        ntpTimestamp = htonl(((NTPPacket_t*)udp)->ntp.stampTransmit[0]) - 2208988800; // time diff from 1970-1900;
        INSIGHT(NTP_TIME, ntpTimestamp);
    }
}