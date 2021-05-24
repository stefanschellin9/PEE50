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
    unsigned long micro = 80000L;
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

void systick_function_timer(void (*func_ptr)())
{
    int start = SysTickValueGet();

    func_ptr();

    int stop = SysTickValueGet();

    int delta = SysTickPeriodGet()&(start-stop);
    printf("processor ticks: %d tijd in US: %f\n", delta, (double)delta/80);
}


