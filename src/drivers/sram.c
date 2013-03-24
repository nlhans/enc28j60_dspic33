
#include "sram_defs.h"
#include "uart.h"
#include "enc28j60.h"
#define HEAP_SIZE 128

UI08_t heap[HEAP_SIZE];

#define HEAP_END ( ((UI16_t)heap) + sizeof(heap) )

struct sramHeapHeader_s
{
    struct sramHeapHeader_s* ptr;
    UI16_t size;
};
typedef struct sramHeapHeader_s sramHeapHeader_t;

typedef struct sram_runtime_info_s
{
    UI32_t extPtr;
    sramHeapHeader_t* heapPtr;
} sram_runtime_info_t;

SRAM_TABLE(SRAM_MEM_CREATE_VARIABLE, SRAM_MEM_CREATE_ARRAY)

sram_runtime_info_t memMngOffsets[MEM_MNG_ENTRIES];
const memMngEntry_t memMngEntries[MEM_MNG_ENTRIES] = {
    SRAM_TABLE(SRAM_MEM_TABLE_VARIABLE, SRAM_MEM_TABLE_ARRAY)
};

bool_t sram_canAlloc(UI16_t size);
bool_t sram_tryAlloc(UI16_t id, UI16_t size);
UI16_t sram_tryFree(UI16_t size, UI16_t lastBlock);

sramHeapHeader_t* sramHeap;

UI08_t* sram_fault(UI16_t addr)
{
    if (addr >= 0xB000) return NULL;

    UI16_t seqHeapBlock = 0xFFFF;
    UI16_t id = (0xAFFF-addr) & 0x3FF; // 1024 entries
    bool_t isVariable = (addr & 0x0400) != 0;
    memMngEntry_t* entry;
#ifdef DEBUG_CONSOLE
    sprintf(debugBuffer, "Accessing %04X, ID %02X, isVar %d\r\n", addr, id, isVariable);
    uartTxString(debugBuffer);
#endif

    // outside table
    if (id > MEM_MNG_ENTRIES) return NULL;

    entry = (memMngEntry_t*) &(memMngEntries[id]);
    if (isVariable == FALSE)
    {
        // Array access
        //
    }
    else
    {
#ifdef DEBUG_CONSOLE
    sprintf(debugBuffer, "Entry size %04X, isArray %d, \r\n", entry->size, entry->isArray);
    uartTxString(debugBuffer);
#endif
        // Access to variable itself
        if(entry->isArray)
        {
            return (UI08_t*) 0xFB00;
        }

        // Is it already on our heap?
        if(memMngOffsets[id].heapPtr != 0)
            return memMngOffsets[id].heapPtr;

        if (sram_canAlloc(entry->size) == FALSE)
        {
            uartTxString("Heap and payload will never fit\r\n");
            return NULL;
        }

        uartTxString("Attempting to allocate memory..\r\n");

        if (sram_tryAlloc(id, entry->size) == FALSE)
        {
            uartTxString("Could not allocate memory, need to free\r\n");

            do
            {
                // Free up
                seqHeapBlock = sram_tryFree(entry->size, seqHeapBlock);
            }
            while (sram_tryAlloc(id, entry->size) == FALSE);
            
        }
        // adjust variable
        * ((UI16_t**)entry->id) = memMngOffsets[id].heapPtr;
        return memMngOffsets[id].heapPtr;
    }
}

UI16_t sram_findBestFit(UI16_t size)
{
    bool_t foundFit = FALSE;
    UI16_t sizeToEnd;
    sramHeapHeader_t* heapPtr;
    sramHeapHeader_t* bestFit = (sramHeapHeader_t*)heap;

    for (heapPtr = sramHeap; heapPtr->ptr != NULL; heapPtr = heapPtr->ptr)
    {
        if (heapPtr->size >= size && heapPtr->size < bestFit->size)
        {
            foundFit = TRUE;
            bestFit = heapPtr;
        }
    }

    sizeToEnd = HEAP_END - ((UI16_t)heapPtr);

    if (sizeToEnd >= size && bestFit->size > sizeToEnd)
    {
        // Last block to end is best matching fit.
        foundFit = TRUE;
        bestFit = heapPtr;
    }

    if (foundFit)
        return heapPtr;
    else
        return 0xFFFF;
}

UI16_t sram_countFree(sramHeapHeader_t* heapPtr)
{
    return ((UI08_t)heapPtr->ptr) - ((UI08_t)heapPtr) - heapPtr->size - sizeof(sramHeapHeader_t);
}

UI16_t sram_tryFree(UI16_t size, UI16_t lastBlock)
{
    sramHeapHeader_t* heapPtr;
    sramHeapHeader_t* newHeapPtr;
    UI16_t free;
    UI16_t fit;

    heapPtr = sram_findBestFit(size);

    if (heapPtr == 0xFFFF)
    {
        // we just work with the first occurance then..
        heapPtr = sramHeap;
    }
    do
    {
        sram_free(newHeapPtr);
        newHeapPtr = heapPtr->ptr;

        free = sram_countFree(heapPtr);
    }
    while(free < size);

    return heapPtr;
    
}

void sram_free(UI16_t ptr)
{
    sramHeapHeader_t* header = ptr - sizeof(sramHeapHeader_t);

}

void sram_init()
{
    sramHeap=heap;

    memset(heap, 0, sizeof(heap));
}

bool_t sram_canAlloc(UI16_t size)
{
    //
    if (sramHeap == NULL) return FALSE;
    if (size > sizeof(heap)-sizeof(sramHeapHeader_t)) return FALSE;

    return TRUE;
}
bool_t sram_tryAlloc(UI16_t id, UI16_t size)
{
    sramHeapHeader_t* heapPtr;
    sramHeapHeader_t* newHeapPtr;
    UI16_t free;

    for (heapPtr = sramHeap; heapPtr->ptr != NULL; heapPtr = heapPtr->ptr)
    {
        // next pos - my pos - my size - headerSize
        // e.g. 2048 - 1024 - 512 -8= 508 free.
        free = ((UI08_t)heapPtr->ptr) - ((UI08_t)heapPtr) - heapPtr->size - sizeof(sramHeapHeader_t);
#ifdef DEBUG_CONSOLE
        sprintf(debugBuffer, "@ %04X, free %04X, next %04X\r\n", heapPtr, free, heapPtr->ptr);
        uartTxString(debugBuffer);
#endif
        if (free > sizeof(heap)) return FALSE;
        if(free >= size)
        {
            newHeapPtr = (sramHeapHeader_t*) (((UI08_t)heapPtr) + heapPtr->size + sizeof(sramHeapHeader_t));
            newHeapPtr->ptr = heapPtr->ptr;
            newHeapPtr->size = size;
            heapPtr->ptr = newHeapPtr;

            memMngOffsets[id].heapPtr = newHeapPtr + sizeof(sramHeapHeader_t);

#ifdef DEBUG_CONSOLE
    sprintf(debugBuffer, "Allocating memory A @ %04X for %02X bytes\r\n", newHeapPtr, size);
    uartTxString(debugBuffer);
#endif
            return TRUE;
        }
    }

    if (heapPtr->ptr == NULL &&
            sramHeap + sizeof(heap) - heapPtr > size)
    {
        // There is still enough left!
        newHeapPtr = (sramHeapHeader_t*) (((UI08_t*)heapPtr) + size + sizeof(sramHeapHeader_t));
        newHeapPtr->ptr = 0;
        heapPtr->ptr = newHeapPtr;
        heapPtr->size = size;

        memMngOffsets[id].heapPtr = heapPtr + sizeof(sramHeapHeader_t);

#ifdef DEBUG_CONSOLE
    sprintf(debugBuffer, "Allocating memory B @ %04X for %02X bytes, total %02X, next %04X\r\n", heapPtr, size, (sizeof(sramHeapHeader_t)+size), newHeapPtr);
    uartTxString(debugBuffer);
#endif
        return TRUE;
    }

    uartTxString("Failed to allocate\r\n");
    return FALSE;
}
