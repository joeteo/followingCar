/*
 * selfDriving.h
 *
 *  Created on: 2022. 7. 7.
 *      Author: J
 */

#ifndef INC_SELFDRIVING_H_
#define INC_SELFDRIVING_H_

#include "motor.h"

typedef enum {
	QUADRANT_ONE = 1,
	QUADRANT_TWO,
	QUADRANT_THREE,
	QUADRANT_FOUR
}_Quadrant;

typedef enum{
	FALSE,
	TRUE
}_bool;

void SelfDriving();
CONTROLLER_SIGNAL chkCWCCW(float target_angle, float car_angle);

#endif /* INC_SELFDRIVING_H_ */
