/*
 * pee50_uart_communicatie.h
 *
 *  Created on: 17 mei 2021
 *      Author: bastiaan van tienhoven
 */

#ifndef PEE50_UART_COMMUNICATIE_H_
#define PEE50_UART_COMMUNICATIE_H_

#include "main.h"
// in de main.h staat een structure voor alle verzend data, als je iets mist
// kan je dat in de structure toevoegen.

/************************** uart_get_next_velocity ***************************/
/* verzend alle data uit de senddata structure */
void uart_send_data(struct senddata data);

/************************** uart_get_next_velocity ***************************/
/* sla nieuwe velocity in de lijst op in de velocity pointer */
void uart_get_next_velocity(float *velocity);

/******************************* regelaar_init ********************************/
/* initaliseert uart voor het versturen en ontvangen */
void uart_init();

/******************************* regelaar_open ********************************/
/* open de uart handle */
void uart_open();

/******************************* regelaar_close *******************************/
/* sluit de uart handle */
void uart_close();

#endif /* PEE50_UART_COMMUNICATIE_H_ */
