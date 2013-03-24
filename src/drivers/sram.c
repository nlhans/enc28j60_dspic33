
#include "sram_defs.h"
#include "uart.h"
#include "enc28j60.h"
#define HEAP_SIZE 128

UI08_t heap[HEAP_SIZE];

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

bool_t sram_tryAlloc(UI16_t id, UI16_t size);

sramHeapHeader_t* sramHeap;

UI08_t* sram_fault(UI16_t addr)
{
    if (addr >= 0xB000) return NULL;
    
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

        uartTxString("Attempting to allocate memory..\r\n");

        if (sram_tryAlloc(id, entry->size) == TRUE)
        {
            //
            return memMngOffsets[id].heapPtr;
        }
        else
        {
            // We need to free up space
            // TODO;
            return &heap;
        }
    }
}

void sram_init()
{
    sramHeap=heap;

    memset(heap, 0, sizeof(heap));
}

bool_t sram_tryAlloc(UI16_t id, UI16_t size)
{
    sramHeapHeader_t* heapPtr;
    sramHeapHeader_t* newHeapPtr;
    UI16_t free;
    //
    if (sramHeap == NULL) return NULL;

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
