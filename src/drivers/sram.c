
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
void sram_free(UI16_t ptr);
sramHeapHeader_t* sram_findBestFit(UI16_t size);

sramHeapHeader_t* sramHeap;

UI08_t* sram_fault(UI16_t addr)
{
    if (addr >= 0xB000) return NULL;

    UI16_t seqHeapBlock = 0xFFFF;
    UI16_t id = (0xAFFF-addr) & 0x3FF; // 1024 entries
    bool_t isVariable = (addr & 0x0400) != 0;
    memMngEntry_t* entry;

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
        // Access to variable itself
        if(entry->isArray)
        {
            return (UI08_t*) 0xFB00;
        }

        // Is it already on our heap?
        if(memMngOffsets[id].heapPtr != 0)
        {
            return (UI08_t*) memMngOffsets[id].heapPtr;
        }

        if (sram_canAlloc(entry->size) == FALSE)
        {
            //uartTxString("Heap and payload will never fit\r\n");
            return NULL;
        }

        //uartTxString("Attempting to allocate memory..\r\n");

        if (sram_tryAlloc(id, entry->size) == FALSE)
        {
            //uartTxString("Could not allocate memory, need to free\r\n");

            do
            {
                // Free up
                seqHeapBlock = sram_tryFree(entry->size, seqHeapBlock);
            }
            while (sram_tryAlloc(id, entry->size) == FALSE);
            
        }
        // adjust variable
        * ((UI16_t**)entry->id) = memMngOffsets[id].heapPtr;
        return (UI08_t*) memMngOffsets[id].heapPtr;
    }
    return NULL;
}

sramHeapHeader_t* sram_findBestFit(UI16_t size)
{
    UI16_t free;
    bool_t foundFit = FALSE;
    UI16_t sizeToEnd;
    sramHeapHeader_t* heapPtr;
    sramHeapHeader_t* bestFit = (sramHeapHeader_t*)heap;

    for (heapPtr = sramHeap; heapPtr->ptr != NULL; heapPtr = heapPtr->ptr)
    {
        free = ((UI08_t)heapPtr->ptr) - ((UI08_t)heapPtr) - heapPtr->size - sizeof(sramHeapHeader_t);
        if(free > sizeof(heap)) break;
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
        return NULL;
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

    if (heapPtr == NULL)
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
    //sramHeapHeader_t* header = (sramHeapHeader_t*) (ptr - sizeof(sramHeapHeader_t));
    
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

        if (free > sizeof(heap)) return FALSE;
        if(free >= size)
        {
            newHeapPtr = (sramHeapHeader_t*) (((UI08_t)heapPtr) + heapPtr->size + sizeof(sramHeapHeader_t));
            newHeapPtr->ptr = heapPtr->ptr;
            newHeapPtr->size = size;
            heapPtr->ptr = newHeapPtr;

            memMngOffsets[id].heapPtr = newHeapPtr + sizeof(sramHeapHeader_t);

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

        return TRUE;
    }

    //uartTxString("Failed to allocate\r\n");
    return FALSE;
}

/*


UI16_t errStkLoc = 0;
UI16_t errPrgLoc = 0;

extern UI16_t getErrLoc(void);
extern UI32_t MemReadLatch(UI08_t psvPag, UI16_t addr);
typedef enum instPtrMode
{
    PtrMode_DirectAccess, // W
    PtrMode_Ptr, // [W]
    PtrMode_PtrPP, // [W++]
    PtrMode_PtrMM, // [W--]
    PtrMode_MMPtr, // [--W]
    PtrMode_PPPtr, // [++W]
    PtrMode_PtrW, // [W+W_secondary] (offset)
} instPtrMode_t;

bool_t wasWrite = FALSE;

UI32_t instData;
UI16_t normalRegAddr;
UI08_t i;

UI08_t id4;
UI08_t id5;
UI08_t id6;
UI08_t id6;
UI08_t id7;
UI08_t id8;
UI16_t id9;
UI16_t id10;
UI16_t id11;
UI16_t id12;
UI08_t srcAddr;
instPtrMode_t srcAddrMode;
UI08_t dstAddr;
instPtrMode_t dstAddrMode;

UI16_t resolvedSrcAddr;
UI16_t resolvedDstAddr;


//void __attribute__((interrupt, no_auto_psv)) _AddressError(void)
 * {
        errStkLoc   = (UI16_t)getErrLoc();
        if(errStkLoc > 0x4000) //16kiB of RAM
            asm volatile("reset");
        errPrgLoc   = * ((UI16_t*)errStkLoc) ;

        instData = MemReadLatch(0, errPrgLoc - 2);

        // parse the instruction, so that we know *what* register was used
        // to read/write this.
        id12 = instData >> 12;
        id11 = id12 >> 1;
        id10 = id11 >> 1;
        id9 = id10 >> 1;
        id8 = id9 >> 1;
        id7 = id8 >> 1;
        id6 = id7 >> 1;
        id5 = id6 >> 1;
        id4 = id5 >> 1;
        //id5 = instData >> 19;

        srcAddrMode = (instPtrMode_t) ( (instData >> 4)  & 0x7 );
        dstAddrMode = (instPtrMode_t) ( (instData >> 11) & 0x7 );
        srcAddr  = instData & 0xF;
        dstAddr = (instData >> 7) & 0xF;

        if (id12 == 0b101111100000) // mov.d
        {
            dstAddrMode = PtrMode_DirectAccess;
            uartTxString("MOV.D [W], W\r\n");
        }
        if (id10 == 0b1011111010) // mov.d
        {
            srcAddrMode = PtrMode_DirectAccess;
            uartTxString("MOV.D W, [W]\r\n");
        }
        if (id5 == 0b10011) // MOV W, [W+slit10]
        {
            srcAddrMode = PtrMode_DirectAccess;
            dstAddrMode = PtrMode_PtrW;
            uartTxString("MOV W, [W+0xSS]\r\n");
        }
        if (id9 == 0b111010110) // CLR W
        {
            srcAddr = 0;
            srcAddrMode = PtrMode_DirectAccess;
            uartTxString("CLR [W]\r\n");
        }
        if(id5 == 0b01010) // SUB Wb, #lit5, [Wd]
        {
            // 0101 0www wBqq qddd d11k kkkk
            // w=base
            // B=word/byte
            // q=dst adr mode
            // d=dst
            // k=lit5
            srcAddrMode = PtrMode_DirectAccess;
            srcAddr = (instData >> 14) & 0xF;
            uartTxString("SUB Wb, #lit5, [Wd]\r\n");
        }
        if (id5 == 0b01111)
        {
            // 0111 1www wBhh hddd dggg ssss
            // 0111 1000 0100 1000 1000 0000
            // id
            // w=offset work register, if required
            // b=byte/word
            // h=dst addr mode
            // d=dst addr
            // g=src addr mode
            // s=src addr
            uartTxString("MOV W, [W+]\r\n");

        }
        sprintf(debugBuffer, "INST SRC %d, DST %d || (MOV {%d}, {%d})\r\n", srcAddr, dstAddr, srcAddrMode, dstAddrMode);
        uartTxString(debugBuffer);


        // Is it write?
        if (srcAddrMode == PtrMode_DirectAccess && dstAddrMode != PtrMode_DirectAccess)
        {
            wasWrite=TRUE;
            // We are writing to a pointer, which failed to access.
            // Put back stack so after this , it's reexecuted.
            *((UI16_t*)errStkLoc) = errPrgLoc-2;
        }
        else
        {
            wasWrite=FALSE;
            // we don't have to adjust the ptr, because reads are reexecuted
        }

        // resolve src
        if (1 || srcAddrMode != PtrMode_DirectAccess)
        {
            // 7-14 is beyond normal XC16 stack
            if (srcAddr < 7)
            {
                // prgStk + 0x06 -> W0?
                // e.g. prgStk = 0x1010
                // W0 = 0x1016
                // W1 = 0x1018 etc
                //0-7, is in stack, 8-15 are not pushed
                resolvedSrcAddr = *((UI16_t*)(errStkLoc + 0x06 + srcAddr*0x2));
            }
            else if (srcAddr == 7)
            {
                asm volatile("mov W7, _resolvedSrcAddr");
            }
            else if (srcAddr == 8)
            {
                asm volatile("mov W8, _resolvedSrcAddr");
            }
            else if (srcAddr == 9)
            {
                asm volatile("mov W9, _resolvedSrcAddr");
            }
            else if (srcAddr == 10)
            {
                asm volatile("mov W10, _resolvedSrcAddr");
            }
            else if (srcAddr == 11)
            {
                asm volatile("mov W11, _resolvedSrcAddr");
            }
            else if (srcAddr == 12)
            {
                asm volatile("mov W12, _resolvedSrcAddr");
            }
            else if (srcAddr == 13)
            {
                asm volatile("mov W13, _resolvedSrcAddr");
            }
            else if (srcAddr == 14)
            {
                asm volatile("mov W14, _resolvedSrcAddr");
            }

        }

        // resolve src
        if (dstAddrMode != PtrMode_DirectAccess)
        {
            // 7-14 is beyond normal XC16 stack
            if (dstAddr < 7)
            {
                // prgStk + 0x06 -> W0?
                // e.g. prgStk = 0x1010
                // W0 = 0x1016
                // W1 = 0x1018 etc
                //0-7, is in stack, 8-15 are not pushed
                resolvedDstAddr = *((UI16_t*)(errStkLoc + 0x06 + dstAddr*0x2));
            }
            else if (dstAddr == 7)
            {
                asm volatile("mov W7, _resolvedDstAddr");
            }
            else if (dstAddr == 8)
            {
                asm volatile("mov W8, _resolvedDstAddr");
            }
            else if (dstAddr == 9)
            {
                asm volatile("mov W9, _resolvedDstAddr");
            }
            else if (dstAddr == 10)
            {
                asm volatile("mov W10, _resolvedDstAddr");
            }
            else if (dstAddr == 11)
            {
                asm volatile("mov W11, _resolvedDstAddr");
            }
            else if (dstAddr == 12)
            {
                asm volatile("mov W12, _resolvedDstAddr");
            }
            else if (dstAddr == 13)
            {
                asm volatile("mov W13, _resolvedDstAddr");
            }
            else if (dstAddr == 14)
            {
                asm volatile("mov W14, _resolvedDstAddr");
            }

        }

        if (wasWrite)
            normalRegAddr = sram_fault(resolvedDstAddr);
        else
            normalRegAddr = sram_fault(resolvedSrcAddr);

        sprintf(debugBuffer, "%lu, WR %d, SRC %04X, DST %04X\r\n", instData, wasWrite, resolvedSrcAddr, resolvedDstAddr);
        uartTxString(debugBuffer);

        // read/modify etc.
        if (wasWrite || 1)
        {
            // 7-14 is beyond normal XC16 stack
            if (dstAddr < 7)
            {
                // prgStk + 0x06 -> W0?
                // prgLoc = 0x1010
                // W0 = 0x1016
                // W1 = 0x1018 etc
                //0-7, is in stack
                *((UI16_t*)(errStkLoc + 0x06 + dstAddr*0x2)) = normalRegAddr;
            }
            else if (dstAddr == 7)
            {
                asm volatile("mov _normalRegAddr, W7");
            }
            else if (dstAddr == 8)
            {
                asm volatile("mov _normalRegAddr, W8");
            }
            else if (dstAddr == 9)
            {
                asm volatile("mov _normalRegAddr, W9");
            }
            else if (dstAddr == 10)
            {
                asm volatile("mov _normalRegAddr, W10");
            }
            else if (dstAddr == 11)
            {
                asm volatile("mov _normalRegAddr, W11");
            }
            else if (dstAddr == 12)
            {
                asm volatile("mov _normalRegAddr, W12");
            }
            else if (dstAddr == 13)
            {
                asm volatile("mov _normalRegAddr, W13");
            }
            else if (dstAddr == 14)
            {
                asm volatile("mov _normalRegAddr, W14");
            }
            sprintf(debugBuffer, "WRITE %04X from %02d\r\n", normalRegAddr, dstAddr);
            uartTxString(debugBuffer);
        }
        else
        {
            sprintf(debugBuffer, "READ %04X from %02d\r\n", normalRegAddr, srcAddr);
            uartTxString(debugBuffer);
        }

        sprintf(debugBuffer, "err @ id %02X prg%04X @ stk %04X\r\n*********************\r\n", id8, errPrgLoc, errStkLoc);
        uartTxString(debugBuffer);

        INTCON1bits.ADDRERR = 0;        //Clear the trap flag*/