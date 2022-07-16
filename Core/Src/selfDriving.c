/*
 * selfDriving.c
 *
 *  Created on: 2022. 7. 7.
 *      Author: J
 */

#include "selfDriving.h"
#include "gps.h"
#include "bluetooth.h"
#include "HMC5883L.h"
#include <math.h>
#include "stdlib.h"
#define _USE_MATH_DEFINES

extern float headingDegrees;

int distance_long;	// b
int distance_lat;	// a
int distance_c;		// c

double angle;			//변환전 각도
double degree_angle;	//각도 변환 0도 ~
double target_angle;	//자동차가 바라봐야할 목표의 각도

int diffAngle;

//CONTROLLER_SIGNAL rotate=STOP;
CONTROLLER_SIGNAL moveSignal;
uint8_t rotate_flag=0;

extern GPS_t GPS;
extern _DestinationGPS waypointGPS;
_DestinationGPS waypointBefore;

_Quadrant quadrant;

//CONTROLLER_SIGNAL chkCWCCW(float target_angle, float car_angle) {
//	if (car_angle < 180) {
//		double distance_angle = target_angle - car_angle;
//		if (distance_angle > 0 && distance_angle < 180) {
//			return CW;
//		}
//		else {
//			return CCW;
//		}
//	}
//	else {
//		double distance_angle = (target_angle + 360) - car_angle;
//		if (distance_angle < 180 || distance_angle >(car_angle + 360)) {
//			return CW;
//		}
//		else {
//			return CCW;
//		}
//	}
//}

void SelfDriving(){

	  distance_long = (waypointGPS.longitude - GPS.dec_longitude) * 1000000;
	  distance_lat = (waypointGPS.latitude - GPS.dec_latitude) * 1000000;
	  distance_c = sqrt((distance_lat * distance_lat) + (distance_long * distance_long));

	  angle = acos((double)distance_long / distance_c);
	  degree_angle = (double)angle * 180 / M_PI;

	  if (distance_long > 0 && distance_lat > 0) {
		  quadrant = QUADRANT_ONE;
	  }
	  else if (distance_long < 0 && distance_lat > 0) {
		  quadrant = QUADRANT_TWO;
	  }
	  else if (distance_long < 0 && distance_lat < 0) {
		  quadrant = QUADRANT_THREE;
	  }
	  else if (distance_long > 0 && distance_lat < 0) {
		  quadrant = QUADRANT_FOUR;
	  }



	  switch (quadrant) {
	  case QUADRANT_ONE:
		  target_angle = 90 - degree_angle;
		  break;
	  case QUADRANT_TWO:
		  degree_angle = 180-degree_angle;
		  target_angle = 270 + degree_angle;
		  break;
	  case QUADRANT_THREE:
		  degree_angle = 180-degree_angle;
		  target_angle = 270 - degree_angle;
		  break;
	  case QUADRANT_FOUR:
		  target_angle = 90 + degree_angle;
		  break;
	  default:
		  break;
	  }


	  diffAngle = ((int)target_angle - (int)headingDegrees + 360) % 360;

	  if(distance_c < 50){
		  moveSignal = STOP;

	  }else {
		  if ((diffAngle >= 345) || (diffAngle < 15)){
			  moveSignal = FORWARD;

		  }else {

			  if ((diffAngle >= 180) && (diffAngle < 345)){
				  moveSignal = CCW;
			  }else if ((diffAngle >= 15) && (diffAngle < 180)){
				  moveSignal = CW;
			  }
		  }
	  }

	  Move(moveSignal);

}
