#ifndef MY_DELAY_H__
#define MY_DELAY_H__

/*#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"*/
#include <stdio.h>

#ifndef uint32_t
typedef unsigned int        uint32_t;
#endif

void  DelayUs(unsigned int data);
void  DelayMs(unsigned int data);
void os_DelayMs(uint32_t data);

#endif /* APP_INCLUDE_DRIVER_DELAY_H_ */