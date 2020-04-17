#include "my_tools.h"
/*
void save_factory(char * ssid,char * password,char * ip,char * bssid,char * token)
{
    uint32_t passwordLen = 0;
    uint32_t ssidLen = 0;
    uint32_t ipLen=4;
    uint32_t bssidLen=6;
    uint32_t tokenLen=1;
    passwordLen = strlen(password);
    ssidLen = strlen(ssid);
    ESP_LOGI(TAG,"%s,%d,%s,%d\n",ssid,ssidLen,password,passwordLen);

    // printf("IP is:%d.%d.%d.%d,%d\r\n",ip[0],ip[1],ip[2],ip[3],4);
    // printf("BSSID is: %x:%x:%x:%x:%x:%x,%d\r\n",bssid[0],bssid[1],bssid[2],bssid[3],bssid[4],bssid[5],6);
    // printf("TOKEN is : %d %d ",token[0],1);

    spi_flash_erase_sector(FLASH_FACTORY_PASSWORD_ADDR / 0x1000);
    spi_flash_write(FLASH_FACTORY_PASSWORD_ADDR, (uint32_t *)&passwordLen, 4);
    spi_flash_write(FLASH_FACTORY_PASSWORD_ADDR + 4, (uint32_t *)password, passwordLen);
    spi_flash_write(FLASH_FACTORY_SSID_ADDR, (uint32_t *)&ssidLen, 4);
    spi_flash_write(FLASH_FACTORY_SSID_ADDR + 4, (uint32_t *)ssid, ssidLen);

    if(ip && bssid && token)
    { 
        spi_flash_write(FLASH_FACTORY_TOKEN_ADDR, (uint32_t *)&tokenLen, 4);
        spi_flash_write(FLASH_FACTORY_TOKEN_ADDR + 4, (uint32_t *)token, tokenLen);

        spi_flash_write(FLASH_FACTORY_IP_ADDR, (uint32_t *)&ipLen, 4);
        spi_flash_write(FLASH_FACTORY_IP_ADDR + 4, (uint32_t *)ip, ipLen);

        spi_flash_write(FLASH_FACTORY_BSSID_ADDR, (uint32_t *)&bssidLen, 4);
        spi_flash_write(FLASH_FACTORY_BSSID_ADDR + 4, (uint32_t *)bssid, bssidLen);
    }

}*/
void Save_ssid_passwd(char *ssid,char *passwd)
{
    uint32_t ssidlen=0;
    uint32_t passwdlen=0;

    ssidlen = strlen(ssid);
    passwdlen = strlen(passwd);
    printf("Save_ssid_passwd1111----------->");
    // ESP_LOGI("\n%s:%d,%s,%d\n",ssid,ssidlen,passwd,passwdlen);
    printf("Save_ssid_passwd2222----------->");

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