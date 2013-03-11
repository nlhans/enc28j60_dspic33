/* 
 * File:   types.h
 * Author: Hans
 *
 * Created on March 9, 2013, 1:32 PM
 */

#ifndef TYPES_H
#define	TYPES_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>

#include <stdio.h>
#include <string.h>
    
typedef unsigned char UI08_t;
typedef unsigned int UI16_t;
typedef unsigned long UI32_t;

typedef char SI08_t;
typedef int SI16_t;
typedef long SI32_t;

typedef unsigned char bool_t;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* TYPES_H */

