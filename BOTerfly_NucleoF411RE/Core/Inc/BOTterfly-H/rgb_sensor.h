/*
 * rgb_sensor.h
 *
 *  Created on: Dec 3, 2021
 *      Author: jerem
 */

#ifndef INC_BOTTERFLY_H_RGB_SENSOR_H_
#define INC_BOTTERFLY_H_RGB_SENSOR_H_

#include "stdio.h"
#include "gpio.h"
#include "tim.h"

// Example at the end of this file


/*************** STRUCTURE ***************/

typedef struct RGB_it_struct{
	uint8_t flag;
	uint8_t colorFilter;
	uint8_t isFirstCaptured;
	uint16_t icVal1;
	uint16_t icVal2;
	uint16_t difference;
	float frequency;
}RGB_it_struct;

typedef struct RGB_struct{
	// Input Capture (Timer)
	TIM_HandleTypeDef* Timer_Handle;
	uint32_t Timer_Channel;

	// Input Param Pins (GPIO Out)
	GPIO_TypeDef* OutputEnable_GPIOx;
	uint16_t OutputEnable_GPIO_Pin;
	GPIO_TypeDef* LED_GPIOx;
	uint16_t LED_GPIO_Pin;

	// Input OutFreq Pins (GPIO Out)
	GPIO_TypeDef* OutFreq1_GPIOx;
	uint16_t OutFreq1_GPIO_Pin;
	GPIO_TypeDef* OutFreq2_GPIOx;
	uint16_t OutFreq2_GPIO_Pin;

	// Input ColorFilter Pins (GPIO Out)
	GPIO_TypeDef* ColorFilter1_GPIOx;
	uint16_t ColorFilter1_GPIO_Pin;
	GPIO_TypeDef* ColorFilter2_GPIOx;
	uint16_t ColorFilter2_GPIO_Pin;

	// Color filter value
	uint16_t red;
	uint16_t green;
	uint16_t blue;

	// Result
	uint8_t isFloorRed; // 1 if yes, else 0

	// Useful to treat the IT
	struct RGB_it_struct it;

}RGB_struct;


/***************   DEFINE   ***************/

#define RGB_ERROR_NONE 0
#define RGB_ERROR_FILTER 1
#define RGB_ERROR_OF_SCALING 2


// Color filter
#define RGB_RED 0
#define RGB_BLUE 1
#define RGB_GREEN 2
#define RGB_CLEAR 3

// Output frequency range
#define RGB_OF_FULL_RANGE 100
#define RGB_OF_20_RANGE 20
#define RGB_OF_02_RANGE 2
#define RGB_OF_POWER_DOWN 0

// Timer - calcul frequence
#define RGB_APBCLOCK 80000000
#define RGB_PRESCALER 80

extern float frequency;


/*************** PROTOTYPES ***************/

uint8_t RGB_Init_SetTimer(RGB_struct* rgbSensor, TIM_HandleTypeDef *htim, uint32_t Channel);

uint8_t RGB_Init_SetParamGPIOs(RGB_struct* rgbSensor, GPIO_TypeDef* OutputEnable_GPIOx, uint16_t OutputEnable_GPIO_Pin,
		GPIO_TypeDef* LED_GPIOx, uint16_t LED_GPIO_Pin);

uint8_t RGB_Init_SetOutFreqGPIOs(RGB_struct* rgbSensor, GPIO_TypeDef* OutFreq1_GPIOx, uint16_t OutFreq1_GPIO_Pin,
		GPIO_TypeDef* OutFreq2_GPIOx, uint16_t OutFreq2_GPIO_Pin);

uint8_t RGB_Init_SetColorFilterGPIOs(RGB_struct* rgbSensor, GPIO_TypeDef* ColorFilter1_GPIOx, uint16_t ColorFilter1_GPIO_Pin,
		GPIO_TypeDef* ColorFilter2_GPIOx, uint16_t ColorFilter2_GPIO_Pin);

uint8_t RGB_Init(RGB_struct* rgbSensor);

uint8_t RGB_SetFilter(RGB_struct* rgbSensor, uint8_t color);
uint8_t RGB_SetOFscaling(RGB_struct* rgbSensor, uint8_t scale);

uint8_t RGB_IsTheFloorRed(RGB_struct* rgbSensor);


/***************  EXAMPLE  ***************/

//#include "BOTterfly-H/config.h"
//
//int main()
//{
//	RGB_Init_SetParamGPIOs(&RGB_Sensor, RGB_OE_GPIO_Port, RGB_OE_Pin, RGB_LED_GPIO_Port, RGB_LED_Pin);
//	RGB_Init_SetOutFreqGPIOs(&RGB_Sensor, RGB_S0_GPIO_Port, RGB_S0_Pin, RGB_S1_GPIO_Port, RGB_S1_Pin);
//	RGB_Init_SetColorFilterGPIOs(&RGB_Sensor, RGB_S2_GPIO_Port, RGB_S2_Pin, RGB_S3_GPIO_Port, RGB_S3_Pin);
//
//	RGB_Init(&RGB_Sensor);
//
//	while(1){
//
//	}
//	return 0;
//}

#endif /* INC_BOTTERFLY_H_RGB_SENSOR_H_ */
