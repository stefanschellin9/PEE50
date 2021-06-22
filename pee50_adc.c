/*
 * pee50_adc.c
 *
 *  Created on: 31 mei 2021
 *      Author: cihat sonmez
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

#define MAX_WAARDE 8
#define MAX_DELING 1/MAX_WAARDE

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

    uint16_t adcValue;
    int i = 0;
    uint16_t som = 0;

    while(i < MAX_WAARDE) {
        if(ADC_convert(adc0, &adcValue) == ADC_STATUS_SUCCESS) {
            som += adcValue*MAX_DELING;
            i++;
        }
    }

    *(float *)stroom = (ADC_convertRawToMicroVolts(adc0, som)*0.000120); //0.000001*58.5589777 //0.000001*82.05128205 //0.000001*144.2029894

}

void adc_meet_spanning_voor (void *spanning_voor) {
    uint16_t adcValue;
    if(ADC_convert(adc1, &adcValue) == ADC_STATUS_SUCCESS) {
        *(float *)spanning_voor = (ADC_convertRawToMicroVolts(adc1, adcValue)*0.00005242718447); //0.000001*52.6299809 //0.000001*52.42718447
    }
}

void adc_meet_spanning_na (void *spanning_na) {
    uint16_t adcValue;
    if(ADC_convert(adc2, &adcValue) == ADC_STATUS_SUCCESS) {
        *(float *)spanning_na = (ADC_convertRawToMicroVolts(adc2, adcValue)*0.0000593220339); //0.000001*59.3919453 //0.000001*59.3220339
    }
}
