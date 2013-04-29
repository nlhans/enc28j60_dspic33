
#include "uart.h"
#include <pps.h>

void uartInit()
{
    U1MODE = 0b1000000000000000;
    U1STA  = 0b0000010000000000;

    //U1BRG = 40000000/2/(16*115200)-1;
    //U1BRG = 11; // 7372800/(16*9600)-1;   19k2
    U1BRG = 12/3; // 115k2 @ FRCPLL stock

    PORTB &= ~(1<<7);
    // 00011 source
    // destination rp7
    /*__builtin_write_OSCCONL(OSCCON &)
    __builtin_write_OSCCONH(0x57)*/

    PPSUnLock;
    iPPSOutput(OUT_PIN_PPS_RP7, OUT_FN_PPS_U1TX);
    PPSLock;
}

void uartTxByte(UI08_t byte)
{
    while (U1STAbits.UTXBF == 1);
    U1TXREG = byte;
    //while((U1STA & (1<<9)) != 0);
}

void uartTxString(char* str)
{
    while(*str != '\0')
    {
        uartTxByte(*str);
        str++;
    }
}