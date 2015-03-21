#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"	   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//2.4/2.8��/3.5�� TFTҺ������	  
//֧������IC�ͺŰ���:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/SPFD5408/SSD1289/1505/B505/C505��	    
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/7
//�汾��V2.2
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved	
//********************************************************************************
//V1.2�޸�˵��
//֧����SPFD5408������,�����Һ��IDֱ�Ӵ�ӡ��HEX��ʽ.����鿴LCD����IC.
//V1.3
//�����˿���IO��֧��
//�޸��˱�����Ƶļ��ԣ�������V1.8���Ժ�Ŀ�����汾��
//����1.8�汾֮ǰ(������1.8)��Һ��ģ��,���޸�LCD_Init������LCD_LED=1;ΪLCD_LED=1;
//V1.4
//�޸���LCD_ShowChar������ʹ�û��㹦�ܻ��ַ���
//�����˺�������ʾ��֧��
//V1.5 20110730
//1,�޸���B505Һ������ɫ�����bug.
//2,�޸��˿���IO�������������÷�ʽ.
//V1.6 20111116
//1,�����LGDP4535Һ��������֧��
//V1.7 20120713
//1,����LCD_RD_DATA����
//2,���Ӷ�ILI9341��֧��
//3,����ILI9325�Ķ�����������
//4,����LCD_Scan_Dir����(����ʹ��)	  
//6,�����޸��˲���ԭ���ĺ���,����Ӧ9341�Ĳ���
//V1.8 20120905
//1,����LCD��Ҫ�������ýṹ��lcddev
//2,����LCD_Display_Dir����,֧�����ߺ������л�
//V1.9 20120911
//1,����RM68042������ID:6804��������6804��֧�ֺ�����ʾ����ԭ�򣺸ı�ɨ�跽ʽ��
//����6804��������ʧЧ���Թ��ܶ෽�������У���ʱ�޽⡣
//V2.0 20120924
//�ڲ�Ӳ����λ�������,ILI9341��ID��ȡ�ᱻ�����9300,�޸�LCD_Init,���޷�ʶ��
//�����������IDΪ9300/�Ƿ�ID��,ǿ��ָ������ICΪILI9341��ִ��9341�ĳ�ʼ����
//V2.1 20120930
//����ILI9325����ɫ��bug��
//V2.2 20121007
//����LCD_Scan_Dir��bug��
//////////////////////////////////////////////////////////////////////////////////	 
				  
//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 

//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
		LCD_WRITE_CMD(0x002A);//Set column start address
		LCD_WRITE_DATA(Xpos>>8);
		LCD_WRITE_DATA(0x00FF&Xpos);
		Xpos+=7;
		LCD_WRITE_DATA(Xpos>>8);
		LCD_WRITE_DATA(0x00FF&Xpos);
		LCD_WRITE_CMD(0x002B);//Set page start address
		LCD_WRITE_DATA(Ypos>>8);
		LCD_WRITE_DATA(0x00FF&Ypos);	
		Ypos+=15;
		LCD_WRITE_DATA(Ypos>>8);
		LCD_WRITE_DATA(0x00FF&Ypos);
} 		 

void LCD_Init(void)
{ 										  
		GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//ʹ��FSMCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//ʹ��PORTB,D,E,G�Լ�AFIO���ù���ʱ��

 	//PORTD�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	  
	//PORTE�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth=FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode=FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity=FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive=FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal=FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode=FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation=FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode=FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst=FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct=&FSMC_NORSRAMTimingInitStructure;

	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime=6;//3<8��TFT����оƬ�ĵ�ַ����ʱ�������10ns�������������һ�еĲ���������Ҫ��֤���õ�ַ������ʱ�����������֣�
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime=6;//1or2<16
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime=15;//5<64��+1��,�������д����ٶ����ܶ� ��˼Ӵ�д����ʱ������������ȶ��ԣ���Ҳ����tft��fpgaͨ��Ψһ����ĵط�
																												//���ϲ���ͨ�������Ѿ��ȼ���Ҫ��һ�㣬����Ҫ�����10MHz�ĵ����ݴ����ٶȡ�
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	
	/* Enable FSMC Bank1_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
	
	LCD_REST=0;
	delay_ms(10);
  LCD_REST=1;

	 LCD_WRITE_CMD(0x0011);
	 delay_ms(10);
	 LCD_WRITE_CMD(0x00D0);
	 LCD_WRITE_DATA(0x0007);
	 LCD_WRITE_DATA(0x0041);
	 LCD_WRITE_DATA(0x0018);

	 LCD_WRITE_CMD(0x00D1);
	 LCD_WRITE_DATA(0x0000);
	 LCD_WRITE_DATA(0x000a);
	 LCD_WRITE_DATA(0x0010);

	 LCD_WRITE_CMD(0x00D2);
	 LCD_WRITE_DATA(0x0001);
	 LCD_WRITE_DATA(0x0011);

	 LCD_WRITE_CMD(0x00C0);
	 LCD_WRITE_DATA(0x0010);
	 LCD_WRITE_DATA(0x003B);
	 LCD_WRITE_DATA(0x0000);
	 LCD_WRITE_DATA(0x0002);
	 LCD_WRITE_DATA(0x0011);

	 LCD_WRITE_CMD(0x00C1);
	 LCD_WRITE_DATA(0x0010);
	 LCD_WRITE_DATA(0x0013);
	 LCD_WRITE_DATA(0x0088);


	 LCD_WRITE_CMD(0x00C5);
	 LCD_WRITE_DATA(0x0002);

	 LCD_WRITE_CMD(0x00C8);
	 LCD_WRITE_DATA(0x0000);
	 LCD_WRITE_DATA(0x0037);
	 LCD_WRITE_DATA(0x0025);
	 LCD_WRITE_DATA(0x0006);
	 LCD_WRITE_DATA(0x0004);
	 LCD_WRITE_DATA(0x001e);
	 LCD_WRITE_DATA(0x0026);
	 LCD_WRITE_DATA(0x0042);
	 LCD_WRITE_DATA(0x0077);
	 LCD_WRITE_DATA(0x0044);
	 LCD_WRITE_DATA(0x000f);
	 LCD_WRITE_DATA(0x0012);

	 LCD_WRITE_CMD(0x00F3);
	 LCD_WRITE_DATA(0x0040);
	 LCD_WRITE_DATA(0x000A);

	 LCD_WRITE_CMD(0x00F6);
	 LCD_WRITE_DATA(0x0080);

	 LCD_WRITE_CMD(0x00F7);
	 LCD_WRITE_DATA(0x0080);

	 LCD_WRITE_CMD(0x0036);//��ʾ��������
	 LCD_WRITE_DATA(0x0029);

	 LCD_WRITE_CMD(0x003a);
	 LCD_WRITE_DATA(0x0055);

	 LCD_WRITE_CMD(0x0020);
//	 LCD_WRITE_CMD(0x0021);

	 LCD_WRITE_CMD(0x002A);
	 LCD_WRITE_DATA(0x0000);
	 LCD_WRITE_DATA(0x0000);
	 LCD_WRITE_DATA(0x0001);
	 LCD_WRITE_DATA(0x003F);

	 LCD_WRITE_CMD(0x002B);
	 LCD_WRITE_DATA(0x0000);
	 LCD_WRITE_DATA(0x0000);
	 LCD_WRITE_DATA(0x0001);
	 LCD_WRITE_DATA(0x00df);

	 delay_ms(10);
	 LCD_WRITE_CMD(0x0029);
	 delay_ms(10);
	 LCD_WRITE_CMD(0x002C);
//		
	
	LCD_Clear1(WHITE);
}

void LCD_ShowHanjian(u16 x,u16 y,u8 size)
{
			u8 a0=1;
    u8 temp,t1,t;
	u16 colortemp=POINT_COLOR;
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	    for(t=size-1;t>0;t--)
	    {   
				temp=asc4_1608[0][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_WRITE_DATA(POINT_COLOR);	
				temp<<=1;
			}  	
			if(((a0==1)&(t==1))==1)
			{t++;
				a0=0;
			}
		}			
	POINT_COLOR=colortemp;
}					  
//��ָ��λ����ʾһ���ַ�
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size)
{  	
		u8 a0=1;
    u8 temp,t1,t;
	u16 colortemp=POINT_COLOR; 
	num=num-' ';	
		LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	    for(t=size-1;t>0;t--)
	    {   
				temp=asc3_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_WRITE_DATA(POINT_COLOR);	
				temp<<=1;
			}  	
			if(((a0==1)&(t==1))==1)
			{t++;
				a0=0;
			}
		}			
	POINT_COLOR=colortemp;	    	   	 	  
}   

void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{    
		 while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
		 { LCD_ShowChar(x,y,*p,size);
        x+=size/2;
        p++;
		 }
}

void LCD_Clear1(u16 Colour)
{	
	u32 i;
	LCD_WRITE_CMD(0x002A);//Set column address
	LCD_WRITE_DATA(0);
	LCD_WRITE_DATA(0);	
	LCD_WRITE_DATA(1);
	LCD_WRITE_DATA(223);
	LCD_WRITE_CMD(0x002B);//Set page address
	LCD_WRITE_DATA(0);
	LCD_WRITE_DATA(0);
	LCD_WRITE_DATA(1);
	LCD_WRITE_DATA(63);
	LCD_WRITE_CMD(0x002C); //write memory start
	for(i=0;i<0x25800;i++)
	LCD_WRITE_DATA(Colour);
}

void LCD_WriteRAM_Prepare()
{
	LCD_WRITE_CMD(0x002C);
}































