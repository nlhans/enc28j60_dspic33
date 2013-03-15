#include "spi.h"
#include "uart.h"
#include "enc28j60.h"
#include <stdio.h>

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

#ifdef DEBUG_CONSOLE
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
            spiReadByteAsm();
            bfrRx[i] = spiReadByte; //enc28j60_spi_transfer(0);// spiReadByte;
            i++;
        }

    } else if (bfrRx == NULL)
    {
        // Only reading
        while (i < length)
        {
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
            bfrRx[i] = enc28j60_spi_transfer(bfrTx[i]);
            i++;
        }
    }
#endif
}
UI08_t enc28j60_spi_read(void)
{
    spiReadByteAsm();
    return spiReadByte;
    //return SPI_ReadWrite(0);

}
void enc28j60_spi_write(UI08_t w)
{
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
}

