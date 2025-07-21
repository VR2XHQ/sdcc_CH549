#ifndef __CH549_UART123_H__
#define __CH549_UART123_H__

/**
 * Convert WCH CH549 library for SDDC and modified by Beny on 2024-01-27.
*/

/********************************** (C) COPYRIGHT *******************************
* File Name          : UART.H
* Author             : WCH
* Version            : V1.0
* Date               : 2018/08/09
* Description        : CH549 串口初始化相关
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/
#include <CH549_sdcc.h> //ch549的头文件，其中定义了单片机的一些特殊功能寄存器
#include <CH549_debug.h> //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数

/********************************************************************
* TIPS:
*  引脚            功能
*  P2.6            默认UART1——RXD1
*  P2.7            默认UART1——TXD1
*  P1.6            UART1引脚映射————RXD1_
*  P1.7            UART1引脚映射————TXD1_
*  P0.4            UART2——RXD2
*  P0.5            UART2——TXD2
*  P0.6            UART3——RXD3
*  P0.7            UART3——TXD3
*********************************************************************/

/* 以下設定, 放到 CH549_debug.h 宣告. */
// /* UART1相关 */
// #define   UART1_INTERRUPT      0                    //定义是否开启串口1中断
// #define   UART1_BAUD           115200               //串口1波特率配置
// /* UART2相关 */
// #define   UART2_INTERRUPT      0                    //定义是否开启串口2中断
// #define   UART2_BAUD           115200               //串口2波特率配置
// /* UART3相关 */
// #define   UART3_INTERRUPT      1                    //定义是否开启串口3中断
// #define   UART3_BAUD           115200               //串口2波特率配置

/* 外部调用子函数 */
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

#if UART1_INTERRUPT
void UART1Interrupt(void) __interrupt(INT_NO_UART1) __using(1);         //串口1中断服务程序,使用寄存器组1
#endif

#if UART2_INTERRUPT
void UART2Interrupt(void) __interrupt(INT_NO_UART2) __using(1);         //串口2中断服务程序,使用寄存器组1
#endif

#if UART3_INTERRUPT
void UART3Interrupt(void) __interrupt(INT_NO_UART3) __using(1);         //串口3中断服务程序,使用寄存器组1
#endif

#endif
