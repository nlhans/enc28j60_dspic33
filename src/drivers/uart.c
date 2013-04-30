
#include "uart.h"
#include <pps.h>

void uartInit()
{
    U1MODE = 0b1000000000000000;
    U1STA  = 0b0000010000000000;

    U1BRG = 12/3; // 115k2 @ FRCPLL stock

    PORTB &= ~(1<<7);
    // 00011 source
    // destination rp7

    PPSUnLock;
    iPPSOutput(OUT_PIN_PPS_RP7, OUT_FN_PPS_U1TX);
    PPSLock;
}

void uartTxByte(UI08_t byte)
{
    while (U1STAbits.UTXBF == 1);
    U1TXREG = byte;
}

void uartTxString(char* str)
{
    while(*str != '\0')
    {
        uartTxByte(*str);
        str++;
    }
}