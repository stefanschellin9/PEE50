/*
 * pee50_uart.h
 *
 *  Created on: 2 jun. 2021
 *      Author: bastiaan van tienhoven
 */

#ifndef PEE50_UART_H_
#define PEE50_UART_H_

#include <ti/drivers/UART.h>

void uart_isr(UART_Handle handle, void *buf, size_t count);

void uart_init_callback();

void uart_open();

void uart_close();

void uart_write_message(char *message);

void uart_send_data(void *data_struct, void *temp1);

void uart_get_next_velocity(void *p, void *arg2);

void check_uart();

#endif /* PEE50_UART_H_ */
