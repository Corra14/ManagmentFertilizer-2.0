#ifndef INC_EC_PH_MAIN_H_
#define INC_EC_PH_MAIN_H_

#include "gpio.h"
#include "string.h"
#include "usart.h"
#include "adc.h"
#include "stdio.h"

#define EC_PH_ADC hadc3
#define EC_CHANNEL 2
#define PH_CHANNEL 3

#define K1 1 //Coeff. calibrazione sonda EC

#define R1 500 // Valore resistenza sonda EC
#define VinEC 3.3
#define VinPH 3.3

#define ph_interval 0.2357 // prima era 0.35 N.B il pin legge max 3.3v non 5v

float ec_read(void); //read ec function
float ph_read(void); //read ph function
void MX_ADC3_Init1_Channel2(void);
void MX_ADC3_Init1_Channel3(void);

#endif /* INC_EC_PH_MAIN_H_ */
