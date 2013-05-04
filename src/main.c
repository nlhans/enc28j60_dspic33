#define MAIN_C

#include "stddefs.h"
#include "spi.h"
#include "uart.h"
#include "enc624j600.h"
//#include "enc28j60.h"
#include "arp.h"
#include "ipv4.h"
#include "icpm.h"
#include "udp.h"
#include "ntp.h"
#include "tcp.h"
#include <stdio.h>
#include "23lc1024.h"
#include "insight.h"

#include "sram_defs.h"
#include "enc624j600.h"

void macRxFrame(UI08_t* packet, UI16_t length)
{ 
    enc624j600RxFrame(packet, length);
}
void macTxFrame(EthernetFrame_t* packet, UI16_t length)
{
    enc624j600TxFrame(packet, length);
}
void macTxReplyFrame(EthernetFrame_t* packet, UI16_t length)
{
    enc624j600TxReplyFrame(packet, length);
}

_FBS(BWRP_WRPROTECT_OFF & BSS_NO_FLASH & RBS_NO_RAM)
_FSS(SWRP_WRPROTECT_OFF & SSS_NO_FLASH & RSS_NO_RAM)
_FGS(GWRP_OFF & GCP_OFF)
_FOSCSEL(FNOSC_FRCPLL & IESO_OFF)
_FOSC(POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSECME)
_FWDT(WDTPOST_PS128 & WDTPRE_PR128 & WINDIS_OFF & FWDTEN_OFF)
_FPOR(FPWRT_PWR1 & ALTI2C_OFF)
_FICD(ICS_PGD2 & JTAGEN_OFF)

#define LED_High  PORTB |= 1<<8
#define LED_Low   PORTB &= ~(1<<8)

#define RST_High  PORTB |= 1<<9
#define RST_Low   PORTB &= ~(1<<9)

//UI08_t mac[6]           = {0x00, 0x04, 0xA3, 0x12, 0x34, 0x56};
UI08_t mac[6] = {0,4,0xA3, 0x50, 0x20, 0xF6}; // fixed by enc624j600
UI08_t ip[4]            = {192, 168, 1, 123};
UI08_t gateway[4]       = {192, 168, 1, 1};
UI08_t pc[4]       = {192, 168, 1, 147};
UI08_t ntpServer[4] = {194, 171, 167, 130};

UI08_t frameBf[0x5EE];

const char* response = "I SHIT YOU NOT";

void handleData(void* con, bool_t push, UI08_t* d, UI16_t s)
{
    //
    INSIGHT(HTTP_RX_HEADERS, d);

    UI16_t getAddrOffset = strchr(d, "GET /");
    if(getAddrOffset == NULL)
    {
        //
    }
    else
    {
        //TcpTx(con, response, strlen(response));
        //TcpClose(con);
    }
}

bool_t handleConnection(void* con)
{
    //
    TcpConnection_t* connection = (TcpConnection_t*) con;

    //
    connection->rxData = handleData;

    return TRUE;

}

#ifdef ENC624J600_H
int main()
{
    AD1PCFGL = 0xFFFF;
    TRISB &= ~(1<<8);

    SPI_Init();
    uartInit();
    insight_init();

    enc624j600Initialize(mac);
    //sram_23lc1024_init();
    arpInit();
    arpAnnounce(mac, ip, gateway);
    ipv4Init();
    tcpInit();
    tcpListen(1234, 32, handleConnection);
    udpInit();
    icmpInit();
    ntpInit();
    ntpRequest(ntpServer);

    while(1)
    {
        if(enc624j600PacketPending())
        {
            INSIGHT(ENC624J600_PACKETS, enc624j600GetPacketCount(), enc624j600GetLinkStatus());
            enc624j600RxFrame(frameBf, sizeof(frameBf));
        }

    }
    while(1);
    return 0;
}
#endif

#ifdef ENC28J60_H
int main()
{
    AD1PCFGL = 0xFFFF;
    TRISB &= ~(1<<8);
    
    SPI_Init();
    uartInit();
    insight_init();

    enc28j60Initialize(mac);
    sram_23lc1024_init();
    arpInit();
    arpAnnounce(mac, ip, gateway);
    ipv4Init();
    udpInit();
    tcpInit();
    icmpInit();
    ntpInit();
    ntpRequest(ntpServer);

    while(1)
    {
        while (!enc28j60PacketPending());
        enc28j60_reset_stat();
        enc28j60RxFrame(frameBf, sizeof(frameBf));
    }
    while(1);
    return 0;
}
#endif
