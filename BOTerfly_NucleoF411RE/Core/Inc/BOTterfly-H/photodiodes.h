/*
 * photodiodes.h
 *
 *  Created on: Dec 3, 2021
 *      Author: quent
 */

#ifndef INC_BOTTERFLY_H_PHOTODIODES_H_
#define INC_BOTTERFLY_H_PHOTODIODES_H_

	   /*
	   BRANCHEMENT PHOTODIODES :
	   1 : Gauche
	   2 : Milieu
	   3 : Droite
	   4 : Inutile
	   */
/* Includes ------------------------------------------------------------------*/

#include "stdint.h"
#include "adc.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <math.h>

/* Define ------------------------------------------------------------------*/

#define N_NOISE 1
#define N_MEASURE 1
#define DELTA_A 100
#define DELTA_B 200

/* Variables Globales ------------------------------------------------------------------*/

uint32_t Ph_DataDMA[4];
uint32_t Ph_Data[4];
uint32_t Ph_DataNoise[4];
uint32_t Ph_Max_Tr;

int32_t Ph_Angle; // Compris entre -100 et 100 --> -100 = Ph_Gauche, 0 = Ph_Milieu, 100 = PhDroite.
uint32_t Ph_Norma; // Comprise entre 0 et 100, plus elle est élevée, plus le robot est loin de la source de lumière, et inversement.

char Ph_toprint[150];

/* Fonctions ------------------------------------------------------------------*/

uint8_t Ph_Init();

uint8_t Ph_GetMeasureSimple(uint32_t value[]);

uint8_t Ph_GetMeasure(uint32_t value[]);

uint8_t Ph_GetValues(uint32_t value[]);

uint8_t Ph_GetAverageN(uint32_t value[], int n);

uint8_t Ph_Treatment(uint32_t value[]);

int Ph_GetPhMaxValue(uint32_t value[]);

int Ph_GetBestAngle(uint32_t value[]);

int Ph_GetBestAngleAncien(uint32_t value[]);

int Ph_GetNorma(uint32_t value[]);

int Ph_GetPercentGood(uint32_t value[]);

int Ph_Print(uint8_t mode);

#endif /* INC_BOTTERFLY_H_PHOTODIODES_H_ */
