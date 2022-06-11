#ifndef _SERIAL_H
#define _SERIAL_H

#include "types.h"

void uart_tx_data(byte* command, int commandLen);
int uart_rx_data(byte* response_buffer);

#endif