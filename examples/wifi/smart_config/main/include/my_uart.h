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

typedef enum {
    FUN_NULL=0,
    FUN_MY_OTA,              /*!< UART data event*/
    FUN_REBOOT,       /*!< UART RX buffer full event*/
    FUN_LEFT,          /*!< UART FIFO overflow event*/
    FUN_RIGHT,         /*!< UART RX frame error event*/
    FUN_SET_SSID_PASSED,
    FUN_GET_TIME,
} parse_event_type_t;


typedef struct {
    parse_event_type_t event_type;          
    char ip[16];         
    int  port;  
    uint32_t address;
    uint32_t length;
    char ssid[32];
    char passwd[32];
} parse_event_struct_t;
 /*struct parse_event_struct_t{
    parse_event_type_t event_type;          
    char ip[16];         
    int  port;  
    uint32_t address;
    uint32_t length;
    
} ;*/

void my_uart_init();
void uart_event_task(void *pvParameters);





#endif

