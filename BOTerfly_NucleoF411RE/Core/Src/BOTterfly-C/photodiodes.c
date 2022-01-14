#include <BOTterfly-H/photodiodes.h>

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

uint8_t Ph_Init()
{
	//Ph_GetAverageN(Ph_Data, N_NOISE);
	Ph_GetMeasure(Ph_Data);
	Ph_GetMeasure(Ph_Data);
	Ph_Angle = 0;
	Ph_Norma = 0;
	for (int i = 0 ; i < 4 ; i++)
	{
		Ph_DataNoise[i] = Ph_Data[i];
	}
	printf("INITFIN PH_DataNoise : %04lu, %04lu, %04lu, %04lu\r\n", Ph_DataNoise[0], Ph_DataNoise[1], Ph_DataNoise[2], Ph_DataNoise[3]);
	return 0;
}

uint8_t Ph_GetMeasureSimple(uint32_t value[])
{
	// Bug si utilisée car printf important dans treatment
	Ph_GetValues(value);
	//printf("PH_Data : %04lu, %04lu, %04lu, %04lu\r\n", value[0], value[1], value[2], value[3]);
	return 0;
}

uint8_t Ph_GetMeasure(uint32_t value[])
{
	Ph_GetAverageN(value, N_MEASURE);
	Ph_Treatment(value);
	//printf("PH_Data : %04lu, %04lu, %04lu, %04lu\r\n", value[0], value[1], value[2], value[3]);
	return 0;
}

uint8_t Ph_GetValues(uint32_t value[])
{
	HAL_ADC_Start_DMA(&hadc1, Ph_DataDMA, 4);// A APPELLER POUR CHAQUE MESURE
	for (int i = 0 ; i < 4 ; i++)
	{
		value[i] = Ph_DataDMA[i];
	}
	// Inversion valeurs 0 et 3 :
	uint32_t Ph_Datatempo = value[0];
	value[0] = value[3];
	value[3] = Ph_Datatempo;

	HAL_ADC_Stop_DMA(&hadc1);
	//return uiAnalogData;
	return 0;
}

uint8_t Ph_GetAverageN(uint32_t value[], int n)
{
	uint32_t Ph_DataTemp[4];
	for (int i = 0 ; i < 4 ; i++)
	{
		Ph_DataTemp[i] = 0;
	}

	for (int i = 0 ; i < n ; i++)
	{
		Ph_GetValues(value);
		for (int j = 0 ; j < 4 ; j++)
		{
			Ph_DataTemp[j] += value[j];
		}
	}

	for (int i = 0 ; i < 4 ; i++)
	{
		value[i] = Ph_DataTemp[i] / n;
	}
	//printf("GetAverageN : PH_Data : %lu, %lu, %lu, %lu - /%d\r\n", value[0], value[1], value[2], value[3], n);
	//return uiAnalogData;
	return 0;
}

uint8_t Ph_Treatment(uint32_t value[])
{
	//char * PrPrintf[50];
	uint32_t Ph_PourPrintf[4] = {value[0], value[1], value[2], value[3]};

	if (0)
	{
		for (int i = 0 ; i < 4 ; i++)
		{
			if ((value[i] - Ph_DataNoise[i] > 0) && (value[i] - Ph_DataNoise[i] < 4096))
			{
				value[i] = (uint32_t)(((float)(value[i]) - (float)(Ph_DataNoise[i])) * 4095 / (4095 - (float)Ph_DataNoise[i]));
			}
			else
			{
				value[i] = 0;
			}
		}
	}
	sprintf(Ph_toprint, "S : %04lu %04lu %04lu %04lu V : %04lu %04lu %04lu %04lu E : %04lu %04lu %04lu %04lu ", value[0], value[1], value[2], value[3], Ph_PourPrintf[0], Ph_PourPrintf[1], Ph_PourPrintf[2], Ph_PourPrintf[3], Ph_DataNoise[0], Ph_DataNoise[1], Ph_DataNoise[2], Ph_DataNoise[3]);
	//return uiAnalogData;
	return 0;
}

int Ph_GetPhMaxValue(uint32_t value[])
{
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
	 Photodiode 1 : -100
	 Photodiode 3 : 100
	 Entre... entre...
	 *
	 */

	int angle = 0;

	// Ranger par max :
	//uint32_t DataPerMAX[3];
	int iMax = 0;
	for (int i = 1 ; i < 3 ; i++) // On ne prend en compte que les 3 premières photodiodes moins la première
	{
		if (value[i] > value[iMax])
		{
			iMax = i;
		}
	}

	if (iMax == 1) // Si le max est en face :
	{
		int dif1 = value[1] - value[0];
		int dif2 = value[1] - value[2];
		int dif = dif1 - dif2;
		angle = (int)(100 * (float)((float)dif / 4095));
	}
	else
	{
		angle = Ph_GetBestAngleAncien(value);
	}

	angle = 40 * angle / 100;

	Ph_Angle = angle;
	return angle;
}

int Ph_GetBestAngleAncien(uint32_t value[])
{
	/*
	 *
	 ANGLE EXPLANATION :
	 Devant : 0
	 Photodiode 1 : -100
	 Photodiode 3 : 100
	 Entre... entre...
	 *
	 */

	int angle = 0;

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

	if (imax1 + imax2 == 2) // 0 et 2 --> Deux sources lumineuses : gauche et droite
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
	else // Milieu avec côté (On considère qu'on a le max1 à gauche et le max2 au milieu)
	{
		if (value[imax1] <= value[imax2] + DELTA_A) // Très proches (à 100 points près)
		{//A
			angle = -50;
		}
		else
		{
			if (((4095 - DELTA_B) <= value[imax1]) && (value[imax1] <= 4095))
			{//B
				angle = -100;
			}
			else
			{//C
				angle = -(int)(100 * (value[imax1] - value[imax2]) / value[imax1]);
			}
		}

		// TRANSPOSITION DE L'ANGLE EN FONCTION DES POSITION DES MAX :
		if ((imax1 == 0) || (imax2 == 0)) // Sur la gauche
		{
			if (imax1 == 1) // Max au milieu
			{
				angle = -100 - angle;
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
				angle = 100 + angle;
			}
			else // Max à droite
			{
				angle = -angle;
			}
		}
	}

	// AFFICHER :
	//	char * temp = " Angle : ";
	//	itoa(angle, temp, 10);
	//	strcat(toprint, temp);
	//	strcat(toprint, "\r\n");
	//	char tprnt[50];
	//	sprintf(tprnt, " Angle : %d", angle);
	//	strcat(toprint, tprnt);

	return angle;
}

int Ph_GetNorma(uint32_t value[])
{
	//Ph_Norma = 100 - (0.9*value[0] + 1.2*value[1] + 0.9*value[2])/(3*40.95);
	uint32_t Ph_DataTemp[3];
	for (int i = 0 ; i < 3 ; i++)
	{
		Ph_DataTemp[i] = value[i] - Ph_DataNoise[i];
	}
	int max = 0;
	for (int i = 1 ; i < 3 ; i++)
	{
		if (Ph_DataTemp[i] > Ph_DataTemp[max])
		{
			max = i;
		}
	}
	Ph_Max_Tr = Ph_DataTemp[max];
	Ph_Norma = (uint32_t)(4791.6*exp(-0.001*value[max]));
	return Ph_Norma;
}

int Ph_GetPercentGood(uint32_t value[])
{
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

int Ph_Print(uint8_t mode)
{
	if (mode == 0)
	{
		//sprintf(Ph_toprint, "%s Norme : %lu Angle : %ld", Ph_toprint, Ph_Norma, Ph_Angle);

		printf("Ph0 : %lu  Ph1 : %lu  Ph2 : %lu Angle : %ld    Norme : %lu \r\n",Ph_Data[0],Ph_Data[1],Ph_Data[2], Ph_Angle,Ph_Norma);
	}
	return 0;
}



