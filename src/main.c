#include "types.h"
#include "spi.h"
#include "uart.h"
#include "enc28j60.h"
#include "arp.h"
#include "ipv4.h"
#include "icpm.h"
#include <stdio.h>

_FBS(BWRP_WRPROTECT_OFF & BSS_NO_FLASH & RBS_NO_RAM)
_FSS(SWRP_WRPROTECT_OFF & SSS_NO_FLASH & RSS_NO_RAM)
_FGS(GWRP_OFF & GCP_OFF)
_FOSCSEL(FNOSC_FRCPLL & IESO_OFF)
_FOSC(POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSECME)
_FWDT(WDTPOST_PS128 & WDTPRE_PR128 & WINDIS_OFF & FWDTEN_OFF)
_FPOR(FPWRT_PWR1 & ALTI2C_OFF)
_FICD(ICS_PGD2 & JTAGEN_OFF)
        

/*
 *  ENC SPI commands
 */
#define RCRCMD  0b00000000      // read control register
#define WCRCMD  0b01000000      // write control register
#define BFCCMD  0b10100000      // bit field clear
#define BFSCMD  0b10000000      // bit field set
#define BFCCMD  0b10100000      // bit field clear

/*
 * ENC SFR
 */
#define MIREGADR        0x14
#define ECON1           0x1f

#define PHLCON          0x14
#define MIWRL           0x16

/*
 * ENC PHY register
 */
#define MIWRH   0x17

/*
 * ENC CS control
 */
#define Select()       PORTC &= ~(1<<6)
#define Deselect()     PORTC |= 1<<6


#define LED_High  PORTB |= 1<<8
#define LED_Low   PORTB &= ~(1<<8)

#define RST_High  PORTB |= 1<<9
#define RST_Low   PORTB &= ~(1<<9)

UI08_t mac[6]           = {0x00, 0x04, 0xA3, 0x12, 0x34, 0x56};
UI08_t ip[4]            = {192, 168, 1, 123};
UI08_t gateway[4]       = {192, 168, 1, 1};
/*
 * 1 ms delay
 */
void    delay1ms()
{
    volatile UI08_t i = 0;
    volatile UI08_t j = 0;

    for(i=0;i<200;i++)
        for(j=0;j<100;j++);
}


/*
 * writes byte v in ENC register addr of the current bank
 */
void    writeReg(unsigned char addr, unsigned short v)
{
    Select() ;
    SPI_Write(WCRCMD | addr) ;
    SPI_Write(v) ;
    Deselect() ;
}
unsigned short readReg(unsigned char addr)
{
    unsigned short v;
    Select() ;
    SPI_Write(RCRCMD | addr) ;
    v= SPI_Write(0) ;
    Deselect() ;

    return v;
}

/*
 * main entry
 */
UI08_t frameBf[1518];

int main()
{
    UI16_t i = 0;
    AD1PCFGL = 0xFFFF;
    TRISB &= ~(1<<8);   // blinky;
    
    Deselect() ;                    // don't talk to ENC

    SPI_Init();
    SPI_SetDebug(0);
    uartInit();
    uartTxString("*********************\r\n");
    uartTxString("* * ENC28J60 demo * *\r\n");
    uartTxString("*********************\r\n");
    /*
    RST_High;
    delay1ms();
    RST_Low;
    delay1ms();
    RST_High;
    delay1ms();
    
    Select() ;
    SPI_Write(BFCCMD | ECON1) ;
    SPI_Write(0b00000001) ;
    Deselect() ;

    Select() ;
    SPI_Write(BFSCMD | ECON1) ;
    SPI_Write(0b00000010) ;
    Deselect() ;

    writeReg(MIREGADR, PHLCON) ;
    writeReg(MIWRL, 0b10100010) ;
    writeReg(MIWRH, 0b00001010) ;*/

    LED_Low;
    SPI_SetDebug(0);
    enc28j60Initialize(mac);
    SPI_SetDebug(0);
    LED_High;

    arpInit();
    arpAnnounce(mac, ip, gateway);
    ipv4Init();
    icmpInit();
    
    while(1)
    {
        do
        {
        
            SPI_SetDebug(0);
            UI08_t packets = enc28j60GetPacketCount();
            /*sprintf(debugBuffer, "Packets : %02X ESTAT: %02X ECON2: %02X ECON1: %02X EIE: %02X EIR: %02X\r\n",
                    packets,
                    enc28j60ReadRegisterUint8(ESTAT),
                    enc28j60ReadRegisterUint8(ECON2),
                    enc28j60ReadRegisterUint8(ECON1),
                    enc28j60ReadRegisterUint8(EIE),
                    enc28j60ReadRegisterUint8(EIR)
                    );*/
            //uartTxString(debugBuffer);
            i++;
            if(i%2 == 0)
                LED_Low;
            else
                LED_High;
            
            while (!enc28j60PacketPending());
            
            enc28j60RxFrame(frameBf, 256);


        }
        while(1);

    }
    while(1);
    return 0;
}
