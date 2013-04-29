#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include "stddefs.h"

typedef struct circularBuffer_s
{
    UI16_t size;
    UI16_t length;
    UI16_t wrPtr;
    UI16_t rdPtr;
    UI08_t* bf;

}circularBuffer_t;

void circularBufferInit(circularBuffer_t* bf, UI08_t* bfPtr, UI16_t size);
void circularBufferWrite(circularBuffer_t* bf, UI08_t byte);
void circularBufferWriteBytes(circularBuffer_t* bf, UI08_t* arr, UI16_t size);

void circularBufferWriteInt16(circularBuffer_t* bf, UI16_t i);
void circularBufferWriteInt32(circularBuffer_t* bf, UI32_t i);
void circularBufferWriteDouble(circularBuffer_t* bf, double i);
void circularBufferWriteString(circularBuffer_t* bf, char* st);

UI08_t circularBufferRead(circularBuffer_t* bf);
UI08_t circularBufferAvailable(circularBuffer_t* bf);

#endif