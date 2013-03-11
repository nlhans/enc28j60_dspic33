#include "ntp.h"
void ntpHandlePacket (UDPPacket_t* udp, bool_t* handled);

UI32_t ntpTimestamp = 0;

NTPPacket_t ntpRequestPacket;

void ntpInit()
{
    udpRegisterHandler(ntpHandlePacket, 123); // port 123
}
void ntpRequest(UI08_t* ip)
{
    ntpRequestPacket.ntp.flags = 0b11100011; // LI 3 unsynchronised; version 4; node
    ntpRequestPacket.ntp.interval = 10;
    ntpRequestPacket.ntp.precision = 0xEC;
    //
    udpTxPacket((UDPPacket_t*) &ntpRequestPacket, sizeof(NTPPacketContent_t), ip, 123);
}



void ntpHandlePacket(UDPPacket_t* udp, bool_t* handled)
{
    *handled = TRUE;

    ntpTimestamp = ((NTPPacket_t*)udp)->ntp.stampTransmit - 2208988800UL; // time diff from 1970-1900
    
    
}