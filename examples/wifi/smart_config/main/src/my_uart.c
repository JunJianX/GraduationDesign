#include "my_uart.h"
#include "my_tools.h"

static const char *TAG = "uart_events";

#define EX_UART_NUM UART_NUM_0

#define BUF_SIZE (130)
#define RD_BUF_SIZE (BUF_SIZE)
extern uint8_t ota_start_flag;
static QueueHandle_t uart0_queue;
parse_event_struct_t my_uart_event;
 parse_event_struct_t Parse(char *s)
{
     parse_event_struct_t t={
    .event_type = 0,.ip = "192.168.1.5",.port = 8080,.address = 0,.length = 0,.ssid="",.passwd=""};
    
    char *pforward= (char*)s,*pbehind = (char*)s,*pend = (char*)s;
    char ip[16]="";
    int port = 0;
    char temp[5] ="";
    if(strcmp((char*)s,"ota")==0) {t.event_type =FUN_MY_OTA; ota_start_flag =1;return t;}
    if(strcmp((char*)s,"reboot")==0) {t.event_type =FUN_REBOOT;return t;}
    if(strcmp((char*)s,"left")==0) {t.event_type = FUN_LEFT;return t;}
    if(strcmp((char*)s,"right")==0) {t.event_type = FUN_RIGHT;return t;}
    if(strcmp((char*)s,"time")==0) {t.event_type = FUN_GET_TIME;return t;}

    ESP_LOGE("my_uart","/***************************/\n");
    /*[IP:1.1.1.1,PORT:8080]*/
    pforward = strstr((char*)s,"[IP:");
    pbehind = strstr((char*)s,",PORT:");
    pend = strstr((char*)s,"]");
    if(pforward==NULL||pbehind==NULL||pend==NULL)
    {
        /*ERROR*/
        ESP_LOGE("my_uart","ERROR Instruction!\n");
    }else
    {
        /* code */
        strncpy((&t)->ip,pforward+4,pbehind-pforward-4);
        strncpy(temp,pbehind+6,pend-pbehind-6);
        t.port = atoi(temp);
        printf("IP:%s,PORT:%d",t.ip,t.port);
    }
    
    char *p = NULL;
    if((p=strstr(s,"setwifi"))!=NULL)
    {
        s=p+strlen("setwifi")+1;
        p = strchr(s,' ');
        if(p!=NULL)
        {
            *p = 0;
            memset(my_uart_event.ssid,0,32);
            memcpy(my_uart_event.ssid,s,strlen(s));
            s= p+1;
            memset(my_uart_event.passwd,0,32);
            memcpy(my_uart_event.passwd,s,strlen(s));
            printf("%s:%d Saving ssid and passwd!\r\n",__FILE__,__LINE__);
            printf("SSID:%s,PASSWORD:%s.\r\n",my_uart_event.ssid,my_uart_event.passwd);
            Save_ssid_passwd(my_uart_event.ssid,my_uart_event.passwd);
            // my_uart_event.event_type =FUN_START_CONNECT;
            // memset(my_uart_event.ssid,0,strlen(my_uart_event.ssid)-1);
            // memset(my_uart_event.passwd,0,strlen(my_uart_event.passwd)-1);
            // esp_restart();
            

        }else
        {
            printf("SSID AND PASSWD ERROR FORMAT!\r\n");
            // return -4;
        }
        

    }
    return t;


}

void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    uint8_t *dtmp = (uint8_t *) malloc(RD_BUF_SIZE);

    for (;;) {
        // Waiting for UART event.
        if (xQueueReceive(uart0_queue, (void *)&event, (portTickType)portMAX_DELAY)) {
            bzero(dtmp, RD_BUF_SIZE);
            ESP_LOGI(TAG, "uart[%d] event:", EX_UART_NUM);

            switch (event.type) {
                // Event of UART receving data
                // We'd better handler data event fast, there would be much more data events than
                // other types of events. If we take too much time on data event, the queue might be full.
                case UART_DATA:
                    ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
                    uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);
                    ESP_LOGI(TAG, "[DATA EVT]:");
                    uart_write_bytes(EX_UART_NUM, (const char *) dtmp, event.size);
                    my_uart_event=Parse((char *)dtmp);
                    break;

                // Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGI(TAG, "hw fifo overflow");
                    // If fifo overflow happened, you should consider adding flow control for your application.
                    // The ISR has already reset the rx FIFO,
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    xQueueReset(uart0_queue);
                    break;

                // Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGI(TAG, "ring buffer full");
                    // If buffer full happened, you should consider encreasing your buffer size
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    xQueueReset(uart0_queue);
                    break;

                case UART_PARITY_ERR:
                    ESP_LOGI(TAG, "uart parity error");
                    break;

                // Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG, "uart frame error");
                    break;

                // Others
                default:
                    ESP_LOGI(TAG, "uart event type: %d", event.type);
                    break;
            }
        }
    }

    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}
void my_uart_init()
{
   uart_config_t uart_config = {
        .baud_rate = 74880,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(EX_UART_NUM, &uart_config);

    // Install UART driver, and get the queue.
    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 100, &uart0_queue, 0); 
}

    