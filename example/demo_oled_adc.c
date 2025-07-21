/**
  ******************************************************************
  * @file    demo_oled_adc.c
  * @author  Beny
  * @version V0.01
  * @date    2023-12-17
  * @brief   AD and DA
  ******************************************************************
  * @attention
  * 參考 verimake ch549 例程
  * 输出pwm通过滤波电路达到DA的效果，在通过AD采集显示在屏幕上
  ******************************************************************
  */
#include <CH549_sdcc.h>  //ch549的头文件，其中定义了单片机的一些特殊功能寄存器
#include <CH549_debug.h> //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数
#include <CH549_bmp.h>   //用于显示图片的头文件
#include <CH549_oled.h> //程式庫驅動 128x64 oled示屏
#include <CH549_spi.h> //SPI主从设置和收发函数定义
#include <CH549_pwm.h> //PWM初始化、设置函数定义
#include <CH549_adc.h> //ADC初始化，采样时钟设定，通道变化和电压比较功能函数定义

/********************************************************************
* TIPS:
* adc各通道对应引脚关系
* P1.1       AIN1
* P1.2       AIN2
* P1.3       AIN3
* P1.4       AIN4
* P1.5       AIN5
* P1.6       AIN6
* P1.7       AIN7
* P0.0       AIN8
* P0.1       AIN9
* P0.2       AIN10  
* P0.3       AIN11 
* P0.4       AIN12 
* P0.5       AIN13 
* P0.6       AIN14 
* P0.7       AIN15 
*********************************************************************/
UINT8 oled_colum;
UINT8 oled_row;
UINT8 fontSize;

/********************************************************************
* 函 数 名       : setCursor
* 函数功能		   : 设置printf到屏幕上的字符串起始位置
* 输    入       : 行坐标 列坐标
* 输    出    	 : 无
********************************************************************/
void setCursor(UINT8 Size, UINT8 column, UINT8 row)
{
  fontSize = Size;
  oled_colum = column;
  oled_row = row;
}

/********************************************************************
* 函 数 名       : main
* 函数功能		   : 主函数
* 输    入       : 无
* 输    出    	 : 无
*********************************************************************/
void main(void)
{
  UINT8 ch;
  UINT16 i=0;
  float Voltage;
  float Output_Voltage;
  float Output_Baud; 
  CfgFsys();  //CH549时钟选择配置
  _delay_ms(20);
/* OLED的SPI初始化程序 */
  SPIMasterModeSet(3); //SPI主机模式设置，模式3
  SPI_CK_SET(12);      //12分频
/* 初始化OLED */
  OLED_Init();
  OLED_Clear();
/* ADC的初始化配置 */
  ADC_ExInit(3);                            //ADC初始化,选择采样时钟
/* PWM的初始化配置 */
  SetPWMClkDiv(1);                          //PWM时钟配置,Fsys/4,Fsys默认为12Mhz
  SetPWMCycle256Clk();                      //PWM周期 Fsys/4/256
  PWM_SEL_CHANNEL(ch3,Enable);              //对ch3，即p2.2的pwm外设初始化
  
  /* 主循环 */
  while (1)
  {
    /* 配置占空比 */
    SetPWM3Dat(i);  //配置通道3即p2.2的占空比，占空比等于i/256占空比
    _delay_ms(2000);
    ch = 15;
    ADC_ChSelect(ch);  //选择通道
    ADC_StartSample();  //启动采样
    while ((ADC_CTRL & bADC_IF) == 0)
    {
      ;  //查询等待标志置位
    }
    ADC_CTRL = bADC_IF; //清标志
    Voltage = (ADC_DAT / 4095.0)*5.0 ;
    /*
    adc采集到的值为ADC_DAT，由于ch549是12位ADC模数转换器因此ADC_DAT为0-4095之间的数，它表达的含义是电压值的相对高低
    因此 实际电压值的算法为ADC_DAT除以4095乘以ch549所使用的电压5v
    */
    if(i>256) i=0;  //PWM設定最大256, 超出時返零
    Output_Baud = (i/256.0)*100;
    setCursor(2,0,0);  //设置printf到屏幕上的字符串起始位置
    printf_fast_f("PWM %.2f%%    ",Output_Baud);  //把p2.2输出pwm波的波特率显示在屏幕上
    Output_Voltage = (i/256.0)*5.0;
    setCursor(2,0,4);  //设置printf到屏幕上的字符串起始位置
    printf_fast_f("DA SC %.2fV    ",Output_Voltage);  //把整流后的电压预测数据显示屏幕上 SC含义为shuchu1（输出）
    setCursor(2,0,6);  //设置printf到屏幕上的字符串起始位置
    printf_fast_f("AD CJ %.2fV    ",Voltage);  //显示数据到屏幕上 CJ含义为caiji（采集）
    i+=32;
  }
}

/********************************************************************
* 函 数 名       : putchar
* 函数功能       : 将printf映射到oled屏幕输出上
* 输    入      : 字符串
* 输    出    	: 字符串
********************************************************************/
int putchar(int a)
{
  OLED_ShowChar(fontSize, oled_colum, oled_row, a);
  oled_colum+=8;
  if (oled_colum>120 && fontSize==1) {oled_colum=0; oled_row+=1;}
  if (oled_colum>120 && fontSize==2) {oled_colum=0; oled_row+=2;}
  return(a);
}