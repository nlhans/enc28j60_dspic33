
#include "enc624j600.h"
#include "uart.h"

/*
rc1	=	sck
rc0	=	mosi
rb3	=	miso
rb2	=	cs
 */

#define ENC624_MOSI_HighASM asm volatile("bset PORTC, #0\n");
#define ENC624_MOSI_LowASM asm volatile("bclr PORTC, #0\n");

#define ENC624_SCK_HighASM asm volatile("bset PORTC, #1\n");
#define ENC624_SCK_LowASM asm volatile("bclr PORTC, #1\n");

#define ENC624_MISO       (PORTB & (1<<3))

#define ENC624_CS_HighASM asm volatile("bset PORTB, #2\n");
#define ENC624_CS_LowASM asm volatile("bclr PORTB, #2\n");

UI08_t enc624j600_spi_write(UI08_t dat)
{
    UI08_t dat_ret = 0;
    short i = 0;
    
    for(i = 7; i >= 0; i--)
    {
        UI08_t b = dat >> i;
        b = b & 0x1;

        if(b==0)
        {
            ENC624_MOSI_LowASM;
        }
        else
        {
            ENC624_MOSI_HighASM;
        }
        if (ENC624_MISO > 0)
            dat_ret |= 1<<i;

        ENC624_SCK_HighASM;
        ENC624_SCK_LowASM;
    }
    return dat_ret;

}
void enc624j600_delay(UI08_t d)
{
    volatile UI16_t i=0,j=0;
    
    while (d-- > 0){
        for(i=0;i< 100; i++)
        {
            for(j=0;j<100;j++)
            {
            }
        }
    }
}

UI16_t enc624j600_readRegister(UI08_t addr)
{
    //
    ENC624_CS_LowASM;

    enc624j600_spi_write((addr & 0b11111)); // 000a aaaa
    UI16_t dat = enc624j600_spi_write(0x00);
    dat |= (enc624j600_spi_write(0x00) << 8);
    ENC624_CS_HighASM;

    return dat;
}

void enc624j600_writeRegister(UI16_t addr, UI16_t v)
{
    //
    ENC624_CS_LowASM;
    enc624j600_spi_write(0b01000000 | (addr & 0b11111)); // 000a aaaa
    enc624j600_spi_write(v & 0xFF);
    enc624j600_spi_write(v >> 8);
    ENC624_CS_HighASM;
}

UI16_t enc624j600_readRegisterU(UI08_t addr)
{
    UI16_t dat = 0;
    //
    ENC624_CS_LowASM;

    enc624j600_spi_write(0x20); // read unbanked
    enc624j600_spi_write(addr); // address

    dat = enc624j600_spi_write(0x00);
    ENC624_CS_HighASM;

    ENC624_CS_LowASM;
    enc624j600_spi_write(0x20); // read unbanked
    enc624j600_spi_write(addr+1); // address

    dat |= (enc624j600_spi_write(0x00) << 8);
    ENC624_CS_HighASM;

    return dat;
}

void enc624j600_setBank(UI08_t bank)
{
    ENC624_CS_LowASM;
    enc624j600_spi_write(0b11000000 | (bank<<1));
    ENC624_CS_HighASM;
}

char bf[64];

#define ENC624_EIDLED 0x14
#define ENC624_EIR 0x1C

void enc624j600_init()
{
    UI08_t i, b;
    TRISB &= ~(1<<2); // cs     OUT
    //TRISB |= (1<<2);
    TRISB |=  (1<<3); // miso   IN
    TRISC &= ~(1<<0); // mosi   OUT
    TRISC &= ~(1<<1); // sck    OUT

    // internal pull-up MISO
    //CNPU1bits.CN7PUE = 1;
    
    ENC624_CS_HighASM;
    ENC624_MOSI_HighASM;
    ENC624_SCK_HighASM;

    //while(1);

    sprintf(bf, "TRISB: %04X, TRISC: %04X\r\n", TRISB, TRISC);
    uartTxString(bf);

    enc624j600_setBank(3);

    // Write LED thing
    enc624j600_writeRegister(ENC624_EIDLED, 0b0010011000000000);

    while(1)
    {
        enc624j600_delay(100);
        for(b = 0; b < 4; b++)
        {
            enc624j600_setBank(b);
            sprintf(bf,"-------\r\nBANK %d\r\n-------", b);
            uartTxString(bf);
            
            for ( i = 0; i < 0x20; i+= 2)
            {
                if (i % 16 == 0)
                {
                    uartTxString("\r\n");
                }
                
                sprintf(bf, "%02X: %04X | ", i, enc624j600_readRegister(i));
                uartTxString(bf);
            }
            uartTxString("\r\n");
            uartTxString("\r\n");
        }

        /*for (i = 0; i < 0x9F; i+=2)
        {
            if (i % 16 == 0)
            {
                uartTxString("\r\n");
            }

            sprintf(bf, "%02X: %04X | ", i, enc624j600_readRegister(i));
            uartTxString(bf);
        }*/

        uartTxString("\r\n");
        uartTxString("\r\n");


        sprintf(bf, "Reg: %04X, EIR: %04X\r\n", enc624j600_readRegister(ENC624_EIDLED), enc624j600_readRegister(ENC624_EIR));
        uartTxString(bf);
    }

}