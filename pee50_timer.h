/*
 * pee50_timer.h
 *
 *  Created on: 15 jun. 2021
 *      Author: stefanschellin9
 */

#ifndef PEE50_TIMER_H_
#define PEE50_TIMER_H_

void timer_isr();

void timer_init(int hertz);

void timer_start();

void timer_stop();

#endif /* PEE50_TIMER_H_ */
