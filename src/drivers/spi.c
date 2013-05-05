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

UI08_t SPI_Read(void);

UI16_t volatile spiReadByte;
UI16_t volatile spiWriteByte;

void enc28j60_spi_transferBytes(UI08_t* bfrTx, UI08_t* bfrRx, UI16_t length)
{
    volatile UI16_t i = 0;

    /** PRODUCTION CODE **/
    if (bfrTx == NULL)
    {
        // Only writing
        while (i < length)
        {
            spiReadByteAsm();
            bfrRx[i] = spiReadByte; 
            i++;
        }

    } else if (bfrRx == NULL)
    {
        // Only reading
        while (i < length)
        {
            spiWriteByte = bfrTx[i];
            spiWriteByteAsm();
            i++;
        }
    }
    else
    {
        // Read&write
        while (i < length)
        {
            bfrRx[i] = enc28j60_spi_transfer(bfrTx[i]);
            i++;
        }
    }
}

UI08_t enc624j600_spi_Rx(void)
{
    spiENC624ReadByteAsm();
    return spiReadByte;

}
void enc624j600_spi_Tx(UI08_t w)
{
    spiWriteByte = w & 0xFF;
    spiENC624WriteByteAsm();
}

UI08_t enc28j60_spi_read(void)
{
    spiReadByteAsm();
    return spiReadByte;

}
void enc28j60_spi_write(UI08_t w)
{
    spiWriteByte = w & 0xFF;
    spiWriteByteAsm();
}

UI08_t enc28j60_spi_transfer(UI08_t data)
{
    return SPI_ReadWrite(data);
}

UI08_t SPI_Read(void)
{
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