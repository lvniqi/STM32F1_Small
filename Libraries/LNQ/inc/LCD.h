#ifndef LCD_H_
#define LCD_H_
#include "common.h"
#include "exti.h"
/*
 * LCD.H
 *
 *  Created on: 2014��5��21��
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

//LCD��Ҫ������

extern u16 POINT_COLOR; //Ĭ�Ϻ�ɫ
extern u16 BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
extern char LCD_BUFFER[100];
//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ

#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
/*Һ�����峤��*/
#define LCD_CURVE_BUFFER_LEN 400
/*���õ�������*/
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
	int Now_x; //����λ��
	int Now_y; //����λ��
	int Start_x; //��ʼx
	int Start_y; //��ʼy
	int End_x; //����x
	int End_y; //����y
	u8 Curve_last_y[480]; //�ϴε�y
	u8 finish_flag; //������ʾ��ɱ�־
	u16 point_color;//������ɫ
	char_sequeue LCD_Curve_Buffer; //���߻���
} Lcd_Curve;
/*���Ҳ�������ʾ*/
#define LCD_ShowChinese_ZHENGXIAN(x,y,size,color)\
{       \
  LCD_ShowChinese(x,y,size,0,color);      \
  LCD_ShowChinese(x+16,y,size,2,color);      \
  LCD_ShowChinese(x+32,y,size,14,color);      \
}
/*����������ʾ*/
#define LCD_ShowChinese_FANGBO(x,y,size,color)\
{       \
  LCD_ShowChinese(x,y,size,8,color);      \
  LCD_ShowChinese(x+32,y,size,14,color);      \
}
/*���ǲ�������ʾ*/
#define LCD_ShowChinese_SANJIAO(x,y,size,color)\
{       \
  LCD_ShowChinese(x,y,size,4,color);      \
  LCD_ShowChinese(x+16,y,size,6,color);      \
  LCD_ShowChinese(x+32,y,size,14,color);      \
}
/*��ϲ�������ʾ*/
#define LCD_ShowChinese_ZUHE(x,y,size,color)\
{       \
  LCD_ShowChinese(x,y,size,10,color);      \
  LCD_ShowChinese(x+16,y,size,12,color);      \
  LCD_ShowChinese(x+32,y,size,14,color);      \
}
/*����������ʾ*/
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
/*��������Դ*/
extern u8 _LCD_Curve_Buffer[LCD_CURVE_BUFFER_LEN];

extern void LCD_Init(void);												//��ʼ��
extern inline void LCD_SetCursor(u16 Xpos, u16 Ypos);					//���ù��
extern void LCD_ShowChar(u16 x, u16 y2, u8 num, u8 size,u16 color);			//��ʾһ���ַ�
extern void LCD_ShowString(u16 x, u16 y, u8 size, char *p,u16 color);	//��ʾһ���ַ���,12/16����
extern void LCD_ShowChinese(u16 x, u16 y, u8 size, u8 num,u16 color);
extern void LCD_ShowCharBig(u16 x,u16 y,u8 num,u16 color);
/*�Ҷ���*/
extern void LCD_ShowNumBig(u16 x,u16 y,u32 num,u16 color);
extern void LCD_ShowNumBig_L(u16 x,u16 x_end,u16 y,u32 num,u16 color);
extern void LCD_Clear1(u16 Colour);
extern inline void LCD_WriteRAM_Prepare(void);
extern u32 LCD_GetChar(u16 x, u16 y2, u8 size);		//�õ���
extern void LCD_Show_Rect(u16 x,u16 y,u16 lenth,u16 width,u16 point_color);
extern void LCD_Curve_Show(Lcd_Curve* Lcd_Curve1, u16 y, u8 setp);
extern void LCD_Clear_Curve(Lcd_Curve * Lcd_Curve1, u8 setp);
extern void LCD_Curve_Init(Lcd_Curve* Lcd_Curve1, u16 start_x1, u16 start_y1,u16 color);
extern void LCD_Curve_Grid(Lcd_Curve* Lcd_Curve1);
/*
 ��ʾ���ӣ�
 sprintf(LCD_BUFFER,"asdsad:%d",i);
 LCD_ShowString(140,220,18,LCD_BUFFER);
 */
#endif

