
#include "enc624j600.h"
#include "uart.h"
#include "insight.h"

/*
rc1	=	sck
rc0	=	mosi
rb3	=	miso
rb2	=	cs
 */

static UI08_t currentBank = 0;

#define ENC624_MOSI_HighASM asm volatile("bset PORTC, #0\n");
#define ENC624_MOSI_LowASM asm volatile("bclr PORTC, #0\n");

#define ENC624_SCK_HighASM asm volatile("bset PORTC, #1\n");
#define ENC624_SCK_LowASM asm volatile("bclr PORTC, #1\n");

#define ENC624_MISO       (PORTB & (1<<3))

#define ENC624_CS_HighASM asm volatile("bset PORTB, #2\n");
#define ENC624_CS_LowASM asm volatile("bclr PORTB, #2\n");


/******************************************************************************/
/****                                                                      ****/
/****                   ENC28J60 SPI Command functions                     ****/
/****                                                                      ****/
/******************************************************************************/
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
    INSIGHT(ENC624J600_SPI, dat, dat_ret);
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

/******************************************************************************/
/****                                                                      ****/
/****               ENC28J60 Register Command functions                    ****/
/****                                                                      ****/
/******************************************************************************/
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

//////// READ
UI16_t enc624j600SpiReadRegister16(UI08_t addr)
{
    UI16_t dat = enc624j600SpiReadRegister8(addr)
              | (enc624j600SpiReadRegister8(addr+1) << 8);
    
    return dat;
}

UI08_t enc624j600SpiReadRegister8(UI08_t addr)
{
    UI08_t dat = 0;

    if ((addr & regGlobal) == 0)
    {
        enc624j600_setBank(addr >> 5);
    }
    
    ENC624_CS_LowASM;
    enc624j600_spi_write((addr & 0b11111)); // 000a aaaa
    dat = enc624j600_spi_write(0x00);
    ENC624_CS_HighASM;

    return dat;
}

//////// WRITE
void enc624j600SpiWriteRegister8(UI08_t addr, UI08_t value)
{
    if ((addr & regGlobal) == 0)
    {
        enc624j600_setBank(addr >> 5);
    }

    ENC624_CS_LowASM;
    enc624j600_spi_write((1<<6) | (addr & 0b11111)); // 010a aaaa
    enc624j600_spi_write(value);
    ENC624_CS_HighASM;
}

void enc624j600SpiWriteRegister16(UI08_t addr, UI16_t value)
{
    enc624j600SpiWriteRegister8(addr, value & 0xFF);
    enc624j600SpiWriteRegister8(addr+1, value >> 8);
}

//////// BSET
void enc624j600SpiBSetRegister8(UI08_t addr, UI08_t value)
{
    if ((addr & regGlobal) == 0)
    {
        enc624j600_setBank(addr >> 5);
    }

    ENC624_CS_LowASM;
    enc624j600_spi_write((1<<7) | (addr & 0b11111)); // 100a aaaa
    enc624j600_spi_write(value);
    ENC624_CS_HighASM;
}

void enc624j600SpiBSetRegister16(UI08_t addr, UI16_t value)
{
    enc624j600SpiBSetRegister8(addr, value & 0xFF);
    enc624j600SpiBSetRegister8(addr+1, value >> 8);
}

void enc624j600_resetBank()
{
    currentBank = 0xFF; // doesn't exist, always set bank at next try!
}

void enc624j600_setBank(UI08_t bank)
{
    if (bank != currentBank)
    {
        ENC624_CS_LowASM;
        enc624j600_spi_write(0b11000000 | (bank<<1));
        ENC624_CS_HighASM;
              
        currentBank = bank;
    }
}

char bf[64];

#define ENC624_EIDLED 0x14
#define ENC624_EIR 0x1C

/******************************************************************************/
/****                                                                      ****/
/****               ENC28J60 User Commands functions                       ****/
/****                                                                      ****/
/******************************************************************************/
bool_t enc624j600PacketPending()
{
    return ((enc624j600ReadRegister8(ESTAT, L) == 0) ? FALSE : TRUE);
}

bool_t enc624j600GetLinkStatus()
{
    return ((enc624j600ReadRegister8(ESTAT, H) & 0x1 != 0) ? TRUE:FALSE);
}

void enc624j600RxFrame(UI08_t* packet, UI16_t length)
{
    //
}

void enc624j600Initialize()
{
    UI08_t i, b;
    TRISB &= ~(1<<2); // cs     OUT
    TRISB |=  (1<<3); // miso   IN
    TRISC &= ~(1<<0); // mosi   OUT
    TRISC &= ~(1<<1); // sck    OUT

    ENC624_CS_HighASM;
    ENC624_MOSI_HighASM;
    ENC624_SCK_HighASM;

    // Write EUDAST to 1234h and verify  ->> com's working
    // TODO: Should have time-out.
    // Then reset and check for 0 on same register
    do
    {
        do
        {
            enc624j600WriteRegister16(EUDAST, 0x1234);
        }
        while (enc624j600ReadRegister16(EUDAST) != 0x1234); // verify comms
        while (enc624j600ReadRegister8(ESTAT, H) & (1<<4) == 0); // ESTAT.CLKRDY
        enc624j600BitSetRegister8(ECON2, L, (1<<4)); // set bit 4
        enc624j600_resetBank();
        enc624j600_delay(100); // wait 25us, but this ain't 25us though.
    }
    while (enc624j600ReadRegister16(EUDAST) != 0x0000);  // succesful reset?
    enc624j600_delay(100); // 256us should be waited before Phy.

    // Turn on RX/link status LED's.
    enc624j600WriteRegister16(EIDLED, 0x2600);

    // Set-up RX/TX buffers start points.
    enc624j600WriteRegister16(ETXST, 0x0);
    enc624j600WriteRegister16(ERXST, 0x3000); // 50/50 split

    // Receive filters.
    // Enable reception of multicast and unicast
    enc624j600WriteRegister16(ERXFCON, 0b0000000000001001);

    // MAC initialization
    enc624j600BitSetRegister8(ECON1, L, 1); // ECON1.RXEN

    // Report MAC over debug port.
    INSIGHT(ENC624J600_MAC, enc624j600ReadRegister8(MADDR1, L), enc624j600ReadRegister8(MADDR1, H)
            , enc624j600ReadRegister8(MADDR2, L), enc624j600ReadRegister8(MADDR2, H)
            , enc624j600ReadRegister8(MADDR3, L), enc624j600ReadRegister8(MADDR3, H))

    while(1)
    {
        enc624j600_delay(200);
        /*for(b = 0; b < 4; b++)
        {
            enc624j600_setBank(b);
            for ( i = 0; i < 0x20; i+= 2)
            {
                INSIGHT(ENC624J600_REG, (b*0x20 + i), enc624j600SpiReadRegister16(i));
            }
        }*/

        INSIGHT(ENC624J600_PACKETS, enc624j600ReadRegister8(ESTAT, L));
    }

}