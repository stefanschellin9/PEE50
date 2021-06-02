/*
 * regelaar.h
 *
 *  Created on: 24 mei 2021
 *      Author: Jarin van veenendaal
 */

#ifndef REGELAAR_H_PEE50
#define REGELAAR_H_PEE50

#include <stdint.h>
#include <ti/drivers/PWM.h>

typedef struct pid_con{
    double ki;
    float kd;
    float kp;
    int16_t integrator_LimitMax;
    int16_t integrator_LimitMin;
    int16_t Output_LimitMax;
    int16_t Output_LimitMin;

    float Error;
    float previous_Error;
    float Output_Measurement;
    float previous_Output_Measurement;
    float previous_Output_Measurement_2;
    float integrator;
    float proportional;
    float differential;
    float Output;
}PIDController;

void regelaar_init();
void regelaar(void *sp);
void regelaar_open();
void regelaar_close();

#endif /* REGELAAR_H_PEE50 */
