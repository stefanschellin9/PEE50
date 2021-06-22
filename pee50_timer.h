/*
 * pee50_timer.h
 *
 *  Created on: 15 jun. 2021
 *      Author: stefanschellin9
 */

#ifndef PEE50_TIMER_H_
#define PEE50_TIMER_H_

#include <ti/drivers/Timer.h>

void timer_isr(Timer_Handle handle, int_fast16_t status);

void timer_init(int hertz);

void timer_start();

void timer_stop();

#endif /* PEE50_TIMER_H_ */
