#include "my_tools.h"

void Save_ssid_passwd(char *ssid,char *passwd)
{
    uint32_t ssidlen=0;
    uint32_t passwdlen=0;

    ssidlen = strlen(ssid);
    passwdlen = strlen(passwd);

    spi_flash_erase_sector(FLASH_FACTORY_PASSWORD_ADDR/1000);
    spi_flash_write(FLASH_FACTORY_PASSWORD_ADDR,(uint32_t *)&passwdlen,4);
    spi_flash_write(FLASH_FACTORY_PASSWORD_ADDR+4,(uint32_t *)passwd,passwdlen);
    spi_flash_write(FLASH_FACTORY_SSID_ADDR,(uint32_t *)&ssidlen,4);
    spi_flash_write(FLASH_FACTORY_SSID_ADDR+4,(uint32_t *)ssid,ssidlen);

    printf("READ TEST!!-------------------------------------->\n");
    spi_flash_read(FLASH_FACTORY_PASSWORD_ADDR,(uint32_t *)&passwdlen,4);
    spi_flash_read(FLASH_FACTORY_SSID_ADDR,(uint32_t *)&ssidlen,4);

    printf("------------------------\n passwdlen:%ud ssidlen:%ud\n------------------------\n",passwdlen,ssidlen);
}
int Read_ssid_passwd(char *ssid,char *passwd)
{
    uint32_t passwdlen = 0;
    uint32_t ssidlen = 0;
    char* tempPassword = NULL;
    char* tempSsid = NULL;

    spi_flash_read(FLASH_FACTORY_PASSWORD_ADDR,(uint32_t *)&passwdlen,4);
    spi_flash_read(FLASH_FACTORY_SSID_ADDR,(uint32_t *)&ssidlen,4);

    printf("------------------------\n passwdlen:%ud ssidlen:%ud\n------------------------\n",passwdlen,ssidlen);
    if(passwdlen<8||passwdlen>64||ssidlen==0||ssidlen>32)
    {
        ESP_LOGI("mytools.c","Read ssid&passwd error!");
        return 1;
    }
    tempPassword = malloc(64+1);
    tempSsid = malloc(32+1);
    memset(tempPassword,0,64+1);
    memset(tempSsid,0,32+1);

    spi_flash_read(FLASH_FACTORY_PASSWORD_ADDR+4,(uint32_t *)tempPassword,passwdlen);
    spi_flash_read(FLASH_FACTORY_SSID_ADDR+4,(uint32_t *)tempSsid,ssidlen);

    printf("------------------------\n");
    printf("\n%s:%d,%s:%d\n",tempSsid,ssidlen,tempPassword,passwdlen);
    printf("------------------------\n");

    memcpy(passwd,tempPassword,passwdlen);
    memcpy(ssid,tempSsid,ssidlen);
    return 0;
}

void Save_ip_port(char *ip,char *port)
{
    uint32_t iplen=0;
    uint32_t portlen=0;

    iplen = strlen(ip);
    portlen = strlen(port);

    spi_flash_erase_sector(FLASH_IP_ADDR/1000);
    spi_flash_write(FLASH_IP_ADDR,(uint32_t *)&iplen,4);
    spi_flash_write(FLASH_IP_ADDR+4,(uint32_t *)ip,iplen);
    spi_flash_write(FLASH_PORT_ADDR,(uint32_t *)&portlen,4);
    spi_flash_write(FLASH_PORT_ADDR+4,(uint32_t *)port,portlen);

    printf("READ TEST!!-------------------------------------->\n");
    // spi_flash_read(FLASH_IP_ADDR,(uint32_t *)&iplen,4);
    // spi_flash_read(FLASH_PORT_ADDR,(uint32_t *)&portlen,4);
    printf("IP is :%s",ip);
    printf("PORT is :%s",port);
    

    printf("------------------------\n iplen:%ud portlen:%ud\n------------------------\n",iplen,portlen);
}
int Read_ip_port(char *ip,char *port)
{
    uint32_t iplen = 0;
    uint32_t portlen = 0;
    char* tempip = NULL;
    char* tempport = NULL;

    spi_flash_read(FLASH_IP_ADDR,(uint32_t *)&iplen,4);
    spi_flash_read(FLASH_PORT_ADDR,(uint32_t *)&portlen,4);

    printf("------------------------\n iplen:%ud portlen:%ud\n------------------------\n",iplen,portlen);
    if(iplen<8||iplen>16||portlen==0||portlen>5)
    {
        ESP_LOGI("mytools.c","Read ip&port error!");
        return 1;
    }
    tempip = malloc(16+1);
    tempport = malloc(6+1);
    memset(tempip,0,16+1);
    memset(tempport,0,6+1);

    spi_flash_read(FLASH_IP_ADDR+4,(uint32_t *)tempip,iplen);
    spi_flash_read(FLASH_PORT_ADDR+4,(uint32_t *)tempport,portlen);

    printf("------------------------\n");
    printf("\n%s:%d,%s:%d\n",tempip,iplen,tempport,portlen);
    printf("------------------------\n");

    memcpy(ip,tempip,iplen);
    memcpy(port,tempport,portlen);
    return 0;
}
