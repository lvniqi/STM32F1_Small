/*
 * AD.h
 *
 *  Created on: 2014Äê6ÔÂ9ÈÕ
 *      Author: lvniqi
 */

#ifndef AD_H_
#define AD_H_
#include "common.h"
#include "Sequeue.h"
#include "PINGPANG.h"
#define ADC_BUFFER_LEN 7000
extern void ADC1_GPIO_Config(void);
extern void ADC1_Mode_Config(void);
extern void ADC1_Init(void);
extern void Ad_Get_Service();
#define Adc_Calibaration(ADCx)	\
{	\
	/* Enable ADC1 reset calibaration register */	\
	ADC_ResetCalibration(ADCx);	\
	/* Check the end of ADC1 reset calibration register */	\
	while (ADC_GetResetCalibrationStatus(ADCx));	\
	/* Start ADC1 calibaration */	\
	ADC_StartCalibration(ADCx);	\
	/* Check the end of ADC1 calibration */	\
	while (ADC_GetCalibrationStatus(ADCx));	\
}

extern u16 _ADC_BUFFER[ADC_BUFFER_LEN];
extern int_sequeue ADC_BUFFER;
#endif /* AD_H_ */
