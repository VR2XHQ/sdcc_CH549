/**
  ******************************************************************
  * @file    pwm_demo_led.c
  * @author  Beny
  * @version V0.02
  * @date    2024-01-24
  * @brief   呼吸灯
  ******************************************************************
  * @attention
  * 參考 verimake 及 WCH ch549 例程
  * 使用pwm外设让板子上的灯展示呼吸灯的效果
  ******************************************************************
  */

#include <CH549_sdcc.h>  //ch549的头文件，其中定义了单片机的一些特殊功能寄存器
#include <CH549_debug.h>  //CH549官方提供库的头文件，定义了一些关于时钟设置等函数
#include <CH549_pwm.h>  //PWM初始化、设置函数定义

/********************************************************************
* TIPS:
*  引脚            功能
*  P2.5            PWM0
*  P2.4            PWM1
*  P2.3            PWM2
*  P2.2            PWM3
*  P2.1            PWM4
*  P2.0            PWM5
*  P2.6            PWM6
*  P2.7            PWM7
*********************************************************************/

void main(void)
{
  CfgFsys();
  _delay_ms(5);
  mInitSTDIO();
  printf("Demo start ...\n");
  SetPWMClkDiv(4); //PWM时钟配置,Fsys/4,Fsys默认为12Mhz
  SetPWMCycle256Clk(); //PWM周期 Fsys/4/256
  // SetPWMCycle64Clk(); //PWM周期 Fsys/4/256

  /* 启动通道 */
  PWM_SEL_CHANNEL(ch1,Enable); //对ch1，即p2.4，即led的pwm外设初始化
  PWM_SEL_CHANNEL(ch3,Enable); //对ch3，即p2.2，即led的pwm外设初始化
  PWM_SEL_CHANNEL(ch6,Enable); //对ch6，即p2.6
  PWM_SEL_CHANNEL(ch7,Enable); //对ch7，即p2.7
  SetPWM1Dat(200);
  SetPWM3Dat(50);
  SetPWM6Dat(100);
  SetPWM7Dat(150);

  PWMInterruptEnable();

  while(1)
  {
    int i = 0;
    /*配置占空比使得led灯从灭逐步到亮*/
    for ( i = 0; i <= 255; i++)
    {
        SetPWM1Dat(i);//配置占空比，占空比等于i/256
        _delay_ms(30);
    }
    /*配置占空比使得led灯从亮逐步到灭*/
    for ( i = 255; i >= 0; i--)
    {
        SetPWM1Dat(i);//配置占空比，占空比等于i/256
        _delay_ms(30);
    }
  }
}