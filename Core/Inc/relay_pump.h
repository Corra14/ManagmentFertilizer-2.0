/*
 * relay_pump.h
 *
 *  Created on: May 22, 2023
 *      Author: Luca Bonori
 */

#ifndef INC_RELAY_PUMP_H_
#define INC_RELAY_PUMP_H_

#define Relay_GPIO_Port GPIOB
#define Relay_Pin GPIO_PIN_6
#define Interval 5000

void pump_control(void);

#endif /* INC_RELAY_PUMP_H_ */
