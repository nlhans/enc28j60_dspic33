
#ifndef UART_H
#define UART_H

#include "stddefs.h"



void uartInit();
void uartTxByte(UI08_t byte);
void uartTxString(char* str);

#endif