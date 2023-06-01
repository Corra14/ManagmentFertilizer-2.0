/*
 * relay_pump.c
 *
 *  Created on: May 22, 2023
 *      Author: Luca Bonori
 */

#include "relay_pump.h"
#include "gpio.h"
#include "string.h"
#include "usart.h"
#include "adc.h"
#include "tim.h"

 void pump_control()
 {
	HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_SET);
	HAL_Delay(Interval);
	HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_RESET);
	HAL_Delay(Interval);
 }
