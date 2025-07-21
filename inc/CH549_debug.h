/**
 * Convert WCH CH549 library for SDDC and modified by Beny on 2023-12-25.
*/

/********************************** (C) COPYRIGHT *******************************
* File Name          : DEBUG.H
* Author             : WCH
* Version            : V1.5
* Date               : 2023/05/31
* Description        : CH5XX DEBUG Interface
                     (1)、主频设置;
                     (2)、us\ms基本延时函数;										 
                     (3)、串口0输出打印信息，波特率可调;            				   
                     (4)、看门狗初始化和赋值函数;     										 
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/
#ifndef	__CH549_DEBUG_H__
#define __CH549_DEBUG_H__

#include <stdio.h>
#include <string.h>
#include <CH549_sdcc.h> //CH549的头文件，其中定义了单片机的一些特殊功能寄存器
#include <CH549_gpio.h> //GPIO设置函数定义
#include <CH549_uart123.h> //CH549官方提供库的头文件，定义了一些关于UART1-3初始化，设置等函数
#include <CH549_timer.h> //Timer0&1&2定时，计数设置和捕捉函数定义
// #include <CH549_spi.h> //SPI主从设置和收发函数定义
// #include <CH549_pwm.h> //PWM初始化、设置函数定义
// #include <CH549_adc.h> //ADC初始化，采样时钟设定，通道变化和电压比较功能函数定义
// #include <CH549_compare.h> //比较器使能，通道选择，中断函数
// #include <CH549_touchkey.h> //TouchKey开启，通道切换和中断采集函数
// #include <CH549_oled.h> //程式庫驅動 128x64 oled示屏
// #include <CH549_ledcontrol.h> //程式庫控制8x8LED陣矩

//定义函数返回值
#ifndef  SUCCESS
#define  SUCCESS  1
#endif
#ifndef  FAIL
#define  FAIL  0
#endif

//定义定时器起始
#ifndef START
#define START  1
#endif
#ifndef STOP
#define STOP  0
#endif

//调试开关, 0=使用UART0, 1~3=使用UART1/2/3
#ifndef DE_PRINTF
#define DE_PRINTF  1
#endif

/**
 * 系統主頻為12MHz時, 標準時鐘為FREQ_SYS/12, 即1MHz, 週期為1uS.
 * 因此在FREQ_SYS=24MHz時, 週期為1/2uS, Timer計數2000為週期1mS. 
*/
#define	FREQ_SYS  24000000	     //系统主频24MHz。注: 48MHz仅对定制芯片使用
#define OSC_EN_XT  1             //外部晶振使能，默认开启内部晶振
#define MCU_WORK_AT_3V3  0		 //MCU工作电压：1-3.3V;0-5V. 注意: 请根据MCU实际工作电压进行设置

/**
 * 定時器產生BAUD時, 因計數值的限制, 太高及低都無法正常運作. 以下範圍供參考:
 * FSYS為12MHz, BAUD由 4800-74880;
 * FSYS為24MHz, BAUD由 9600-115200;
*/

/* 定义是否开启串口0中断及波特率配置 */
#ifndef UART0_BAUD
#define UART0_INTERRUPT  0
#define UART0_BAUD  115200
#endif

/* UART1 定义是否开启中断及波特率配置 */
#define   UART1_INTERRUPT  0
#define   UART1_BAUD  115200
/* UART2 定义是否开启中断及波特率配置 */
#define   UART2_INTERRUPT  0
#define   UART2_BAUD  115200
/* UART 3定义是否开启中断及波特率配置 */
#define   UART3_INTERRUPT  0
#define   UART3_BAUD  115200

/* Timer 中斷應用 0=disable, 1=enable */
#define  T0_INT   0
#define  T1_INT   0
#define  T2_INT   0
#define  T2_CAP   0

/* 定義是否使用Timer0中斷閃亮內建LED */
#define T0_LED_BLINK  0
#define LED_BUILTIN  P2_2
#define BLINK_SPEED  1000

#ifndef MIN
#define MIN(a,b)  (((a) <= (b)) ? (a) : (b))
#endif

/* WCH官方程序 */
void CfgFsys(void);                   // CH549时钟选择和配置
void mDelayuS(UINT16 n);              // 以uS为单位延时
void mDelaymS(UINT16 n);              // 以mS为单位延时
void CH549UART0Alter(void);           // CH549串口0引脚映射到P0.2/P0.3
void mInitSTDIO(void);                // T1作为波特率发生器
void CH549SoftReset(void);            // CH549软复位
void CH549WDTModeSelect(UINT8 mode);  // 看门狗模式选择
void CH549WDTFeed(UINT8 tim);         // 喂狗

/* 用於SDCC編繹時的mS單位延時 */
void _delay_ms(UINT32 i); 

/* 增加UART0呼叫 */
UINT8 CH549UART0RcvByte(void);
void CH549UART0SendByte(UINT8 SendDat);

/* 設置串口0是否使用中斷程序 */
#if UART0_INTERRUPT
void UART0Interrupt(void) __interrupt(INT_NO_UART0);
#endif

/* UART123调用子函数 */
void CH549UART1Init(void);                      //UART1初始化函数
void CH549UART1Alter(void);                     //串口1引脚映射
UINT8 CH549UART1RcvByte(void);                  //UART1接收一个字节
void CH549UART1SendByte(UINT8 SendDat);         //UART1发送一个字节
void CH549UART2Init(void);                      //UART2初始化函数
UINT8 CH549UART2RcvByte(void);                  //UART2接收一个字节
void CH549UART2SendByte(UINT8 SendDat);         //UART2发送一个字节
void CH549UART3Init(void);                      //UART3初始化函数
UINT8 CH549UART3RcvByte(void);                  //UART3接收一个字节
void CH549UART3SendByte(UINT8 SendDat);         //UART3发送一个字节

/* 設置串口123是否使用中斷程序 */
#if UART1_INTERRUPT
void UART1Interrupt(void) __interrupt(INT_NO_UART1) __using(1);         //串口1中断服务程序,使用寄存器组1
#endif
#if UART2_INTERRUPT
void UART2Interrupt(void) __interrupt(INT_NO_UART2) __using(1);         //串口2中断服务程序,使用寄存器组1
#endif
#if UART3_INTERRUPT
void UART3Interrupt(void) __interrupt(INT_NO_UART3) __using(1);         //串口3中断服务程序,使用寄存器组1
#endif

/* 設置Time0中斷使用內建LED閃亮 */
#if T0_LED_BLINK
void Debug_LED(void);
void Timer0(void) __interrupt(INT_NO_TMR0);
#endif

/* 倒轉字完順序及取字元的位值 */
UINT8 reverse(UINT8 MSB);
int readbit(UINT8 byte, int bit);
	
#endif
