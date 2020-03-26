#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"

#include "lcd.h"
#include "font.h"


#define USE_LANDSCAPE

//±¾²âÊÔ³ÌÐòÊ¹ÓÃµÄÊÇÄ£ÄâSPI½Ó¿ÚÇý¶¯
//¿É×ÔÓÉ¸ü¸Ä½Ó¿ÚIOÅäÖÃ£¬Ê¹ÓÃÈÎÒâ×îÉÙ4 IO¼´¿ÉÍê³É±¾¿îÒº¾§Çý¶¯ÏÔÊ¾
/******************************************************************************
½Ó¿Ú¶¨ÒåÔÚLcd_Driver.hÄÚ¶¨Òå£¬Çë¸ù¾Ý½ÓÏßÐÞ¸Ä²¢ÐÞ¸ÄÏàÓ¦IO³õÊ¼»¯LCD_GPIO_Init()


LED    VCC
CLK D2 GPIO4
SDI D1 GPIO5
RS  D5 GPIO14
RST D6 GPIO12
CS  D0 GPIO16



*******************************************************************************/
//?��SPI��???????????8??????
void  SPI_WriteData(uchar Data)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)	
		LCD_SDA_SET_1; // sda=1; //????????
		else LCD_SDA_CLR_0;//sda=0;
		LCD_SCL_CLR_0;//scl=0;
		LCD_SCL_SET_1;//scl=1;
		Data<<=1;
	}
}
//?��???��????????8??????
void  Lcd_WriteIndex(uchar Data)
{
		
		LCD_CS_CLR_0;//cs=0;
		LCD_RS_CLR_0;//rs=0;
		SPI_WriteData(Data); 		
		LCD_CS_SET_1;//cs=1;
}
//?��???��????????8??????
void  Lcd_WriteData(uchar Data)
{	
		// unsigned char i=0;
		LCD_CS_CLR_0;//cs=0;
		LCD_RS_SET_1;//rs=1;
		SPI_WriteData(Data); 	
		LCD_CS_SET_1;//cs=1;
}
//?��???��????????16??????
void  LCD_WriteData_16Bit(unsigned int Data)
{
	// unsigned char i=0;
	LCD_CS_CLR_0;//cs=0;cs=0;
	LCD_RS_SET_1;//rs=1;rs=1;
	SPI_WriteData(Data>>8); 	//??????8??????
	SPI_WriteData(Data); 			//??????8??????
	LCD_CS_SET_1;//cs=1;cs=1;

}
void Reset()
{
    LCD_RST_CLR_0;//reset=0;
    vTaskDelay(100 / portTICK_RATE_MS);
    LCD_RST_SET_1;//reset=1;
    vTaskDelay(100 / portTICK_RATE_MS);
}
//Òº¾§IO³õÊ¼»¯ÅäÖÃ
void LCD_GPIO_Init(void)
{

	/*GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);*/
    gpio_config_t io_conf;

    io_conf.intr_type = GPIO_INTR_DISABLE;

    io_conf.mode = GPIO_MODE_OUTPUT;

    io_conf.pin_bit_mask = ((1ULL<<4)|(1ULL<<5)|(1ULL<<16)|(1ULL<<14)|(1ULL<<12));

    io_conf.pull_down_en =0;

    io_conf.pull_up_en = 0;
	printf("1.1------------------------------------\n");
    gpio_config(&io_conf);
	printf("1.2------------------------------------\n");

}



void lcd_initial()
{	Reset();//Reset before LCD Init.
		
	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Lcd_WriteIndex(0x11);//Sleep exit 
	vTaskDelay(120 / portTICK_RATE_MS);//delay_ms (120);
		
	//ST7735R Frame Rate
	Lcd_WriteIndex(0xB1); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB2); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB3); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	
	Lcd_WriteIndex(0xB4); //Column inversion 
	Lcd_WriteData(0x07); 
	
	//ST7735R Power Sequence
	Lcd_WriteIndex(0xC0); 
	Lcd_WriteData(0xA2); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x84); 
	Lcd_WriteIndex(0xC1); 
	Lcd_WriteData(0xC5); 

	Lcd_WriteIndex(0xC2); 
	Lcd_WriteData(0x0A); 
	Lcd_WriteData(0x00); 

	Lcd_WriteIndex(0xC3); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0x2A); 
	Lcd_WriteIndex(0xC4); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0xEE); 
	
	Lcd_WriteIndex(0xC5); //VCOM 
	Lcd_WriteData(0x0E); 
	
	Lcd_WriteIndex(0x36); //MX, MY, RGB mode 
#ifdef USE_LANDSCAPE
	Lcd_WriteData(0xA8); //?��??C8 ?��??08 A8
#else
	Lcd_WriteData(0xC8); //?��??C8 ?��??08 A8
#endif		
	//ST7735R Gamma Sequence
	Lcd_WriteIndex(0xe0); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1a); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x18); 
	Lcd_WriteData(0x2f); 
	Lcd_WriteData(0x28); 
	Lcd_WriteData(0x20); 
	Lcd_WriteData(0x22); 
	Lcd_WriteData(0x1f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x23); 
	Lcd_WriteData(0x37); 
	Lcd_WriteData(0x00); 	
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x10); 

	Lcd_WriteIndex(0xe1); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x17); 
	Lcd_WriteData(0x33); 
	Lcd_WriteData(0x2c); 
	Lcd_WriteData(0x29); 
	Lcd_WriteData(0x2e); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x39); 
	Lcd_WriteData(0x3f); 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0x10);  
	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x80+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00+3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x80+3);
	
	Lcd_WriteIndex(0xF0); //Enable test command  
	Lcd_WriteData(0x01); 
	Lcd_WriteIndex(0xF6); //Disable ram power save mode 
	Lcd_WriteData(0x00); 
	
	Lcd_WriteIndex(0x3A); //65k mode 
	Lcd_WriteData(0x05); 
	
	
	Lcd_WriteIndex(0x29);//Display on

}


void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{	
#ifdef USE_LANDSCAPE//?????��??????
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start+3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end+3);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end+2);

#else//?��??????	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start+3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end+3);	
#endif
	Lcd_WriteIndex(0x2c);
}


void PutPixel(uint x_start,uint y_start,uint color)
{
	Lcd_SetRegion(x_start,y_start,x_start+1,y_start+1);
	LCD_WriteData_16Bit(color);
	
}



void dsp_single_colour(int color)
{
 	uchar i,j;
	Lcd_SetRegion(0,0,128-1,128-1);
 	for (i=0;i<128;i++)
    	for (j=0;j<128;j++)
        	LCD_WriteData_16Bit(color);
}

void Display_ASCII8X16(uint x0,uint y0,char *s,uint color)
{
	int i,j,k,x,y,xx;
	
	unsigned char qm;
	
	long int ulOffset;
	
	char  ywbuf[32],temp[2];

	for(i = 0; i<strlen((char*)s);i++)
	{
		if(((unsigned char)(*(s+i))) >= 161)
		{
			temp[0] = *(s+i);
			temp[1] = '\0';
			// printf("LCD_Display_ASCII8X161---------------------------------\n");
			return;
		}
		else
		{
			qm = *(s+i);

			ulOffset = (long int)(qm) * 16;
			printf("qm is :%d\n",qm);
			for (j = 0; j < 16; j ++)
			{
						ywbuf[j]=Zk_ASCII8X16[ulOffset+j];
			}
			// printf("LCD_Display_ASCII8X162---------------------------------\n");		
			for(y = 0;y < 16;y++)
			{
					for(x=0;x<8;x++) 
					{
						k=x % 8;
							
						if(ywbuf[y]&(0x80 >> k))
						{
							xx=x0+x+i*8;
							PutPixel(xx,y+y0,color);
						}
									
					}
			}
			// printf("LCD_Display_ASCII8X163---------------------------------\n");		

		}
	}     	
}



void Display_Image(uint x0,uint y0,uint width,uint height,char *s)
{
	Lcd_SetRegion(x0,y0,x0+width-1,y0+height-1);
	uchar i,j;
	uint color;
	for(i=0;i<width;i++)
	{
		for(j = 0;j<height;j++)
		{
			color=0;
			color= (  (s[i*width+j]<<8)  |  s[i*width+j+1]   );
			LCD_WriteData_16Bit(color);
		}
	}
}