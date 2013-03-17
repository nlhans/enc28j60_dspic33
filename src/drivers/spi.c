#include "spi.h"
#include "uart.h"
#include "enc28j60.h"
#include <stdio.h>
#include "pps.h"

#define MOSI_HighASM asm volatile("bset PORTC, #8\n");
#define MOSI_LowASM asm volatile("bclr PORTC, #8\n");

#define SCK_HighASM asm volatile("bset PORTC, #7\n");
#define SCK_LowASM asm volatile("bclr PORTC, #7\n");

#define MOSI_High  PORTC |= 1<<8
#define MOSI_Low   PORTC &= ~(1<<8)

#define SCK_High  PORTC |= 1<<7
#define SCK_Low   PORTC &= ~(1<<7)

#define MISO       (PORTC & 0x200)
#define MISOASM asm volatile("btsc PORTC, #9\n");

UI08_t debugOn;
UI08_t SPI_Read(void);

UI16_t volatile spiReadByte;
UI16_t volatile spiWriteByte;

UI16_t spiReadStat = 0;
UI16_t spiWriteStat = 0;


UI08_t isHumanChar(UI08_t c)
{
    if (c >= 0x20 && c <= 0x7E)
        return 1;
    else
        return 0;
}


void enc28j60_spi_transferBytes(UI08_t* bfrTx, UI08_t* bfrRx, UI16_t length)
{
    volatile UI16_t i = 0;

//#ifdef DEBUG_CONSOLE
#if 0
    UI08_t written = 0;
    UI08_t read = 0;

    UI08_t dbg = 0;

    dbg = debugOn;
    SPI_SetDebug(0);

    if(dbg)
    {
        if (bfrTx == NULL)
            uartTxString("\r\n\t\t\t\t\t\t\t\t\t\t\t");
    }
    while (i < length)
    {
        if (bfrTx == NULL)
        {
            written = 0;
        }
        else
        {
            written = bfrTx[i];
        }
        read = enc28j60_spi_transfer(written);
        
        if (bfrRx != NULL)
            bfrRx[i] = read;

        if (dbg)
        {
            if (bfrTx == NULL)
                sprintf(debugBuffer, "%02X ", read);
            else if (bfrRx == NULL)
                sprintf(debugBuffer, "%02X ", written);
            else
                sprintf(debugBuffer, "T%02X|R%02X ", written, read);
            uartTxString(debugBuffer);
            if (i % 16 == 15)
            {
                if (bfrTx == NULL)
                    uartTxString("\r\n\t\t\t\t\t\t\t\t\t\t\t");
                else
                    uartTxString("\r\n");
            }
        }
        i++;
    }

    if(dbg)
        uartTxString("\r\n");
    if(dbg && 0)
    {
        if (bfrTx != NULL)
        {
            uartTxString("\r\nIN: ");
            for (i = 0; i < length; i++)
            {
                if (isHumanChar(bfrTx[i]))
                    uartTxByte(bfrTx[i]);
                else
                    uartTxByte('?');
            }
        }
        if (bfrRx != NULL)
        {
            uartTxString("\r\nOUT: ");
            for (i = 0; i < length; i++)
            {
                if (isHumanChar(bfrRx[i]))
                    uartTxByte(bfrRx[i]);
                else
                    uartTxByte('?');
            }
        }
    }
    SPI_SetDebug(dbg);
#else
    /** PRODUCTION CODE **/
    if (bfrTx == NULL)
    {
        // Only writing
        while (i < length)
        {
            spiReadStat++;
            spiReadByteAsm();
            bfrRx[i] = spiReadByte; //enc28j60_spi_transfer(0);// spiReadByte;
            i++;
        }

    } else if (bfrRx == NULL)
    {
        // Only reading
        while (i < length)
        {
            spiWriteStat++;
            spiWriteByte = bfrTx[i];
            spiWriteByteAsm();
            //enc28j60_spi_transfer(bfrTx[i]);
            i++;
        }
    }
    else
    {
        // Read&write
        while (i < length)
        {
            spiReadStat++;
            spiWriteStat++;
            bfrRx[i] = enc28j60_spi_transfer(bfrTx[i]);
            i++;
        }
    }
#endif
}
UI16_t enc28j60_get_statRx()
{
    return spiReadStat;
}
UI16_t enc28j60_get_statTx()
{
    return spiWriteStat;
}
void enc28j60_reset_stat()
{
    spiWriteStat = 0;
    spiReadStat = 0;
}

UI08_t enc28j60_spi_read(void)
{
    spiReadStat ++;
    spiReadByteAsm();
    return spiReadByte;
    //return SPI_ReadWrite(0);

}
void enc28j60_spi_write(UI08_t w)
{
    spiWriteStat ++;
    spiWriteByte = w & 0xFF;
    spiWriteByteAsm();
    
    //SPI_ReadWrite(w);

}
UI08_t enc28j60_spi_transfer(UI08_t data)
{
    return SPI_ReadWrite(data);
}

void SPI_SetDebug(UI08_t on)
{
    debugOn=on;
}

UI08_t SPI_Read(void)
{
    //spiReadByteAsm();
    //return spiReadByte;

    spiReadStat += 8;
    UI08_t dat_ret = 0;
    short i = 0;

    MOSI_LowASM;
    for(i = 7; i >= 0; i--)
    {
        SCK_HighASM;
        if (MISO)
        {
            dat_ret |= 1<<i;
        }

        SCK_LowASM;
    }

    return dat_ret;
}

UI08_t SPI_ReadWrite(UI08_t dat)
{
    UI08_t dat_ret = 0;
    short i = 0;

    spiReadStat += 8;
    spiWriteStat += 8;
    for(i = 7; i >= 0; i--)
    {
        UI08_t b = dat >> i;
        b = b & 0x1;

        if(b==0)
        {
            MOSI_LowASM;
        }
        else
        {
            MOSI_HighASM;
        }

        SCK_HighASM;
        if (MISO > 0)
            dat_ret |= 1<<i;
        SCK_LowASM;
    }
    return dat_ret;

}
void SPI_Init()
{
    TRISC |= (1<<9);   // so
    TRISC &= ~(1<<8);   // si
    TRISC &= ~(1<<7);   // sck
    TRISC &= ~(1<<6);   // cs
    TRISB &= ~(1<<9);   // reset
    
    SCK_Low;
    MOSI_Low;

#ifndef SOFT_SPI
    PPSUnLock;
    iPPSOutput(OUT_PIN_PPS_RP23, OUT_FN_PPS_SCK1);
    iPPSOutput(OUT_PIN_PPS_RP24, OUT_FN_PPS_SDO1);
    iPPSInput(IN_FN_PPS_SDI1, IN_PIN_PPS_RP25);
    PPSLock;

    IFS0bits.SPI1IF = 0;
    IEC0bits.SPI1IE = 0;

    SPI1CON1 = 0b0000000100111010;
    SPI1CON2 = 0b0;
    SPI1STAT |= 0x1 << 15;
#endif
}

UI08_t SPI_hw_Read()
{
    SPI1BUF = 0;
    while(SPI1STATbits.SPIRBF == 0);
    return SPI1BUF;
}

void SPI_hw_Write(UI08_t d)
{
    volatile UI16_t aha = 0;
    SPI1BUF = d;
    while(SPI1STATbits.SPIRBF == 0);
    aha = SPI1BUF;
}