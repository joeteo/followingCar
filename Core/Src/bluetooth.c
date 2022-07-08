/*
 * bluetooth.c
 *
 *  Created on: Jun 26, 2022
 *      Author: J
 */

#include "gps.h"
#include "bluetooth.h"
#include <stdio.h>
#include <usart.h>
#include <string.h>

_DestinationGPS phoneGPS;
_DestinationGPS waypointGPS;

uint8_t rx7_data;
uint8_t rx7_buffer[BTBUFSIZE];
uint8_t rx7_index;
uint8_t Mode_Flag;
int controlCMD;

extern float headingDegrees;
extern GPS_t GPS;

void BT_Init()
{
	HAL_UART_Receive_IT(&huart7, &rx7_data, sizeof(rx7_data));
}

void Phone_UART_CallBack(){


	// AUTO_MODE_SIGNAL == 'A'
	if((Mode_Flag == CONTROL_MODE || Mode_Flag == WAYPOINT_MODE) && rx7_data==AUTO_MODE_SIGNAL){
		memset(rx7_buffer, 0, sizeof(rx7_buffer));
		Mode_Flag=AUTO_MODE;

	// WAYPOINT_MODE_SIGNAL == 'W'
	}else if((Mode_Flag == AUTO_MODE || Mode_Flag == CONTROL_MODE) && rx7_data==WAYPOINT_MODE_SIGNAL){
		memset(rx7_buffer, 0, sizeof(rx7_buffer));
		Mode_Flag=WAYPOINT_MODE;

	// CONTROL_MODE_SIGNAL == 'C'
	}else if((Mode_Flag == AUTO_MODE || Mode_Flag == WAYPOINT_MODE) && rx7_data==CONTROL_MODE_SIGNAL){
		memset(rx7_buffer, 0, sizeof(rx7_buffer));
		Mode_Flag=CONTROL_MODE;
	}


	if(Mode_Flag==CONTROL_MODE){
		if (rx7_data != '\n' && rx7_index < sizeof(rx7_buffer)) {
			rx7_buffer[rx7_index++] = rx7_data;
		} else {
			rx7_buffer[rx7_index]=0;
			rx7_index = 0;
			sscanf((const char*)(rx7_buffer+2), "%d", &controlCMD);
		}

	}else if(Mode_Flag==WAYPOINT_MODE){
		if (rx7_data != '\n' && rx7_index < sizeof(rx7_buffer)) {
			rx7_buffer[rx7_index++] = rx7_data;
		} else {
			rx7_buffer[rx7_index]=0;
			rx7_index = 0;
			sscanf((const char*)(rx7_buffer+1), "%f,%f", &phoneGPS.latitude, &phoneGPS.longitude);
			if(phoneGPS.latitude > 34 && phoneGPS.latitude< 38 && phoneGPS.longitude > 126 && phoneGPS.longitude < 130){
				waypointGPS.latitude=phoneGPS.latitude;
				waypointGPS.longitude=phoneGPS.longitude;
			}
		}

	}else if(Mode_Flag==AUTO_MODE){
		if (rx7_data != '\n' && rx7_index < sizeof(rx7_buffer)) {
			rx7_buffer[rx7_index++] = rx7_data;
		} else {
			rx7_buffer[rx7_index]=0;
			rx7_index = 0;
			sscanf((const char*)(rx7_buffer+1), "%f,%f", &phoneGPS.latitude, &phoneGPS.longitude);
		}
	}



	HAL_UART_Transmit(&huart3, &rx7_data, sizeof(rx7_data), 10);
	HAL_UART_Receive_IT(&huart7, &rx7_data, sizeof(rx7_data));
}

void transmit_To_Phone(){
	char buf[GPSBUFSIZE] = {0,};
	sprintf(buf, "A,%.13f\n\r", GPS.dec_latitude);
	//HAL_UART_Transmit(&huart3, (unsigned char *)buf, strlen(buf), 10);
	HAL_UART_Transmit(&huart7, (unsigned char *)buf, strlen(buf), 10);
	sprintf(buf, "O,%.13f\n\r", GPS.dec_longitude);
	//HAL_UART_Transmit(&huart3, (unsigned char *)buf, strlen(buf), 10);
	HAL_UART_Transmit(&huart7, (unsigned char *)buf, strlen(buf), 10);
	sprintf(buf, "C,%.13f\n\r", headingDegrees);
	// transmit data to bluetooth module
	//HAL_UART_Transmit(&huart3, (unsigned char *)buf, strlen(buf), 10);
	HAL_UART_Transmit(&huart7, (unsigned char *)buf, strlen(buf), 10);

}
