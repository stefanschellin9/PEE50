/*
 * pee50_systick.h
 *
 *  Created on: 14 mei 2021
 *      Author: stefanschellin9
 */

#ifndef SYSTICK_H_PEE50
#define SYSTICK_H_PEE50

void systick_init(void);
void systick_start(void);
void systick_stop(void);
void systick_function_timer(void (*func_ptr)());

#endif /* SYSTICK_H_PEE50 */
