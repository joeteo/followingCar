/*
 * bluetooth.h
 *
 *  Created on: Jun 26, 2022
 *      Author: J
 */

#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_

#define	BT_USART	&huart7
#define BTBUFSIZE  20

#define CONTROL_MODE_SIGNAL 0x43
#define AUTO_MODE_SIGNAL 0x41
#define WAYPOINT_MODE_SIGNAL 0x57

enum{
	CONTROL_MODE,
	WAYPOINT_MODE,
	AUTO_MODE
};

typedef struct{
    float longitude;
    float latitude;
}_DestinationGPS;

void BT_Init();
void Phone_UART_CallBack();

void transmit_To_Phone();


#endif /* INC_BLUETOOTH_H_ */
