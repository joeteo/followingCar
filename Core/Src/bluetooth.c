/*
 * bluetooth.c
 *
 *  Created on: Jun 26, 2022
 *      Author: J
 */

#include "bluetooth.h"
#include <stdio.h>
#include <usart.h>

PhoneGPS phoneGPS;

uint8_t rx7_data;
uint8_t rx7_buffer[BTBUFSIZE];
uint8_t rx7_index = 0;

void BT_Init()
{
	HAL_UART_Receive_IT(&huart7, &rx7_data, sizeof(rx7_data));
}

void Phone_UART_CallBack(){
	if (rx7_data != '\n' && rx7_index < sizeof(rx7_buffer)) {
		rx7_buffer[rx7_index++] = rx7_data;
	} else {
		rx7_buffer[rx7_index]=0;
		rx7_index = 0;
		sscanf((const char*)(rx7_buffer+1), "%f,%f", &phoneGPS.latitude, &phoneGPS.longitude);

//		memset(rx7_buffer, 0, sizeof(rx7_buffer));
	}

	//char tempbuf[20];
	//sprintf(tempbuf, "%s \r")

	HAL_UART_Transmit(&huart3, &rx7_data, sizeof(rx7_data), 10);
	HAL_UART_Receive_IT(&huart7, &rx7_data, sizeof(rx7_data));
}
