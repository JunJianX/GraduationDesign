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
#include "my_uart.h"
#include "my_ota.h"
#include "lcd.h"
#include "adc.h"
#include "font.h"
#include "my_mqtt_client.h"
#include "eagle_soc.h"
#include "my_gpio.h"
#include "my_dht11.h"
// #include "test.h"



/* FreeRTOS event group to signal when we are connected & ready to make a request */
EventGroupHandle_t wifi_event_group;
EventGroupHandle_t boot_status_group;
const int BOOT_BIT = BIT0;
int WiFi_Init_Or_Not=0;
int mqtt_init_or_not=0;
/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
const int CONNECTED_BIT = BIT0;
static const int ESPTOUCH_DONE_BIT = BIT1;
static const char *TAG = "sc";
static const char *version = "ver 1.1";
uint8_t ota_start_flag = 0;
uint8_t sntp_flag =1;
uint8_t fun_left_flag = 0;
uint8_t fun_right_flag = 0;
uint8_t aliyun_flag = 0,aliyun_flag_last=0;
uint8_t net_flag = 0,net_flag_last=0,time_symbol_initial=0;

float Temperature=13.12;
uint8_t humidity=50;
uint16_t Gas=20;
uint8_t controller=0;

char *passwd=NULL;
char *ssid=NULL;
// TaskHandle_t sntp_handle;
extern parse_event_struct_t my_uart_event;
extern int sntp_ok_flag;
extern const unsigned char wifi_image[] ;
extern const unsigned char aliyun_image[] ;
void smartconfig_example_task(void * parm);

void clear_uart_event(void)
{
    my_uart_event.event_type=0;
}
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
static void task_status(void ){
    TaskStatus_t *StatusArray;
    UBaseType_t task_num;
    int x;
    uint32_t TotalRunTime;
    UBaseType_t ArraySize;
    task_num=uxTaskGetNumberOfTasks();      //获取系统任务数量
    printf("uxTaskGetNumberOfTasks %d\r\n", task_num);

    StatusArray=pvPortMalloc(task_num*sizeof(TaskStatus_t));//申请内存
    if(StatusArray!=NULL)                   //内存申请成功
    {
        ArraySize=uxTaskGetSystemState((TaskStatus_t*   )StatusArray,   //任务信息存储数组
                                    (UBaseType_t     )task_num,  //任务信息存储数组大小
                                    (uint32_t*       )&TotalRunTime);//保存系统总的运行时间
        printf("TaskName\t\tPriority\t\tTaskNumber\t\t\r\n");
        for(x=0;x<task_num;x++)
        {
            printf("%s\t\t%d\t\t\t%d\t\t\t\r\n",                
                    StatusArray[x].pcTaskName, //任务名称
                    (int)StatusArray[x].uxCurrentPriority, //任务优先级
                    (int)StatusArray[x].xTaskNumber); //任务编号

        }
    }
    vPortFree(StatusArray); //释放内存

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
            else if(WiFi_Init_Or_Not==0 )
            {
                xTaskCreate(&smartconfig_example_task,"smartconfig_example_task", 1024+1024, NULL, 7, NULL);
                WiFi_Init_Or_Not=1;
                break;
            }

            // xTaskCreate(&smartconfig_example_task,"smartconfig_example_task", 1024+1024, NULL, 7, NULL);break;
        
        case SYSTEM_EVENT_STA_GOT_IP:
            net_flag=1;
            printf("---------------------\n");
            printf("GOT IP!\n");
            xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
            
            // task_status();
            if(sntp_flag==1)
            {
                sntp_flag=0;
                printf("start sntp!");
                xTaskCreate(&sntp_example_task,"sntp_example_task", 1024+1024, NULL, 7, NULL);
                mqtt_init_or_not=1;
                break;

            }
            printf("---------------------\n");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            net_flag=0;
            printf("---------------------\n");
            printf("DISCONNECTED!Trying \n");
            xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
            esp_wifi_connect();
            printf("---------------------\n");

            break;
        default:break;

    }
    return ESP_OK;
}
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

void display_task(void * parm)
{   
    u8 start_t=0;
    struct tm timeinfo;
    time_t t=0;
    u8 last_sec=61,last_min=61,last_hour=25,last_day=32,last_month=13;
    u8 buffer_TH[5];
    char buffer[3]="";
    uint8_t time_flag = 0;
    // char strftime_buf[32];
    uint8_t i=0;
    time(&t);
 
    while(1)
    {
        i++;
        // printf("Test TIME t:%ld\n",t);
        if(ota_start_flag==0)
        {
            time(&t);
            if(i%2==0)//every 1 seconds flush
            {
                if(t<3600)
                {
                    //时间校准中…
                    if(time_flag == 0) {Display_chinese16X16(72,0,13,BLACK);time_flag=1;}else if(time_flag == 1){Display_chinese16X16(72,0,13,WHITE);time_flag=0;}
                }
            
            }
            if(i%5 == 0)
            {
                
                if(dht11_read_data(buffer_TH)==0)
                {
                    humidity = (uint8_t)buffer_TH[0] + buffer_TH[1] / 10.0;
                    Temperature = buffer_TH[2] + buffer_TH[3] / 10.0;
                    printf("___{\"temperature\": %.2f, \"humidness\": %02d}___\n\r", Temperature, humidity);
                }else
                {
                    printf("!!DHT11 Read Error!\n");
                }

                if (ESP_OK == adc_read(&Gas)) {
                    // printf("ADC value is %d\n",adc_data[0]);
                    printf("Gas is %d\n",Gas);
                }

                /*显示湿度*/
                dsp_single_colour_x_region(48,32,24,16,BLACK);
                sprintf(buffer,"%02d%%",humidity);
                Display_ASCII8X16(48,32,buffer,WHITE);
                /*开关状态*/
                dsp_single_colour_x_region(48,64,16,16,BLACK);
                if(controller==1)
                {

                    Display_chinese16X16(48,64,6,WHITE);
                }else if(controller==0)
                {
                    Display_chinese16X16(48,64,7,WHITE);
                }

                /*显示温度*/
                dsp_single_colour_x_region(48,16,40,16,BLACK);
                sprintf(buffer,"%.2f",Temperature);
                Display_ASCII8X16(48,16,buffer,WHITE);
                /*显示气体*/
                dsp_single_colour_x_region(48,48,24,16,BLACK);
                sprintf(buffer,"%3dppm",Gas);
                Display_ASCII8X16(48,48,buffer,WHITE);
            }

            if(/*i%5==0&&*/t>3600)//every 5 seconds flush
            {   
                localtime_r(&t,&timeinfo);
                /*sprintf(buffer,"%4d-%02d-%02d %02d:%02d",timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min);
                dsp_single_colour_x_region(0,0,128,16,BLACK);
                // Display_ASCII8X16(100,0,buffer,WHITE);
                Display_ASCII8X16(0,0,buffer,WHITE);

                Display_chinese16X16(0,16,0,WHITE);
                printf("\n%s\n",buffer);
                printf("\n%ld\n",t);*/
                if(time_symbol_initial==0)
                {
                    dsp_single_colour_x_region(0,0,128,16,BLACK);
                    Display_ASCII8X16(0,0,"  -     :  :  ",WHITE);
                    time_symbol_initial = 1;
                }
                if(timeinfo.tm_sec!=last_sec)
                {
                    sprintf(buffer,"%02d",timeinfo.tm_sec);
                    dsp_single_colour_x_region(96,0,16,16,BLACK);
                    Display_ASCII8X16(96,0,buffer,WHITE);
                    // printf("last_sec:%d,tm_sec:%d\n",last_sec,timeinfo.tm_sec);
                    last_sec = timeinfo.tm_sec;
                }
                if(timeinfo.tm_min!=last_min)
                {
                    sprintf(buffer,"%02d",timeinfo.tm_min);
                    dsp_single_colour_x_region(72,0,16,16,BLACK);
                    Display_ASCII8X16(72,0,buffer,WHITE);
                    // printf("last_min:%d,tm_min:%d\n",last_min,timeinfo.tm_min);
                    last_min = timeinfo.tm_min;
                }
                if(timeinfo.tm_hour!=last_hour)
                {
                    sprintf(buffer,"%02d",timeinfo.tm_hour);
                    dsp_single_colour_x_region(48,0,16,16,BLACK);
                    Display_ASCII8X16(48,0,buffer,WHITE);
                    // printf("last_hour:%d,tm_hour:%d\n",last_hour,timeinfo.tm_hour);
                    last_hour = timeinfo.tm_hour;
                }
                if(timeinfo.tm_mday!=last_day)
                {
                    sprintf(buffer,"%02d",timeinfo.tm_mday);
                    dsp_single_colour_x_region(24,0,16,16,BLACK);
                    Display_ASCII8X16(24,0,buffer,WHITE);
                    // printf("last_day:%d,tm_mday:%d\n",last_day,timeinfo.tm_mday);
                    last_day = timeinfo.tm_mday;
                }
                if(timeinfo.tm_mon+1!=last_month)
                {
                    sprintf(buffer,"%02d",timeinfo.tm_mon+1);
                    dsp_single_colour_x_region(0,0,16,16,BLACK);
                    Display_ASCII8X16(0,0,buffer,WHITE);
                    // printf("last_month:%d,tm_mon+1:%d\n",last_month,timeinfo.tm_mon+1);
                    last_month = timeinfo.tm_mon+1;
                }

            }
            if(i%5==0)
            {
                time(&t);
            }
        }else if(ota_start_flag==1)
        {
            printf("DELETE display_task!\n\n");
            vTaskDelete(NULL);
            dsp_single_colour(BLACK);
            /*Display_chinese16X16(0,0,25,WHITE);Display_chinese16X16(16,0,26,WHITE);Display_chinese16X16(32,0,27,WHITE);Display_chinese16X16(48,0,28,WHITE);
            i=0;
            while(1)
            {
                if(i==4)
                {
                    i=0;
                    Display_chinese16X16(72,0,13,BLACK);
                }else if(i==2)
                {
                    Display_chinese16X16(72,0,13,WHITE);
                }

                i++;
                vTaskDelay(1000/portTICK_PERIOD_MS);
            }*/
        }/*else if(ota_start_flag==2)
        {
            Display_Image(0,0,128,128,boot_image);
            while(1)
            {
                start_t++;
                if(start_t>5)
                {
                    ota_start_flag=0;
                    break;
                }else
                {
                    ;
                }
                
                vTaskDelay(1000/portTICK_PERIOD_MS);
            }

        }*/
        
        if(i==99)
        {
            i=0;
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
void test_task(void* param)
{
    char* pbuffer = (char*) malloc(2048);
    memset(pbuffer, 0x0, 2048);
    while(1) {
        printf("-------------------- heap:%u --------------------------\r\n", esp_get_free_heap_size());
        vTaskList(pbuffer);
        printf("%s", pbuffer);
        printf("----------------------------------------------\r\n");
        vTaskDelay(3000 / portTICK_RATE_MS);
    }
    free(pbuffer);
}
void GPIO0_D3OutputConfig(int val)
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, 0);	// GPIO5设为IO口
	GPIO_OUTPUT_SET(GPIO_ID_PIN(2), val);//主机发送val
}
void show_ota_picture(void)
{
    uint8_t i=0;
    dsp_single_colour(BLACK);
    Display_chinese16X16(0,0,25,WHITE);Display_chinese16X16(16,0,26,WHITE);Display_chinese16X16(32,0,27,WHITE);Display_chinese16X16(48,0,28,WHITE);
    while(1)
    {
        if(i==4)
        {
            i=0;
            Display_chinese16X16(72,0,13,BLACK);
        }
        else if(i==2)
        {
            Display_chinese16X16(72,0,13,WHITE);
        }

        i++;
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
void boot_display_task(void * parm)
{
    uint8_t i=0;
    dsp_single_colour(BLACK);
    // Display_Image(0,0,128,128,boot_image);
    /*监测系统*/
    Display_chinese24x24(16,16,0,WHITE);Display_chinese24x24(40,16,1,WHITE);Display_chinese24x24(64,16,2,WHITE);Display_chinese24x24(88,16,3,WHITE);
    /*启动中...*/
    Display_chinese16X16(32,80,29,WHITE);Display_chinese16X16(48,80,30,WHITE);Display_chinese16X16(64,80,26,WHITE);Display_chinese16X16(80,80,13,WHITE);
    /*201627074*/
    Display_ASCII8X16(28,108,"201627074",WHITE);
    while(1)
    {
        i++;
        if(i>6)
        {
            xEventGroupSetBits(boot_status_group,BOOT_BIT);
            vTaskDelete(NULL);
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
void app_main()
{
    time_t t;
    uint8_t i=0;
    u8 buffer[5];
    struct tm timeinfo;
    EventBits_t uxBits;
    boot_status_group = xEventGroupCreate();
    // char buffer[17]="2020-04-22 21:38";
    // char strftime_buf[32];
    char adc_data[10];
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    /**/
    printf("/***************************/\n");
    printf("%s\n",version);
    printf("/***************************/\n");

    /****************************/
    Adc_Init();
    printf("MAIN1---------------------------------\n");
    LCD_GPIO_Init();
    printf("MAIN2---------------------------------\n");
    lcd_initial();
    printf("MAIN3---------------------------------\n");
    // dsp_single_colour(BLACK);
    /****************************/
    xEventGroupClearBits(boot_status_group,BOOT_BIT);
    xTaskCreate(&boot_display_task,"boot_display_task", 1024, NULL, 8, NULL); 
    uxBits = xEventGroupWaitBits(boot_status_group, BOOT_BIT, true, false, portMAX_DELAY); 
    dsp_single_colour(BLACK);
    /*时间校准中…*/
    Display_chinese16X16(0,0,8,WHITE);Display_chinese16X16(16,0,9,WHITE);Display_chinese16X16(32,0,10,WHITE);Display_chinese16X16(48,0,11,WHITE);Display_chinese16X16(60,0,12,WHITE);
    Display_chinese16X16(72,0,13,WHITE);
    /*温度:*/
    Display_chinese16X16(0,16,0,WHITE);Display_chinese16X16(16,16,1,WHITE);Display_chinese16X16(32,16,2,WHITE);Display_chinese16X16(96,16,22,WHITE);
    /*湿度:*/
    Display_chinese16X16(0,32,3,WHITE);Display_chinese16X16(16,32,1,WHITE);Display_chinese16X16(32,32,2,WHITE);//Display_chinese16X16(32,16,23,WHITE);
    /*气体:*/
    Display_chinese16X16(0,48,4,WHITE);Display_chinese16X16(16,48,5,WHITE);Display_chinese16X16(32,48,2,WHITE);
    /*开关:*/
    Display_chinese16X16(0,64,6,WHITE);Display_chinese16X16(16,64,7,WHITE);Display_chinese16X16(32,64,2,WHITE);
    /*版本*/
    Display_chinese16X16(0,80,23,WHITE);Display_chinese16X16(16,80,24,WHITE);Display_chinese16X16(32,80,2,WHITE);Display_ASCII8X16(48,80,version,WHITE);
     
    
    vTaskDelay(1000/portTICK_PERIOD_MS);
    ets_delay_us(1000000);
    my_uart_init();
    //////////
    printf("-------------------- heap:%u --------------------------\r\n", esp_get_free_heap_size());
    xTaskCreate(&uart_event_task,"uart_event_task", 1024, NULL, 8, NULL); 
    xTaskCreate(&display_task,"display_task",1024, NULL, 12, NULL);
    printf("-------------------- heap:%u --------------------------\r\n", esp_get_free_heap_size());
    // xTaskCreate(&test_task, "test_task", 1024, NULL, 7, NULL);
    printf("\nStart MQTT TASK\n");
    // xTaskCreate(&My_mqtt_task,"My_mqtt_task",4096+2048+1024,NULL,6,NULL);
   
    passwd = malloc(64+1);
    ssid = malloc(32+1);

    memset(passwd,0,64+1);
    memset(ssid,0,32+1);
    

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
        if(net_flag==1&&net_flag!=net_flag_last)
        {
            Display_Image(0,96,50,30,wifi_image);
        }else if(net_flag == 0&&net_flag!=net_flag_last)
        {
            dsp_single_colour_x_region(0,96,50,30,BLACK);
        }
        if(aliyun_flag==1&&aliyun_flag_last!=aliyun_flag)
        {
            Display_Image(50,96,54,30,aliyun_image);
        }else if(aliyun_flag == 0&&aliyun_flag_last!=aliyun_flag)
        {
            dsp_single_colour_x_region(50,96,54,30,BLACK);
        }
        aliyun_flag_last = aliyun_flag;
        net_flag_last = net_flag;

        if(controller==0)
            GPIO0_D3OutputConfig(0);
        else if(controller==1)
            GPIO0_D3OutputConfig(1);
        
        if(mqtt_init_or_not==1&&sntp_ok_flag==1)
        {
            xTaskCreate(&My_mqtt_task,"My_mqtt_task",8192,NULL,8,NULL);
            mqtt_init_or_not=0;
            sntp_ok_flag=0;
        }
   
        switch(my_uart_event.event_type){
        case FUN_MY_OTA: 
            printf("Execute OTA!\n");
            xTaskCreate(&ota_example_task,"ota_example_task",8192,NULL,6,NULL);
            show_ota_picture();
            break;
        case FUN_REBOOT: 
            esp_restart();
            break;
        case FUN_LEFT: 
            fun_left_flag=1;
            break;
        case FUN_RIGHT:
            fun_left_flag=1;
            break;
        case FUN_SET_SSID_PASSED:
            break;
        case FUN_GET_TIME:
            time(&t);
            printf("\nThe time is %ld \n",t);
            break;
            
        default :
            break;
        }
        /**/
        i++;
        // if(i%11==0)//every 5 seconds flush
        // // {
        //    if(dht11_read_data(buffer)==0)
        //     {
        //         humidity = (uint8_t)buffer[0] + buffer[1] / 10.0;
        //         Temperature = buffer[2] + buffer[3] / 10.0;
        //         printf("___{\"temperature\": %.2f, \"humidness\": %02d}___\n\r", Temperature, humidity);
        //     }
        // }
        if(i%20==0)//every 5 seconds flush
        {
           printf("CONTROLLER is %d\n",controller);
            i=0;
        }
        my_uart_event.event_type=0;
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }


}

