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

/*  */
#include <global_use.h>
#include <global_use.h>
#include <stdint.h>
#include <stddef.h>

/* Driver configuration */
#include <ti/drivers/Board.h>
#include "ti_drivers_config.h"

/* nortos framework configuration file */
#include <NoRTOS.h>

/* personal header files */
#include "global_use.h"
#include "pee50_systick.h"
#include "pee50_scheduler.h"

/* not yet functional header files*/
#include "pee50_adc.h"                                          /* todo */
#include "pee50_uart_communicatie.h"                            /* todo */
#include "pee50_regelaar.h"                                     /* todo */

send_data_t data;

void system_status_change(void *stat_ptr, void *temp1)
{
    status = *(sys_stat_t *)stat_ptr;
}

void func(void *temp1, void *temp2)
{
    /* dummy placeholder function for function pointer testing */
}

/************************************ main ***********************************/
/* the main function of the electrolyzer MCU */
int main(void)
{
    /* cc3220 and nortos initialization */
    Board_init();
    NoRTOS_start();

    /* initialize once */
    systick_init();
//    gpio_init();                                                /* todo */
//    uart_init();                                                /* todo */
//    i2c_init();                                                 /* todo */

    while(1) {
        while(status == reset) {

            if(data.current == 0) {
                status = wacht;
            }
        }

        while(status == wacht) {
            // uart_send wacht signaal

        }
        while(status == gereed) {
            // uart_send gereed signaal
            // wacht voor uart start signaal
        }

        /* attach tasks to scheduler */
        scheduler_task_attach(&func, 48, 0);
        scheduler_task_attach(&func, 48, 16);
        scheduler_task_attach(&func, 48, 32);

        systick_start();
        while(status == start) {
            scheduler_tasks_execute();
        }
        systick_stop();
        scheduler_task_detach_all();

        sys_stat_t temp1 = reset;

        // setpoint set to 0
        scheduler_task_attach(&func, 1, 0);                                 // attach adc_meet_stroom check every ms
        scheduler_task_attach(&system_status_change, 10, 25, &temp1);       // attach system_status_change change status after 25 ms
        scheduler_task_attach(&func, 10, 20);                               // attach gpio_power_rail_switch change after 20 ms

        systick_start();
        while(status == nood) {
            scheduler_tasks_execute();
        }
        systick_stop();
        scheduler_task_detach_all();
    }
    return 0;
}
