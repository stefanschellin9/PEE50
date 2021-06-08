/*
 * pee50_setpoint.h
 *
 *  Created on: 4 jun. 2021
 *      Author: stefa
 */

#ifndef PEE50_SETPOINT_H_
#define PEE50_SETPOINT_H_

float calc_cp(double lambda, int beta);

float calc_area(float radius);

float calc_pressure(float height);

float calc_density(float temperature, float height);

void calc_setpoint(void *windsnelheid, void *set_point);

#endif /* PEE50_SETPOINT_H_ */
