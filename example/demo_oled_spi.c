/**
  ******************************************************************
  * @file    demo_oled_spi.c
  * @author  Beny
  * @version V0.01
  * @date    2023-12-16
  * @brief   SPI
  ******************************************************************
  * @attention
  * 參考 verimake ch549 例程
  * 使用SPI驱动oled 128x64屏幕
  *   oled引脚接线表
  *   CS     P1_4 //CS片选
  *   RST    P3_5 //LED复位
  *   DC     P2_7 //数据/命令控制
  *   D0     P1_7 //SCLK时钟信号 
  *   D1     P1_5 //MOSI数据
  ******************************************************************
  */
#include <CH549_sdcc.h> 	//ch549的头文件，其中定义了单片机的一些特殊功能寄存器 
#include <CH549_debug.h>  //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数
#include <CH549_spi.h>    //SPI主从设置和收发函数定义
#include <CH549_oled.h>   //程式庫驅動 128x64 oled示屏
#include <CH549_bmp.h>    //用于显示图片的头文件

/********************************************************************
* TIPS:
*   oled引脚接线表
*   CS     P1_4 //CS片选
*   RST    P3_5 //LED复位
*   DC     P2_7 //数据/命令控制
*   D0     P1_7 //SCLK时钟信号 
*   D1     P1_5 //MOSI数据
*********************************************************************/
// #pragma disable_warning 110

#define S 1  //fontSize 6x8, use 1 row
#define L 2  //fontSize 8x16, use 2 row

void main(void)
{
	CfgFsys();
  _delay_ms(20);                          //调整主频，建议稍加延时等待内部时钟稳定
  SPIMasterModeSet(3);                    //SPI主机模式设置，模式3
  SPI_CK_SET(12);                         //设置spi sclk 时钟信号为12分频
	OLED_Init();			                      //初始化OLED  
	OLED_Clear();                           //将oled屏幕上内容清除
 
  while(1)
  {
    OLED_ShowString(S,0,0,"CH549 SPI demo");
    OLED_ShowString(S,0,1,"I make it!");
    OLED_ShowString(S,0,2,"Thank you");
    OLED_ShowString(S,0,3,"Verimake!");
    OLED_ShowString(L,16,4,"Ben Yuen");
    OLED_ShowString(L,32,6,"2023-12-16");
    _delay_ms(2000);
    OLED_Clear();
    for(UINT8 i=0; i<7; i++) OLED_ShowCHinese(i*18,0,i);
    _delay_ms(2000);
    OLED_Clear();
    OLED_DrawBMP(0,0,Max_Column,Max_Row,BMP1);
    _delay_ms(2000);
    OLED_Clear();
    OLED_DrawBMP(0,0,Max_Column,Max_Row,BMP2);
    _delay_ms(2000);
    OLED_Clear();
    OLED_DrawBMP(0,4,Max_Column,5,BMP3);
    _delay_ms(2000);
    OLED_Display_Off();
    _delay_ms(5000);
    OLED_Display_On();
    _delay_ms(3000);
    OLED_Clear();

  }
}
