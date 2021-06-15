/*
 * pee50_systick.c
 *
 *  Created on: 14 mei 2021
 *      Author: stefanschellin9
 */

#include <ti/devices/cc32xx/driverlib/systick.h>
#include <stdio.h>
#include "pee50_systick.h"
#include "pee50_scheduler.h"

void systick_init(void)
{
    unsigned long micro = 16000L;                   // timer voor elke 200us/5000Hz
    SysTickPeriodSet(micro);
    SysTickIntRegister(&scheduler_isr);
}

void systick_start(void)
{
    SysTickEnable();
    SysTickIntEnable();
}

void systick_stop(void)
{
    SysTickIntUnregister();
    SysTickDisable();
    SysTickIntDisable();
}

void systick_function_timer(void (*func_ptr)(void *), void *temp)
{
    int start = SysTickValueGet();

    func_ptr(temp);

    int stop = SysTickValueGet();

    int delta = SysTickPeriodGet()&(start-stop);
    printf("processor ticks: %d tijd in US: %f\n", delta, (double)delta*0.0125);
}


