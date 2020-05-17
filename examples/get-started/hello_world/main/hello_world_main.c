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
#include "dht.h"
#include "driver/gpio.h"
#include "my_delay.h"
#include "my_gpio.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "driver/spi.h"
#define uchar unsigned char
#define u8 unsigned char
uint16_t adc_data[20];
extern u8 DHT11_Data_Array[6];
extern const unsigned char wifi_image[];

static const char *TAG = "spi_oled";
static uint8_t oled_dc_level = 0;
#define OLED_DC_GPIO     12
#define OLED_RST_GPIO    15
#define OLED_PIN_SEL  (1ULL<<OLED_DC_GPIO) | (1ULL<<OLED_RST_GPIO)

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

static void IRAM_ATTR spi_event_callback(int event, void *arg)
{
    switch (event) {
        case SPI_INIT_EVENT: {

        }
        break;

        case SPI_TRANS_START_EVENT: {
            gpio_set_level(OLED_DC_GPIO, oled_dc_level);
        }
        break;

        case SPI_TRANS_DONE_EVENT: {

        }
        break;

        case SPI_DEINIT_EVENT: {
        }
        break;
    }
}
void app_main()
{

    int i=0,j=0;
    u8 buffer[5];
    double hum;
    double temp;
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
        uint8_t x = 0;

    ESP_LOGI(TAG, "init gpio");
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = OLED_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    ESP_LOGI(TAG, "init hspi");
    spi_config_t spi_config;
    // Load default interface parameters
    // CS_EN:1, MISO_EN:1, MOSI_EN:1, BYTE_TX_ORDER:1, BYTE_TX_ORDER:1, BIT_RX_ORDER:0, BIT_TX_ORDER:0, CPHA:0, CPOL:0
    spi_config.interface.val = SPI_DEFAULT_INTERFACE;
    // Load default interrupt enable
    // TRANS_DONE: true, WRITE_STATUS: false, READ_STATUS: false, WRITE_BUFFER: false, READ_BUFFER: false
    spi_config.intr_enable.val = SPI_MASTER_DEFAULT_INTR_ENABLE;
    // Cancel hardware cs
    spi_config.interface.cs_en = 0;
    // MISO pin is used for DC
    spi_config.interface.miso_en = 0;
    // CPOL: 1, CPHA: 1
    spi_config.interface.cpol = 1;
    spi_config.interface.cpha = 1;
    // Set SPI to master mode
    // 8266 Only support half-duplex
    spi_config.mode = SPI_MASTER_MODE;
    // Set the SPI clock frequency division factor
    spi_config.clk_div = SPI_10MHz_DIV;
    // Register SPI event callback function
    spi_config.event_cb = spi_event_callback;
    spi_init(HSPI_HOST, &spi_config);


    // Adc_Init();
    printf("MAIN1---------------------------------\n");
    LCD_GPIO_Init();
    printf("MAIN2---------------------------------\n");
    lcd_initial();
    printf("MAIN3---------------------------------\n");
    dsp_single_colour(BLACK);
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
    vTaskDelay(1000/portTICK_PERIOD_MS);
    // Dht11OutputConfig(0);
    // vTaskDelay(10000/portTICK_PERIOD_MS);
    // Dht11OutputConfig(1);
    // vTaskDelay(10000/portTICK_PERIOD_MS);
    // Dht11OutputConfig(0);
    // vTaskDelay(10000/portTICK_PERIOD_MS);
    dsp_single_colour(BLACK);
    while (1)
    {
        // if(dht11_read_data(buffer)==0)
        // {
        //     hum = buffer[0] + buffer[1] / 10.0;
        //     temp = buffer[2] + buffer[3] / 10.0;
        //     printf("___{\"temperature\": %.2f, \"humidness\": %.2f}___\n\r", temp, hum);
        // }
        // Display_Image(0,0,20,10,wifi_image);
        //  Display_Image(0,0,128,128,gImage_image);
        Display_ASCII8X16(0,0,"HELLO",RED);

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



            // uint8_t r = DHT11ReadDataComplete();
            // if(r==0)
            // {
            //     printf("%d\n",DHT11_Data_Array[0]);
            //     printf("%d\n",DHT11_Data_Array[1]);
            //     printf("%d\n",DHT11_Data_Array[2]);
            //     printf("%d\n",DHT11_Data_Array[3]);
            //     printf("%d\n",DHT11_Data_Array[4]);
            //     // printf("%d\n",DHT11_Data_Array[5]);
            // }else
            // {
                
            //     printf("error:%ud\n",r);
            // }



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