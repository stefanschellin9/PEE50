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

}

void check_overwaarde(void *temp1, void *temp2)
{
    sys_stat_t temp = nood;
    float stroom, spanning_voor, spanning_na, celsius;
    adc_meet_stroom(&stroom);
    if(stroom > MAX_STROOM) {
        system_status_change(&temp, NULL);
    }
    adc_meet_spanning_voor(&spanning_voor);
    if(spanning_voor > MAX_SPANNING_VOOR) {
        system_status_change(&temp, NULL);
    }
    adc_meet_spanning_na(&spanning_na);
    if(spanning_na > MAX_SPANNING_NA) {
        system_status_change(&temp, NULL);
    }
    tmp117_read_temp_c(&celsius);
    if(celsius > MAX_TEMPERATUUR) {
        system_status_change(&temp, NULL);
    }
}

void fan_control(void *temp1, void *temp2)
{
    float celsius;
    tmp117_read_temp_c(&celsius);
    if(celsius > TEMP_FAN_ON) {
        // gpio high (pin 53 gpio 30)
    } else {
        // gpio low (pin 53 gpio 30)
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


/************************************ main ***********************************/
/* the main function of the electrolyzer MCU */
int main(void)
{
    /* cc3220 and nortos initialization */
    Board_init();
    NoRTOS_start();

    /* initialize once */
    systick_init();                                                         // init systicks
    gpio_init();                                                            // init gpio
    uart_init_callback();                                                   // init uart in callback mode
    tmp117_init();                                                          // init i2c temp sensor
    adc_init();                                                             // init adc


    while(1) {
        uart_write_message("systeem wordt gereset");
        while(sys_status == reset) {

            if(0 == 0) {
                sys_status = wacht;
                uart_write_message("\e[1;1H\e[2J");
            }
        }

        uart_write_message("voer uw 8 windsnelheden in:\n");
        while(sys_status == wacht) {
            check_uart();
        }
        while(sys_status == gereed) {
            uart_write_message("het systeem is gereed\n");
            check_uart();
        }

        /* attach tasks to scheduler */
        scheduler_task_attach(&check_overwaarde, 48, 0);                    // check voor overwaardes
        scheduler_task_attach(&func, 48, 16);
        scheduler_task_attach(&func, 48, 32);

        systick_start();
        while(sys_status == start) {
            scheduler_tasks_execute();
        }
        systick_stop();
        scheduler_task_detach_all();

        sys_stat_t temp1 = reset;

        // setpoint set to 0
        scheduler_task_attach(&check_stroom_nul, 1, 0);                     // attach adc_meet_stroom check every ms
        scheduler_task_attach(&system_status_change, 10, 25, &temp1);       // attach system_status_change change status after 25 ms
        scheduler_task_attach(&func, 10, 20);                               // attach gpio_power_rail_switch change after 20 ms

        systick_start();
        while(sys_status == nood) {
            scheduler_tasks_execute();
        }
        systick_stop();
        scheduler_task_detach_all();
    }
    return 0;
}
