/* 
 * File:   sram_defs.h
 * Author: Hans
 *
 * Created on March 23, 2013, 8:00 PM
 */

#ifndef SRAM_DEFS_H
#define	SRAM_DEFS_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct largeStruct_s
{
    UI08_t data[128];
} largeStruct_t;

#define SRAM_TABLE(SRAM_MEM_VARIABLE, SRAM_MEM_ARRAY) \
    SRAM_MEM_VARIABLE   (UI32_t,        myVariable) \
    SRAM_MEM_ARRAY      (largeStruct_t, myVariable2, 128) \

#ifdef	__cplusplus
}
#endif

#endif	/* SRAM_DEFS_H */

