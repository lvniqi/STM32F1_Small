#include "stm32f10x.h"
#include <stdio.h>
#include "common.h"
#include "USART.h"
#include "Lcd.h"
#include "exti.h"
#include "AD.h"
#include "FPGA.h"
static u8 EXTI4_GET = 0;
const u8   WaveData_s[256]=
{
0x7F,0x82,0x85,0x88,0x8B,0x8F,0x92,0x95,0x98,0x9B,0x9E,0xA1,0xA4,0xA7,0xAA,0xAD
,0xB0,0xB3,0xB6,0xB8,0xBB,0xBE,0xC1,0xC3,0xC6,0xC8,0xCB,0xCD,0xD0,0xD2,0xD5,0xD7
,0xD9,0xDB,0xDD,0xE0,0xE2,0xE4,0xE5,0xE7,0xE9,0xEB,0xEC,0xEE,0xEF,0xF1,0xF2,0xF4
,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFB,0xFC,0xFD,0xFD,0xFE,0xFE,0xFE,0xFE,0xFE
,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFD,0xFD,0xFC,0xFB,0xFB,0xFA,0xF9,0xF8,0xF7,0xF6
,0xF5,0xF4,0xF2,0xF1,0xEF,0xEE,0xEC,0xEB,0xE9,0xE7,0xE5,0xE4,0xE2,0xE0,0xDD,0xDB
,0xD9,0xD7,0xD5,0xD2,0xD0,0xCD,0xCB,0xC8,0xC6,0xC3,0xC1,0xBE,0xBB,0xB8,0xB6,0xB3
,0xB0,0xAD,0xAA,0xA7,0xA4,0xA1,0x9E,0x9B,0x98,0x95,0x92,0x8F,0x8B,0x88,0x85,0x82
,0x7F,0x7C,0x79,0x76,0x73,0x6F,0x6C,0x69,0x66,0x63,0x60,0x5D,0x5A,0x57,0x54,0x51
,0x4E,0x4B,0x48,0x46,0x43,0x40,0x3D,0x3B,0x38,0x36,0x33,0x31,0x2E,0x2C,0x29,0x27
,0x25,0x23,0x21,0x1E,0x1C,0x1A,0x19,0x17,0x15,0x13,0x12,0x10,0x0F,0x0D,0x0C,0x0A
,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x03,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x05,0x06,0x07,0x08
,0x09,0x0A,0x0C,0x0D,0x0F,0x10,0x12,0x13,0x15,0x17,0x19,0x1A,0x1C,0x1E,0x21,0x23
,0x25,0x27,0x29,0x2C,0x2E,0x31,0x33,0x36,0x38,0x3B,0x3D,0x40,0x43,0x46,0x48,0x4B
,0x4E,0x51,0x54,0x57,0x5A,0x5D,0x60,0x63,0x66,0x69,0x6C,0x6F,0x73,0x76,0x79,0x7C
};  	//正弦波数据 
const 	u8   WaveData_sq[256]=
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};//方波数据
const u8 WaveData_t[256]=
{
	0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e,
0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e,
0x40,0x42,0x44,0x46,0x48,0x4a,0x4c,0x4e,0x50,0x52,0x54,0x56,0x58,0x5a,0x5c,0x5e,
0x60,0x62,0x64,0x66,0x68,0x6a,0x6c,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e,
0x80,0x81,0x83,0x85,0x87,0x89,0x8b,0x8d,0x8f,0x91,0x93,0x95,0x97,0x99,0x9b,0x9d,
0x9f,0xa1,0xa3,0xa5,0xa7,0xa9,0xab,0xad,0xaf,0xb1,0xb3,0xb5,0xb7,0xb9,0xbb,0xbd,
0xbf,0xc1,0xc3,0xc5,0xc7,0xc9,0xcb,0xcd,0xcf,0xd1,0xd3,0xd5,0xd7,0xd9,0xdb,0xdd,
0xdf,0xe1,0xe3,0xe5,0xe7,0xe9,0xeb,0xed,0xef,0xf1,0xf3,0xf5,0xf7,0xf9,0xfb,0xfd,
0xff,0xfd,0xfb,0xf9,0xf7,0xf5,0xf3,0xf1,0xef,0xed,0xeb,0xe9,0xe7,0xe5,0xe3,0xe1,
0xdf,0xdd,0xdb,0xd9,0xd7,0xd5,0xd3,0xd1,0xcf,0xcd,0xcb,0xc9,0xc7,0xc5,0xc3,0xc1,
0xbf,0xbd,0xbb,0xb9,0xb7,0xb5,0xb3,0xb1,0xaf,0xad,0xab,0xa9,0xa7,0xa5,0xa3,0xa1,
0x9f,0x9d,0x9b,0x99,0x97,0x95,0x93,0x91,0x8f,0x8d,0x8b,0x89,0x87,0x85,0x83,0x81,
0x80,0x7e,0x7c,0x7a,0x78,0x76,0x74,0x72,0x70,0x6e,0x6c,0x6a,0x68,0x66,0x64,0x62,
0x60,0x5e,0x5c,0x5a,0x58,0x56,0x54,0x52,0x50,0x4e,0x4c,0x4a,0x48,0x46,0x44,0x42,
0x40,0x3e,0x3c,0x3a,0x38,0x36,0x34,0x32,0x30,0x2e,0x2c,0x2a,0x28,0x26,0x24,0x22,
0x20,0x1e,0x1c,0x1a,0x18,0x16,0x14,0x12,0x10,0x0e,0x0c,0x0a,0x08,0x06,0x04,0x02
};
#define FPGA_RAM_DATABASE 0x60030600
#define FPGA_RAM_WRITE(_x,i) (*(volatile unsigned short *)(FPGA_RAM_DATABASE+_x) = i)
#define FPGA_SET_FREQ_L(i) ((*((volatile unsigned short *) 0x600304B8)) = i)
#define FPGA_SET_FREQ_H(i) ((*((volatile unsigned short *) 0x600304DA)) = i)

void EXTI4_IRQHandler(void)	//键盘中断
{
        EXTI4_GET = 1;
        volatile u8 temp,i;
        FPGA_AD_END();
	EXTI_ClearITPendingBit(EXTI_Line4);
}
u16 i = 0;
u16 temp;

int main(void)
{
        LCD_Init();
	EXTI_init();
        FPGA_EXTI4_init();
        FPGA_AD_END();
        
        for(i=0;i<256;i++)
        {
          temp = FPGA_AD_READ();
        }
        FPGA_AD_START();
        //LCD_ShowString(140, 250, 18, "Dispaly Test ^_^");
        FPGA_SET_FREQ_L(0x46DC);
        delay_ms(100);
        FPGA_SET_FREQ_H(0x0003);
        for(i=0;i<256;i++)
        {
          FPGA_RAM_WRITE(i,WaveData_s[i]);
        }
        LCD_Curve_Init(&Lcd_Curve1,2,60,LIGHTBLUE);
        LCD_ShowString(0,0,16,"FREQ: ",YELLOW);
	while (1)
	{
		//Ad_Get_Service();
		//while (Sequeue_Getlen(&ADC_BUFFER) and not Sequeue_Full(&(Lcd_Curve1.LCD_Curve_Buffer)))
		/*{
			temp = Sequeue_Get_Front(&ADC_BUFFER);
                        if(not Lcd_Curve1.finish_flag)
                        {
                          Sequeue_In_Queue(&(Lcd_Curve1.LCD_Curve_Buffer),temp>>4);
                          Sequeue_Out_Queue(&ADC_BUFFER);
                        }
                        else
                        {
                          if(temp +10 <Sequeue_Get_One(&ADC_BUFFER,1) and temp  <500 and Sequeue_Get_One(&ADC_BUFFER,1)+10 < Sequeue_Get_One(&ADC_BUFFER,2))
                          {
                            Lcd_Curve1.finish_flag = 0;
                            (Lcd_Curve1.Now_x) = 0;
                            Sequeue_Init(&(Lcd_Curve1.LCD_Curve_Buffer),_LCD_Curve_Buffer,LCD_CURVE_BUFFER_LEN);
                          }
                          else
                          {
                            Sequeue_Out_Queue(&ADC_BUFFER);
                          }
                        }
		}
                Ad_Get_Service();*/
                /*if(EXTI4_GET and not Sequeue_Full(&(Lcd_Curve1.LCD_Curve_Buffer)))
                {
                    FPGA_AD_START();
                    temp = FPGA_AD_READ();
                    temp = FPGA_AD_READ();
                    temp = FPGA_AD_READ();
                    for(i=0;i<300;i++)
                    {
                      //Fpga_Freq_Get_Service();
                      temp = FPGA_AD_READ();
                      temp &= 0xfff;
                      temp >>= 4;
                      if(not Sequeue_Full(&(Lcd_Curve1.LCD_Curve_Buffer)))
                      {
                        Sequeue_In_Queue(&(Lcd_Curve1.LCD_Curve_Buffer),temp);
                      }
                    }
                }
                while(Sequeue_Getlen(&(Lcd_Curve1.LCD_Curve_Buffer)))
                {
                  temp = Sequeue_Get_Front(&(Lcd_Curve1.LCD_Curve_Buffer));
                  LCD_Curve_Show(&Lcd_Curve1,temp,2);
                  Sequeue_Out_Queue(&(Lcd_Curve1.LCD_Curve_Buffer));
                }
                if(Lcd_Curve1.finish_flag)
                {
                  Lcd_Curve1.finish_flag = 0;
                  (Lcd_Curve1.Now_x) = 0;
                }
                Fpga_Freq_Get_Service();
                if(FPGA_FREQ < 1000)
                {
                  sprintf(LCD_BUFFER,"%d Hz      ",FPGA_FREQ);
                }
                else if(FPGA_FREQ < 1000000)
                {
                  sprintf(LCD_BUFFER,"%d.%d KHz    ",(FPGA_FREQ/1000),(FPGA_FREQ%1000)/100);
                }
                else
                {
                  sprintf(LCD_BUFFER,"%d.%d MHz    ",(FPGA_FREQ/1000000),((FPGA_FREQ/1000)%1000)/100);
                }
                
                LCD_ShowString(40,0,16,LCD_BUFFER,YELLOW);*/
	}
        /*
        FPGA_AD_START = 0;
        while (1)
        {
          ;
        }*/
}
