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
#define hoogte 10
#define rad 4
#define lamb M_PI*1.3333333
#define bet  80

float calc_cp(double lambda, int beta)
{
        float x = (1/(lambda + 0.08*beta) - 0.035/(pow(beta, 3) + 1));
        float y = 0.5176*(116*x - 0.4*beta - 5)*exp(-21*x) + 0.068*lambda;
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

float calc_setpoint(void *windsnelheid, void *set_point)
{
    0.5 * calc_density(temperatuur, hoogte) * calc_area(rad) * pow(*(int *)windsnelheid,3) * calc_cp(lamb, bet);
}

//int main()
//{
//        clock_t start, end;
//        start = clock();
//        for(int i = 20; i <= 100; i++)
//        {
//            printf("%d",i);
//            printf("  %f",calc_jarin(i));
//            printf("  %f",calc_jarin2(i));
//            printf("  %f\n",calc_jarin3(i));
//        }
//        end = clock();
//        printf("%f ticks\n",(double)(end-start));
//
//
//
//        //printf("%f\n",calc_cp(120, 120));
//        //printf("%f\n",calc_area(2));
//        //printf("%f\n", calc_pressure(20));
//        //printf("%f\n",calc_density(20,20));
//
//        return 0;
//}
