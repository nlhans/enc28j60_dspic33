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

#include "types.h"


UI08_t enc28j60_spi_transfer(UI08_t dataOut);
void enc28j60_spi_transferBytes(UI08_t* bfrIn, UI08_t* bfrOut, UI16_t length);

UI08_t SPI_Write(UI08_t dat);
void SPI_SetDebug(UI08_t on);
void SPI_Init();


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

