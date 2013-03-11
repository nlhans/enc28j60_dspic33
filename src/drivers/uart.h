
#ifndef UART_H
#define UART_H

#include "types.h"

void uartInit();
void uartTxByte(UI08_t byte);
void uartTxString(char* str);

#endif