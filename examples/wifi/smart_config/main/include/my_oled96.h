#ifndef _MY_OLED96__H_
#define _MY_OLED96__H_
#include "driver/gpio.h"
#define  uchar unsigned char 
#define uint unsigned int 
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int uint32_t;

//ºº×Ö´óÐ¡£¬Ó¢ÎÄÊý×Ö´óÐ¡
#define 	TYPE8X16		1
#define 	TYPE16X16		2
#define 	TYPE6X8			3

//-----------------OLED¶Ë¿Ú¶¨Òå----------------  					   

#define LCD_SCL_CLR()	;//GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define LCD_SCL_SET()	;//GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define LCD_SDA_CLR()	;//GPIO_ResetBits(GPIOB,GPIO_Pin_2)
#define LCD_SDA_SET()	;//GPIO_SetBits(GPIOB,GPIO_Pin_2)

#define LCD_RST_CLR()	gpio_set_level(12,0);//GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define LCD_RST_SET()	gpio_set_level(12,1);//GPIO_SetBits(GPIOB,GPIO_Pin_3)

#define LCD_DC_CLR()	gpio_set_level(15,0);//GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define LCD_DC_SET()	gpio_set_level(15,1);//GPIO_SetBits(GPIOB,GPIO_Pin_4)

// #define	LCD_CS_SET_1  	gpio_set_level(16,1) //gpio_set_level(,1) //LCD_CTRL->BSRR=LCD_CS    
#define	LCD_RS_SET_1  	gpio_set_level(15,1); //LCD_CTRL->BSRR=LCD_RS    
// #define	LCD_SDA_SET_1  	gpio_set_level(14,1) //LCD_CTRL->BSRR=LCD_SDA    
// #define	LCD_SCL_SET_1  	gpio_set_level(12,1) //LCD_CTRL->BSRR=LCD_SCL    
#define	LCD_RST_SET_1  	gpio_set_level(12,1);//gpio_set_level(,1) //LCD_CTRL->BSRR=LCD_RST    

 
//Òº¾§¿ØÖÆ¿ÚÖÃ0²Ù×÷Óï¾äºê¶¨Òå
// #define	LCD_CS_CLR_0  	gpio_set_level(16,0)//LCD_CTRL->BRR=LCD_CS    
#define	LCD_RS_CLR_0  	gpio_set_level(15,0);//gpio_set_level(12,0)//LCD_CTRL->BRR=LCD_RS    
// #define	LCD_SDA_CLR_0  	gpio_set_level(14,0)//LCD_CTRL->BRR=LCD_SDA    
// #define	LCD_SCL_CLR_0  	gpio_set_level(12,0)//LCD_CTRL->BRR=LCD_SCL    
#define	LCD_RST_CLR_0  	gpio_set_level(12,0);//gpio_set_level(15,0);//LCD_CTRL->BRR=LCD_RST  

//CS¹Ü½ÅÇë½ÓµØ
extern const u8 nonside[];

extern void LCD_Init(void);
extern void LCD_CLS(void);
extern void LCD_CLS_y(char y);
extern void LCD_CLS_line_area(u8 start_x,u8 start_y,u8 width);
extern void LCD_P6x8Str(u8 x,u8 y,u8 *ch,const u8 *F6x8);
extern void LCD_P8x16Str(u8 x,u8 y,u8 *ch,const u8 *F8x16);
extern void LCD_P14x16Str(u8 x,u8 y,u8 ch[],const u8 *F14x16_Idx,const u8 *F14x16);
extern void LCD_P16x16Str(u8 x,u8 y,u8 *ch,const u8 *F16x16_Idx,const u8 *F16x16);
//extern void LCD_Print(u8 x, u8 y, u8 *ch);
extern void LCD_PutPixel(u8 x,u8 y);
extern void LCD_Print(u8 x, u8 y, u8 *ch,u8 char_size, u8 ascii_size);
extern void LCD_Rectangle(u8 x1,u8 y1,u8 x2,u8 y2,u8 gif);
extern void Draw_BMP(u8 x,u8 y,const u8 *bmp); 
extern void LCD_Fill(u8 dat);

#endif


