/**
 * Convert WCH CH549 library for SDDC and modified by Beny on 2024-01-24.
*/

/********************************** (C) COPYRIGHT *******************************
* File Name          : PWM.H
* Author             : WCH
* Version            : V1.0
* Date               : 2018/08/29
* Description        : CH549 PWM配置与可选的中断处理（PWM 循环周期结束中断）
*                      引脚           功能
*                      P25            PWM0
*                      P24            PWM1
*                      P23            PWM2
*                      P22            PWM3
*                      P21            PWM4
*                      P20            PWM5
*                      P26            PWM6
*                      P27            PWM7
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/
#ifndef __CH549_PWM_H__
#define __CH549_PWM_H__

#include <CH549_sdcc.h>	//CH549的头文件，其中定义了单片机的一些特殊功能寄存器
#include <CH549_debug.h> //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数

#define PWM_INTERRUPT

/* PWM通道定义（可以组合） */
#define   ch0        0x01
#define   ch1        0x02
#define   ch2        0x04
#define   ch3        0x08
#define   ch4        0x10
#define   ch5        0x20
#define   ch6        0x40
#define   ch7        0x80

/* 输出使能状态 */
#define  Disable     0x00                                                 //关闭输出
#define  Enable      0x01                                                 //使能输出

/* 外部调用恒定义 */
#define SetPWMClkDiv(n)          PWM_CK_SE = n                            //PWM时钟配置,FREQ_SYS/n  
#define SetPWMCycle64Clk()       PWM_CTRL |= bPWM_MOD_6BIT                //PWM周期为 FREQ_SYS/n/64
#define SetPWMCycle256Clk()      PWM_CTRL &= ~bPWM_MOD_6BIT               //PWM周期为 FREQ_SYS/n/256
#define PWM0OutPolarHighAct()    PWM_CTRL &= ~bPWM0_POLAR                 //PWM0输出默认低，高有效(默认)
#define PWM0OutPolarLowAct()     PWM_CTRL |= bPWM0_POLAR                  //PWM0输出默认高，低有效
#define PWM1OutPolarHighAct()    PWM_CTRL &= ~bPWM1_POLAR                 //PWM1输出默认低，高有效(默认)          
#define PWM1OutPolarLowAct()     PWM_CTRL |= bPWM1_POLAR                  //PWM1输出默认高，低有效
#define SetPWM0Dat(dat)          PWM_DATA0 = dat                          //设置PWM0输出占空比
#define SetPWM1Dat(dat)          PWM_DATA1 = dat                          //设置PWM1输出占空比
#define SetPWM2Dat(dat)          PWM_DATA2 = dat                          //设置PWM2输出占空比
#define SetPWM3Dat(dat)          PWM_DATA3 = dat                          //设置PWM3输出占空比
#define SetPWM4Dat(dat)          PWM_DATA4 = dat                          //设置PWM4输出占空比
#define SetPWM5Dat(dat)          PWM_DATA5 = dat                          //设置PWM5输出占空比
#define SetPWM6Dat(dat)          PWM_DATA6 = dat                          //设置PWM6输出占空比
#define SetPWM7Dat(dat)          PWM_DATA7 = dat                          //设置PWM7输出占空比

/* 外部调用子函数 */
void PWM_SEL_CHANNEL(UINT8 Channel, UINT8 NewState); //PWM通道输出使能,包含IO端口配置

#ifdef PWM_INTERRUPT
void PWMInterruptEnable(void); //使能PWM周期溢出中断
void PWMInterrupt(void) __interrupt(INT_NO_PWMX) __using(1);
#endif

#endif
