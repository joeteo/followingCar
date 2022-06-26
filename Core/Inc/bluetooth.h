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

typedef struct{
    float longitude;
    float latitude;
}PhoneGPS;

void BT_Init();
void Phone_UART_CallBack();


#endif /* INC_BLUETOOTH_H_ */
