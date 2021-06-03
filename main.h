/*
 * main.h
 *
 *  Created on: 17 mei 2021
 *      Author: stefa
 */

#ifndef MAIN_H_PEE50
#define MAIN_H_PEE50

typedef struct send_data {
    float stroom;
    float spanning_voor;
    float spanning_na;
} send_data_t;

typedef enum sys_stat {
    reset,
    wacht,
    gereed,
    start,
    nood
} sys_stat_t;

void system_status_change(void *stat_ptr, void *temp1);
void system_status_get(void *stat_ptr, void *temp1);

#endif /* MAIN_H_PEE50 */
