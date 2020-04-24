/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/adc.h"
#include "esp_spi_flash.h"
#include "lcd.h"
#include "adc.h"
#include "font.h"
#include "dht11.h"
#include "driver/gpio.h"
#include "my_delay.h"
#include "my_gpio.h"
#include "esp_timer.h"

#define uchar unsigned char
#define u8 unsigned char
uint16_t adc_data[20];
extern u8 DHT11_Data_Array[6];
void display()
{
        char s_adc[20]="";
        sprintf(s_adc,"ADC:%d",adc_data[0]);
        // dsp_single_colour(BLACK);
        vTaskDelay(1000/portTICK_PERIOD_MS);
        Display_ASCII8X16(0,0, "  Sun Yuhong, ",WHITE);
        Display_ASCII8X16(0,16,"   I LOVE U!",WHITE);
        // Display_ASCII8X16(0,32,"           ",BLACK);
        
        Draw_any_color(32,32,64,12,BLACK);
        Display_ASCII8X16(0,32,s_adc,WHITE);
}
// void Read_DHT11_task(void *param)
// {
//     u8 res=0;
//     while(1)
//     {
//         res = DHT11ReadDataComplete();
//         vTaskDelay(10000/portTICK_PERIOD_MS);
//     }
// }
void GPIO_set_mode(uint8_t mode)
{
    gpio_config_t io_conf;
    if(mode==1)//output mode
    {   
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = (1ULL<<0);
            //disable pull-down mode
        io_conf.pull_down_en = 0;
        //disable pull-up mode
        io_conf.pull_up_en = 0;
        gpio_config(&io_conf);
    }else if(mode==0)//input mode
    {
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pin_bit_mask = (1ULL<<0);
        //disable pull-down mode
        io_conf.pull_down_en = 0;
        //disable pull-up mode
        io_conf.pull_up_en = 0;
        gpio_config(&io_conf);
    }
}
void app_main()
{

    int i=0,j=0;

    TickType_t now_tick;
    
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP8266 chip with %d CPU cores, WiFi, ",
            chip_info.cores);

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    // Adc_Init();
    printf("MAIN1---------------------------------\n");
    // LCD_GPIO_Init();
    printf("MAIN2---------------------------------\n");
    // lcd_initial();
    printf("MAIN3---------------------------------\n");
    // dsp_single_colour(BLACK);
    vTaskDelay(1000/portTICK_PERIOD_MS);

    //xTaskCreate(&Read_DHT11_task,"Read_DHT11_task", 1024, NULL, 8, NULL); 

    /*gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL<<0);
     //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);*/
    // Dht11OutputConfig(1);
    vTaskDelay(3000/portTICK_PERIOD_MS);
    // Dht11OutputConfig(0);
    // vTaskDelay(10000/portTICK_PERIOD_MS);
    // Dht11OutputConfig(1);
    // vTaskDelay(10000/portTICK_PERIOD_MS);
    // Dht11OutputConfig(0);
    // vTaskDelay(10000/portTICK_PERIOD_MS);
    while (1)
    {
            // GPIO_set_mode(1);
            // printf("Output HIGH after 10s\n");
            // vTaskDelay(10000/portTICK_PERIOD_MS);
            // gpio_set_level(0,1);

            // GPIO_set_mode(1);
            // printf("Output LOW after 10s\n");
            // vTaskDelay(10000/portTICK_PERIOD_MS);
            // gpio_set_level(0,0);

            // GPIO_set_mode(0);
            // printf("Reading input after 10s\n");
            // vTaskDelay(10000/portTICK_PERIOD_MS);
            // printf("The Level is %d\n",gpio_get_level(0));
            // vTaskDelay(10000/portTICK_PERIOD_MS);.
            uint8_t r = DHT11ReadDataComplete();
            if(r==0)
            {
                printf("%d\n",DHT11_Data_Array[0]);
                printf("%d\n",DHT11_Data_Array[1]);
                printf("%d\n",DHT11_Data_Array[2]);
                printf("%d\n",DHT11_Data_Array[3]);
                printf("%d\n",DHT11_Data_Array[4]);
                printf("%d\n",DHT11_Data_Array[5]);
            }else
            {
                
                printf("error:%ud\n",r);
            }
           /* printf("----------------------\n");
            printf("\n%f\n",(double)(esp_timer_get_time()/1000));
            // ets_delay_us(10000);
            DelayMs(25);
            printf("\n%f\n",(double)(esp_timer_get_time()/1000));
            printf("----------------------\n");*/

            vTaskDelay(10000/portTICK_PERIOD_MS);
            /*Dht11InputConfig();
            printf("\n------------------\n");
            printf("%d",GPIO_INPUT_GET(GPIO_ID_PIN(5)));
            printf("\n------------------\n");
            vTaskDelay(500/portTICK_PERIOD_MS);
            */
            // printf("\ntest DelayMs\n");
            // DelayMs(3000);
            /*for(i = 0; i < 3000; i ++)
            {
                for(j = 0; j < 1000; j ++)
                {
                    os_delay_us(1);
                }
            }*/


        // i++;
        // if(i==10000)
        // {
        //     i=0;
        //     /*display();
            
        //     j++;
        //     if(j==10)
        //     {
        //         j=0;
        //         if (ESP_OK == adc_read(&adc_data[0])) {
        //         printf("%d\n",adc_data[0]);
        //         }
        //     }
        //     Display_Image(0,0,128,128,gImage_image);*/
            
        // }
    }
}