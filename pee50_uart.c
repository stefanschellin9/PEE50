/*
 * pee50_uart.c
 *
 *  Created on: 2 jun. 2021
 *      Author: bastiaan van tienhoven
 */
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <ti/drivers/UART.h>

#include "ti_drivers_config.h"

#include "main.h"
#include "pee50_uart.h"
#include "pee50_verwerker.h"
#include "pee50_ftoa.h"

UART_Params uart_params;
UART_Handle uart_handle;

#define MAX_BUFF_SIZE    8

static int tijd_index = 0;

static int buffer_uart_init[MAX_BUFF_SIZE];

static int houder = 0;

int nummer = 0;

void uart_isr(UART_Handle handle, void *buf, size_t count)
{
    // add status reset and status change when everything is received
    sys_stat_t status;
    system_status_get(&status, NULL);

    if(status == reset) {
        if(*(char *)buf == '\b') {
            char *bs_dl = "\b \b";
            UART_write(handle, bs_dl, sizeof(bs_dl));
            buffer_del_char();
        } else if(*(char *)buf == ' ') {
            UART_write(handle, buf, 1);
        } else if(*(char *)buf == '\n') {
            UART_write(handle, buf, 1);
            converter(&nummer);
            buffer_uart_init[houder] = nummer;
            houder++;
        } else if(*(char *)buf >= '0' && *(char *)buf <= '9') {
            UART_write(handle, buf, 1);
            buffer_put(*(char *)buf);
        }

        if(houder >= MAX_BUFF_SIZE) {
            sys_stat_t temp = gereed;
            system_status_change(&temp, NULL);
            uart_write_message("data is opgeslagen\n");
            houder = 0;
        }
    } else if(status == gereed) {
        if(*(char *)buf == 's') {
            status = start;
            system_status_change(&status, NULL);
            uart_write_message("het systeem wordt gestart [druk op elke toets voor een reset]");
        } else {
            uart_write_message("om te starten druk op s\n");
        }
    } else {
        status = nood;
        system_status_change(&status, NULL);
        uart_write_message("\e[1;1H\e[2J");
    }
}

void uart_init_callback()
{
    UART_init();
    UART_Params_init(&uart_params);
    uart_params.baudRate = 115200;
    uart_params.readMode = UART_MODE_CALLBACK;
    uart_params.readCallback = &uart_isr;
    uart_params.readEcho = UART_ECHO_OFF;
}

void uart_open()
{
    uart_handle = UART_open(CONFIG_UART_0, &uart_params);
    if(uart_handle == NULL) {
        uart_write_message("uart_open failed\n");
        while(1);
    }
}

void uart_close()
{
    UART_close(uart_handle);
}

void uart_write_message(char *message)
{
    UART_write(uart_handle, message, strlen(message));
}

void uart_send_data(void *data_struct, void *temp1)
{
    uart_write_message("\e[1;1H\e[2J");
    char chr[5];
    ftoa(((send_data_t *)data_struct)->stroom, &chr, 5);
    UART_write(uart_handle,"\r\n stroom = ", 13);
    UART_write(uart_handle, chr, sizeof(chr));

    ftoa(((send_data_t *)data_struct)->spanning_na, &chr, 5);
    UART_write(uart_handle,"\r\n spanning uit = ", 19);
    UART_write(uart_handle, chr, sizeof(chr));

    ftoa(((send_data_t *)data_struct)->spanning_voor, &chr, 5);
    UART_write(uart_handle, "\r\n spanning in =", 17);
    UART_write(uart_handle, chr, sizeof(chr));

    //ftoa(((send_data_t *)data_struct)->temperatuur, &chr, 5);
    sprintf(chr, "%.4f",((send_data_t *)data_struct)->temperatuur);
    UART_write(uart_handle, "\r\n temp in =", 13);
    UART_write(uart_handle, chr, sizeof(chr));

    float vermogen = ((send_data_t *)data_struct)->stroom;
    vermogen *= ((send_data_t *)data_struct)->spanning_na;

    ftoa(vermogen, &chr, 5);
    UART_write(uart_handle, "\r\n vermogen = ", 15);
    UART_write(uart_handle, chr, sizeof(chr));

//    ftoa(((send_data_t *)data_struct)->temperatuur, &chr, 5);
//    UART_write(uart_handle, "\r\n temperatuur = ", 18);
//    UART_write(uart_handle, chr, sizeof(chr));
}

void uart_get_next_velocity(void *p, void *arg2){
    *(int *)p = buffer_uart_init[tijd_index];
    tijd_index++;
    //UART_write(uart_handle, *p, sizeof(int));
}

void check_uart()
{
    char input;
    int test = 0;
    UART_control(uart_handle, UART_CMD_GETRXCOUNT ,&test);
    if(test != 0) {
        UART_read(uart_handle, &input, 1);
    }
}
