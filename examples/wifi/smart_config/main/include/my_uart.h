#ifndef MY_UART_H__
#define MY_UART_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"


void my_uart_init();
void uart_event_task(void *pvParameters);


#endif

