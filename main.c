/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== uartecho.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#include <NoRTOS.h>

#include <ti/drivers/Board.h>

#include "main.h"
#include "pee50_uart.h"
#include "pee50_regelaar.h"
#include "pee50_adc.h"
#include "pee50_tmp117.h"
#include "pee50_systick.h"
#include "pee50_scheduler.h"
#include "pee50_gpio.h"
#include "pee50_setpoint.h"

#define MAX_STROOM          40
#define MAX_SPANNING_VOOR   38
#define MAX_SPANNING_NA     38
#define MAX_TEMPERATUUR     150
#define TEMP_FAN_ON         60


sys_stat_t sys_status = reset;

void system_status_change(void *stat_ptr, void *temp1)
{
    sys_status = *(sys_stat_t *)stat_ptr;
}

void system_status_get(void *stat_ptr, void *temp1)
{
    *(sys_stat_t *)stat_ptr = sys_status;
}

void func(void *temp1, void *temp2)
{
    printf("test\n");
}

void check_overwaarde(void *arg1, void *arg2)
{
    sys_stat_t temp = nood;

    adc_meet_stroom(&((send_data_t *)arg1)->stroom);
    if(((send_data_t *)arg1)->stroom > MAX_STROOM) {
        system_status_change(&temp, NULL);
    }
    adc_meet_spanning_voor(&((send_data_t *)arg1)->spanning_voor);
    if(((send_data_t *)arg1)->spanning_voor > MAX_SPANNING_VOOR) {
        system_status_change(&temp, NULL);
    }
    adc_meet_spanning_na(&((send_data_t *)arg1)->spanning_na);
    if(((send_data_t *)arg1)->spanning_na > MAX_SPANNING_NA) {
        system_status_change(&temp, NULL);
    }
    tmp117_read_temp_c(&((send_data_t *)arg1)->temperatuur);
    if(((send_data_t *)arg1)->temperatuur > MAX_TEMPERATUUR) {
        system_status_change(&temp, NULL);
    }
}

void fan_control(void *temp1, void *temp2)
{
    float celsius;
    tmp117_read_temp_c(&celsius);
    if(celsius > TEMP_FAN_ON) {
        gpio_fan_on();
    } else {
        gpio_fan_off();
    }
}

void check_stroom_nul(void *temp1, void *temp2)
{
    sys_stat_t temp = reset;
    float stroom;
    adc_meet_stroom(&stroom);
    if(stroom < 1) {
        system_status_change(&temp, NULL);
    }
}

void switch_relay(void *arg1, void *arg2)
{
    gpio_schakelaar_off();       /* switch on relay */
}

/************************************ main ***********************************/
/* the main function of the electrolyzer MCU */
int main(void)
{
    /* cc3220 and nortos initialization */
    Board_init();
    NoRTOS_start();

    /* variable definition */
    send_data_t data_struct = {0.0, 0.0, 0.0, 0.0};
    float velocity = 0.0;
    float set_point = 0.0;

    /* initialize once */
    gpio_init();                /* initialize gpio */
    gpio_schakelaar_on();       /* switch on relay */

    systick_init();             /* initialize systick */
    uart_init_callback();       /* initialize uart for callback mode */
    tmp117_init();              /* initialize tmp117 temperature sensor */
    adc_init();                 /* initialize adc */
    regelaar_init();            /* initialize regelaar */

    regelaar_open();            /* open regelaar at 1% pwm */

    adc_open();

    float testing = 50.0;
    while(1) {
        //regelaar_set(&testing, NULL);
        scheduler_task_attach(&regelaar_set, 1000000, 5000000, &testing);
        systick_start();
        while(1) {
            scheduler_tasks_execute();
        }
        systick_stop();
//        scheduler_task_detach_all();
//        uart_write_message("systeem wordt gereset");
//        while(sys_status == reset) {
//
//            // setpoint nul maken
//            float stroom;
//            adc_meet_stroom(&stroom);
//            if(stroom < 1) {
//                sys_status = wacht;
//                uart_write_message("\e[1;1H\e[2J");
//            }
//        }
//
//        uart_write_message("voer uw 8 windsnelheden in:\n");
//        while(sys_status == wacht) {
//            check_uart();
//        }
//
//        while(sys_status == gereed) {
//            uart_write_message("het systeem is gereed\n");
//            check_uart();
//        }
//
//        /* attach tasks to scheduler */
//        scheduler_task_attach(&uart_get_next_velocity, 300000000, 0, &velocity);        // om de 5 minuten
//        scheduler_task_attach(&calc_setpoint, 300000000, 0, &velocity, &set_point);
//        scheduler_task_attach(&regelaar, 200, 10000, &velocity, &set_point);            // regelaar functie om de 200 us na 10 ms delay
//        scheduler_task_attach(&check_overwaarde, 1000000, 0, &data_struct);             // check voor overwaardes elke seconde
//        scheduler_task_attach(&uart_send_data, 1000000, 0, &data_struct);               // send data every second
//
//        systick_start();
//        while(sys_status == start) {
//            scheduler_tasks_execute();
//        }
//        systick_stop();
//        scheduler_task_detach_all();
//
//        sys_stat_t temp1 = reset;
//
//        // setpoint set to 0
//        scheduler_task_attach(&check_stroom_nul, 1000, 0);                              // attach adc_meet_stroom check every ms
//        scheduler_task_attach(&system_status_change, 10000, 25000, &temp1);             // attach system_status_change change status after 25 ms
//        scheduler_task_attach(&switch_relay, 10000, 20000);                             // attach gpio_power_rail_switch change after 20 ms
//
//        systick_start();
//        while(sys_status == nood) {
//            scheduler_tasks_execute();
//        }
//        systick_stop();
//        scheduler_task_detach_all();
    }

    regelaar_close();

    return 0;
}
