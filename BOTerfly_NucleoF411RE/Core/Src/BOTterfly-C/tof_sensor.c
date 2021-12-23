/*
 * tof_sensor.c
 *
 *  Created on: 8 déc. 2021
 *      Author: jerem
 */

#include "BOTterfly-H/tof_sensor.h"
#include "BOTterfly-H/config.h"

uint8_t TOF_Init_SetI2C(VL53L0X_Dev_t* device, I2C_HandleTypeDef *hi2c, uint8_t I2cAddr){
	device->I2cHandle = hi2c;
	device->I2cAddr = I2cAddr;

	return 0;
}

uint8_t TOF_Init_SetGPIOs(VL53L0X_Dev_t* device, GPIO_TypeDef* XSHUT_GPIOx, uint16_t XSHUT_GPIO_Pin,
		GPIO_TypeDef* EXTI_GPIOx, uint16_t EXTI_GPIO_Pin){
	device->XSHUT_GPIOx = XSHUT_GPIOx;
	device->XSHUT_GPIO_Pin = XSHUT_GPIO_Pin;
	device->EXTI_GPIOx = EXTI_GPIOx;
	device->EXTI_GPIO_Pin = EXTI_GPIO_Pin;

	return 0;
}

uint8_t TOF_Init_SetEXTI(VL53L0X_Dev_t* device, IRQn_Type EXTIx_IRQn){
	device->EXTI_IRQn = EXTIx_IRQn;

	return 0;
}

uint8_t TOF_Init(VL53L0X_Dev_t* device){
	for(int i=0; i<TOF_nbOfSensor; i++){
		device[i].I2cDevAddr = 0x52;
		device[i].comms_speed_khz = 400;
		device[i].comms_type = 1;
	}

	// Disable the interruptions
	// Useful for the INITIALIZATION FLOW before changing the address of each device
	for(int i=0; i<TOF_nbOfSensor; i++){
		HAL_GPIO_WritePin(device[i].XSHUT_GPIOx, device[i].XSHUT_GPIO_Pin, GPIO_PIN_RESET);
		HAL_NVIC_DisableIRQ(device[i].EXTI_IRQn);
	}
	HAL_Delay(200);

	// INITIALIZATION FLOW
	for(int i=0; i<TOF_nbOfSensor; i++){
		HAL_GPIO_WritePin(device[i].XSHUT_GPIOx, device[i].XSHUT_GPIO_Pin, GPIO_PIN_SET);
		HAL_Delay(200);
		TOF_InitializationFlow(&device[i], (uint8_t)device[i].EXTI_GPIOx->ODR);
		TOF_SetDeviceAddr(&device[i], device[i].I2cAddr);
		HAL_Delay(200);
	}

	// Enable the Interruptions
	for(int i=0; i<TOF_nbOfSensor; i++){
		HAL_NVIC_EnableIRQ(device[i].EXTI_IRQn);
	}
	HAL_Delay(200);

	return 0;
}

uint8_t TOF_InitializationFlow(VL53L0X_Dev_t* device, uint8_t interruptPin){
	// Device initialization (~ 40ms)
	TOF_Initialization(device);
	HAL_Delay(50);

	// Calibration data loading (~ 1ms)
	TOF_Calibration(device);
	HAL_Delay(5);

	// System settings (~ 1ms)
	TOF_Settings(device, interruptPin);
	HAL_Delay(5);

	VL53L0X_StartMeasurement(device);

	return 0;
}

uint8_t TOF_Initialization(VL53L0X_Dev_t* device){
	uint8_t status = VL53L0X_ERROR_NONE;

	// DataInit
	if((status = VL53L0X_DataInit(device)) != VL53L0X_ERROR_NONE){
		printf("ToF - Error DataInit : %d\r\n", status);
		return 1;
	}

	// StaticInit
	if((status = VL53L0X_StaticInit(device)) != VL53L0X_ERROR_NONE){
		printf("ToF - Error StaticInit : %d\r\n", status);
		return 1;
	}

	return 0;
}


// SPADs calibration
static uint32_t *refSpadCount; static uint8_t *isApertureSpads;
// Temperature calibration
static uint8_t *pVhvSettings; static uint8_t *pPhaseCal;

uint8_t TOF_Calibration(VL53L0X_Dev_t* device){
	uint8_t status = VL53L0X_ERROR_NONE;

	// SPADs calibration (~ 10ms)
	if((status = VL53L0X_PerformRefSpadManagement(device, refSpadCount, isApertureSpads)) != VL53L0X_ERROR_NONE){
		printf("ToF - Error SPADs calibration : %d\r\n", status);
		return 1;
	}

	// Temperature calibration (~ 40ms)
	if((status = VL53L0X_PerformRefCalibration(device, pVhvSettings, pPhaseCal)) != VL53L0X_ERROR_NONE){
		printf("ToF - Error Temperature calibration : %d\r\n", status);
		return 1;
	}

	return 0;
}

uint8_t TOF_Settings(VL53L0X_Dev_t* device, uint8_t interruptPin){
	uint8_t status = VL53L0X_ERROR_NONE;

	// Device mode
	if((status = VL53L0X_SetDeviceMode(device, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING)) != VL53L0X_ERROR_NONE){
		printf("ToF - Error SetDeviceMode : %d\r\n", status);
		return 1;
	}

	// Set GPIO : Pour interruptions
	status = VL53L0X_SetGpioConfig(device, interruptPin, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING, VL53L0X_GPIOFUNCTIONALITY_NEW_MEASURE_READY, VL53L0X_INTERRUPTPOLARITY_HIGH);
	if(status != VL53L0X_ERROR_NONE){
		printf("ToF - Error SetGPIO : %d %d\r\n",status,interruptPin);
		return 1;
	}

	return 0;
}

uint8_t TOF_SetDeviceAddr(VL53L0X_Dev_t* device, uint8_t new_addr){
	uint8_t status = VL53L0X_ERROR_NONE;

	if((status = VL53L0X_SetDeviceAddress(device, new_addr)) != VL53L0X_ERROR_NONE){
		printf("ToF - Error SetDeviceAddress : %d\r\n", status);
		return 1;
	}
	device->I2cDevAddr = new_addr;

	return 0;
}

uint8_t TOF_GetDeviceInfo(VL53L0X_Dev_t* myDevice, VL53L0X_DeviceInfo_t* deviceInfo){
	uint8_t status = VL53L0X_ERROR_NONE;

	if((status = VL53L0X_GetDeviceInfo(myDevice, deviceInfo)) != VL53L0X_ERROR_NONE){
		printf("ToF - Error GetDeviceInfo : %d\r\n", status);
		return 1;
	}

	printf("VL53L0X_GetDeviceInfo:\r\n");
	printf("Device Name : %s\r\n", deviceInfo->Name);
	printf("Device Type : %s\r\n", deviceInfo->Type);
	printf("Device ID : %s\r\n", deviceInfo->ProductId);
	printf("ProductRevisionMajor : %d\r\n", deviceInfo->ProductRevisionMajor);
	printf("ProductRevisionMinor : %d\r\n", deviceInfo->ProductRevisionMinor);

	return 0;
}

VL53L0X_RangingMeasurementData_t VL53L0X_RangingMeasurementData;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == TOF_Sensor[0].EXTI_GPIO_Pin){
		uint32_t InterruptMask = 0;
		VL53L0X_GetRangingMeasurementData(&TOF_Sensor[0], &VL53L0X_RangingMeasurementData);
		TOF_Sensor[0].rangeMillimeter = VL53L0X_RangingMeasurementData.RangeMilliMeter;
		VL53L0X_ClearInterruptMask(&TOF_Sensor[0], InterruptMask);
	}else
		if(GPIO_Pin == TOF_Sensor[1].EXTI_GPIO_Pin){
			uint32_t InterruptMask = 0;
			VL53L0X_GetRangingMeasurementData(&TOF_Sensor[1], &VL53L0X_RangingMeasurementData);
			TOF_Sensor[1].rangeMillimeter = VL53L0X_RangingMeasurementData.RangeMilliMeter;
			VL53L0X_ClearInterruptMask(&TOF_Sensor[1], InterruptMask);
		}else
			if(GPIO_Pin == TOF_Sensor[2].EXTI_GPIO_Pin){
				uint32_t InterruptMask = 0;
				VL53L0X_GetRangingMeasurementData(&TOF_Sensor[2], &VL53L0X_RangingMeasurementData);
				TOF_Sensor[2].rangeMillimeter = VL53L0X_RangingMeasurementData.RangeMilliMeter;
				VL53L0X_ClearInterruptMask(&TOF_Sensor[2], InterruptMask);
			}
}



