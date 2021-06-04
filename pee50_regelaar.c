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


#define ti  0.0002      // 14.7;
#define td  0.00033     // 3.6;
#define ts  0.001       // 1ms
#define kc  0.11        // 8.4;

PIDController PID;

float start_Duty = 1;
PIDController ParamsPID;
PWM_Handle pwm;
PWM_Params pwmParams;
float dutyValue;

uint32_t dutyCycleMin = (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * 1) / 100);
uint32_t dutyCycleMax = (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * 20 ) / 100);

void regelaar_init()
{
    ParamsPID.ki = kc*(ts/ti * 2);
    ParamsPID.kd = kc*(td/ts);
    ParamsPID.kp = kc;

 // Initialize the PWM parameters
    PWM_init();

    PWM_Params_init (&pwmParams);
    pwmParams.idleLevel = PWM_IDLE_LOW;   // Output low when PWM is not running
    pwmParams.periodUnits = PWM_PERIOD_HZ;// Period is in Hz
    pwmParams.periodValue = 30000;// 1MHz
    pwmParams.dutyUnits = PWM_DUTY_FRACTION;// Duty is in fractional percentage
    pwmParams.dutyValue = 10;// 0% initial duty cycle
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
    dutyValue = (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * start_Duty)
            / 100);
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

void regelaar(void *sp, void *temp2)
{
    float setpoint = *(float *)sp;
    float voltage, current;
    adc_meet_stroom(&current);
    adc_meet_spanning_na(&voltage);

    float current_Setpoint = setpoint/voltage;
    PID.Output_Measurement = current;
    PID.Error = current_Setpoint - PID.Output_Measurement;

    PID.proportional = PID.kp * (PID.previous_Output_Measurement - PID.Output_Measurement);
    PID.integrator = PID.ki * (PID.Error + PID.previous_Error);
    PID.differential = PID.kd * (2 * PID.previous_Output_Measurement - PID.Output_Measurement - PID.previous_Output_Measurement_2);
    PID.Output += PID.proportional + PID.integrator + PID.differential;

    dutyValue = (uint32_t) (((uint64_t) PWM_DUTY_FRACTION_MAX * PID.Output)*0.01);
    if (dutyValue >= dutyCycleMax)
    {
      dutyValue = dutyCycleMax;
      PID.Output = 20;

    }
    else if (dutyValue <= dutyCycleMin)
    {
      dutyValue = dutyCycleMin;
      PID.Output = 1;
    }
    else
    {
      dutyValue = PID.Output;
    }
    PWM_setDuty(pwm, dutyValue);
    PID.previous_Error = PID.Error;
    PID.previous_Output_Measurement_2 = PID.previous_Output_Measurement;
    PID.previous_Output_Measurement = PID.Output_Measurement;
}


