/*
 * bluetooth.c
 *
 *  Created on: Jun 26, 2022
 *      Author: J
 */

#include "bluetooth.h"
#include <stdio.h>
#include <usart.h>
#include <string.h>

PhoneGPS phoneGPS;

uint8_t rx7_data;
uint8_t rx7_buffer[BTBUFSIZE];
uint8_t rx7_index;
uint8_t Mode_Flag;
int controlCMD;

void BT_Init()
{
	HAL_UART_Receive_IT(&huart7, &rx7_data, sizeof(rx7_data));
}

void Phone_UART_CallBack(){


	if(Mode_Flag==CONTROL_MODE && rx7_data==AUTO_MODE_SIGNAL){
		memset(rx7_buffer, 0, sizeof(rx7_buffer));
		Mode_Flag=AUTO_MODE;
	}else if(Mode_Flag==AUTO_MODE && rx7_data==CONTROL_MODE_SIGNAL){
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
