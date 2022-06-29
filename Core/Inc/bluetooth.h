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

enum{
	CONTROL_MODE,
	AUTO_MODE
};

typedef struct{
    float longitude;
    float latitude;
}PhoneGPS;

void BT_Init();
void Phone_UART_CallBack();


#endif /* INC_BLUETOOTH_H_ */
