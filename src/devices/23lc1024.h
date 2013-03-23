/* 
 * File:   23lc1024.h
 * Author: Hans
 *
 * Created on March 23, 2013, 6:21 PM
 */

#ifndef SRAM_23LC1024_H
#define	SRAM_23LC1024_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "types.h"

#define CS_23LC1024_HIGH        asm volatile("bset PORTA, #10\n");
#define CS_23LC1024_LOW         asm volatile("bclr PORTA, #10\n");

void sram_23lc1024_init();
void sram_23lc1024_read (UI08_t bank, UI16_t address, UI08_t* bf,  UI16_t size);
void sram_23lc1024_write(UI08_t bank, UI16_t address, UI08_t* bf,  UI16_t size);

#ifdef	__cplusplus
}
#endif

#endif	/* 23LC1024_H */

