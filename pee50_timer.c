/*
 * pee50_timer.c
 *
 *  Created on: 15 jun. 2021
 *      Author: stefanshcellin9
 */
#include <ti/drivers/Timer.h>
#include <stdio.h>

#include "ti_drivers_config.h"
#include "pee50_timer.h"
#include "pee50_regelaar.h"
#include "pee50_uart.h"
#include "main.h"

Timer_Handle    timer_handle;
Timer_Params    timer_params;
int delta = 0;

void timer_isr(Timer_Handle handle, int_fast16_t status)
{
    float sp = 0.0;             // initaliseer setpoint
    get_setpoint(&sp, NULL);    // vraagt de laatste setpoint
    regelaar(&sp, NULL);        // regelaar aanroepen
}

void timer_init(int hertz)
{
    Timer_init();

    Timer_Params_init(&timer_params);
    timer_params.period = hertz;
    timer_params.periodUnits = Timer_PERIOD_HZ;
    timer_params.timerMode  = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = timer_isr;

}

void timer_start()
{
    timer_handle = Timer_open(CONFIG_TIMER_0, &timer_params);
    if(timer_handle == NULL) {
        uart_write_message("timer handle NULL\n");
    }
    if(Timer_start(timer_handle) == Timer_STATUS_ERROR){
            uart_write_message("START ERROR\n");
    }
}

void timer_stop()
{
    Timer_stop(timer_handle);
    Timer_close(timer_handle);
}
