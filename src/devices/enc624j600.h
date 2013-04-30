#ifndef ENC624J600_H
#define ENC624J600_H

#include "stddefs.h"
#include "ethdefs.h"
#include "spi.h"

// SPI register definitions
#define regBank0 0x00
#define regBank1 0x20
#define regBank2 0x40
#define regBank3 0x60

#define regDefine(addr, bank) (addr | regBank##bank)
#define regGlobal 0x80

typedef enum enc624j600RegistersMap_e
{
    /********************** BANK 0 *********************/
    ETXST                   = regDefine(0x00, 0),
    ETXLEN                  = regDefine(0x02, 0),
    ERXST                   = regDefine(0x04, 0),
    ERXTAIL                 = regDefine(0x06, 0),
    ERXHEAD                 = regDefine(0x08, 0),
    EDMAST                  = regDefine(0x0A, 0),
    EDMALEN                 = regDefine(0x0C, 0),
    EDMADST                 = regDefine(0x0E, 0),
    EDMACS                  = regDefine(0x10, 0),
    ETXSTAT                 = regDefine(0x12, 0),
    ETXWIRE                 = regDefine(0x14, 0),

    /********************** BANK 1 *********************/
    ETH1                    = regDefine(0x00, 1),
    ETH2                    = regDefine(0x02, 1),
    ETH3                    = regDefine(0x04, 1),
    ETH4                    = regDefine(0x06, 1),
    EPMM1                   = regDefine(0x08, 1),
    EPMM2                   = regDefine(0x0A, 1),
    EPMM3                   = regDefine(0x0C, 1),
    EPMM4                   = regDefine(0x0E, 1),
    EPMCS                   = regDefine(0x10, 1),
    EPMO                    = regDefine(0x12, 1),
    ERXFCON                 = regDefine(0x14, 1),

    /********************** BANK 2 *********************/
    MACON1                  = regDefine(0x00, 2),
    MACON2                  = regDefine(0x02, 2),
    MABBIPG                 = regDefine(0x04, 2),
    MAIPG                   = regDefine(0x06, 2),
    MACLCON                 = regDefine(0x08, 2),
    MAMXFL                  = regDefine(0x0A, 2),
    MICMD                   = regDefine(0x12, 2),
    MIREGADR                = regDefine(0x14, 2),

    /********************** BANK 3 *********************/
    MADDR3                  = regDefine(0x00, 3),
    MADDR2                  = regDefine(0x02, 3),
    MADDR1                  = regDefine(0x04, 3),
    MIWR                    = regDefine(0x06, 3),
    MIRD                    = regDefine(0x08, 3),
    MISTAT                  = regDefine(0x0A, 3),
    EPAUS                   = regDefine(0x0C, 3),
    ECON2                   = regDefine(0x0E, 3),
    ERXWM                   = regDefine(0x10, 3),
    EIE                     = regDefine(0x12, 3),
    EIDLED                  = regDefine(0x14, 3),

    /**************** GENERAL BANK ********************/
    EUDAST                  = regDefine(0x16, 0) | regGlobal,
    EUDAND                  = regDefine(0x18, 0) | regGlobal,
    ESTAT                   = regDefine(0x1A, 0) | regGlobal,
    EIR                     = regDefine(0x1C, 0) | regGlobal,
    ECON1                   = regDefine(0x1E, 0) | regGlobal,
} enc624j600RegistersMap_t;

typedef enum enc624j600CommandSet_e
{
    BANK_SEL  = 0b11000000, // select ban
    SETETHRST = 0b11000110, // reset ethernet controller?
    FCDISABLE = 0b11100000, // flow-control
    FCSINGLE = 0b11100010,
    FCMULTIPLE = 0b11100100,
    FCCLEAR = 0b11100110,

    SETPKTDEC = 0b11001100, //decrement pkt counter
    DMASTOP = 0b11010010, // dma control
    DMACKSUM = 0b11011000,
    DMACKSUMS = 0b11011010,
    DMACOPY = 0b11011100,
    DMACOPYS = 0b1101110,

    SETTXRTS = 0b11010100, // enable tx packet
    ENABLERX = 0b11101000, // enable rx reception
    DISABLERX = 0b11101010, // disable rx reception

    SETEIE = 0b11101100, //set/clr interrupts
    CLREIE = 0b11101110,

    RBSEL = 0b11001000, // get bank selection

    //GP = General Purpose
    //Rx = Receive buffer pointer
    //UDA = User-defined area
    WGPRDPT = 0b01100000, // Write/read E-GP-Read-Pointer
    RGPRDPT = 0b01100010,

    WRXRDPT = 0b01100100, // Write/read E-Rx-Read-Pointer
    RRXRDPT = 0b01100110,

    WUDARDPT = 0b01101000, // Write/read E-UDA-Read-Pointer
    RUDARDPT = 0b01101010,

    WGPWRPT = 0b01101100, // Write/read E-GP-Write-Pointer
    RGPWRPT = 0b01101110,

    WRXWRPT = 0b01110000, // Write/read E-Rx-Write-Pointer
    RRXWRPT = 0b01110010,

    WUDAWRPT  = 0b01110100, // Write/read E-UDA-Write-Pointer
    RUDAWRPT  = 0b01110110,

    RCR = 0b00000000, //000a aaaa
    WCR = 0b01000000, //010a aaaa

    RCRU = 0b00100000,
    WCRU = 0b00100010,

    BFS = 0b10000000, // 100a aaaa
    BFC = 0b10100000, // 101a aaaa
    BFSU = 0b00100100,
    BFCU = 0b00100110,

    // Read/write data

    //GP = General Purpose
    //Rx = Receive buffer pointer
    //UDA = User-defined area
    RGPDATA = 0b00101000, //  data
    WGPDATA = 0b00101010,

    RRXDATA = 0b00101100, // rx data
    WRXDATA = 0b00101110,

    RUDADATA = 0b00110000, // uda data
    WUDADATA = 0b00110010,

} enc624j600CommandSet_t;

#define ENC624J600_RXBUF_START 0x5800

void enc624j600_delay(UI08_t d);
void enc624j600SpiCommand(UI08_t cmd, UI08_t* bf, UI08_t size);
void enc624j600_setBank(UI08_t bank);
UI08_t enc624j600_spi_write(UI08_t d);
void enc624j600Initialize();

UI08_t enc624j600GetPacketCount();
bool_t enc624j600PacketPending();
bool_t enc624j600GetLinkStatus();
bool_t enc624j600GetTxBusyStatus();

void enc624j600RxFrame(UI08_t* packet, UI16_t length);
void enc624j600TxFrame(EthernetFrame_t* packet, UI16_t length);
void enc624j600TxReplyFrame(EthernetFrame_t* frame, UI16_t length);

UI08_t enc624j600SpiReadRegister8(UI08_t addr);
UI16_t enc624j600SpiReadRegister16(UI08_t addr);

void enc624j600SpiWriteRegister8(UI08_t addr, UI08_t value);
void enc624j600SpiWriteRegister16(UI08_t addr, UI16_t value);

void enc624j600SpiBSetRegister8(UI08_t addr, UI08_t value);
void enc624j600SpiBSetRegister16(UI08_t addr, UI16_t value);

void enc624j600SpiReadData(UI16_t ptr, UI08_t* bf, UI16_t size);
void enc624j600SpiReadRxData(UI16_t ptr, UI08_t* bf, UI16_t size);
void enc624j600SpiWriteData(UI16_t ptr, UI08_t* bf, UI16_t size);

/******************* SPI ********************/
#define L 0x55
#define H 0xAA

#define enc624j600Command(CMD, BF, SIZE) enc624j600SpiCommand(CMD, BF, SIZE);
#define enc624j600ReadGpData(ADDR, BF, SIZE) enc624j600SpiReadData(ADDR, BF, SIZE);
#define enc624j600ReadRxData(ADDR, BF, SIZE) enc624j600SpiReadRxData(ADDR, BF, SIZE);
#define enc624j600WriteGpData(ADDR, BF, SIZE) enc624j600SpiWriteData(ADDR, BF, SIZE);

#define enc624j600ReadRegister8(ADDR, TYPE) ((TYPE == L) ? enc624j600SpiReadRegister8(ADDR) : enc624j600SpiReadRegister8(ADDR+1))
#define enc624j600ReadRegister16(ADDR) enc624j600SpiReadRegister16(ADDR)

#define enc624j600WriteRegister8(ADDR, TYPE, V) ((TYPE == L) ? enc624j600SpiWriteRegister8(ADDR, V) : enc624j600SpiWriteRegister8(ADDR+1, V))
#define enc624j600WriteRegister16(ADDR, V) enc624j600SpiWriteRegister16(ADDR, V)

#define enc624j600BitSetRegister8(ADDR, TYPE, MASK) ((TYPE == L) ? enc624j600SpiBSetRegister8(ADDR, MASK) : enc624j600SpiBSetRegister8(ADDR+1, MASK))


#endif
