#include "circularBuffer.h"

void circularBufferInit(circularBuffer_t* bf, UI08_t* bfPtr, UI16_t size)
{
    bf->bf = bfPtr;
    bf->size = size;
    bf->rdPtr = 0;
    bf->wrPtr = 0;
    bf->length = 0;

}
void circularBufferWrite(circularBuffer_t* bf, UI08_t byte)
{
    if(bf->length == bf->size) return;
    bf->bf[bf->wrPtr] = byte;
    bf->length++;
    bf->wrPtr ++;
    if (bf->wrPtr == bf->size)
        bf->wrPtr = 0;
}
void circularBufferWriteBytes(circularBuffer_t* bf, UI08_t* arr, UI16_t size)
{
    UI16_t i = 0;
    while (i < size)
    {
        circularBufferWrite(bf, *(arr++));
        i++;
    }
}

void circularBufferWriteInt16(circularBuffer_t* bf, UI16_t i)
{
    UI08_t* arr = (UI08_t*)(&i);
    circularBufferWriteBytes(bf, arr, sizeof(UI16_t));
}

void circularBufferWriteInt32(circularBuffer_t* bf, UI32_t i)
{
    UI08_t* arr = (UI08_t*)(&i);
    circularBufferWriteBytes(bf, arr, sizeof(UI32_t));
}

void circularBufferWriteDouble(circularBuffer_t* bf, double i)
{
    UI08_t* arr = (UI08_t*)(&i);
    circularBufferWriteBytes(bf, arr, sizeof(double));
}

void circularBufferWriteString(circularBuffer_t* bf, char* st)
{
    UI16_t length = 0;
    while (st[length] != '\0')
    {
        length++;
    }
    circularBufferWriteInt16(bf, length);
    circularBufferWriteBytes(bf, (UI08_t*)st, length);
}

UI08_t circularBufferRead(circularBuffer_t* bf)
{
    UI08_t b;
    
    if (bf->length == 0) return 0;
    
    b = bf->bf[bf->rdPtr];
    bf->length--;
    bf->rdPtr++;
    if (bf->rdPtr == bf->size)
        bf->rdPtr = 0;
    return b;
}
UI08_t circularBufferAvailable(circularBuffer_t* bf)
{
    return ((bf->length > 0) ? 1:0);
}