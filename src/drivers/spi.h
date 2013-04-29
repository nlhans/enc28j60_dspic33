/* 
 * File:   spi.h
 * Author: Hans
 *
 * Created on March 9, 2013, 1:32 PM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "stddefs.h"


UI08_t enc28j60_spi_transfer(UI08_t dataOut);
void enc28j60_spi_transferBytes(UI08_t* bfrIn, UI08_t* bfrOut, UI16_t length);
UI08_t enc28j60_spi_read(void);
void enc28j60_spi_write(UI08_t w);

UI08_t SPI_ReadWrite(UI08_t dat);
void SPI_SetDebug(UI08_t on);
void SPI_Init();

UI08_t SPI_hw_Read();
void SPI_hw_Write(UI08_t d);


void enc28j60_reset_stat();
UI16_t enc28j60_get_statRx();
UI16_t enc28j60_get_statTx();

#define SOFT_SPI

extern UI16_t volatile spiReadByte;
extern UI16_t volatile spiWriteByte;

//extern void spiReadByteAsm(void);
//extern void spiWriteByteAsm(void);
//#define spiReadByteAsm() spiReadByte = SPI_ReadWrite(0);
//#define spiWriteByteAsm() SPI_ReadWrite(spiWriteByte);
#ifdef SOFT_SPI
#define spiReadByteAsm() asm volatile(" CALL spiReadByteS\n");
#define spiWriteByteAsm() asm volatile(" CALL spiWriteByteS\n");
#else
#define spiReadByteAsm() spiReadByte = SPI_hw_Read();
#define spiWriteByteAsm() SPI_hw_Write(spiWriteByte);
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

