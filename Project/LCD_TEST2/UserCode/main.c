#include "stm32f10x.h"
#include <stdio.h>
#include "common.h"
#include "USART.h"
#include "Lcd.h"
#include "exti.h"
#include "AD.h"
#include "FPGA.h"
static u8 EXTI4_GET = 0;
void EXTI4_IRQHandler(void)	//º¸≈Ã÷–∂œ
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
	//TIM2_Configuration();
        //ADC1_Init();
        FPGA_AD_END();
        for(i=0;i<256;i++)
        {
          temp = FPGA_AD_READ();
        }
        FPGA_AD_START();
        //LCD_ShowString(140, 250, 18, "Dispaly Test ^_^");
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
                if(EXTI4_GET and not Sequeue_Full(&(Lcd_Curve1.LCD_Curve_Buffer)))
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
                
                LCD_ShowString(40,0,16,LCD_BUFFER,YELLOW);
	}
        /*
        FPGA_AD_START = 0;
        while (1)
        {
          ;
        }*/
}
