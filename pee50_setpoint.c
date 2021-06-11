/*
 * pee50_setpoint.c
 *
 *  Created on: 4 jun. 2021
 *      Author: stefa
 */

#include <math.h>
#include <stdio.h>
#include <time.h>
#include "pee50_setpoint.h"

#define temperatuur 25
#define hoogte 18
#define rad 1.56
#define lamb 6.9
#define bet  5

float calc_cp(double lambda, int beta)
{
        float x = (1/(lambda + 0.08*beta) - 0.035/(pow(beta, 3) + 1));
        float y = 0.5176*(116*x - 0.4*beta - 5)*exp(-21*x) + 0.0068*lambda;
        return y;
}

float calc_area(float radius)
{
        float x = M_PI*pow(radius, 2);
        return x;
}

float calc_pressure(float height)
{
        float x = (288.15 - 0.0065*height)/288.15;
        float y = (-9.80655*0.00289647)/(8.3144598*-0.0065);
        float z = 101325*pow(x, y);
        return z;
}

float calc_density(float temperature, float height)
{
        float x = calc_pressure(height);
        float y = x/(287*(temperature + 273.15));
        return y;
}

float calc_offset(int i)
{
    float x = 0.00673002 * i * i - 0.22051 * i + 2.1373;
    if(x > 1.45) {
        x = 1.45;
    }
    return x;
}

void calc_setpoint(void *windsnelheid, void *set_point)
{
    int i = *(int *)windsnelheid;
    *(float *)set_point = 0.5 * calc_offset(i) *calc_density(temperatuur, hoogte) * calc_area(rad) * i * i * i * calc_cp(lamb, bet);
    if(*(float *)set_point > 1400) {
        *(float *)set_point = 1400;
    }
}

