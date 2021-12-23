#include "photodiodes.h"

//unsigned int GetValuePh1(int NumPh)
//{
//	NumPh--;
//	//int Channel[4] = {0, 0,0,0};//&hadc2, &hadc3, &hadc4};
//	HAL_ADC_Start(&hadc1); // Commence l'ADC
//	HAL_ADC_PollForConversion(&hadc1,0xFFFFFFFU); // ??????
//	uiAnalogData = HAL_ADC_GetValue(&hadc1); // Fetch valeur ADC
//	HAL_ADC_Stop(&hadc1);
//
//	uint16_t adc_6, adc_7, adc_9;
//	HAL_ADC_Start(&hadc1);
//	HAL_ADC_PollForConversion(&hadc1, 5);
//	adc_6 = HAL_ADC_GetValue(&hadc1);
//	HAL_ADC_Start(&hadc1);
//	HAL_ADC_PollForConversion(&hadc1, 5);
//	adc_7 = HAL_ADC_GetValue(&hadc1);
//	HAL_ADC_Start(&hadc1);
//	HAL_ADC_PollForConversion(&hadc1, 5);
//	adc_9 = HAL_ADC_GetValue(&hadc1);
//
////	HAL_ADC_Start(&hadc1); // Commence l'ADC
////	HAL_ADC_PollForConversion(&hadc1,0xFFFFFFFU); // ??????
////	uiAnalogData = HAL_ADC_GetValue(&hadc1); // Fetch valeur ADC
////	HAL_ADC_Stop(&hadc1);
//	return uiAnalogData;
//}



	   /*
	   BRANCHEMENT PHOTODIODES :
	   1 : Gauche
	   2 : Milieu
	   3 : Droite
	   4 : Haut-Milieu
	   */



uint8_t Ph_GetValues(uint32_t value[])
{
	HAL_ADC_Start_DMA(&hadc1, value, 4);
	//int V[4] = {value[0], value[1],value[2],value[3]};
	//return uiAnalogData;
	return 0;
}

uint8_t Ph_Traitement(uint32_t value[])
{
	// On retire les valeurs à vide - 2000
	value[0] -= 16;
	value[1] -= 85;
	value[2] -= 85;
	value[3] -= 93;
	//int V[4] = {value[0], value[1],value[2],value[3]};
	//return uiAnalogData;
	return 0;
}

int Ph_GetPhMaxValue(uint32_t value[])
{
	//int V[4] = {value[0], value[1],value[2],value[3]};// Pour visualiser dans le mode débug
	//GetValues(value);
	int max = 0;
	int indice = 0;
	for (int i = 0; i < 4; i++)
	{
		if (value[i] > max)
		{
			max = value[i];
			indice = i;
		}
	}
	indice++;
	return indice;
}

int Ph_GetBestAngle(uint32_t value[])
{
	/*
	 *
	 ANGLE EXPLANATION :
	 Devant : 0
	 Photodiode 1 : 100
	 Photodiode 3 : -100
	 Entre... entre...
	 *
	 */

	int angle = 0;
	//uint32_t V[4] = {value[0], value[1],value[2],value[3]};// Pour visualiser dans le mode débug

	//GetValues(value);

	// RECHERCHE DES 2 PREMIERS MAXIMUMS :

	int imax1 = 0;
	int imax2 = 0;
	int maxtemp = 0;//////////////////////////////////////////////////////////////////////////PB
	for (int i = 0 ; i < 3 ; i++) // On ne prend en compte que les 3 premières photodiodes
	{
		if (value[i] > maxtemp)
		{
			maxtemp = value[i];
			imax1 = i;
		}
	}
	maxtemp = 0;
	for (int i = 0 ; i < 3 ; i++) // On ne prend en compte que les 3 premières photodiodes
	{
		if (i != imax1)
		{
			if (value[i] > maxtemp)
			{
				maxtemp = value[i];
				imax2 = i;
			}
		}
	}

	// ASSIGNATION DE L'ANGLE :

	if ((imax1 == 0) && (imax2 == 2)) // Deux sources lumineuses : gauche et droite
	{
		if (imax1 > imax2)
		{
			angle = 100;
		}
		else
		{
			angle = -100;
		}
	}
	else // Milieu avec côté
	{
		if (value[imax1] <= value[imax2] + 100) // Très proches (à 100 points près)
		{//A
			angle = 50;
		}
		else
		{
			if ((3895 <= value[imax1]) && (value[imax1] <= 4095))
			{//B
				angle = 100;
			}
			else
			{//C
				angle = (int)(100 * (value[imax1] - value[imax2]) / value[imax1]);
			}
		}

		// TRANSPOSITION DE L'ANGLE EN FONCTION DES POSITION DES MAX :
		if ((imax1 == 0) || (imax2 == 0)) // Sur la gauche
		{
			if (imax1 == 1) // Max au milieu
			{
				angle = 100 - angle;
			}
			else // Max à gauche
			{
				// On est bon
			}
		}
		else // Sur la droite
		{
			if (imax1 == 1) // Max au  milieu
			{
				angle = -(100 - angle);
			}
			else // Max à droite
			{
				angle = -angle;
			}
		}
	}
	return angle;
}

int Ph_GetPercentGood(uint32_t value[])
{
	//uint32_t V[4] = {value[0], value[1],value[2],value[3]};// Pour visualiser dans le mode débug
	//GetValues(value);
	float good;
	good = value[3]/40.95; // ATTENTION PROBLÈME SIGNAL À ZERO ! À RÉGLER EN PHASE DE TESTS !
	if (good < 0)
	{
		good = 0;
	}
	else if (good > 100)
	{
		good = 100;
	}
	return (int)good;
}




