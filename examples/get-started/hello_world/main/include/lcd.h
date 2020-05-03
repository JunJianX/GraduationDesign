#ifndef LCD__H_
#define LCD__H_

#define  uchar unsigned char 
#define uint unsigned int 

#include "font.h"


#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//»ÒÉ«0 3165 00110 001011 00101
#define GRAY1   0x8410      	//»ÒÉ«1      00000 000000 00000
#define GRAY2   0x4208      	//»ÒÉ«2  1111111111011111




#define LCD_CTRL   	  	GPIOB		//¶¨ÒåTFTÊý¾Ý¶Ë¿Ú
#define LCD_LED        	GPIO_Pin_9  //MCU_PB9--->>TFT --BL
#define LCD_RS         	GPIO_Pin_10	//PB11--->>TFT --RS/DC
#define LCD_CS        	GPIO_Pin_11  //MCU_PB11--->>TFT --CS/CE
#define LCD_RST     	GPIO_Pin_12	//PB10--->>TFT --RST
#define LCD_SCL        	GPIO_Pin_13	//PB13--->>TFT --SCL/SCK
#define LCD_SDA        	GPIO_Pin_15	//PB15 MOSI--->>TFT --SDA/DIN


//#define LCD_CS_SET(x) LCD_CTRL->ODR=(LCD_CTRL->ODR&~LCD_CS)|(x ? LCD_CS:0)

// //Òº¾§¿ØÖÆ¿ÚÖÃ1²Ù×÷Óï¾äºê¶¨Òå
// #define	LCD_CS_SET_1  	gpio_set_level(16,1) //gpio_set_level(,1) //LCD_CTRL->BSRR=LCD_CS    
// #define	LCD_RS_SET_1  	gpio_set_level(14,1) //LCD_CTRL->BSRR=LCD_RS    
// #define	LCD_SDA_SET_1  	gpio_set_level(5,1) //LCD_CTRL->BSRR=LCD_SDA    
// #define	LCD_SCL_SET_1  	gpio_set_level(4,1) //LCD_CTRL->BSRR=LCD_SCL    
// #define	LCD_RST_SET_1  	gpio_set_level(12,1)//gpio_set_level(,1) //LCD_CTRL->BSRR=LCD_RST    
// #define	LCD_LED_SET_1  	;//gpio_set_level(,1) //LCD_CTRL->BSRR=LCD_LED   

// //Òº¾§¿ØÖÆ¿ÚÖÃ0²Ù×÷Óï¾äºê¶¨Òå
// #define	LCD_CS_CLR_0  	gpio_set_level(16,0)//LCD_CTRL->BRR=LCD_CS    
// #define	LCD_RS_CLR_0  	gpio_set_level(14,0)//LCD_CTRL->BRR=LCD_RS    
// #define	LCD_SDA_CLR_0  	gpio_set_level(5,0)//LCD_CTRL->BRR=LCD_SDA    
// #define	LCD_SCL_CLR_0  	gpio_set_level(4,0)//LCD_CTRL->BRR=LCD_SCL    
// #define	LCD_RST_CLR_0  	gpio_set_level(12,0)//LCD_CTRL->BRR=LCD_RST    
// #define	LCD_LED_CLR_0  	;//LCD_CTRL->BRR=LCD_LED 


#define	LCD_CS_SET_1  	gpio_set_level(16,1) //gpio_set_level(,1) //LCD_CTRL->BSRR=LCD_CS    
#define	LCD_RS_SET_1  	gpio_set_level(4,1) //LCD_CTRL->BSRR=LCD_RS    
#define	LCD_SDA_SET_1  	gpio_set_level(14,1) //LCD_CTRL->BSRR=LCD_SDA    
#define	LCD_SCL_SET_1  	gpio_set_level(12,1) //LCD_CTRL->BSRR=LCD_SCL    
#define	LCD_RST_SET_1  	gpio_set_level(5,1)//gpio_set_level(,1) //LCD_CTRL->BSRR=LCD_RST    

 
//Òº¾§¿ØÖÆ¿ÚÖÃ0²Ù×÷Óï¾äºê¶¨Òå
#define	LCD_CS_CLR_0  	gpio_set_level(16,0)//LCD_CTRL->BRR=LCD_CS    
#define	LCD_RS_CLR_0  	gpio_set_level(4,0)//LCD_CTRL->BRR=LCD_RS    
#define	LCD_SDA_CLR_0  	gpio_set_level(14,0)//LCD_CTRL->BRR=LCD_SDA    
#define	LCD_SCL_CLR_0  	gpio_set_level(12,0)//LCD_CTRL->BRR=LCD_SCL    
#define	LCD_RST_CLR_0  	gpio_set_level(5,0)//LCD_CTRL->BRR=LCD_RST    


void  SPI_WriteData(uchar Data);
void  Lcd_WriteIndex(uchar Data);
void  Lcd_WriteData(uchar Data);
void  LCD_WriteData_16Bit(unsigned int Data);
void  Reset();
void  LCD_GPIO_Init(void);
void  SPI_WriteData(uchar Data);
void  Lcd_WriteIndex(uchar Data);
void  Lcd_WriteData(uchar Data);
void  LCD_WriteData_16Bit(unsigned int Data);
void  lcd_initial();
void  Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end);
void  PutPixel(uint x_start,uint y_start,uint color);
void  dsp_single_colour(int color);
void  Display_ASCII8X16(uint x0,uint y0,char *s,uint color);
void  Display_Image(uint x0,uint y0,uint width,uint height,char *s);
void  Draw_any_color(uint x,uint y,uint width,uint height,int color);
#endif


