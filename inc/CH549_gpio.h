/**
 * Convert WCH CH549 library for SDDC and modified by Beny on 2023-12-07.
*/

/********************************** (C) COPYRIGHT *******************************
* File Name          : GPIO.H
* Author             : WCH
* Version            : V1.0
* Date               : 2018/08/09
* Description        : CH549 GPIO相关函数
********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/
#ifndef __CH549_GPIO_H__
#define __CH549_GPIO_H__

#include <CH549_sdcc.h>  //CH549的头文件，其中定义了单片机的一些特殊功能寄存器
#include <CH549_debug.h>  //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数

/****************** GPIO_Init 参数定义 ******************/
/* 端口定义 */
#define  PORT0      0x00
#define  PORT1      0x01
#define  PORT2      0x02
#define  PORT3      0x03
#define  PORT4      0x04
/* 引脚定义（可以组合） */
#define  PIN0       0x01
#define  PIN1       0x02
#define  PIN2       0x04
#define  PIN3       0x08
#define  PIN4       0x10
#define  PIN5       0x20
#define  PIN6       0x40
#define  PIN7       0x80
/* 模式定义 */
#define  MODE0      0x00           //高阻输入模式，引脚没有上拉电阻
#define  MODE1      0x01           //推挽输出模式，具有对称驱动能力，可以输出或者吸收较大电流
#define  MODE2      0x02           //开漏输出，支持高阻输入，引脚没有上拉电阻
#define  MODE3      0x03           //准双向模式(标准 8051)，开漏输出，支持输入，引脚有上拉电阻(默认模式)

/****************** GPIO_INT_Init 参数定义 ********************/
/* IntSrc 外部中断(可组合) */
#define  INT_RXD0_L  0x0001
#define  INT_P46_L   0x0002
#define  INT_P57_H   0x0004        //唯一一个高电平（上升沿）有效中断
#define  INT_P03_L   0x0008
#define  INT_P14_L   0x0010
#define  INT_P15_L   0x0020
#define  INT_RXD1_L  0x0040
#define  INT_INT0_L  0x0080
#define  INT_INT1_L  0x0100
/* 中断触发模式 */
#define  INT_LEVEL   0x00          //电平模式
#define  INT_EDGE    0x01          //边沿模式
/* 中断状态 */
#define  Disable     0x00          //中断无效
#define  Enable      0x01          //中断使能

/****************** 外部调用子函数 ****************************/
extern void GPIO_Init(UINT8 PORTx, UINT8 PINx, UINT8 MODEx);  //GPIO初始化
extern void GPIO_INT_Init(UINT16 IntSrc, UINT8 Mode, UINT8 NewState);  //外部中断初始化

#endif
