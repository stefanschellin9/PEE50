/*
 * pee50_adc.h
 *
 *  Created on: 17 mei 2021
 *      Author: stefa
 */

#ifndef PEE50_ADC_H_
#define PEE50_ADC_H_

/*
 * pee50_regelaar.h
 *
 *  Created on: 17 mei 2021
 *      Author: cihat sonmez
 */

#ifndef REGELAAR_H_PEE50
#define REGELAAR_H_PEE50

// #include <ti/drivers/ADC.h> <- dit is de driver voor de adc include deze in je .c bestand
// https://dev.ti.com/tirex/content/simplelink_cc32xx_sdk_4_20_00_07/docs/tidrivers/doxygen/html/_a_d_c_8h.html
// ADC_Handle adc;
// ADC_Params params;
// deze twee mogen globaal (buiten een functie) in je .c bestand staan

/***************************** adc_meet_stroom *******************************/
/* stroom meting met adc, waarde wordt opgeslagen in stroom pointer */
void adc_meet_stroom(float *stroom);

/************************** adc_meet_spanning_voor ***************************/
/* spanning meting met adc, waarde wordt opgeslagen in spanning pointer */
void adc_meet_spanning_voor(float *spanning_voor);

/*************************** adc_meet_spanning_na ****************************/
/* spanning meting met adc, waarde wordt opgeslagen in spanning pointer */
void adc_meet_spanning_na(float *spanning_na);

/******************************* regelaar_init ********************************/
/* initaliseert adc voor het uitmeten van de adc kanalen */
void adc_init();

/******************************* regelaar_open ********************************/
/* open de adc handle voor uitlezen */
void adc_open();

/******************************* regelaar_close *******************************/
/* sluit de adc handle */
void adc_close();

#endif /* REGELAAR_H_PEE50 */

#endif /* PEE50_ADC_H_ */
