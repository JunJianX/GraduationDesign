#ifndef __MY_TOOLS_H
#define __MY_TOOLS_H

#include "esp_system.h"
#include "esp_log.h"
#include "spi_flash.h"
// #include "spi_flash_emulation.h"
#include "string.h"
#include "stdlib.h"

#define FLASH_FACTORY_PASSWORD_ADDR 0x1E4000 //password length adress
#define FLASH_FACTORY_SSID_ADDR     0x1E4500 //ssid length adress

#define FLASH_IP_ADDR   0x1E5000
#define FLASH_PORT_ADDR 0x1E5500

void Save_ssid_passwd(char *ssid,char *passwd);
int Read_ssid_passwd(char *ssid,char *passwd);
void Save_ip_port(char *ip,char *port);
int Read_ip_port(char *ip,char *port);

#endif
