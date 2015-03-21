#include "stm32f10x.h"
#include <stdio.h>
#include "common.h"
#include "USART.h"
#include "Lcd.h"
#include "exti.h"
#include "AD.h"
#include "FPGA.h"
#include "DAC_CODE.h"
#include "DA.h"
u16 i = 0;
volatile u16 temp;
volatile u16 temp_2;
volatile u8 SELECT = 1;


int main(void)
{
        LCD_Init();
	EXTI_init();
        DAC_Config();
        FPGA_Set_Freq(100000);
        /*图形边框*/
        LCD_Curve_Init(&Lcd_Curve1,2,60,LIGHTBLUE);
        /*设置框*/
        LCD_Show_Rect(417,100,52,215,WHITE);
        LCD_ShowChinese_SHEZHI(420,290,16,YELLOW);
        /*显示汉字*/
        LCD_Show_Rect(418,276,50,2,BLUE);
        LCD_Show_Rect(418,275,50,2,BLUE);
        LCD_ShowChinese_ZHENGXIAN(420,240,16,YELLOW);
        LCD_ShowChinese_FANGBO(420,200,16,YELLOW);
        LCD_ShowChinese_SANJIAO(420,160,16,YELLOW);
        LCD_ShowChinese_ZUHE(420,120,16,YELLOW);
        LCD_ShowNumBig_L(180,36,0,FREQ,YELLOW);
        LCD_ShowCharBig(186,0,12,YELLOW);
        LCD_ShowCharBig(212,0,13,YELLOW);
        LCD_ShowCharBig(370,0,15,YELLOW);
        LCD_ShowCharBig(386,0,16,YELLOW);
        LCD_ShowNumBig_L(370,230,0,VPP,YELLOW);
        Select_R(1);
        DAC_Task(4095);
        Set_Vpp(2000);
	while (1)
	{  
          /*for(i=0;i<4095;i+= 100)
          {
            DAC_Task(i);
            LCD_ShowNumBig_L(370,230,0,i,YELLOW);
            delay_ms(1000);
          }*/
          if(MAIN_KEY.keysign)
          {
            if(MAIN_KEY.keycode == 7)
            {
              Select_R(0);
            }
            if(MAIN_KEY.keycode == 12)
            {
              if(SELECT)
              {
                Freq_Service(5);
                Vpp_Service(0);
              }
              else
              {
                Vpp_Service(5);
                Freq_Service(0);
              }
              SELECT = !SELECT;
            }
            else if(MAIN_KEY.keycode == 11)
            {
              Select_R(1);
            }
            else if(MAIN_KEY.keycode == 10)
            {
              if(SELECT)
              {
                /*频率设置*/
                Freq_Service(3);
                FPGA_Set_Freq(FREQ);
                LCD_ShowNumBig_L(180,6,0,FREQ,YELLOW);
                Freq_Service(0);
              }
              else
              {
                /*幅度设置*/
                Vpp_Service(3);
                Set_Vpp(VPP);
                LCD_ShowNumBig_L(370,230,0,VPP,YELLOW);
                Vpp_Service(0);
              }
            }
            else if(MAIN_KEY.keycode == 14)
            {
              if(SELECT)
              {
                Freq_Service(4);
                FPGA_Set_Freq(FREQ);
                LCD_ShowNumBig_L(180,6,0,FREQ,YELLOW);
                Freq_Service(0);
              }
              else
              {
                /*幅度设置*/
                Vpp_Service(4);
                Set_Vpp(VPP);
                LCD_ShowNumBig_L(370,230,0,VPP,YELLOW);
                Vpp_Service(0);
              }
            }
            else if(MAIN_KEY.keycode == 13)
            {
              if(SELECT)
              {
                Freq_Service(1);
              }
              else
              {
                Vpp_Service(1);
              }
            }
            else if(MAIN_KEY.keycode == 15)
            {
              if(SELECT)
              {
                Freq_Service(2);
              }
              else
              {
                Vpp_Service(2);
              }
            }
            else if(MAIN_KEY.keycode == 8)
            {
              u32 temp;
              MAIN_KEY.keysign = 0;
              if(SELECT)
              {
                LCD_ShowChinese(0,30,16,20,YELLOW);
                LCD_ShowChinese(16,30,16,22,YELLOW);
                LCD_ShowChinese(32,30,16,24,YELLOW);
                LCD_ShowChinese(48,30,16,26,YELLOW);
                LCD_ShowChinese(64,30,16,28,YELLOW);
                temp = LCD_GetChar(150,30,16);
                LCD_ShowString(0,30,16,"                                                  ",BACK_COLOR);
                if(temp < 4000000)
                {
                  FPGA_Set_Freq(temp);
                  LCD_ShowNumBig_L(180,6,0,FREQ,YELLOW);
                  Freq_Service(0);
                }
              }
              else
              {
                LCD_ShowChinese(0,30,16,20,YELLOW);
                LCD_ShowChinese(16,30,16,22,YELLOW);
                LCD_ShowChinese(32,30,16,24,YELLOW);
                LCD_ShowChinese(48,30,16,30,YELLOW);
                LCD_ShowChinese(64,30,16,32,YELLOW);
                temp = LCD_GetChar(150,30,16);
                LCD_ShowString(0,30,16,"                                                  ",BACK_COLOR);
                if(temp <= 5000 and temp >= 2000)
                {
                  Set_Vpp(temp);
                  LCD_ShowNumBig_L(370,230,0,VPP,YELLOW);
                  Vpp_Service(0);
                }
              }
            }
            MAIN_KEY.keysign = 0;
          }
        };
		
}
