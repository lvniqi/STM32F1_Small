#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"	   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//2.4/2.8寸/3.5寸 TFT液晶驱动	  
//支持驱动IC型号包括:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/SPFD5408/SSD1289/1505/B505/C505等	    
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/10/7
//版本：V2.2
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved	
//********************************************************************************
//V1.2修改说明
//支持了SPFD5408的驱动,另外把液晶ID直接打印成HEX格式.方便查看LCD驱动IC.
//V1.3
//加入了快速IO的支持
//修改了背光控制的极性（适用于V1.8及以后的开发板版本）
//对于1.8版本之前(不包括1.8)的液晶模块,请修改LCD_Init函数的LCD_LED=1;为LCD_LED=1;
//V1.4
//修改了LCD_ShowChar函数，使用画点功能画字符。
//加入了横竖屏显示的支持
//V1.5 20110730
//1,修改了B505液晶读颜色有误的bug.
//2,修改了快速IO及横竖屏的设置方式.
//V1.6 20111116
//1,加入对LGDP4535液晶的驱动支持
//V1.7 20120713
//1,增加LCD_RD_DATA函数
//2,增加对ILI9341的支持
//3,增加ILI9325的独立驱动代码
//4,增加LCD_Scan_Dir函数(慎重使用)	  
//6,另外修改了部分原来的函数,以适应9341的操作
//V1.8 20120905
//1,加入LCD重要参数设置结构体lcddev
//2,加入LCD_Display_Dir函数,支持在线横竖屏切换
//V1.9 20120911
//1,新增RM68042驱动（ID:6804），但是6804不支持横屏显示！！原因：改变扫描方式，
//导致6804坐标设置失效，试过很多方法都不行，暂时无解。
//V2.0 20120924
//在不硬件复位的情况下,ILI9341的ID读取会被误读成9300,修改LCD_Init,将无法识别
//的情况（读到ID为9300/非法ID）,强制指定驱动IC为ILI9341，执行9341的初始化。
//V2.1 20120930
//修正ILI9325读颜色的bug。
//V2.2 20121007
//修正LCD_Scan_Dir的bug。
//////////////////////////////////////////////////////////////////////////////////	 
				  
//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
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

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//使能FSMC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//使能PORTB,D,E,G以及AFIO复用功能时钟

 	//PORTD复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	  
	//PORTE复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
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

	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime=6;//3<8，TFT控制芯片的地址建立时间最快是10ns，所以这个和下一行的参数加起来要保证能让地址建立的时间高于这个数字，
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime=6;//1or2<16
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime=15;//5<64（+1）,相比译码写入的速度慢很多 因此加大写入延时可以提高数据稳定性，这也是与tft和fpga通形唯一差异的地方
																												//以上参数通过调试已经比极限要高一点，极限要求低于10MHz的点数据传输速度。
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

	 LCD_WRITE_CMD(0x0036);//显示行列设置
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
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	    for(t=size-1;t>0;t--)
	    {   
				temp=asc4_1608[0][t];		 //调用1608字体 	                          
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
//在指定位置显示一个字符
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size)
{  	
		u8 a0=1;
    u8 temp,t1,t;
	u16 colortemp=POINT_COLOR; 
	num=num-' ';	
		LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	    for(t=size-1;t>0;t--)
	    {   
				temp=asc3_1608[num][t];		 //调用1608字体 	                          
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
		 while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
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































