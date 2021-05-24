/*
 * pee50_regelaar.h
 *
 *  Created on: 17 mei 2021
 *      Author: jarin van veenendaal
 */

#ifndef REGELAAR_H_PEE50
#define REGELAAR_H_PEE50

/********************************* regelaar **********************************/
/* pid regeling die naar een setpoint regelt en pwm uit stuurt */
void regelaar(float stroom, float spanning, float setpoint);

/******************************* regelaar_init ********************************/
/* initilisatie van regelaar */
void regelaar_init();

/******************************* regelaar_open ********************************/
/* start de regelaar als dit nodig is voor bijvoorbeeld pwm */
void regelaar_open();

/******************************* regelaar_close *******************************/
/* stopt de regelaar als ditnodig is voor bijvoorbeeld pwm */
void regelaar_close();

#endif /* REGELAAR_H_PEE50 */
