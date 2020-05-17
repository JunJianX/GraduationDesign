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

/*
CS  16
RST  5
RS  4
SDI 14
CLK 12
    LED
    NC
    NC
GND
GND
VCC    



16 14 5 4 12



*/
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
void  dsp_single_colour_x_region(int s_x,int s_y,int width,int height,int color);
void  dsp_single_colour(int color);
void  Display_ASCII8X16(uint x0,uint y0,char *s,uint color);
void Display_chinese16X16(uint x0,uint y0,uint s,uint color);
void Display_chinese24x24(uint x0,uint y0,uint s,uint color);
void  Display_Image(uint x0,uint y0,uint width,uint height,char *s);
void  Draw_any_color(uint x,uint y,uint width,uint height,int color);
void Display_Circle(int x0,int y0,uint width,uint radius,uint color);
#endif


