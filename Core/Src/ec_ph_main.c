#include "ec_ph_main.h"
#include "gpio.h"
#include "string.h"
#include "usart.h"
#include "adc.h"
#include "stdio.h"
#include "tim.h"

// Definizione variabili EC meter

float EC = 0;
float adEC = 0;
float VdropEC = 0;
float RWater = 0;

// Definizione Variabili Ph meter

float adPH = 0;
float VdropPH = 0;
float PH = 0;

// Funzionamento EC meter
float ec_read(void){ //funzione dichiarata nel .h e chiamata dal main

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //Imposto il pin ECPower(PC0 -> A5) a livello alto: 3.3v

	HAL_Delay(6000); //Attendo che il PC0 si stabilizzi a 3.3v

	// Seleziona il canale ADC desiderato (CHANNEL2)
	MX_ADC3_Init1_Channel2();

	// UTILIZZO ADC1 SU PC0 -> A5
//	HAL_ADC_Start(&hadc1); // avvia la conversione ADC da EC_Read(PC1 -> A4)
//	HAL_ADC_PollForConversion(&hadc1, 100); // attendi la fine della conversione
//	adEC = HAL_ADC_GetValue(&hadc1);//Leggo il valore analogico dal pin EC_Read (sono tot bit)

	HAL_ADC_Start(&EC_PH_ADC); // avvia la conversione ADC da EC_Read(PC1 -> A4)
	HAL_ADC_PollForConversion(&EC_PH_ADC, 100); // attendi la fine della conversione
	adEC = HAL_ADC_GetValue(&EC_PH_ADC);//Leggo il valore analog

	char *q="adEC: ";
	HAL_UART_Transmit(&huart2, (uint8_t*)q, strlen(q), 0xFFFF);
	char i[32];
	sprintf(i, "%.2f\n", adEC); //converte float a stringa
	HAL_UART_Transmit(&huart2, (uint8_t*)i, strlen(i), 0xFFFF);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); //Imposto il pin ECPower(PC0 -> A5) a livello basso: 0v

	VdropEC= (VinEC*adEC) / 1024.0; //converto tot bit(adEC) in tensione(VdropEC) (precision_ADC = 2^10 -> = 1024)
	RWater = (VdropEC*R1) / (VinEC-VdropEC); // prova ad aumentare la precisione a 2^12
	EC = 1000/ (RWater*K1); //mS/cm

	//## stampo EC pre gestione ##
	char *t="Conducibilita' elettrica pre-gestione[mS/cm]: ";
	HAL_UART_Transmit(&huart2, (uint8_t*)t, strlen(t), 0xFFFF);
	char r[32];
	sprintf(r, "%.2f\n", EC); //converte float a stringa
	HAL_UART_Transmit(&huart2, (uint8_t*)r, strlen(r), 0xFFFF);

    // GESTIONE DATI EC
	if (EC <= 0.9)
	{
	    EC = EC - 0.3;
	}
	else if (EC > 1.5 && EC <= 1.6)
	{
	    EC = EC + 0.25;
	}
	else if (EC > 1.6 && EC <= 1.7)
	{
	    EC = EC + 0.35;
	}

//##STAMPO A TERMINALE##
	char *mgs="Conducibilita' elettrica[mS/cm]: ";
	HAL_UART_Transmit(&huart2, (uint8_t*)mgs, strlen(mgs), 0xFFFF);
	char msg[32];
	sprintf(msg, "%.2f\n", EC); //converte float a stringa
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFFFF);

	HAL_Delay(2000); // attendo che il PA7 si stabilizzi a 0v

	return EC;
}

float ph_read(void){ //legge PH

//	hadc3.Instance->SQR3 = PH_CHANNEL; //Setto canale3 dell'ADC3

	MX_ADC3_Init1_Channel3();

	HAL_ADC_Start(&EC_PH_ADC); // Avvia la conversione ADC
	HAL_ADC_PollForConversion(&EC_PH_ADC, 100);// Attendi la fine della conversione
	adPH = HAL_ADC_GetValue(&EC_PH_ADC); // Leggo il valore analogico dal pin PH (sono tot bit)

	char *r="adPH: ";
	HAL_UART_Transmit(&huart2, (uint8_t*)r, strlen(r), 0xFFFF);
	char t[32];
	sprintf(t, "%.2f\n", adPH); //converte float a stringa
	HAL_UART_Transmit(&huart2, (uint8_t*)t, strlen(t), 0xFFFF);

	VdropPH = (VinPH*adPH) / 1024.0; //converto bit(adPH) in tensione(VdropPH) (precision_ADC = 2^10 -> = 1024)

	//##STAMPO A TERMINALE##
	char *mgs="VdropPH: ";
	HAL_UART_Transmit(&huart2, (uint8_t*)mgs, strlen(mgs), 0xFFFF);
	char msg[32];
	sprintf(msg, "%.2f\n", VdropPH); //converte float a stringa
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFFFF);

	if (VdropPH == 0)
		PH = 0;
	else if (VdropPH > 0 && VdropPH <= ph_interval)
		PH = 1;
	else if (VdropPH > ph_interval && VdropPH <= ph_interval*2)
		PH = 2;
	else if (VdropPH > ph_interval*2 && VdropPH <= ph_interval*3)
		PH = 3;
	else if (VdropPH > ph_interval*3 && VdropPH <= ph_interval*4)
		PH = 4;
	else if (VdropPH > ph_interval*4 && VdropPH <= ph_interval*5)
		PH = 5;
	else if (VdropPH > ph_interval*5 && VdropPH <= ph_interval*6)
		PH = 6;
	else if (VdropPH > ph_interval*6 && VdropPH <= ph_interval*7)
		PH = 7;
	else if (VdropPH > ph_interval*7 && VdropPH <= ph_interval*8)
		PH = 8;
	else if (VdropPH > ph_interval*8 && VdropPH <= ph_interval*9)
		PH = 9;
	else if (VdropPH > ph_interval*9 && VdropPH <= ph_interval*10)
		PH = 10;
	else if (VdropPH > ph_interval*10 && VdropPH <= ph_interval*11)
		PH = 11;
	else if (VdropPH > ph_interval*11 && VdropPH <= ph_interval*12)
		PH = 12;
	else if (VdropPH > ph_interval*12 && VdropPH <= ph_interval*13)
		PH = 13;
	else
	{
		PH = 14;
	}

	char *ms="PH: ";
	HAL_UART_Transmit(&huart2, (uint8_t*)ms, strlen(ms), 0xFFFF);
	char mg[32];
	sprintf(mg, "%.2f\n\n", PH); //converte float a stringa
	HAL_UART_Transmit(&huart2, (uint8_t*)mg, strlen(mg), 0xFFFF);

	HAL_Delay(1000);

	return PH;
}

void MX_ADC3_Init1_Channel2()
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc3.Init.Resolution = ADC_RESOLUTION_10B;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */
}

void MX_ADC3_Init1_Channel3()
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc3.Init.Resolution = ADC_RESOLUTION_10B;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */
}
