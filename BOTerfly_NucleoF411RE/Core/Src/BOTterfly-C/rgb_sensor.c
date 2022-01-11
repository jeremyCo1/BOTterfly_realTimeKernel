/*
 * rgb_sensor.c
 *
 *  Created on: 8 déc. 2021
 *      Author: jerem
 */

#include "BOTterfly-H/rgb_sensor.h"


uint8_t RGB_Init_SetTimer(RGB_struct* rgbSensor, TIM_HandleTypeDef *htim, uint32_t Channel){
	rgbSensor->Timer_Handle = htim;
	rgbSensor->Timer_Channel = Channel;

	//HAL_TIM_IC_Start_IT(rgbSensor->Timer_Handle, rgbSensor->Timer_Channel);
	return 0;
}

uint8_t RGB_Init_SetParamGPIOs(RGB_struct* rgbSensor, GPIO_TypeDef* OutputEnable_GPIOx, uint16_t OutputEnable_GPIO_Pin,
		GPIO_TypeDef* LED_GPIOx, uint16_t LED_GPIO_Pin){
	rgbSensor->OutputEnable_GPIOx = OutputEnable_GPIOx;
	rgbSensor->OutputEnable_GPIO_Pin = OutputEnable_GPIO_Pin;
	rgbSensor->LED_GPIOx = LED_GPIOx;
	rgbSensor->LED_GPIO_Pin = LED_GPIO_Pin;

	return 0;
}

uint8_t RGB_Init_SetOutFreqGPIOs(RGB_struct* rgbSensor, GPIO_TypeDef* OutFreq1_GPIOx, uint16_t OutFreq1_GPIO_Pin,
		GPIO_TypeDef* OutFreq2_GPIOx, uint16_t OutFreq2_GPIO_Pin){
	rgbSensor->OutFreq1_GPIOx = OutFreq1_GPIOx;
	rgbSensor->OutFreq1_GPIO_Pin = OutFreq1_GPIO_Pin;
	rgbSensor->OutFreq2_GPIOx = OutFreq2_GPIOx;
	rgbSensor->OutFreq2_GPIO_Pin = OutFreq2_GPIO_Pin;

	return 0;
}

uint8_t RGB_Init_SetColorFilterGPIOs(RGB_struct* rgbSensor, GPIO_TypeDef* ColorFilter1_GPIOx, uint16_t ColorFilter1_GPIO_Pin,
		GPIO_TypeDef* ColorFilter2_GPIOx, uint16_t ColorFilter2_GPIO_Pin){
	rgbSensor->ColorFilter1_GPIOx = ColorFilter1_GPIOx;
	rgbSensor->ColorFilter1_GPIO_Pin = ColorFilter1_GPIO_Pin;
	rgbSensor->ColorFilter2_GPIOx = ColorFilter2_GPIOx;
	rgbSensor->ColorFilter2_GPIO_Pin = ColorFilter2_GPIO_Pin;

	return 0;
}

uint8_t RGB_Init(RGB_struct* rgbSensor){

	rgbSensor->it.flag = 0;
	rgbSensor->it.isFirstCaptured = 0;
	rgbSensor->it.icVal1 = 0;
	rgbSensor->it.icVal2 = 0;
	rgbSensor->it.difference = 0;
	rgbSensor->it.frequency = 0;

	// OE : Output Enable -> DISABLE
	HAL_GPIO_WritePin(rgbSensor->OutputEnable_GPIOx, rgbSensor->OutputEnable_GPIO_Pin, GPIO_PIN_SET);
	HAL_Delay(200);

	RGB_SetOFscaling(rgbSensor, RGB_OF_02_RANGE);
	RGB_SetFilter(rgbSensor, RGB_RED);

	// Turn on the LEDs to lights the floor
	HAL_GPIO_WritePin(rgbSensor->LED_GPIOx, rgbSensor->LED_GPIO_Pin, GPIO_PIN_SET);

	// OE : Output Enable -> ENABLE
	HAL_Delay(200);
	HAL_GPIO_WritePin(rgbSensor->OutputEnable_GPIOx, rgbSensor->OutputEnable_GPIO_Pin, GPIO_PIN_RESET);

	return 0;
}

uint8_t RGB_SetFilter(RGB_struct* rgbSensor, uint8_t color){
	uint8_t status = RGB_ERROR_NONE;

	switch(color)
	{
	case RGB_RED:
		HAL_GPIO_WritePin(rgbSensor->ColorFilter1_GPIOx, rgbSensor->ColorFilter1_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(rgbSensor->ColorFilter2_GPIOx, rgbSensor->ColorFilter2_GPIO_Pin, GPIO_PIN_RESET);
		break;
	case RGB_BLUE:
		HAL_GPIO_WritePin(rgbSensor->ColorFilter1_GPIOx, rgbSensor->ColorFilter1_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(rgbSensor->ColorFilter2_GPIOx, rgbSensor->ColorFilter2_GPIO_Pin, GPIO_PIN_SET);
		break;
	case RGB_GREEN:
		HAL_GPIO_WritePin(rgbSensor->ColorFilter1_GPIOx, rgbSensor->ColorFilter1_GPIO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(rgbSensor->ColorFilter2_GPIOx, rgbSensor->ColorFilter2_GPIO_Pin, GPIO_PIN_SET);
		break;
	case RGB_CLEAR:
		HAL_GPIO_WritePin(rgbSensor->ColorFilter1_GPIOx, rgbSensor->ColorFilter1_GPIO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(rgbSensor->ColorFilter2_GPIOx, rgbSensor->ColorFilter2_GPIO_Pin, GPIO_PIN_RESET);
		break;
	default:
		status = RGB_ERROR_FILTER;
		printf("RGB - Error setFilter : %d\r\n", status);
	}
	rgbSensor->it.colorFilter = color;

	return status;
}

uint8_t RGB_SetOFscaling(RGB_struct* rgbSensor, uint8_t scale){
	uint8_t status = RGB_ERROR_NONE;

	switch(scale)
	{
	case 0:
		HAL_GPIO_WritePin(rgbSensor->OutFreq1_GPIOx, rgbSensor->OutFreq1_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(rgbSensor->OutFreq2_GPIOx, rgbSensor->OutFreq2_GPIO_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(rgbSensor->OutFreq1_GPIOx, rgbSensor->OutFreq1_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(rgbSensor->OutFreq2_GPIOx, rgbSensor->OutFreq2_GPIO_Pin, GPIO_PIN_SET);
		break;
	case 20:
		HAL_GPIO_WritePin(rgbSensor->OutFreq1_GPIOx, rgbSensor->OutFreq1_GPIO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(rgbSensor->OutFreq2_GPIOx, rgbSensor->OutFreq2_GPIO_Pin, GPIO_PIN_RESET);
		break;
	case 100:
		HAL_GPIO_WritePin(rgbSensor->OutFreq1_GPIOx, rgbSensor->OutFreq1_GPIO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(rgbSensor->OutFreq2_GPIOx, rgbSensor->OutFreq2_GPIO_Pin, GPIO_PIN_SET);
		break;
	default:
		status = RGB_ERROR_OF_SCALING;
		printf("RGB - Error setOFscaling : %d\r\n", status);
	}

	return status;
}


uint8_t RGB_IsTheFloorRed(RGB_struct* rgbSensor){
	uint8_t status = RGB_ERROR_NONE;

	rgbSensor->it.difference = rgbSensor->it.icVal2 - rgbSensor->it.icVal1;
	float refClock = RGB_APBCLOCK/(RGB_PRESCALER);
	rgbSensor->it.frequency = refClock/rgbSensor->it.difference;

	switch(rgbSensor->it.colorFilter)
	{
	case RGB_RED:
		rgbSensor->red = (uint16_t)rgbSensor->it.frequency;
		rgbSensor->it.colorFilter = RGB_GREEN;
		RGB_SetFilter(rgbSensor, rgbSensor->it.colorFilter);
		break;
	case RGB_GREEN:
		rgbSensor->green = (uint16_t)rgbSensor->it.frequency;
		rgbSensor->it.colorFilter = RGB_BLUE;
		RGB_SetFilter(rgbSensor, rgbSensor->it.colorFilter);
		break;
	case RGB_BLUE:
		rgbSensor->blue = (uint16_t)rgbSensor->it.frequency;
		rgbSensor->it.colorFilter = RGB_RED;
		RGB_SetFilter(rgbSensor, rgbSensor->it.colorFilter);
		break;
	default:
		printf("RGB - Error setFilter\r\n");
	}
	if((rgbSensor->red >= (1.5*rgbSensor->green)) & (rgbSensor->red >= (1.5*rgbSensor->blue))){
		rgbSensor->isFloorRed = 1;
	}else{
		rgbSensor->isFloorRed = 0;
	}

	return status;
}



