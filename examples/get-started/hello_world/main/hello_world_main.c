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

#define uchar unsigned char

void display()
{
        
        vTaskDelay(1000/portTICK_PERIOD_MS);
        Display_ASCII8X16(0,0, "  Sun Yuhong, ",WHITE);
        Display_ASCII8X16(0,16,"   I LOVE U!",WHITE);
        /* PutPixel(64,64,RED);
         PutPixel(64,65,RED);
         PutPixel(64,66,RED);
         PutPixel(64,67,RED);
         PutPixel(64,68,RED);*/
}

void app_main()
{

    int i=0;
    TickType_t now_tick;
    uint16_t adc_data[20];
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP8266 chip with %d CPU cores, WiFi, ",
            chip_info.cores);

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    // for (int i = 10; i >= 0; i--) {
    //     printf("Restarting in %d seconds...\n", i);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }
    // printf("Restarting now.\n");
    // fflush(stdout);
    // esp_restart();
    Adc_Init();
    printf("MAIN1---------------------------------\n");
    LCD_GPIO_Init();
    printf("MAIN2---------------------------------\n");
    lcd_initial();
    printf("MAIN3---------------------------------\n");
    // now_tick=xTaskGetTickCount();
    // vTaskDelayUntil(now_tick,500);
    dsp_single_colour(BLACK);
    while (1)
    {
        i++;
        if(i==1000)
        {
            i=0;
            display();
            if (ESP_OK == adc_read(&adc_data[0])) {
                printf("%d\n",adc_data[0]);
            }

            // Display_Image(0,0,128,128,gImage_image);
        }

        
  
    }
    
    



}
