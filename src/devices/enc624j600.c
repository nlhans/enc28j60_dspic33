
#include "enc624j600.h"
#include "uart.h"
#include "insight.h"
#include "ipv4.h"
#include "arp.h"


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
/****                   ENC624J600 SPI Command functions                     ****/
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

void enc624j600SpiCommand(UI08_t cmd, UI08_t* bf, UI08_t size)
{
    UI08_t tmp;
    //
    ENC624_CS_LowASM;
    enc624j600_spi_write(cmd);
    
    while(size > 0)
    {
        tmp = enc624j600_spi_write(*bf);
        *bf = tmp;
        bf++;
        size--;
    }
    ENC624_CS_HighASM;
    
}

void enc624j600SpiReadRxData(UI16_t ptr, UI08_t* bf, UI16_t size)
{
    enc624j600Command(WRXRDPT, (UI08_t*)&ptr, sizeof(UI16_t));
    enc624j600Command(RRXDATA, bf, size);
}


void enc624j600SpiReadData(UI16_t ptr, UI08_t* bf, UI16_t size)
{
    enc624j600Command(WGPRDPT, (UI08_t*)&ptr, sizeof(UI16_t));
    enc624j600Command(RGPDATA, bf, size);
}

void enc624j600SpiWriteData(UI16_t ptr, UI08_t* bf, UI16_t size)
{
    enc624j600Command(WGPWRPT, (UI08_t*)&ptr, sizeof(UI16_t));
    enc624j600Command(WGPDATA, bf, size);
}

/******************************************************************************/
/****                                                                      ****/
/****               ENC624J060 Register Command functions                    ****/
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
/****               ENC624J600 User Commands functions                       ****/
/****                                                                      ****/
/******************************************************************************/
bool_t enc624j600PacketPending()
{
    return ((enc624j600GetPacketCount() == 0) ? FALSE : TRUE);
}
UI08_t enc624j600GetPacketCount()
{
    return enc624j600ReadRegister8(ESTAT, L);
}

bool_t enc624j600GetLinkStatus()
{
    return (((enc624j600ReadRegister8(ESTAT, H) & 0x1) != 0) ? TRUE:FALSE);
}

bool_t enc624j600GetTxBusyStatus()
{
    return (((enc624j600ReadRegister8(ECON1, L) & 0x2) != 0) ? TRUE:FALSE);
    // ECON1.TXRTS <1>
}

void enc624j600TxFrame(EthernetFrame_t* packet, UI16_t length)
{
    UI16_t txPtr = 0;
    bool_t waitForTxToComplete = FALSE;

    // Is the mac still busy sending a previous packet?
    // Try to sneak the SPI data into the transmit buffer if it fits.
    if (enc624j600GetTxBusyStatus())
    {
        // then we find a free spot in SRAM for this packet, preferably after
        // the current transmitting (last write pointer).
        // If not, we see if it fits at 0x0000
        // If not, we wait till transmit is done and place it at 0x0000
        txPtr = enc624j600ReadRegister16(ETXST) + enc624j600ReadRegister16(ETXLEN);
        waitForTxToComplete = FALSE;
        
        if (ENC624J600_RXBUF_START - txPtr <= length)
        {
            // it doesn't fit after current packet.. try at start of buffer
            // note: no buffer wrapping implemented!
            txPtr = 0;

            // doesn't fit? -> wait for TX to complete
            if (enc624j600ReadRegister16(ETXST) >= length)
            {
                waitForTxToComplete = TRUE;
            }
        }
    }
    if(waitForTxToComplete)
    {
        while(enc624j600GetTxBusyStatus());
    }

    // Write data
    enc624j600WriteGpData(txPtr, (UI08_t*) packet, length);

    // Now we do have to always wait for TX to complete, so we can change ptrs
    // and issue the next command
    while(enc624j600GetTxBusyStatus());

    // Set ETXST/ETXLEN
    enc624j600WriteRegister16(ETXST, txPtr);
    enc624j600WriteRegister16(ETXLEN, length);

    // Issue TX command
    enc624j600Command(SETTXRTS, NULL, 0);

    INSIGHT(ENC624J600_TX, length, packet->dstMac[0], packet->dstMac[1], packet->dstMac[2], packet->dstMac[3], packet->dstMac[4], packet->dstMac[5]);
    INSIGHT(ENC624J600_TX_PKT, length, txPtr, length);
    
}
void enc624j600TxReplyFrame(EthernetFrame_t* frame, UI16_t length)
{
    UI08_t tmpMac[6];
    
    // Swap MAC addresses.
    memcpy(tmpMac,          frame->dstMac   , 6);
    memcpy(frame->dstMac,   frame->srcMac   , 6);
    memcpy(frame->srcMac,   tmpMac          , 6);

    // TX frame, MAC & protocol are already done.
    enc624j600TxFrame(frame, sizeof(EthernetFrame_t) + length);
}

void enc624j600RxFrame(UI08_t* packet, UI16_t length)
{
    UI08_t              packetCount         = enc624j600GetPacketCount();
    static UI16_t       dataPtr             = ENC624J600_RXBUF_START;
    UI08_t              packetHeader[8];
    EthernetFrame_t*    frame;
    UI08_t*             data;
    bool_t              dummy = TRUE;
    UI16_t              tailPtr;

    while(packetCount > 0)
    {
        // Move data ptr to start of packet
        enc624j600ReadRxData(dataPtr, packetHeader, 8);

        UI16_t nextPacketOffset    = packetHeader[0] | (packetHeader[1] << 8);
        UI16_t packetSize = packetHeader[2] | ( packetHeader[3] << 8);

        INSIGHT(ENC624J600_RX_PKT, dataPtr, packetSize,
                packetHeader[0], packetHeader[1], packetHeader[2], packetHeader[3],
                packetHeader[4], packetHeader[5], packetHeader[6], packetHeader[7]);
        //if(nextPacketOffset > dataPtr)
        //    packetSize = nextPacketOffset - dataPtr;
        //else
        //    packetSize = (0x5FFF - nextPacketOffset) - (dataPtr - ENC624J600_RXBUF_START);

        if ((packetHeader[2+2] & (1<<4)) == 0) // no CRC error
        {
            // Receive packet itself
            enc624j600ReadRxData(dataPtr + 6, packet, packetSize+2-4); // -4 for CRC, +2 for some status bytes or something?

            frame = (EthernetFrame_t*) (packet+2);
            frame-> type = htons(frame->type); // reverse byte order
            data = (UI08_t*) (packet + 2+sizeof(EthernetFrame_t));

            INSIGHT(ENC624J600_RX, packetSize+2-4, frame->srcMac[0], frame->srcMac[1], frame->srcMac[2], frame->srcMac[3], frame->srcMac[4], frame->srcMac[5]);

            dummy = FALSE;
            ipv4HandlePacket(frame, &dummy);

            if (dummy==FALSE)
            {
                arpProcessPacket(frame, &dummy);
            }

        }

        if(dataPtr == ENC624J600_RXBUF_START)
            tailPtr = 0x5FFE; // put back to end of buffer, -2 bytes!
        else
            tailPtr = dataPtr - 2;
        enc624j600WriteRegister16(ERXTAIL, tailPtr);
        
        dataPtr = nextPacketOffset;
        enc624j600Command(SETPKTDEC, NULL, 0); // decrease packet counter
        packetCount = enc624j600GetPacketCount();
    }

    return;
}

void enc624j600Initialize()
{
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
    enc624j600WriteRegister16(ERXST, ENC624J600_RXBUF_START); // 50/50 split

    // Receive filters.
    // Enable reception of multicast and unicast
    enc624j600WriteRegister16(ERXFCON, 0b0000000000001001);

    // MAC initialization
    enc624j600BitSetRegister8(ECON1, L, 1); // ECON1.RXEN

    // Report MAC over debug port.
    INSIGHT(ENC624J600_MAC, enc624j600ReadRegister8(MADDR1, L), enc624j600ReadRegister8(MADDR1, H)
            , enc624j600ReadRegister8(MADDR2, L), enc624j600ReadRegister8(MADDR2, H)
            , enc624j600ReadRegister8(MADDR3, L), enc624j600ReadRegister8(MADDR3, H))

}