/*
 * pee50_scheduler.h
 *
 *  Created on: 30 apr. 2021
 *      Author: stefanschellin9
 */

#ifndef SCHEDULER_H_PEE50
#define SCHEDULER_H_PEE50

#include <stdint.h>

#define TASK_READY      (0x01)
struct func_args {
    void *arg1;
    void *arg2;
};

struct task_struct {
        void (*func_ptr)(void *, void *);
        uint16_t delay;
        uint16_t interval;
        uint16_t timer;
        uint16_t status;
        struct func_args param;
};

struct node {
	struct task_struct task;
	struct node *next;
};

void test();
/******************************* scheduler_isr *******************************/
/* counts down delay and timer before task is ready to be executed again */
void scheduler_isr(void);

/*************************** scheduler_task_attach ***************************/
/* attaches a task to linked list and initializes its structure members */
int8_t scheduler_task_attach(void (*task_ptr)(void *, void *),
                             uint16_t interval, uint16_t delay, ...);

/*************************** scheduler_task_detach ***************************/
/* detaches a task from the linked list and frees its allocated memory */
void scheduler_task_detach(void (*task_ptr)(void *, void *));

/************************* scheduler_task_attach_all *************************/
/* detaches all tasks from the linked list and frees all allocated memory */
void scheduler_task_detach_all();

/*************************** scheduler_task_execute **************************/
/* executes the tasks in the linked list if there status is ready */
void scheduler_tasks_execute();

#endif /* SCHEDULER_H_PEE50 */
