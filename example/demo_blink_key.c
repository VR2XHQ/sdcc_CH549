/**
  ******************************************************************
  * @file    demo_blink_key.c
  * @author  Beny
  * @version V0.01
  * @date    2023-12-01
  * @brief   LED BLINK
  ******************************************************************
  * @attention
  * 參考 verimake ch549 例程
  * Simple code for led blink and key input via external interrupt
  ******************************************************************
  */

#include <CH549_sdcc.h>	 //ch549的头文件，其中定义了单片机的一些特殊功能寄存器
#include <CH549_debug.h> //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数

#define led_builtin P2_2  //将单片机的P2.2端口定义为led_builtin
#define led_A P2_4  //将单片机的P2.4端口定义为led_A
#define key_A P3_2  //将单片机的P3.2端口定义为key_A
#define key_B P3_3  //将单片机的P3.3端口定义为key_B

BOOL key_state = FALSE;  //按鍵狀態的全域變量

/********************************************************************
* TIPS:
* ch549外部中断包括外部中断0(P3.2)和外部中断1(P3.3)
* 初始化一个外部中断需要三步
* 第一步：设置跳变沿触发方式 例：IT0=0 选择外部中断0为低电平触发；IT0=1 选择外部中断0为下降边沿触发（IT1同理）
* 第二步：打开外部中断0或外部中断1的中断允许 例：EX0=1 打开INT0的中断允许；EX0=0 关闭INT0的中断允许（IT1同理）
* 第三步：打开总中断 例：EA=1 打开总中断；EA=0,关闭总中断
*********************************************************************/

/********************************************************************
* 函 数 名         : Int0Init()
* 函数功能		     : 设置外部中断0
* 输    入         : 无
* 输    出         : 无
*********************************************************************/
void Int0Init(void)
{
	//设置INT0
	IT0=1;  //跳变沿触发方式（下降沿）
	EX0=1;  //打开INT0的中断允许。	
	EA=1;   //打开总中断	
}

/********************************************************************
* 函 数 名         : Int1Init()
* 函数功能		     : 设置外部中断1
* 输    入         : 无
* 输    出         : 无
*********************************************************************/
void Int1Init(void)
{
	//设置INT1
	IT1=1;  //跳变沿触发方式（下降沿）
	EX1=1;  //打开INT1的中断允许。	
	EA=1;   //打开总中断	
}

/*******************************************************************
* 函 数 名         : Int0()	__interrupt(0)
* 函数功能		     : 外部中断0的中断函数
* 输    入         : 无
* 输    出         : 无
* TIPS：1.Int0 可以修改成任意函数名
*       2.举例：外部中断1 void Int1()	__interrupt(2)
*       3.外部中断0       中断号0
*         定时器0中断     中断号1
*         外部中断1       中断号2
*         定时器1中断     中断号3
*******************************************************************/
void Int0(void)	__interrupt(INT_NO_INT0)		//外部中断0的中断函数
{	
  _delay_ms(120);	 //延时消抖
	if(key_A == 0) key_state = !key_state;
}

void Int1(void)	__interrupt(INT_NO_INT1)
{	
  _delay_ms(120);	 //延时消抖
	if(key_B == 0) led_A = !led_A;
}


void main(void)
{
  int duration = 1000;
  led_A = 0;

  P2_MOD_OC &= ~(1<<4);
  P2_DIR_PU |= (1<<4);

  CfgFsys();  //在debug頭文件設定Fsys
  _delay_ms(20);
  Int0Init();  //对外部中断0（P3.2）初始化
  // Int1Init();  //对外部中断0（P3.3）初始化

  while(1) {

    switch (key_state)
    {
    case TRUE:
      duration = 1000;
      break;
    
    case FALSE:
      duration = 100;
      break;
    
    default:
      break;
    }

    /* 自鎖開關不用中斷 */
    (key_B==1) ? (led_A=1) : (led_A=0);

    led_builtin = !led_builtin;  //取反使內建led熌亮
    _delay_ms(duration);         //控制led熌亮的速度
  }
}