/*
 * regelaar.h
 *
 *  Created on: 24 mei 2021
 *      Author: Jarin van veenendaal
 */

#ifndef REGELAAR_H_
#define REGELAAR_H_
#include <stdint.h>
#include <ti/drivers/PWM.h>

typedef struct{
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

uint32_t dutyCycleMin = (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * 20) / 100);
uint32_t dutyCycleMax = (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * 1 ) / 100);

void regelaar_Init(void);
void regelaar(int16_t setpoint, float current, float voltage);
void regelaar_open(void);
void regelaar_close(void);

#endif /* REGELAAR_H_ */
