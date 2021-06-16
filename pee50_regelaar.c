/*
 * pee50_regelaar.c
 *
 *  Created on: 31 mei 2021
 *      Author: jarin van veenendaal
 */

#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>

/* Example/Board Header files */
#include <ti/drivers/Timer.h>
#include <ti/drivers/PWM.h>

#include "ti_drivers_config.h"
#include "pee50_regelaar.h"
#include "pee50_adc.h"

#define ti  0.00012      // 14.7;
#define td  0.00036     // 3.6;
#define ts  0.0002       // 1ms
#define kc  0.00005625        // 0.0005625

PIDController PID;

float start_Duty = 1;

PWM_Handle pwm;
PWM_Params pwmParams;
float dutyValue;

uint32_t dutyCycleMin = (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * 1) / 100);
uint32_t dutyCycleMax = (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * 14) / 100);

void regelaar_init()
{
    PID.ki = kc*(ts/(ti * 2));
    PID.kd = kc*(td/ts);
    PID.kp = kc;

 // Initialize the PWM parameters
    PWM_init();

    PWM_Params_init (&pwmParams);
    pwmParams.idleLevel = PWM_IDLE_LOW;   // Output low when PWM is not running
    pwmParams.periodUnits = PWM_PERIOD_HZ;// Period is in Hz
    pwmParams.periodValue = 30000;// 1MHz
    pwmParams.dutyUnits = PWM_DUTY_FRACTION;// Duty is in fractional percentage
    pwmParams.dutyValue = 1;// 0% initial duty cycle
    // Open the PWM instance
    pwm = PWM_open(CONFIG_PWM_0, &pwmParams);
    if (pwm == NULL)
    {
        // PWM_open() failed
        while (1);
    }
    PID.previous_Error = PID.Error = 0;
    PID.previous_Output_Measurement_2 = PID.previous_Output_Measurement = 0;
    PID.previous_Output_Measurement = PID.Output_Measurement = 0;
    PID.Output = 0;
}

void regelaar_open(void)
{
    PWM_start(pwm);                          // start PWM with 0% duty cycle
    dutyValue = (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * start_Duty) / 100);
    PWM_setDuty(pwm, dutyValue);
    PID.previous_Error = PID.Error = 0;
    PID.previous_Output_Measurement_2 = PID.previous_Output_Measurement = 0;
    PID.previous_Output_Measurement = PID.Output_Measurement = 0;
    PID.Output = 0;
}

void regelaar_set(void *arg1, void *arg2)
{
    PWM_start(pwm);
    dutyValue = (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * *(float *)arg1) / 100);
    PWM_setDuty(pwm, dutyValue);
    PID.previous_Error = PID.Error = 0;
    PID.previous_Output_Measurement_2 = PID.previous_Output_Measurement = 0;
    PID.previous_Output_Measurement = PID.Output_Measurement = 0;
    PID.Output = 0;
}

void regelaar_close(void)
{
    PID.previous_Error = PID.Error = 0;
    PID.previous_Output_Measurement_2 = PID.previous_Output_Measurement = 0;
    PID.previous_Output_Measurement = PID.Output_Measurement = 0;
    PID.Output =  0;
    PWM_stop(pwm);
}
float setpoint = 128; //*(float *)sp;

void regelaar(void *sp, void *temp2)
{
    float current_Setpoint = 0;

    float voltage, current;
    adc_meet_stroom(&current);
    adc_meet_spanning_na(&voltage);
    if(voltage <2) {
        PID.Output_Measurement = 0;
        PID.Error = 0;
        current_Setpoint = 0;
    }
    else {
        current_Setpoint = setpoint/voltage;
        PID.Output_Measurement = current;
        PID.Error = current_Setpoint - PID.Output_Measurement;
    }

//    if(current < 0.5) {
//        current = 1;
//    }
    //current = 1;
    //voltage = 1;

    PID.proportional = PID.kp * (PID.previous_Output_Measurement - PID.Output_Measurement);
    PID.integrator = PID.ki * (PID.Error + PID.previous_Error);
    PID.differential = PID.kd * ((2 * PID.previous_Output_Measurement) - PID.Output_Measurement - PID.previous_Output_Measurement_2);
    PID.Output += (PID.proportional + PID.integrator + PID.differential);

    dutyValue = (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * PID.Output)*0.01);
    if (dutyValue >= dutyCycleMax)
    {
      dutyValue = dutyCycleMax;
      PID.Output = 14;

    }
    else if(dutyValue <= dutyCycleMin)
    {
      dutyValue = dutyCycleMin;
      PID.Output = 1;
    }

    //dutyValue = (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * 14)*0.01);

    PWM_setDuty(pwm, dutyValue);
    PID.previous_Error = PID.Error;
    PID.previous_Output_Measurement_2 = PID.previous_Output_Measurement;
    PID.previous_Output_Measurement = PID.Output_Measurement;
}


