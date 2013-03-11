#include "spi.h"
#include "uart.h"
#include "enc28j60.h"
#include <stdio.h>

#define MOSI_High  PORTC |= 1<<8
#define MOSI_Low   PORTC &= ~(1<<8)

#define SCK_High  PORTC |= 1<<7
#define SCK_Low   PORTC &= ~(1<<7)

#define MISO       (((PORTC & 0x200)>0)?1:0)

UI08_t debugOn;

UI08_t isHumanChar(UI08_t c)
{
    if (c >= 0x20 && c <= 0x7E)
        return 1;
    else
        return 0;
}


void enc28j60_spi_transferBytes(UI08_t* bfrIn, UI08_t* bfrOut, UI16_t length)
{
    UI16_t i = 0;
    UI08_t written = 0;
    UI08_t read = 0;

#ifdef DEBUG_CONSOLE
    UI08_t dbg = 0;

    dbg = debugOn;
    SPI_SetDebug(0);

    if(dbg)
    {
        if (bfrIn == NULL)
            uartTxString("\r\n\t\t\t\t\t\t\t\t\t\t\t");
    }
#endif
    while (i < length)
    {
        if (bfrIn == NULL)
            written = 0;
        else
            written = bfrIn[i];
        
        read = enc28j60_spi_transfer(written);

        if (bfrOut != NULL)
            bfrOut[i] = read;

#ifdef DEBUG_CONSOLE
        if (dbg)
        {
            if (bfrIn == NULL)
                sprintf(debugBuffer, "%02X ", read);
            else if (bfrOut == NULL)
                sprintf(debugBuffer, "%02X ", written);
            else
                sprintf(debugBuffer, "T%02X|R%02X ", written, read);
            uartTxString(debugBuffer);
            if (i % 16 == 15)
            {
                if (bfrIn == NULL)
                    uartTxString("\r\n\t\t\t\t\t\t\t\t\t\t\t");
                else
                    uartTxString("\r\n");
            }
        }
#endif
        i++;
    }

#ifdef DEBUG_CONSOLE
    if(dbg)
        uartTxString("\r\n");
    if(dbg && 0)
    {
        if (bfrIn != NULL)
        {
            uartTxString("\r\nIN: ");
            for (i = 0; i < length; i++)
            {
                if (isHumanChar(bfrIn[i]))
                    uartTxByte(bfrIn[i]);
                else
                    uartTxByte('?');
            }
        }
        if (bfrOut != NULL)
        {
            uartTxString("\r\nOUT: ");
            for (i = 0; i < length; i++)
            {
                if (isHumanChar(bfrOut[i]))
                    uartTxByte(bfrOut[i]);
                else
                    uartTxByte('?');
            }
        }
    }
    SPI_SetDebug(dbg);
#endif
}

UI08_t enc28j60_spi_transfer(UI08_t data)
{
    return SPI_Write(data);
}

void SPI_SetDebug(UI08_t on)
{
    debugOn=on;
}

UI08_t SPI_Write(UI08_t dat)
{
    UI08_t dat_ret = 0;
    short i = 0;

    for(i = 7; i >= 0; i--)
    {
        UI08_t b = dat >> i;
        b = b & 0x1;

        if(b==0)
        {
            MOSI_Low;
        }
        else
        {
            MOSI_High;
        }

        SCK_High;
        if (MISO)
        {
            dat_ret |= 1<<i;
        }

        SCK_Low;
    }

    //if(debugOn > 0)
    //{
    //    sprintf(debugBuffer, "TX: 0x%02X RX: 0x%02X\r\n", dat, dat_ret);
    //    uartTxString(debugBuffer);
    //}
    
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
}

