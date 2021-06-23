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

const char* enum_names[] = {"reset", "wacht", "gereed", "start", "nood"};


void uart_isr(UART_Handle handle, void *buf, size_t count)
{
    // add status reset and status change when everything is received
    sys_stat_t status;
    system_status_get(&status, NULL);

    if(status == wacht) {
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
            uart_write_message("data is opgeslagen [om te starten druk op s]\n");
            houder = 0;
        }
    } else if(status == gereed) {
        if(*(char *)buf == 's') {
            status = start;
            system_status_change(&status, NULL);
            uart_write_message("het systeem wordt gestart [druk op elke toets voor een reset]\n");
        } else {
            uart_write_message("om te starten druk op s\n");
        }
    } else if(status == nood) {
        if(*(char *)buf == 'r') {
            status = wacht;
            system_status_change(&status, NULL);
        } else {
            uart_write_message("om te resetten druk op r\n");
        }
    } else {
        status = reset;
        system_status_change(&status, NULL);
        uart_write_message("\e[1;1H\e[2J");
    }
}

void uart_init_callback()
{
    UART_init();
    UART_Params_init(&uart_params);
    uart_params.baudRate = 921600;
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

void uart_send_data(void *data_struct, void *sys_stat)
{
    uart_write_message("\e[1;1H\e[2J");
    char chr[25]; // max 25 characters must include '/0'

    sprintf(chr, "stroom = %.3f\n",((send_data_t *)data_struct)->stroom);
    uart_write_message(chr);

    sprintf(chr, "spanning uit = %.3f\n",((send_data_t *)data_struct)->spanning_na);
    uart_write_message(chr);

    sprintf(chr, "spanning in = %.3f\n",((send_data_t *)data_struct)->spanning_voor);
    uart_write_message(chr);

    sprintf(chr, "temperatuur = %.3f\n",((send_data_t *)data_struct)->temperatuur);
    uart_write_message(chr);

    sprintf(chr, "setpoint = %.3f\n",((send_data_t *)data_struct)->set_point);
    uart_write_message(chr);

    sprintf(chr, "windsnelheid = %d\n",((send_data_t *)data_struct)->velocity);
    uart_write_message(chr);

    float vermogen = ((send_data_t *)data_struct)->stroom;
    vermogen *= ((send_data_t *)data_struct)->spanning_na;

    sprintf(chr, "vermogen = %.3f\n",vermogen);
    uart_write_message(chr);

    sprintf(chr, "systeem status = %s", enum_names[*(sys_stat_t *)sys_stat]);
    uart_write_message(chr);
}

void uart_get_next_velocity(void *p, void *arg2){
    *(int *)p = buffer_uart_init[tijd_index];
    tijd_index++;
    tijd_index %= MAX_BUFF_SIZE;
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
