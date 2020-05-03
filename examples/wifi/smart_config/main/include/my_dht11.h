/*
*  Created on: 2019年1月15日
*      Author: Marshal
*/

#ifndef _MY_DHT11__H_
#define _MY_DHT11__H_

// #include "ets_sys.h"
// #include "osapi.h"
// #include "user_interface.h"
// #include "driver/uart.h"
// #include "gpio.h"
#include "my_delay.h"
#include "eagle_soc.h"


#ifndef u8
typedef unsigned char u8;
#endif
#ifndef u16
typedef unsigned short u16;
#endif

u16 dht11_read_data(u8 buffer[]);

#endif /* APP_INCLUDE_DRIVER_DHT11_H_ */
