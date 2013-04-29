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

UI08_t enc624j600_spi_write(UI08_t d);
void enc624j600Initialize();

bool_t enc624j600PacketPending();
bool_t enc624j600GetLinkStatus();

void enc624j600RxFrame(UI08_t* packet, UI16_t length);
void enc624j600TxFrame(EthernetFrame_t* packet, UI16_t length);
void enc624j600TxReplyFrame(EthernetFrame_t* frame, UI16_t length);

UI08_t enc624j600SpiReadRegister8(UI08_t addr);
UI16_t enc624j600SpiReadRegister16(UI08_t addr);

void enc624j600SpiWriteRegister8(UI08_t addr, UI08_t value);
void enc624j600SpiWriteRegister16(UI08_t addr, UI16_t value);

void enc624j600SpiBSetRegister8(UI08_t addr, UI08_t value);
void enc624j600SpiBSetRegister16(UI08_t addr, UI16_t value);

/******************* SPI ********************/
#define L 0x55
#define H 0xAA

#define enc624j600ReadRegister8(ADDR, TYPE) ((TYPE == L) ? enc624j600SpiReadRegister8(ADDR) : enc624j600SpiReadRegister8(ADDR+1))
#define enc624j600ReadRegister16(ADDR) enc624j600SpiReadRegister16(ADDR)

#define enc624j600WriteRegister8(ADDR, TYPE, V) ((TYPE == L) ? enc624j600SpiWriteRegister8(ADDR, V) : enc624j600SpiWriteRegister8(ADDR+1, V))
#define enc624j600WriteRegister16(ADDR, V) enc624j600SpiWriteRegister16(ADDR, V)

#define enc624j600BitSetRegister8(ADDR, TYPE, MASK) ((TYPE == L) ? enc624j600SpiBSetRegister8(ADDR, MASK) : enc624j600SpiBSetRegister8(ADDR+1, MASK))

#endif
