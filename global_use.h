/*
 * main.h
 *
 *  Created on: 17 mei 2021
 *      Author: stefa
 */

#ifndef MAIN_H_PEE50
#define MAIN_H_PEE50

/* structures and enums */
typedef struct send_data {
    float current;
    float voltage_before;
    float voltage_after;
    float temperature;
} send_data_t;

typedef enum sys_status {
	reset,
	wacht,
	gereed,
	start,
	nood
} sys_status_t;

/* global variables */
extern send_data_t data;
extern sys_status_t status = reset;

#endif /* MAIN_H_PEE50 */
