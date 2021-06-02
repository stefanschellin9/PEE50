/*
 * pee50_adc.c
 *
 *  Created on: 31 mei 2021
 *      Author: stefa
 */


#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* POSIX Header files */
//#include <pthread.h>
#include "pee50_adc.h"

/* Driver Header files */
#include <ti/drivers/ADC.h>
#include <ti/display/Display.h>

/* Driver configuration */
#include "ti_drivers_config.h"
/* Header file */

/* ADC conversion result variables */
//uint16_t adcValue0;
//double adcValue0MicroVolt;
//uint16_t adcValue1;
//double adcValue1MicroVolt;
//uint16_t adcValue2;
//double adcValue2MicroVolt;



/* Globale variabele */
ADC_Handle   adc0;
ADC_Handle   adc1;
ADC_Handle   adc2;
ADC_Params   params;
//ADC_Params   params1;
//ADC_Params   params2;

void adc_init()
{
    ADC_init();
    ADC_Params_init(&params);
}

void adc_open()
{
    adc0 = ADC_open(CONFIG_ADC_0, &params);
    if (adc0 == NULL) {

        while (1);
    }
    adc1 = ADC_open(CONFIG_ADC_1, &params);
    if (adc1 == NULL) {

        while (1);
    }
    adc2 = ADC_open(CONFIG_ADC_2, &params);
    if (adc2 == NULL) {

        while (1);
    }
}

void adc_close()
{
    ADC_close(adc0);
    ADC_close(adc1);
    ADC_close(adc2);
}

void adc_meet_stroom (void *stroom) {
    int_fast16_t res0;

    uint16_t adcValue;

    res0 = ADC_convert(adc0, &adcValue);

    if (res0 == ADC_STATUS_SUCCESS) {

        *(float *)stroom = (ADC_convertRawToMicroVolts(adc0, adcValue)*0.000001);
    }
}

void adc_meet_spanning_voor (void *spanning_voor) {
    int_fast16_t res1;
    uint16_t adcValue;

    res1 = ADC_convert(adc1, &adcValue);

    if (res1 == ADC_STATUS_SUCCESS) {

        *(float *)spanning_voor = (ADC_convertRawToMicroVolts(adc1, adcValue)*0.000001);
    }

}

void adc_meet_spanning_na (void *spanning_na) {
    int_fast16_t res2;
    uint16_t adcValue;

    res2 = ADC_convert(adc2, &adcValue);

    if (res2 == ADC_STATUS_SUCCESS) {

        *(float *)spanning_na = ADC_convertRawToMicroVolts(adc2, adcValue)*0.000001;
    }

}
