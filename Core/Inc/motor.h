/*
 * motor.h
 *
 *  Created on: Jun 30, 2022
 *      Author: cjhno
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"
#include "tim.h"

#define SPEED 950

typedef enum {
	STOP,
	FORWARD,
	RIGHT,
	BACKWARD,
	LEFT,
	CW,
	CCW
}CONTROLLER_SIGNAL;

void Move(int controlcmd);
void Forward();
void Backward();
void Right();
void Left();
void Cw();
void Ccw();
void Stop();
void Motor_Init();



#endif /* INC_MOTOR_H_ */
