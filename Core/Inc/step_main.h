/*
 * step_main.h
 *
 *  Created on: May 19, 2023
 *      Author: Luca Bonori
 */

#ifndef INC_STEP_MAIN_H_
#define INC_STEP_MAIN_H_

#define stepsperrev 4096
#define anglepersequence 0.703125 // 360 = 512 sequences

void delay (int);
void stepper_set_rpm (int);  // Set rpm--> max 13, min 1,,,  went to 14 rev/min
void stepper_half_drive (int);
void stepper_step_angle (float, int, int);

#endif /* INC_STEP_MAIN_H_ */
