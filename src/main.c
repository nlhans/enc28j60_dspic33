#include "types.h"
#include "spi.h"
#include "uart.h"
#include "enc28j60.h"
#include "arp.h"
#include "ipv4.h"
#include "icpm.h"
#include "udp.h"
#include "ntp.h"
#include "tcp.h"
#include <stdio.h>
#include "23lc1024.h"

#include "sram_defs.h"

_FBS(BWRP_WRPROTECT_OFF & BSS_NO_FLASH & RBS_NO_RAM)
_FSS(SWRP_WRPROTECT_OFF & SSS_NO_FLASH & RSS_NO_RAM)
_FGS(GWRP_OFF & GCP_OFF)
_FOSCSEL(FNOSC_FRCPLL & IESO_OFF)
_FOSC(POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSECME)
_FWDT(WDTPOST_PS128 & WDTPRE_PR128 & WINDIS_OFF & FWDTEN_OFF)
_FPOR(FPWRT_PWR1 & ALTI2C_OFF)
_FICD(ICS_PGD2 & JTAGEN_OFF)

UI16_t errStkLoc = 0;
UI16_t errPrgLoc = 0;

extern UI16_t getErrLoc(void);
extern UI32_t MemReadLatch(UI08_t psvPag, UI16_t addr);

#define LED_High  PORTB |= 1<<8
#define LED_Low   PORTB &= ~(1<<8)

#define RST_High  PORTB |= 1<<9
#define RST_Low   PORTB &= ~(1<<9)

UI08_t mac[6]           = {0x00, 0x04, 0xA3, 0x12, 0x34, 0x56};
UI08_t ip[4]            = {192, 168, 1, 123};
UI08_t gateway[4]       = {192, 168, 1, 1};
UI08_t pc[4]       = {192, 168, 1, 147};
UI08_t ntpServer[4] = {194, 171, 167, 130};


typedef enum instPtrMode
{
    PtrMode_DirectAccess, // W
    PtrMode_Ptr, // [W]
    PtrMode_PtrPP, // [W++]
    PtrMode_PtrMM, // [W--]
    PtrMode_MMPtr, // [--W]
    PtrMode_PPPtr, // [++W]
    PtrMode_PtrW, // [W+W_secondary] (offset)
} instPtrMode_t;

bool_t wasWrite = FALSE;

UI32_t instData;
UI16_t normalRegAddr;
UI08_t i;

UI08_t id4;
UI08_t id5;
UI08_t id6;
UI08_t id6;
UI08_t id7;
UI08_t id8;
UI16_t id9;
UI16_t id10;
UI16_t id11;
UI16_t id12;
UI08_t srcAddr;
instPtrMode_t srcAddrMode;
UI08_t dstAddr;
instPtrMode_t dstAddrMode;

UI16_t resolvedSrcAddr;
UI16_t resolvedDstAddr;

void __attribute__((__interrupt__)) _AddressError(void);
void __attribute__((interrupt, no_auto_psv)) _AddressError(void)
{
        errStkLoc   = (UI16_t)getErrLoc();
        if(errStkLoc > 0x4000) //16kiB of RAM
            asm volatile("reset");
        errPrgLoc   = * ((UI16_t*)errStkLoc) ;
        
        instData = MemReadLatch(0, errPrgLoc - 2);

        // parse the instruction, so that we know *what* register was used
        // to read/write this.
        id12 = instData >> 12;
        id11 = id12 >> 1;
        id10 = id11 >> 1;
        id9 = id10 >> 1;
        id8 = id9 >> 1;
        id7 = id8 >> 1;
        id6 = id7 >> 1;
        id5 = id6 >> 1;
        id4 = id5 >> 1;
        //id5 = instData >> 19;

        srcAddrMode = (instPtrMode_t) ( (instData >> 4)  & 0x7 );
        dstAddrMode = (instPtrMode_t) ( (instData >> 11) & 0x7 );
        srcAddr  = instData & 0xF;
        dstAddr = (instData >> 7) & 0xF;
        
        if (id12 == 0b101111100000) // mov.d
        {
            dstAddrMode = PtrMode_DirectAccess;
            uartTxString("MOV.D [W], W\r\n");
        }
        if (id10 == 0b1011111010) // mov.d
        {
            srcAddrMode = PtrMode_DirectAccess;
            uartTxString("MOV.D W, [W]\r\n");
        }
        if (id5 == 0b10011) // MOV W, [W+slit10]
        {
            srcAddrMode = PtrMode_DirectAccess;
            dstAddrMode = PtrMode_PtrW;
            uartTxString("MOV W, [W+0xSS]\r\n");
        }
        if (id9 == 0b111010110) // CLR W
        {
            srcAddr = 0;
            srcAddrMode = PtrMode_DirectAccess;
            uartTxString("CLR [W]\r\n");
        }
        if(id5 == 0b01010) // SUB Wb, #lit5, [Wd]
        {
            // 0101 0www wBqq qddd d11k kkkk
            // w=base
            // B=word/byte
            // q=dst adr mode
            // d=dst
            // k=lit5
            srcAddrMode = PtrMode_DirectAccess;
            srcAddr = (instData >> 14) & 0xF;
            uartTxString("SUB Wb, #lit5, [Wd]\r\n");
        }
        if (id5 == 0b01111)
        {
            // 0111 1www wBhh hddd dggg ssss
            // 0111 1000 0100 1000 1000 0000
            // id
            // w=offset work register, if required
            // b=byte/word
            // h=dst addr mode
            // d=dst addr
            // g=src addr mode
            // s=src addr
            uartTxString("MOV W, [W+]\r\n");
            
        }
        sprintf(debugBuffer, "INST SRC %d, DST %d || (MOV {%d}, {%d})\r\n", srcAddr, dstAddr, srcAddrMode, dstAddrMode);
        uartTxString(debugBuffer);


        // Is it write?
        if (srcAddrMode == PtrMode_DirectAccess && dstAddrMode != PtrMode_DirectAccess)
        {
            wasWrite=TRUE;
            // We are writing to a pointer, which failed to access.
            // Put back stack so after this , it's reexecuted.
            *((UI16_t*)errStkLoc) = errPrgLoc-2;
        }
        else
        {
            wasWrite=FALSE;
            // we don't have to adjust the ptr, because reads are reexecuted
        }

        // resolve src
        if (1 || srcAddrMode != PtrMode_DirectAccess)
        {
            // 7-14 is beyond normal XC16 stack
            if (srcAddr < 7)
            {
                // prgStk + 0x06 -> W0?
                // e.g. prgStk = 0x1010
                // W0 = 0x1016
                // W1 = 0x1018 etc
                //0-7, is in stack, 8-15 are not pushed
                resolvedSrcAddr = *((UI16_t*)(errStkLoc + 0x06 + srcAddr*0x2));
            }
            else if (srcAddr == 7)
            {
                asm volatile("mov W7, _resolvedSrcAddr");
            }
            else if (srcAddr == 8)
            {
                asm volatile("mov W8, _resolvedSrcAddr");
            }
            else if (srcAddr == 9)
            {
                asm volatile("mov W9, _resolvedSrcAddr");
            }
            else if (srcAddr == 10)
            {
                asm volatile("mov W10, _resolvedSrcAddr");
            }
            else if (srcAddr == 11)
            {
                asm volatile("mov W11, _resolvedSrcAddr");
            }
            else if (srcAddr == 12)
            {
                asm volatile("mov W12, _resolvedSrcAddr");
            }
            else if (srcAddr == 13)
            {
                asm volatile("mov W13, _resolvedSrcAddr");
            }
            else if (srcAddr == 14)
            {
                asm volatile("mov W14, _resolvedSrcAddr");
            }

        }

        // resolve src
        if (dstAddrMode != PtrMode_DirectAccess)
        {
            // 7-14 is beyond normal XC16 stack
            if (dstAddr < 7)
            {
                // prgStk + 0x06 -> W0?
                // e.g. prgStk = 0x1010
                // W0 = 0x1016
                // W1 = 0x1018 etc
                //0-7, is in stack, 8-15 are not pushed
                resolvedDstAddr = *((UI16_t*)(errStkLoc + 0x06 + dstAddr*0x2));
            }
            else if (dstAddr == 7)
            {
                asm volatile("mov W7, _resolvedDstAddr");
            }
            else if (dstAddr == 8)
            {
                asm volatile("mov W8, _resolvedDstAddr");
            }
            else if (dstAddr == 9)
            {
                asm volatile("mov W9, _resolvedDstAddr");
            }
            else if (dstAddr == 10)
            {
                asm volatile("mov W10, _resolvedDstAddr");
            }
            else if (dstAddr == 11)
            {
                asm volatile("mov W11, _resolvedDstAddr");
            }
            else if (dstAddr == 12)
            {
                asm volatile("mov W12, _resolvedDstAddr");
            }
            else if (dstAddr == 13)
            {
                asm volatile("mov W13, _resolvedDstAddr");
            }
            else if (dstAddr == 14)
            {
                asm volatile("mov W14, _resolvedDstAddr");
            }

        }

        if (wasWrite)
            normalRegAddr = sram_fault(resolvedDstAddr);
        else
            normalRegAddr = sram_fault(resolvedSrcAddr);

        sprintf(debugBuffer, "%lu, WR %d, SRC %04X, DST %04X\r\n", instData, wasWrite, resolvedSrcAddr, resolvedDstAddr);
        uartTxString(debugBuffer);

        // read/modify etc.
        if (wasWrite || 1)
        {
            // 7-14 is beyond normal XC16 stack
            if (dstAddr < 7)
            {
                // prgStk + 0x06 -> W0?
                // prgLoc = 0x1010
                // W0 = 0x1016
                // W1 = 0x1018 etc
                //0-7, is in stack
                *((UI16_t*)(errStkLoc + 0x06 + dstAddr*0x2)) = normalRegAddr;
            }
            else if (dstAddr == 7)
            {
                asm volatile("mov _normalRegAddr, W7");
            }
            else if (dstAddr == 8)
            {
                asm volatile("mov _normalRegAddr, W8");
            }
            else if (dstAddr == 9)
            {
                asm volatile("mov _normalRegAddr, W9");
            }
            else if (dstAddr == 10)
            {
                asm volatile("mov _normalRegAddr, W10");
            }
            else if (dstAddr == 11)
            {
                asm volatile("mov _normalRegAddr, W11");
            }
            else if (dstAddr == 12)
            {
                asm volatile("mov _normalRegAddr, W12");
            }
            else if (dstAddr == 13)
            {
                asm volatile("mov _normalRegAddr, W13");
            }
            else if (dstAddr == 14)
            {
                asm volatile("mov _normalRegAddr, W14");
            }
            sprintf(debugBuffer, "WRITE %04X from %02d\r\n", normalRegAddr, dstAddr);
            uartTxString(debugBuffer);
        }
        else
        {
            sprintf(debugBuffer, "READ %04X from %02d\r\n", normalRegAddr, srcAddr);
            uartTxString(debugBuffer);
        }

        sprintf(debugBuffer, "err @ id %02X prg%04X @ stk %04X\r\n*********************\r\n", id8, errPrgLoc, errStkLoc);
        uartTxString(debugBuffer);
        
        INTCON1bits.ADDRERR = 0;        //Clear the trap flag
 //       while (1);
}

void    delay1ms()
{
    volatile UI08_t i = 0;
    volatile UI08_t j = 0;

    for(i=0;i<200;i++)
        for(j=0;j<100;j++);
}

UI08_t frameBf[1024];

UI08_t sramBf[64];

int main()
{
    volatile UI08_t i,j;

    AD1PCFGL = 0xFFFF;
    TRISB &= ~(1<<8);   // blinky;
    SPI_Init();
    SPI_SetDebug(0);
    uartInit();
    uartTxString("*********************\r\n");
    uartTxString("* * ENC28J60 demo * *\r\n");
    uartTxString("*********************\r\n");

    LED_Low;
    SPI_SetDebug(0);
    enc28j60Initialize(mac);
    SPI_SetDebug(0);
    LED_High;

    sram_23lc1024_init();
    sram_init();
    /*arpInit();
    arpAnnounce(mac, ip, gateway);
    ipv4Init();
    udpInit();
    tcpInit();
    icmpInit();
    ntpInit();
    ntpRequest(ntpServer);*/

    sram_23lc1024_read(0,0, sramBf, 64);
    j=0;
    for (i=0;i < 64; i++)
    {
        if (sramBf[i] != 0xAA-i)
            j=1;
    }
    if(j==0)
        uartTxString("Warm boot\r\n");
    else
        uartTxString("Cold boot\r\n");

    for (i = 0; i < 64; i++)
        sramBf[i] = 0xAA-i;
    sram_23lc1024_write(0, 0, sramBf, 64);
    memset(sramBf,0,64);
    sram_23lc1024_read(0,0, sramBf, 64);

    for (i=0;i < 64; i++)
    {
        if(sramBf[i] != 0xAA-i)
        {
            uartTxString("err\r\n");
        }
    }

    *myVariable = 1024;
    myLarge->d = 1234;
    //myLarge->data[0] = 0;
    for (i=0;i <80; i++)
    {
        uartTxByte('.');
    sprintf(debugBuffer, "%04X\r\n", myLarge->d);
    uartTxString(debugBuffer);
        myLarge->d  = myLarge->d + 123;
        myLarge->data[i] = 0xAA - i;
    }
    myLarge2->d = 4321;
    while(1)
    {
        i++;
        j++;
        i=j-i;
    }

    while(1)
    {

        while (!enc28j60PacketPending());
        enc28j60_reset_stat();
        enc28j60RxFrame(frameBf, sizeof(frameBf));
        
#ifdef DEBUG_CONSOLE
        sprintf(debugBuffer, "[spi] RX: %04d, TX: %04d\r\n", enc28j60_get_statRx(), enc28j60_get_statTx());
        uartTxString(debugBuffer);
#endif
        

    }
    while(1);
    return 0;
}
