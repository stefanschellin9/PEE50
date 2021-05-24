/*
 * pee50_scheduler.c
 *
 *  Created on: 30 apr. 2021
 *      Author: stefanschellin9
 */
/* standard headers */
#include <stdint.h>             /* standardized integer library     */
#include <stdlib.h>             /* general purpose C library        */
#include <string.h>             /* string operations library        */
#include <stdarg.h>             /* variable argument lists library  */

/* custom headers */
#include "pee50_scheduler.h"    /* scheduler header file            */

/* global variables */
struct node *head = NULL;

/******************************* scheduler_isr *******************************/
/* counts down delay and timer before task is ready to be executed again */
void scheduler_isr()
{
    struct node *direct = head;

	while(direct != NULL) {
        if(direct->task.delay > 0) {
            direct->task.delay--;

        } else if(direct->task.timer > 0) {
            direct->task.timer--;

            if(direct->task.timer == 0) {
                direct->task.status = TASK_READY;
            }
        }
		direct = direct->next;
	}
}

/*************************** scheduler_task_attach ***************************/
/* attaches a task to linked list and initializes its structure members */
int8_t scheduler_task_attach(void (*task_ptr)(void *, void *),
                              uint16_t interval, uint16_t delay, ...)
{
    va_list ap;
    va_start(ap, delay);

	struct node *helper = (struct node *)malloc(sizeof(struct node));
	if(helper == NULL) {
		return -1;
	}
	helper->next = head;
	head = helper;
	
	head->task.func_ptr = task_ptr;
	head->task.interval = interval;
	head->task.delay = delay;
	head->task.timer = interval;
	head->task.status = ~TASK_READY;
	head->task.param.arg1 = va_arg(ap, void *);
	head->task.param.arg2 = va_arg(ap, void *);

	va_end(ap);
	return 0;
}

/*************************** scheduler_task_detach ***************************/
/* detaches a task from the linked list and frees its allocated memory */
void scheduler_task_detach(void (*task_ptr)(void *, void *))
{
	struct node **indirect = &head;
	struct node *helper = NULL;

	while((*indirect)->task.func_ptr != task_ptr) {
		indirect = &(*indirect)->next;
	}

	helper = (*indirect)->next;
	free(*indirect);
	*indirect = helper;
}

/************************* scheduler_task_detach_all *************************/
/* detaches all tasks from the linked list and frees all allocated memory */
void scheduler_task_detach_all()
{
    struct node **indirect = &head;
    struct node *helper = NULL;

    while((*indirect) != NULL) {
        helper = (*indirect)->next;
        free(*indirect);
        *indirect = helper;
    }
}

/*************************** scheduler_task_execute **************************/
/* executes the tasks in the linked list if there status is ready */
void scheduler_tasks_execute()
{
    struct node *direct = head;

    while(direct != NULL) {
        if(direct->task.status == TASK_READY) {
            direct->task.func_ptr(direct->task.param.arg1, direct->task.param.arg2);
            direct->task.status = ~TASK_READY;
            direct->task.timer = direct->task.interval;
        }
        direct = direct->next;
    }
}
