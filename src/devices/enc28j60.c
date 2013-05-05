
#include "spi.h"
#include "uart.h"
#include "enc28j60.h"
#include "udp.h"
#include "arp.h"
#include "insight.h"

#define ETHERNET_HANDLERS_COUNT 2


char debugBuffer[256];

typedef struct EthernetPacketHandlerInfo_s
{
    bool_t used;
    EthernetPacketHandler_t handler;
} EthernetPacketHandlerInfo_t;

void enc28j60WriteUint8(UI08_t reg, UI08_t value);
void enc28j60WriteUint16(UI08_t reg, UI16_t value);
UI08_t enc28j60ReadUint8(UI08_t reg);
UI16_t enc28j60ReadUint16(UI08_t reg);


void enc28j60SetBank(enc28j60Register_t bank);
UI08_t enc28j60ReadRegisterUint16(UI08_t address);

void enc28j60WriteRegisterUint8(UI08_t address, UI08_t value);
void enc28j60WriteRegisterUint16(UI08_t address, UI16_t value);


void enc28j60FireTxHandlers(EthernetFrame_t* frame);

static UI08_t* myMac;
static UI08_t currentBank = 0;

static EthernetPacketHandlerInfo_t handlers[ETHERNET_HANDLERS_COUNT];

/******************************************************************************/
/****                                                                      ****/
/****                   ENC28J60 SPI Command functions                     ****/
/****                                                                      ****/
/******************************************************************************/
void enc28j60WriteUint8(UI08_t reg, UI08_t value)
{
    ENC28J60_CS_LOW;
    enc28j60_spi_write(WCR | (reg & 0x1F));
    enc28j60_spi_write(value);
    ENC28J60_CS_HIGH;
    
    INSIGHT(ENC28J60_WRITE_REG, reg, value);
}
void enc28j60BitSetUint8(UI08_t reg, UI08_t value)
{
    ENC28J60_CS_LOW;
    enc28j60_spi_write(BFS | (reg & 0x1F));
    enc28j60_spi_write(value);
    ENC28J60_CS_HIGH;

    INSIGHT(ENC28J60_BITSET_REG, reg, value);
}
void enc28j60BitClrUint8(UI08_t reg, UI08_t value)
{
    ENC28J60_CS_LOW;
    enc28j60_spi_write(BFC | (reg & 0x1F));
    enc28j60_spi_write(value);
    ENC28J60_CS_HIGH;

    INSIGHT(ENC28J60_BITCLR_REG, reg, value);
}

void enc28j60WriteUint16(UI08_t reg, UI16_t value)
{
    ENC28J60_CS_LOW;
    enc28j60_spi_write(WCR | (reg & 0x1F));
    enc28j60_spi_write(value & 0x00FF);
    enc28j60_spi_write((value & 0xFF00) >> 8);
    ENC28J60_CS_HIGH;

    INSIGHT(ENC28J60_WRITE_REG, reg, value);
}

UI08_t enc28j60ReadUint8(UI08_t reg)
{
    UI08_t d;
    ENC28J60_CS_LOW;
    enc28j60_spi_write(RCR | (reg & 0x1F));
    d = enc28j60_spi_read();
    ENC28J60_CS_HIGH;

    if (reg != EIR)
    INSIGHT(ENC28J60_READ_REG, reg, d);

    return d;
}

UI08_t enc28j60ReadMacUint8(UI08_t reg)
{
    UI08_t d;
    ENC28J60_CS_LOW;
    enc28j60_spi_write(RCR | (reg & 0x1F));
    enc28j60_spi_write(0x00); // dummy
    d = enc28j60_spi_read();
    ENC28J60_CS_HIGH;

    INSIGHT(ENC28J60_READ_REG, reg, d);

    return d;
}

UI16_t enc28j60ReadUint16(UI08_t reg)
{
    UI16_t d  = 0;
    d =      enc28j60ReadUint8(reg);                // low
    d = d | (enc28j60ReadUint8(reg+1) << 8);        // high

    return d;
}

void enc28j60SetBank(enc28j60Register_t reg)
{
    if (currentBank > 0)
    {
        ENC28J60_CS_LOW;
        enc28j60_spi_write(BFC | (ECON1 & 0x1F));
        enc28j60_spi_write(0b00000011);
        ENC28J60_CS_HIGH;
    }
    currentBank = reg.registerObj.bank & 0x3;

    if (currentBank > 0)
    {
        ENC28J60_CS_LOW;
        enc28j60_spi_write(BFS | (ECON1 & 0x1F));
        enc28j60_spi_write(currentBank);
        ENC28J60_CS_HIGH;
    }

}

void enc28j60WriteData(UI16_t memAddress, UI08_t* bf, UI16_t size)
{
    //enc28j60WriteRegisterUint16(EWRPTL, memAddress);

    ENC28J60_CS_LOW;
    enc28j60_spi_write(WBM | 0x1A);
    enc28j60_spi_transferBytes(bf, NULL, size);
    ENC28J60_CS_HIGH;

}

void enc28j60ReadData(UI16_t memAddress, UI08_t* bf, UI16_t size)
{
    enc28j60WriteRegisterUint16(ERDPTL, memAddress);

    ENC28J60_CS_LOW;
    enc28j60_spi_write(RBM | 0x1A);
    enc28j60_spi_transferBytes(NULL, bf, size);
    ENC28J60_CS_HIGH;

}


/******************************************************************************/
/****                                                                      ****/
/****               ENC28J60 Register Command functions                    ****/
/****                                                                      ****/
/******************************************************************************/

UI08_t enc28j60ReadRegisterUint8(UI08_t address)
{
    enc28j60Register_t reg;
    reg.registerByte = address;

    if (reg.registerObj.global == 0
     && reg.registerObj.bank   != currentBank)
        enc28j60SetBank(reg);

    return enc28j60ReadUint8(address);
}

UI08_t enc28j60ReadRegisterUint16(UI08_t address)
{
    enc28j60Register_t reg;
    reg.registerByte = address;

    if (reg.registerObj.global == 0
     && reg.registerObj.bank   != currentBank)
        enc28j60SetBank(reg);

    return enc28j60ReadUint16(address);
}

void enc28j60WriteRegisterUint8(UI08_t address, UI08_t value)
{
    enc28j60Register_t reg;
    reg.registerByte = address;

    if (reg.registerObj.global == 0
     && reg.registerObj.bank   != currentBank)
        enc28j60SetBank(reg);

    enc28j60WriteUint8(address, value);
}


void enc28j60BitSetRegisterUint8(UI08_t address, UI08_t value)
{
    enc28j60Register_t reg;
    reg.registerByte = address;

    if (reg.registerObj.global == 0
     && reg.registerObj.bank   != currentBank)
        enc28j60SetBank(reg);

    enc28j60BitSetUint8(address, value);
}


void enc28j60BitClrRegisterUint8(UI08_t address, UI08_t value)
{
    enc28j60Register_t reg;
    reg.registerByte = address;

    if (reg.registerObj.global == 0
     && reg.registerObj.bank   != currentBank)
        enc28j60SetBank(reg);

    enc28j60BitClrUint8(address, value);
}

void enc28j60WriteRegisterUint16(UI08_t address, UI16_t value)
{
    enc28j60Register_t reg;
    reg.registerByte = address;

    if (reg.registerObj.global == 0
     && reg.registerObj.bank   != currentBank)
        enc28j60SetBank(reg);

    //enc28j60WriteUint16(address, value);
    enc28j60WriteUint8(address+0,  value & 0x00FF    );
    enc28j60WriteUint8(address+1, (value & 0xFF00)>>8);
}

void enc28j60WritePhyRegisterUint16(UI08_t address, UI16_t value)
{
    // write MIREGADR
    enc28j60WriteRegisterUint8(MIREGADR, address);

    // write MIWR
    enc28j60WriteRegisterUint16(MIWRL, value);
}

UI16_t enc28j60ReadPhyRegisterUint16(UI08_t address)
{
    UI16_t temp;
    
    // write MIREGADR
    enc28j60WriteRegisterUint8(MIREGADR, address);

    // Enforce MII to read this value
    enc28j60BitSetRegisterUint8(MICMD, 0x01); // MIRD
    while(enc28j60ReadMacUint8(MISTAT) & 0x1); // is busy?
    enc28j60BitClrRegisterUint8(MICMD, 0x01); // MIRD

    ENC28J60_CS_LOW;
    enc28j60_spi_write(RCR | (MIRDL & 0x1F));
    enc28j60_spi_write(0);
    temp = enc28j60_spi_read();
    
    ENC28J60_CS_HIGH;

    ENC28J60_DelayShort();

    ENC28J60_CS_LOW;
    enc28j60_spi_write(RCR | ((MIRDL+1) & 0x1F));
    enc28j60_spi_write(0);
    temp |= enc28j60_spi_read() << 8;
    ENC28J60_CS_HIGH;
    // write MIWR
    return temp;
}


/******************************************************************************/
/****                                                                      ****/
/****               ENC28J60 User Commands functions                       ****/
/****                                                                      ****/
/******************************************************************************/
void enc28j60Initialize(UI08_t* mac)
{
    ENC28J60_CS_HIGH; // deselect chip

    myMac = mac;
    memset(handlers, NULL, ETHERNET_HANDLERS_COUNT);

    // reset
    enc28j60Reset();
    
    ENC28J60_DelayShort();

    enc28j60ReadPhyRegisterUint16(PHID1);
    enc28j60ReadPhyRegisterUint16(PHID2);
    enc28j60ReadRegisterUint8(EREVID);
    
    enc28j60BitSetRegisterUint8(ECON2, 0b1000000);
    
    // Wait for OST
    while ((enc28j60ReadRegisterUint8(ESTAT) & ESTAT_CLKRDY) == 0);

    enc28j60WritePhyRegisterUint16(PHLCON, 0b0000011110100010); // B: blink fast, A: blink, no stretching

    // RX buffer
    enc28j60WriteRegisterUint16(ERXSTL, ENC28J60_RXBUF_START);
    enc28j60WriteRegisterUint16(ERXNDL, ENC28J60_RXBUF_END);
    enc28j60WriteRegisterUint16(ERXRDPTL, ENC28J60_RXBUF_START);

    // TX Buffer
    enc28j60WriteRegisterUint16(ETXSTL, ENC28J60_TXBUF_START);
    enc28j60WriteRegisterUint16(ETXNDL, ENC28J60_TXBUF_END);

    // Receive filters
    // Default enables Unicast, OR, CRC check and Broadcasts
    //enc28j60WriteRegisterUint8(ERXFCON, 0b10110001); // my target, OR, CRC validation, pattern match
    //enc28j60WriteRegisterUint8(ERXFCON, 0b10110001); // my target, OR, CRC validation, pattern match
    //enc28j60WriteRegisterUint16(EPMM0, 0x303f);  // figure out what is what..
    //enc28j60WriteRegisterUint16(EPMCSL, 0xf7f9); // Broadcast FF:FF:FF:FF:FF:FF
  //---Setup packet filter---
  //This is pinched from Guido Socher AVR enc28j60 driver
  //For broadcast packets we allow only ARP packtets
  //All other packets should be unicast only for our mac (MAADR)
  //The pattern to match on is therefore
  //Type     ETH.DST
  //ARP      BROADCAST
  //06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
  //in binary these poitions are:11 0000 0011 1111
  //This is hex 303F->EPMM0=0x3f,EPMM1=0x30

    // MAC
    enc28j60WriteRegisterUint8(MACON1, 0b00001101);
    //enc28j60WriteRegisterUint8(MACON1, 0);
    enc28j60WriteRegisterUint8(MACON3, 0b11110110); // half duplex, no proprietary header sent/crc'ed  >>> PDPXMD must be clear <<<
    enc28j60WriteRegisterUint8(MACON4, 0b01000000); // todo
    enc28j60WriteRegisterUint16(MAMXFLL, 1518); // max frame length
    enc28j60WriteRegisterUint8(MABBIPG, 0x12); // Delay between back-to-back packets
    enc28j60WriteRegisterUint16(MAIPGL, 0xC12); // Delay between non-back-to-back packets


    enc28j60WriteRegisterUint8(MAADR5, mac[0]);
    enc28j60WriteRegisterUint8(MAADR4, mac[1]);
    enc28j60WriteRegisterUint8(MAADR3, mac[2]);
    enc28j60WriteRegisterUint8(MAADR2, mac[3]);
    enc28j60WriteRegisterUint8(MAADR1, mac[4]);
    enc28j60WriteRegisterUint8(MAADR0, mac[5]);
    
    enc28j60WritePhyRegisterUint16(PHCON2, 0b000000100000000); // hdldis
    enc28j60BitSetRegisterUint8(ECON1, 0b00000100); // hdldis

    // set phy LED status settings:
    //enc28j60WritePhyRegisterUint16(PHLCON, 0b0000010010100000); // B: blink fast, A: link status, no stretching
}

void enc28j60TxFrame(EthernetFrame_t* packet, UI16_t length)
{
    UI08_t controlByte = 0x00;

    // clear status/error flags
    // Errata B7.10
    while ((enc28j60ReadRegisterUint8(ECON1) & 0x8) != 0) // TXRTS
    {
        enc28j60BitSetRegisterUint8(ECON1, 0x08);
        enc28j60BitClrRegisterUint8(ECON1, 0x08);
        uartTxByte('.');
    }
    
    // Set buffer write pointer to start of TXBUFFER
    enc28j60WriteRegisterUint16(EWRPTL, ENC28J60_TXBUF_START);
    
    // Set tx pointer indicating the end
    enc28j60WriteRegisterUint16(ETXNDL, ENC28J60_TXBUF_START + length);

    // Control byte (0x00) + packet
    enc28j60WriteData(ENC28J60_TXBUF_START, &controlByte, 1);
    enc28j60WriteData(ENC28J60_TXBUF_START+1, (UI08_t*)packet, length);

    // Todo: ext interrupts
    enc28j60BitClrRegisterUint8(EIR,    0b00001010); // TX complete, TX error
    enc28j60BitSetRegisterUint8(ECON1, 0b00001000); // TXRTS

    while ((enc28j60ReadRegisterUint8(EIR) & 0x08) == 0);

    // Todo: check interruption flags
    // Todo: report status
    INSIGHT(ENC28J60_TX, length, packet->dstMac[0], packet->dstMac[1], packet->dstMac[2], packet->dstMac[3], packet->dstMac[4], packet->dstMac[5]);
    
    return;
}

void enc28j60TxReplyFrame(EthernetFrame_t* frame, UI16_t length)
{
    UI08_t tmpMac[6];

    // Swap MAC addresses.
    memcpy(tmpMac,          frame->dstMac   , 6);
    memcpy(frame->dstMac,   frame->srcMac   , 6);
    memcpy(frame->srcMac,   tmpMac          , 6);

    // TX frame, MAC & protocol are already done.
    enc28j60TxFrame(frame, sizeof(EthernetFrame_t) + length);
}

void enc28j60RxFrame(UI08_t* packet, UI16_t length)
{
    UI08_t              packetCount         = enc28j60GetPacketCount();
    static UI16_t       dataPtr             = ENC28J60_RXBUF_START;
    UI08_t              packetHeader[6];
    EthernetFrame_t*    frame;
    UI08_t*             data;
    bool_t              dummy = TRUE;

    while(packetCount > 0)
    {
        //memset(packet, 0, length); // 2 status bytes?

        // Move data ptr to start of packet
        enc28j60WriteRegisterUint16(ERDPTL, dataPtr);
        
        // Read the ENC header.
        enc28j60ReadData(dataPtr, packetHeader, 6);

        UI16_t  nextPacketOffset    = packetHeader[0] | (packetHeader[1] << 8);
        UI16_t  packetSize          = packetHeader[2] | (packetHeader[3] << 8);
        UI16_t  receiveStatus       = packetHeader[4] | (packetHeader[5] << 8);

        /*
        if (nextPacketOffset == 0xFFFF && packetSize > 1518)
        {
            dataPtr = ENC28J60_RXBUF_START;
            while (enc28j60GetPacketCount() != 0)
            {
                enc28j60BitSetRegisterUint8(ECON2, 0b01000000); // decrease packet counter
                
            }
            break;
        }
        */
        if (((receiveStatus >> 4) & 0x1) == 0)
        {
            // CRC error

            //if(packetSize>sizeof(UDPPacket_t)+32)
            //    packetSize=sizeof(UDPPacket_t)+32;
            // Receive packet itself
            enc28j60ReadData(dataPtr + 4, packet, packetSize+2-4); // -4 for CRC, +2 for some status bytes or something?

            frame = (EthernetFrame_t*) (packet+2);
            frame-> type = htons(frame->type); // reverse byte order
            data = (UI08_t*) (packet + 2+sizeof(EthernetFrame_t));

            INSIGHT(ENC28J60_RX, packetSize+2-4, frame->srcMac[0], frame->srcMac[1], frame->srcMac[2], frame->srcMac[3], frame->srcMac[4], frame->srcMac[5]);

            dummy = FALSE;
            ipv4HandlePacket(frame, &dummy);

            if (dummy==FALSE)
            {
                arpProcessPacket(frame, &dummy);
            }
            //enc28j60FireTxHandlers(frame);

        }
        
        dataPtr = ENC28J60_RXBUF_START + nextPacketOffset;
        // write to ERXRDPTL
        // don't write an even number though
        if (dataPtr == enc28j60ReadRegisterUint16(ERXSTL))
        {
            enc28j60WriteRegisterUint16(ERXRDPTL, enc28j60ReadRegisterUint16(ERXNDL));
        }
        else
        {
            enc28j60WriteRegisterUint16(ERXRDPTL, dataPtr-1);
        }
        enc28j60BitSetRegisterUint8(ECON2, 0b01000000); // decrease packet counter

        packetCount = enc28j60GetPacketCount();

        // todo: handler?
    }
    
    return;
}

UI08_t enc28j60PacketPending()
{
    return (enc28j60ReadRegisterUint8(EIR) & 0x40) > 0;
}

UI08_t enc28j60GetPacketCount()
{
    return enc28j60ReadRegisterUint8(EPKTCNT);
}

bool_t enc28j60RegisterTxHandler(EthernetPacketHandler_t handler)
{
    UI08_t i = 0;

    while (i < ETHERNET_HANDLERS_COUNT)
    {
        if(handlers[i].used == FALSE)
        {
            handlers[i].used    = TRUE;
            handlers[i].handler = handler;
            return TRUE;
        }

        i++;
    }

    return FALSE;
}

void enc28j60UnregisterTxHandler(EthernetPacketHandler_t handler)
{
    UI08_t i = 0;

    while (i < ETHERNET_HANDLERS_COUNT)
    {
        if(handlers[i].handler == handler)
        {
            handlers[i].used = FALSE;

        }

        i++;
    }
}

void enc28j60FireTxHandlers(EthernetFrame_t* frame)
{
    UI08_t i = 0;
    bool_t done = FALSE;

    while (i < ETHERNET_HANDLERS_COUNT && done == FALSE)
    {
        if(handlers[i].used == TRUE)
        {
            handlers[i].handler (frame, &done);
        }

        i++;
    }
}

void enc28j60Reset(void)
{
    ENC28J60_RST_HIGH;
    ENC28J60_DelayShort();
    ENC28J60_RST_LOW;
    ENC28J60_DelayShort();
    ENC28J60_RST_HIGH;
    ENC28J60_DelayShort();

    ENC28J60_DelayShort();

    ENC28J60_CS_LOW;
    enc28j60_spi_transfer(SC);
    ENC28J60_CS_HIGH;
}