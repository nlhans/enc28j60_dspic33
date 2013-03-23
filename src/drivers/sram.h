/* 
 * File:   sram.h
 * Author: Hans
 *
 * Created on March 23, 2013, 7:59 PM
 */

#ifndef SRAM_H
#define	SRAM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "types.h"
#include "sram_defs.h"

typedef struct memMngEntry_s
{
    UI08_t isArray;
    UI16_t size:15;
} memMngEntry_t;

#define cPreProcConcat(a,b) a##b

#define SRAM_MEM_COUNT_VARIABLE(t,n) cPreProcConcat(memMngVar, n)   ,
#define SRAM_MEM_COUNT_ARRAY(t,n,c) cPreProcConcat(memMngArr, n)    ,

#define SRAM_MEM_CREATE(t,n) t* n = (t*) (0xFFFF-__COUNTER__);
#define SRAM_MEM_CREATE_VARIABLE(t,n) SRAM_MEM_CREATE(t,n)
#define SRAM_MEM_CREATE_ARRAY(t,n,c) SRAM_MEM_CREATE(t*,n)

#define SRAM_MEM_TABLE_VARIABLE(t,n)    { sizeof(t), 0},
#define SRAM_MEM_TABLE_ARRAY(t,n,c)     { sizeof(t)*c, 1},

enum
{
    SRAM_TABLE(SRAM_MEM_COUNT_VARIABLE, SRAM_MEM_COUNT_ARRAY)
    MEM_MNG_ENTRIES
};

SRAM_TABLE(SRAM_MEM_CREATE_VARIABLE, SRAM_MEM_CREATE_ARRAY)

const static memMngEntry_t memMngEntries[MEM_MNG_ENTRIES] = {
    SRAM_TABLE(SRAM_MEM_TABLE_VARIABLE, SRAM_MEM_TABLE_ARRAY)
};

volatile UI16_t memMngOffsets[MEM_MNG_ENTRIES];

#ifdef	__cplusplus
}
#endif

#endif	/* SRAM_H */

