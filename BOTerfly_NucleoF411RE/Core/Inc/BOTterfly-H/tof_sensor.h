/*
 * tof_sensor.h
 *
 *  Created on: Dec 3, 2021
 *      Author: jerem
 */

#ifndef INC_BOTTERFLY_H_TOF_SENSOR_H_
#define INC_BOTTERFLY_H_TOF_SENSOR_H_

#include <stdio.h>
#include <gpio.h>
#include <i2c.h>

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_device.h"

// Example at the end of this file

/* -------------- STRUCTURE -------------- */

//cf ->   vl53l0x_platform.h   <- file from the API
//
//typedef struct {
//	I2C_HandleTypeDef *I2cHandle;
//	uint8_t I2cAddr;
//	uint16_t  rangeMillimeter;
//	// Inputs Pins (GPIO Out)
//	GPIO_TypeDef* XSHUT_GPIOx;
//	uint16_t XSHUT_GPIO_Pin;
//	// Output Pins (GPIO In)
//	GPIO_TypeDef* EXTI_GPIOx;
//	uint16_t EXTI_GPIO_Pin;
//	IRQn_Type EXTI_IRQn;
//} VL53L0X_Dev_t;
//
//VL53L0X_Dev_t tof_sensor[nbOfSensors];


/* -------------- PROTOTYPE -------------- */

uint8_t TOF_Init_SetI2C(VL53L0X_Dev_t* device, I2C_HandleTypeDef *hi2c, uint8_t I2cAddr);

uint8_t TOF_Init_SetGPIOs(VL53L0X_Dev_t* device, GPIO_TypeDef* XSHUT_GPIOx, uint16_t XSHUT_GPIO_Pin,
		GPIO_TypeDef* EXTI_GPIOx, uint16_t EXTI_GPIO_Pin);

uint8_t TOF_Init_SetEXTI(VL53L0X_Dev_t* device, IRQn_Type EXTIx_IRQn);

uint8_t TOF_Init(VL53L0X_Dev_t* device);

// Initialization Flow
uint8_t TOF_InitializationFlow(VL53L0X_Dev_t* device, uint8_t interruptPin);

// Device initialization
uint8_t TOF_Initialization(VL53L0X_Dev_t* device);

// Device calibration
uint8_t TOF_Calibration(VL53L0X_Dev_t* device);

// System settings
uint8_t TOF_Settings(VL53L0X_Dev_t* device, uint8_t interruptPin);

uint8_t TOF_SetDeviceAddr(VL53L0X_Dev_t* device, uint8_t new_addr);

uint8_t TOF_GetDeviceInfo(VL53L0X_Dev_t* myDevice, VL53L0X_DeviceInfo_t* deviceInfo);



/* --------------  EXAMPLE  -------------- */

//#include "BOTterfly-H/config.h"
//
//int main()
//{
//TOF_Init_SetI2C(&TOF_Sensor[0], &hi2c1, 0x55);
//TOF_Init_SetGPIOs(&TOF_Sensor[0], XSHUT0_GPIO_Port, XSHUT0_Pin, tof_0_GPIO_Port, tof_0_Pin);
//TOF_Init_SetEXTI(&TOF_Sensor[0], tof_0_EXTI_IRQn);
//
//TOF_Init_SetI2C(&TOF_Sensor[1], &hi2c1, 0x58);
//TOF_Init_SetGPIOs(&TOF_Sensor[1], XSHUT1_GPIO_Port, XSHUT1_Pin, tof_1_GPIO_Port, tof_1_Pin);
//TOF_Init_SetEXTI(&TOF_Sensor[1], tof_1_EXTI_IRQn);
//
//TOF_Init_SetI2C(&TOF_Sensor[2], &hi2c1, 0x5b);
//TOF_Init_SetGPIOs(&TOF_Sensor[2], XSHUT2_GPIO_Port, XSHUT2_Pin, tof_2_GPIO_Port, tof_2_Pin);
//TOF_Init_SetEXTI(&TOF_Sensor[2], tof_2_EXTI_IRQn);
//
//TOF_Init(TofSensor);
//
//	while(1){
//		printf("tof_0 : Range %d mm\r\n",TOF_Sensor[0].rangeMillimeter);
//		printf("tof_1 : Range %d mm\r\n",TOF_Sensor[1].rangeMillimeter);
//		printf("tof_2 : Range %d mm\r\n",TOF_Sensor[2].rangeMillimeter);
//		HAL_Delay(250);
//	}
// return 0;
//}
//
//
// // XSHUTx -> GPIO Out
// // tof_x  -> GPIO In EXTI

#endif /* INC_BOTTERFLY_H_TOF_SENSOR_H_ */
