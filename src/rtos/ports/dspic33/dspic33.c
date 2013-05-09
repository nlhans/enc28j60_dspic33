#include "stddefs.h"
#include "task.h"

void RtosKernelPortInitStack(RtosTask_t* task)
{
    UI08_t i = 0;
    UI16_t* TOS = task->stack;

    *TOS = (int)task->method;   TOS++;                  // low byte PC
    *TOS = 0;                   TOS++; 			// high byte PC
    *TOS = 0;                   TOS++; 			// initial SR
    *TOS = 0;                   TOS++;			// W0.

    // Add W1-W14, RCOUNT and TBLPAG
    for (i = 0; i <  16; i++)
    {
        *TOS = 0;               TOS++;
    }

    // Add CORCON and PSVPAG
    *TOS = CORCON;              TOS++;			// CORCON
    *TOS = PSVPAG;              TOS++;			// PSVPAG
    *TOS = 0;                   TOS++; 			// Nesting depth

    task->stackPosition = TOS;
}

void RtosKernelPortTimerStart()
{
    T1CON = 0;
    TMR1=0;

    PR1 = 10000000/1000; // 1kHz

    RtosKernelPortTimerClear();
    IPC0bits.T1IP = 0x01; // Priority level
    IEC0bits.T1IE = 1; // Enable Timer1 interrupts

    T1CONbits.TCKPS = 0b00; // No prescale
    T1CONbits.TON=1;
}

void RtosKernelPortTimerClear()
{
    IFS0bits.T1IF = 0;
}

void __attribute__((__interrupt__, __shadow__, auto_psv)) _T1Interrupt(void)
{
    RtosKernelPortTimerClear();
    RtosKernelContextSuspend();
}

inline void RtosKernelContextStart()
{
    asm volatile("CALL _RtosKernelStart");
}

inline void RtosKernelContextSuspend()
{
    asm volatile("CALL _RtosKernelContext");
}