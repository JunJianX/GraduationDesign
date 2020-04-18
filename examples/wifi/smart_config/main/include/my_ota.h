#ifndef MY_OTA_H__
#define MY_OTA_H__


#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
// #include "protocol_examples_common.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_ota_ops.h"

#define EXAMPLE_SERVER_IP   CONFIG_SERVER_IP
#define EXAMPLE_SERVER_PORT CONFIG_SERVER_PORT
#define EXAMPLE_FILENAME CONFIG_EXAMPLE_FILENAME
#define BUFFSIZE 1500
#define TEXT_BUFFSIZE 1024

void ota_example_task(void *pvParameter);

#endif