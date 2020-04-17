/* Esptouch example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"
#include "esp_smartconfig.h"
#include "smartconfig_ack.h"
#include "my_tools.h"
#include "my_sntp.h"
// #include "test.h"

/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;
int WiFi_Init_Or_Not=0;
/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
static const int CONNECTED_BIT = BIT0;
static const int ESPTOUCH_DONE_BIT = BIT1;
static const char *TAG = "sc";

char *passwd=NULL;
char *ssid=NULL;

void smartconfig_example_task(void * parm);

static void on_wifi_disconnect(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    system_event_sta_disconnected_t *event = (system_event_sta_disconnected_t *)event_data;

    ESP_LOGI(TAG, "Wi-Fi disconnected, trying to reconnect...");
    if (event->reason == WIFI_REASON_BASIC_RATE_NOT_SUPPORT) {
        /*Switch to 802.11 bgn mode */
        esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCAL_11B | WIFI_PROTOCAL_11G | WIFI_PROTOCAL_11N);
    }
    ESP_ERROR_CHECK(esp_wifi_connect());
}

static void on_got_ip(void *arg, esp_event_base_t event_base,
                      int32_t event_id, void *event_data)
{
    xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
}

static esp_err_t My_wifi_init(void *ctx,system_event_t *event)
{
    switch(event->event_id)
    {
        case SYSTEM_EVENT_STA_START: 
            printf("SYSTEM_EVENT_STA_START\n");
            if(WiFi_Init_Or_Not==1)
            {

            }
            else if(WiFi_Init_Or_Not ==0 )
            {
                xTaskCreate(&smartconfig_example_task,"smartconfig_example_task", 1024+1024, NULL, 7, NULL);break;
            }
            // xTaskCreate(&smartconfig_example_task,"smartconfig_example_task", 1024+1024, NULL, 7, NULL);break;
        
        case SYSTEM_EVENT_STA_GOT_IP:
            printf("---------------------\n");
            printf("GOT IP!\n");
            printf("start sntp!");
            // xTaskCreate(&sntp_example_task,"sntp_example_task", 1024+1024, NULL, 7, NULL);break;
            printf("---------------------\n");

            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            printf("---------------------\n");
            printf("DISCONNECTED!\n");
            printf("---------------------\n");
            break;
        default:break;

    }
    return ESP_OK;
}
/*
static void initialise_wifi(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();

    // ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_event_loop_init(My_wifi_init,NULL));
    printf("222222222222222222\n");
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &on_wifi_disconnect, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &on_got_ip, NULL));
    
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    // wifi_config_t wifi_config = {
    //     .sta = {
    //         .ssid = "zhe_tian_xia_dou_shi_zhen_de",
    //         .password = "17806334985"
    //     },
    // };
    
    // ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA,&wifi_config));
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );
}*/
static void initialise_wifi(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK( esp_event_loop_init(My_wifi_init, NULL) );
    wifi_config_t config;
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );


    if(WiFi_Init_Or_Not==1)
    {
        memset(&config,0,(sizeof(config)));
        memcpy(config.sta.ssid,ssid,(strlen(ssid)));
        memcpy(config.sta.password,passwd,(strlen(passwd)));
    
        printf("config read is:%s\n", config.sta.ssid);
        printf("config read is:%s\n", config.sta.password);
        ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &config));
    }
    
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );
}

static void sc_callback(smartconfig_status_t status, void *pdata)
{
    switch (status) {
        case SC_STATUS_WAIT:
            ESP_LOGI(TAG, "SC_STATUS_WAIT");
            break;
        case SC_STATUS_FIND_CHANNEL:
            ESP_LOGI(TAG, "SC_STATUS_FINDING_CHANNEL");
            break;
        case SC_STATUS_GETTING_SSID_PSWD:
            ESP_LOGI(TAG, "SC_STATUS_GETTING_SSID_PSWD");
            break;
        case SC_STATUS_LINK:
            ESP_LOGI(TAG, "SC_STATUS_LINK");
            wifi_config_t *wifi_config = pdata;
            ESP_LOGI(TAG, "SSID:%s", wifi_config->sta.ssid);
            ESP_LOGI(TAG, "PASSWORD:%s", wifi_config->sta.password);
            /*Save SSID and PASSWORD.*/
            Save_ssid_passwd( (char*)(wifi_config->sta.ssid),(char*)(wifi_config->sta.password));
            ESP_ERROR_CHECK( esp_wifi_disconnect() );
            ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, wifi_config) );
            ESP_ERROR_CHECK( esp_wifi_connect() );
            break;
        case SC_STATUS_LINK_OVER:
            ESP_LOGI(TAG, "SC_STATUS_LINK_OVER");
            if (pdata != NULL) {
                sc_callback_data_t *sc_callback_data = (sc_callback_data_t *)pdata;
                switch (sc_callback_data->type) {
                    case SC_ACK_TYPE_ESPTOUCH:
                        ESP_LOGI(TAG, "Phone ip: %d.%d.%d.%d", sc_callback_data->ip[0], sc_callback_data->ip[1], sc_callback_data->ip[2], sc_callback_data->ip[3]);
                        ESP_LOGI(TAG, "TYPE: ESPTOUCH");
                        break;
                    case SC_ACK_TYPE_AIRKISS:
                        ESP_LOGI(TAG, "TYPE: AIRKISS");
                        break;
                    default:
                        ESP_LOGE(TAG, "TYPE: ERROR");
                        break;
                }
            }
            xEventGroupSetBits(wifi_event_group, ESPTOUCH_DONE_BIT);
            break;
        default:
            break;
    }
}

void smartconfig_example_task(void * parm)
{
    EventBits_t uxBits;
    ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_ESPTOUCH) );
    ESP_ERROR_CHECK( esp_smartconfig_start(sc_callback) );
    while (1) {
        uxBits = xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY); 
        if(uxBits & CONNECTED_BIT) {
            ESP_LOGI(TAG, "WiFi Connected to ap");
        }
        if(uxBits & ESPTOUCH_DONE_BIT) {
            ESP_LOGI(TAG, "smartconfig over");
            esp_smartconfig_stop();
            ESP_LOGI(TAG, "smartconfig task DELETE!");
            vTaskDelete(NULL);
        }
    }
}
/*
static void connect_ap(char *apName, char *apPassword)
{
    
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK( esp_event_loop_init(My_wifi_init, NULL) );
    wifi_config_t config;
    
 
    
    
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );


    if(WiFi_Init_Or_Not==1)
    {
        memset(&config,0,(sizeof(config)));
        memcpy(config.sta.ssid,apName,(strlen(apName)));
        memcpy(config.sta.password,apPassword,(strlen(apPassword)));
    
        printf("config read is:%s\n", config.sta.ssid);
        printf("config read is:%s\n", config.sta.password);
    }
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &config));
    ESP_ERROR_CHECK( esp_wifi_start() );

}*/

void app_main()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    ESP_ERROR_CHECK(err);
    printf("app_main1111111111111111111\n");
    
    printf("app_main2222222222222222222\n");
    // xTaskCreate();

    passwd = malloc(64+1);
    ssid = malloc(32+1);

    memset(passwd,0,64+1);
    memset(ssid,0,32+1);
    

    /*if(Read_ssid_passwd(ssid,passwd)==0)
    {
        connect_ap(ssid, passwd);
        printf("Read passwd&&ssid sccucessful!!\n");
    } 
    else 
    {
        printf("Read passwd&&ssid failed !!\n");
        initialise_wifi();
    }*/

    if(Read_ssid_passwd(ssid,passwd)==0)
    {
        printf("Read passwd&&ssid sccucessful!!\n");
        WiFi_Init_Or_Not =1;

    }
    else
    {
        printf("Read passwd&&ssid failed !!\n");
        WiFi_Init_Or_Not = 0;
    }
    initialise_wifi();
    while(1)
    {
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }


}

