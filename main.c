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
#include "pee50_timer.h"

#define MAX_STROOM          40
#define MAX_SPANNING_VOOR   35
#define MAX_SPANNING_NA     40
#define MAX_TEMPERATUUR     35
#define TEMP_FAN_ON         60

sys_stat_t sys_status = wacht;
send_data_t data_struct = {0.0, 0.0, 0.0, 0.0, 0.0, 0};



void system_status_change(void *stat_ptr, void *temp1)
{
    sys_status = *(sys_stat_t *)stat_ptr;
}



void get_setpoint(void *arg1, void *arg2)
{
    *(float *)arg1 = data_struct.set_point;
}



void system_status_get(void *stat_ptr, void *temp1)
{
    *(sys_stat_t *)stat_ptr = sys_status;
}



void func(void *temp1, void *temp2)
{
    gpio_led_toggle();
}



void check_overwaarde(void *arg1, void *arg2)
{
    sys_stat_t temp = nood;

    adc_meet_stroom(&data_struct.stroom);
    if(data_struct.stroom > MAX_STROOM) {
        uart_write_message("\e[1;1H\e[2J");
        uart_write_message("OVERSTROOM\n");
        system_status_change(&temp, NULL);
    }
    adc_meet_spanning_voor(&data_struct.spanning_voor);
    if(data_struct.spanning_voor > MAX_SPANNING_VOOR) {
        uart_write_message("\e[1;1H\e[2J");
        uart_write_message("OVERSPANNING OP DE INGANG\n");
        system_status_change(&temp, NULL);
    }
    adc_meet_spanning_na(&data_struct.spanning_na);
    if(data_struct.spanning_na > MAX_SPANNING_NA) {
        uart_write_message("\e[1;1H\e[2J");
        uart_write_message("OVERSPANNING OP DE UITGANG\n");
        system_status_change(&temp, NULL);
    }
    tmp117_read_temp_c(&data_struct.temperatuur);
    if(data_struct.temperatuur > MAX_TEMPERATUUR) {
        uart_write_message("\e[1;1H\e[2J");
        uart_write_message("OVERVERHIT\n");
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
    sys_stat_t temp = wacht;
    char chr[25];

    float stroom;
    adc_meet_stroom(&stroom);
    sprintf(chr, "stroom = %.3f\n",stroom);
    uart_write_message("\e[1;1H\e[2J");
    uart_write_message("systeem staat in reset tot stroom lager is dan 13 ampere\n");
    uart_write_message(chr);

    if(stroom < 13) {
        system_status_change(&temp, NULL);
    }
}



void switch_relay(void *arg1, void *arg2)
{
    gpio_schakelaar_off();       /* switch off relay */
}



float set_points[3] = {380.5, 415.5, 470.3};
int index = 0;

void next_setpoint(void *velocity, void *setpoint)
{
    data_struct.set_point = set_points[index];
    index++;
    index %= 3;
    uart_get_next_velocity(velocity, NULL);

    //calc_setpoint(&velocity, &data_struct.set_point);     // calculate the next setpoint with
}



/************************************ main ***********************************/
/* the main function of the electrolyzer MCU */
int main(void)
{
    /* cc3220 and nortos initialization */
    Board_init();
    NoRTOS_start();

    /* variable definition */

    /* initialize once */
    regelaar_init();            /* initialize regelaar */
    regelaar_open();            /* open regelaar at 1% pwm */

    gpio_init();                /* initialize gpio */
    gpio_schakelaar_on();       /* switch on relay */


    //uart_init_callback();       /* initialize uart for callback mode */
    tmp117_init();              /* initialize tmp117 temperature sensor */


    adc_init();                 /* initialize adc */
    adc_open();
    timer_init(5000);
    while(1) {
        uart_init_callback();
        uart_open();
        systick_init();

        regelaar_open();
        //regelaar_open();
        /*********************************************************************/
        uart_write_message("\e[1;1H\e[2J");
        uart_write_message("voer uw 8 windsnelheden in:\n");
        gpio_schakelaar_on();
        while(sys_status == wacht) {
            check_uart();
        }

        /*********************************************************************/
        while(sys_status == gereed) {
            check_uart();
        }

        /*********************************************************************/
        /* attach tasks to scheduler */
        scheduler_task_attach(&next_setpoint, 15000, 0, &data_struct.velocity, &data_struct.set_point);     // om de 5 minuten
        scheduler_task_attach(&check_overwaarde, 1000, 0, &data_struct);             // check voor overwaardes elke seconde
        scheduler_task_attach(&uart_send_data, 1000, 0, &data_struct, &sys_status);               // send data every second

        systick_start();
        timer_start();
        while(sys_status == start) {
            scheduler_tasks_execute();
            check_uart();
        }
        timer_stop();
        systick_stop();
        scheduler_task_detach_all();



        /*********************************************************************/
        //uart_write_message("\e[1;1H\e[2J");

        data_struct.velocity = 0;
        calc_setpoint(&data_struct.velocity, &data_struct.set_point);

        scheduler_task_attach(&check_stroom_nul, 100, 0);
        systick_start();
        timer_start();
        while(sys_status == reset) {
            scheduler_tasks_execute();
        }
        timer_stop();
        systick_stop();
        scheduler_task_detach_all();


        /*********************************************************************/
        //sys_stat_t temp1 = wacht;
        //scheduler_task_attach(&system_status_change, 25, 2000, &temp1);             // attach system_status_change change status after 25 ms
        systick_start();
        while(sys_status == nood) {
            gpio_schakelaar_off();
            check_uart();
        }
        systick_stop();
        scheduler_task_detach_all();

        uart_close();
    }
        /*********************************************************************/
//        sys_stat_t temp1 = wacht;
//
//        data_struct.velocity = 0;
//        calc_setpoint(&data_struct.velocity, &data_struct.set_point);
//
//        scheduler_task_attach(&check_stroom_nul, 1, 0);                           // attach adc_meet_stroom check every ms
//        scheduler_task_attach(&system_status_change, 10, 25, &temp1);             // attach system_status_change change status after 25 ms
//        scheduler_task_attach(&switch_relay, 10, 20);                             // attach gpio_power_rail_switch change after 20 ms
//
//        systick_start();
//        while(sys_status == nood) {
//            scheduler_tasks_execute();
//        }
//        systick_stop();
//        scheduler_task_detach_all();

    //}
    gpio_schakelaar_off();
    uart_close();
    regelaar_close();

    return 0;
}


/* test uart_send */
//        scheduler_task_attach(&uart_send_data, 100, 0, &data_struct); // 100 ms 0 ms delay
//        systick_start();
//        while(1) {
//            scheduler_tasks_execute();
//        }
//        systick_stop();
//        scheduler_task_detach_all();



/* test regelaar with setpoints */
//        uart_write_message("voer uw 8 windsnelheden in:\n");
//        while(sys_status == reset) {
//            check_uart();
//        }
//        uart_write_message("het systeem is gereed\n");
//        while(sys_status == gereed) {
//            check_uart();
//        }

//        scheduler_task_attach(&next_setpoint, 5000, 0, &data_struct.velocity, &data_struct);      // om de 5 minuten
//        scheduler_task_attach(&uart_send_data, 1000, 0, &data_struct); // 100 ms 0 ms delay
//        scheduler_task_attach(&check_overwaarde, 1000,0);
        //systick_start();
//        timer_start();
//        while(1) {
//            scheduler_tasks_execute();
//        }
//        timer_stop();
//        systick_stop();
//
//        while(sys_status == nood) {
//            sys_status == reset;
//        }


//        scheduler_task_attach(&regelaar_set, 1000, 5000, &testing);
//        scheduler_task_attach(&check_overwaarde, 1000, 1000, &data_struct);
//        scheduler_task_attach(&uart_send_data, 1000, 1000, &data_struct);
//
//        systick_start();
//        while(1) {
//            scheduler_tasks_execute();
//        }
//        systick_stop();
//        scheduler_task_detach_all();
