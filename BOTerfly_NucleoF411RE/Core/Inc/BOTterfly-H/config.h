/*
 * config.h
 *
 *  Created on: Dec 3, 2021
 *      Author: jerem
 */

#ifndef INC_BOTTERFLY_H_CONFIG_H_
#define INC_BOTTERFLY_H_CONFIG_H_

#include "tof_sensor.h"
#include "rgb_sensor.h"

/* TOF SENSORS */
#define TOF_nbOfSensor 3

struct VL53L0X_Dev_t TOF_Sensor[TOF_nbOfSensor];

/* RGB SENSORS */
struct RGB_struct RGB_Sensor;

#endif /* INC_BOTTERFLY_H_CONFIG_H_ */

