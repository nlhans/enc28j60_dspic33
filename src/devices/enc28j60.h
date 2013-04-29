/* 
 * File:   enc28j60.h
 * Author: Hans
 *
 * Created on March 9, 2013, 1:31 PM
 */

#ifndef ENC28J60_H
#define	ENC28J60_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "stddefs.h"
#include "ethdefs.h"
#include "spi.h"

//#define DEBUG_CONSOLE

typedef union enc28j60Register_u
{
    struct
    {
        UI08_t addr:5;
        UI08_t bank:2;
        UI08_t global:1;
    } registerObj;
    UI08_t registerByte;

} enc28j60Register_t;

#define regBank0 0x00
#define regBank1 0x20
#define regBank2 0x40
#define regBank3 0x60
#define regGlobal 0x80
#define regDefine(addr, bank) (addr | regBank##bank)

typedef enum enc28j60Addressing_e
{
    RCR = 0b00000000, // read control
    RBM = 0b00111010, // read buffer

    WCR = 0b01000000, // write control
    WBM = 0b01111010, // write buffer

    BFS = 0b10000000, // bit set +[data]
    BFC = 0b10100000, // bit clear +[data]

    SC  = 0b11111111 // reset
} enc28j60Addressing_t;

typedef enum enc28j60PhyRegisters_e
{
    PHCON1          = 0x00,
    PHSTAT1         = 0x01,
    PHID1           = 0x02,
    PHID2           = 0x03,
    PHCON2          = 0x10,
    PHSTAT2         = 0x11,
    PHIE            = 0x12,
    PHIR            = 0x13,
    PHLCON          = 0x14,

}   enc28j60PhyRegisters_t;

typedef enum enc28j60Registers_e
{
            // bank 0
    ERDPTL          = regDefine(0x00, 0),
    ERDPTH          = regDefine(0x01, 0),
            
    EWRPTL          = regDefine(0x02, 0),
    EWRPTH          = regDefine(0x03, 0),

    ETXSTL          = regDefine(0x04, 0),
    ETXSTH          = regDefine(0x05, 0),
            
    ETXNDL          = regDefine(0x06, 0),
    ETXNDH          = regDefine(0x07, 0),

    ERXSTL          = regDefine(0x08, 0),
    ERXSTH          = regDefine(0x09, 0),

    ERXNDL          = regDefine(0x0A, 0),
    ERXNDH          = regDefine(0x0B, 0),

    ERXRDPTL        = regDefine(0x0C, 0),
    ERXRDPTH        = regDefine(0x0D, 0),

    ERXWRPTL        = regDefine(0x0E, 0),
    ERXWRPTH        = regDefine(0x0F, 0),

    EDMASTL         = regDefine(0x10, 0),
    EDMASTH         = regDefine(0x11, 0),
            
    EDMANDL         = regDefine(0x12, 0),
    EDMANDH         = regDefine(0x13, 0),

    EDMADSTL        = regDefine(0x14, 0),
    EDMADSTH        = regDefine(0x15, 0),

    EDMACSL         = regDefine(0x16, 0),
    EDMACSH         = regDefine(0x17, 0),

            // bank 1
    EHT0            = regDefine(0x00, 1),
    EHT1            = regDefine(0x01, 1),
    EHT2            = regDefine(0x02, 1),
    EHT3            = regDefine(0x03, 1),
    EHT4            = regDefine(0x04, 1),
    EHT5            = regDefine(0x05, 1),
    EHT6            = regDefine(0x06, 1),
    EHT7            = regDefine(0x07, 1),

    EPMM0           = regDefine(0x08, 1),
    EPMM1           = regDefine(0x09, 1),
    EPMM2           = regDefine(0x0A, 1),
    EPMM3           = regDefine(0x0B, 1),
    EPMM4           = regDefine(0x0C, 1),
    EPMM5           = regDefine(0x0D, 1),
    EPMM6           = regDefine(0x0E, 1),
    EPMM7           = regDefine(0x0F, 1),

    EPMCSL          = regDefine(0x10, 1),
    EPMCSH          = regDefine(0x11, 1),
    EPMOL           = regDefine(0x14, 1),
    EPMOH           = regDefine(0x15, 1),
    EWOLIE          = regDefine(0x16, 1),
    EWOLIR          = regDefine(0x17, 1),
    ERXFCON         = regDefine(0x18, 1),
    EPKTCNT         = regDefine(0x19, 1),

            // bank 2
    MACON1          = regDefine(0x00, 2),
    MACON2          = regDefine(0x01, 2),
    MACON3          = regDefine(0x02, 2),
    MACON4          = regDefine(0x03, 2),
    MABBIPG         = regDefine(0x04, 2),

    MAIPGL          = regDefine(0x06, 2),
    MAIPGH          = regDefine(0x07, 2),
    MACLCON1        = regDefine(0x08, 2),
    MACLCON2        = regDefine(0x09, 2),
    MAMXFLL         = regDefine(0x0A, 2),
    MAMXFLH         = regDefine(0x0B, 2),
    MAPHSUP         = regDefine(0x0D, 2),

    MICON           = regDefine(0x11, 2),
    MICMD           = regDefine(0x12, 2),
            
    MIREGADR        = regDefine(0x14, 2),
            
    MIWRL           = regDefine(0x16, 2),
    MIWRH           = regDefine(0x17, 2),
    MIRDL           = regDefine(0x18, 2),
    MIRDH           = regDefine(0x19, 2),

            // bank 3
    MAADR1          = regDefine(0x00, 3),
    MAADR0          = regDefine(0x01, 3),
    MAADR3          = regDefine(0x02, 3),
    MAADR2          = regDefine(0x03, 3),
    MAADR5          = regDefine(0x04, 3),
    MAADR4          = regDefine(0x05, 3),

    EBSTSD          = regDefine(0x06, 3),
    EBSTCON         = regDefine(0x07, 3),
    EBSTCSL         = regDefine(0x08, 3),
    EBSTCSH         = regDefine(0x09, 3),
    MISTAT          = regDefine(0x0A, 3),

    EREVID          = regDefine(0x12, 3),

    ECOCON          = regDefine(0x15, 3),
    EFLOCON         = regDefine(0x17, 3),
    EPAUSL          = regDefine(0x18, 3),
    EPAUSH          = regDefine(0x19, 3),

            // global:
    EIE             = regDefine(0x1B, 0) | regGlobal,
    EIR             = regDefine(0x1C, 0) | regGlobal,
    ESTAT           = regDefine(0x1D, 0) | regGlobal,
    ECON2           = regDefine(0x1E, 0) | regGlobal,
    ECON1           = regDefine(0x1F, 0) | regGlobal,
            
}enc28j60Registers_t;

// ESTAT mask definitions
#define ESTAT_CLKRDY            0x01

#define ENC28J60_CS_HIGH        asm volatile("bset PORTC, #6\n");
#define ENC28J60_CS_LOW         asm volatile("bclr PORTC, #6\n");

//#define ENC28J60_CS_LOW         PORTC &= ~(1<<6)
//#define ENC28J60_CS_HIGH        PORTC |= 1<<6

#define ENC28J60_RST_LOW        PORTB &= ~(1<<9)
#define ENC28J60_RST_HIGH       PORTB |= 1<<9

#define ENC28J60_DelayShort()    do {} while(0);// do { volatile UI08_t i = 0; while (i++ < 25); } while(0);

#define ENC28J60_RXBUF_START    0
#define ENC28J60_RXBUF_END      4095
#define ENC28J60_TXBUF_START    4096
#define ENC28J60_TXBUF_END      8191

void enc28j60Initialize(UI08_t* mac);
void enc28j60Reset(void);

UI08_t enc28j60PacketPending();
UI08_t enc28j60GetPacketCount();
UI08_t enc28j60ReadRegisterUint8(UI08_t address);

UI16_t enc28j60ReadPhyRegisterUint16(UI08_t address);

void enc28j60RxFrame(UI08_t* packet, UI16_t length);
void enc28j60TxFrame(EthernetFrame_t* packet, UI16_t length);
void enc28j60TxReplyFrame(EthernetFrame_t* frame, UI16_t length);

bool_t enc28j60RegisterTxHandler(EthernetPacketHandler_t handler);
void enc28j60UnregisterTxHandler(EthernetPacketHandler_t handler);

#ifdef DEBUG_CONSOLE
extern char debugBuffer[256];
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* ENC28J60_H */

