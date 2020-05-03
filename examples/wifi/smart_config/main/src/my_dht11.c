#include "my_dht11.h"
#include "my_gpio.h"

#define RESET 0
#define SET 1

#define PIN_NUM 0
/*
	*函数名称：void Dht11OutputConfig(bool val)
	*函数功能：设置DHT11为输出模块
	*函数形参：bool val 高低电平
	*返  回  值：无
	*硬件连接：GPIO5
	* */
void Dht11OutputConfig(int val)
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);	// GPIO5设为IO口
	GPIO_OUTPUT_SET(GPIO_ID_PIN(0), val);//主机发送高电平
}

/*
*函数名称：void Dht11InputConfig(void)
*函数功能：设置DHT11为输入模块
*函数形参：无
*返  回  值：无
*硬件连接：GPIO5
* */
void Dht11InputConfig(void)
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);	// GPIO5设为IO口
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(0));
}


void dht11_reset(void)
{
    
    Dht11OutputConfig(1);
    Dht11OutputConfig(0);// DHT11_OUT_L;
    DelayMs(19);// delay_us(19000);
    Dht11OutputConfig(1);// DHT11_OUT_H;
    DelayUs(30);// delay_us(30);

    Dht11InputConfig();
}
u16 dht11_scan(void)
{
    return GPIO_INPUT_GET(GPIO_ID_PIN(PIN_NUM));
}
u16 dht11_read_bit(void)
{
    u8 i=0;
    while(GPIO_INPUT_GET(GPIO_ID_PIN(PIN_NUM))==RESET);
    DelayUs(40);
    if(GPIO_INPUT_GET(GPIO_ID_PIN(PIN_NUM))==SET)
    {
        while(GPIO_INPUT_GET(GPIO_ID_PIN(PIN_NUM))==SET&&i<250)
        {
            i++;
        }
        if(i>250)
            return 2;
        return 1;
    }else 
    {
        return 0;
    }
}
u16 dht11_read_byte(void)
{
    u16 i;
    u16 data = 0;
    u16 temp = 0;
    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        temp = dht11_read_bit();
        if(temp==2) return 0;
        data |= temp;
    }
    return data;
}
u16 dht11_read_data(u8 buffer[])
{
    u16 i = 0;
    u8 checksum=0 ;
    dht11_reset();
		
    if (dht11_scan() == RESET)
    {
        //???DHT11??
        while (dht11_scan() == RESET);
        while (dht11_scan() == SET);
        for (i = 0; i < 5; i++)
        {
            buffer[i] = dht11_read_byte();
        }
        
        while (dht11_scan() == RESET);
        // dht11outputconfig();
         Dht11OutputConfig(1);// DHT11_OUT_H;
        
        checksum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
        if (checksum != buffer[4])
        {
            // checksum error
            return 1;
        }
    }
    
    return 0;
}