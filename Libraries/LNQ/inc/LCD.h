#ifndef LCD_H_
#define LCD_H_
#include "common.h"
#include "exti.h"
/*
 * LCD.H
 *
 *  Created on: 2014年5月21日
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */

//#define LCD_CMD              (*((volatile unsigned short *) 0x60000000)) /* RS = 0 */
//#define LCD_DATA              (*((volatile unsigned short *) 0x60000002)) /* RS = 1 */
//#define LCD_REST              (*((volatile unsigned short *) 0x60000004)) //0x0002
#define LCD_CMD              (*((volatile unsigned short *) 0x60000000)) /* RS = 0 */
#define LCD_DATA              (*((volatile unsigned short *) 0x60000002)) /* RS = 1 */
#define LCD_REST              (*((volatile unsigned short *) 0x60000008)) //0x0002
#define LCD_WRITE_CMD(Command)  (LCD_CMD= (Command) )
#define LCD_READ_DATA()  (LCD_DATA)
#define LCD_READ_CMD()  (LCD_CMD)
#define LCD_WRITE_DATA(Data)    (LCD_DATA= (Data) )

//LCD重要参数集

extern u16 POINT_COLOR; //默认红色
extern u16 BACK_COLOR; //背景颜色.默认为白色
extern char LCD_BUFFER[100];
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
/*液晶缓冲长度*/
#define LCD_CURVE_BUFFER_LEN 400
/*设置单点像素*/
#define _LCD_Set_Pixel(x,y,color)	\
{	\
	LCD_WRITE_CMD(0x002A);\
	LCD_WRITE_DATA(x >> 8);	\
	LCD_WRITE_DATA(0x00FF & x);	\
	LCD_WRITE_DATA(x >> 8);	\
	LCD_WRITE_DATA(0x00FF & x);	\
	LCD_WRITE_CMD(0x002B);  	\
	LCD_WRITE_DATA(y >> 8);	\
	LCD_WRITE_DATA(0x00FF & y);	\
	LCD_WRITE_DATA(y >> 8);	\
	LCD_WRITE_DATA(0x00FF & y);	\
	LCD_WriteRAM_Prepare();\
	LCD_WRITE_DATA(color);\
}
typedef struct _Lcd_Curve
{
	int Now_x; //现在位置
	int Now_y; //现在位置
	int Start_x; //起始x
	int Start_y; //起始y
	int End_x; //结束x
	int End_y; //结束y
	u8 Curve_last_y[480]; //上次的y
	u8 finish_flag; //单次显示完成标志
	u16 point_color;//画笔颜色
	char_sequeue LCD_Curve_Buffer; //曲线缓冲
} Lcd_Curve;
/*正弦波字体显示*/
#define LCD_ShowChinese_ZHENGXIAN(x,y,size,color)\
{       \
  LCD_ShowChinese(x,y,size,0,color);      \
  LCD_ShowChinese(x+16,y,size,2,color);      \
  LCD_ShowChinese(x+32,y,size,14,color);      \
}
/*方波字体显示*/
#define LCD_ShowChinese_FANGBO(x,y,size,color)\
{       \
  LCD_ShowChinese(x,y,size,8,color);      \
  LCD_ShowChinese(x+32,y,size,14,color);      \
}
/*三角波字体显示*/
#define LCD_ShowChinese_SANJIAO(x,y,size,color)\
{       \
  LCD_ShowChinese(x,y,size,4,color);      \
  LCD_ShowChinese(x+16,y,size,6,color);      \
  LCD_ShowChinese(x+32,y,size,14,color);      \
}
/*组合波字体显示*/
#define LCD_ShowChinese_ZUHE(x,y,size,color)\
{       \
  LCD_ShowChinese(x,y,size,10,color);      \
  LCD_ShowChinese(x+16,y,size,12,color);      \
  LCD_ShowChinese(x+32,y,size,14,color);      \
}
/*设置字体显示*/
#define LCD_ShowChinese_SHEZHI(x,y,size,color)\
{       \
  LCD_ShowChinese(x,y,size,16,color);      \
  LCD_ShowChinese(x+32,y,size,18,color);      \
}
#define LCD_Curve_Go_0(Lcd_Curve1)\
{       \
  (Lcd_Curve1->finish_flag) = 0;        \
  (Lcd_Curve1->Now_x) = 0;\
}
extern Lcd_Curve Lcd_Curve1;
/*缓冲数组源*/
extern u8 _LCD_Curve_Buffer[LCD_CURVE_BUFFER_LEN];

extern void LCD_Init(void);												//初始化
extern inline void LCD_SetCursor(u16 Xpos, u16 Ypos);					//设置光标
extern void LCD_ShowChar(u16 x, u16 y2, u8 num, u8 size,u16 color);			//显示一个字符
extern void LCD_ShowString(u16 x, u16 y, u8 size, char *p,u16 color);	//显示一个字符串,12/16字体
extern void LCD_ShowChinese(u16 x, u16 y, u8 size, u8 num,u16 color);
extern void LCD_ShowCharBig(u16 x,u16 y,u8 num,u16 color);
/*右对齐*/
extern void LCD_ShowNumBig(u16 x,u16 y,u32 num,u16 color);
extern void LCD_ShowNumBig_L(u16 x,u16 x_end,u16 y,u32 num,u16 color);
extern void LCD_Clear1(u16 Colour);
extern inline void LCD_WriteRAM_Prepare(void);
extern u32 LCD_GetChar(u16 x, u16 y2, u8 size);		//得到数
extern void LCD_Show_Rect(u16 x,u16 y,u16 lenth,u16 width,u16 point_color);
extern void LCD_Curve_Show(Lcd_Curve* Lcd_Curve1, u16 y, u8 setp);
extern void LCD_Clear_Curve(Lcd_Curve * Lcd_Curve1, u8 setp);
extern void LCD_Curve_Init(Lcd_Curve* Lcd_Curve1, u16 start_x1, u16 start_y1,u16 color);
extern void LCD_Curve_Grid(Lcd_Curve* Lcd_Curve1);
/*
 显示例子：
 sprintf(LCD_BUFFER,"asdsad:%d",i);
 LCD_ShowString(140,220,18,LCD_BUFFER);
 */
#endif

