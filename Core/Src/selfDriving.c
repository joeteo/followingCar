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

extern GPS_t GPS;
extern _DestinationGPS waypointGPS;

_Quadrant quadrant;

_bool chkCWCCW(float target_angle, float car_angle) {
	if (car_angle < 180) {
		double distance_angle = target_angle - car_angle;
		if (distance_angle > 0 && distance_angle < 180) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else {
		double distance_angle = (target_angle + 360) - car_angle;
		if (distance_angle < 180 || distance_angle >(car_angle + 360)) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
}

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


	  diffAngle = abs((int)headingDegrees - (int)target_angle);

	  if (diffAngle > 30 && diffAngle < 330) {

		  if (chkCWCCW((float)target_angle, headingDegrees)) {
			  Move(CW);
		  }
		  else {
			  Move(CCW);
		  }

	  }
	  else if (distance_c > 50) {
		  Move(FORWARD);
	  }
	  else {
		  Move(STOP);
	  }


}
