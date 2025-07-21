/**
 * Convert WCH CH549 library for SDDC and modified by Beny on 2023-12-07.
*/

/********************************** (C) COPYRIGHT *******************************
* File Name          : SPI.H
* Author             : WCH
* Version            : V1.0
* Date               : 2018/08/23
* Description        : CH549 SPI主、从模式接口函数
注：片选有效时，从机会自动加载SPI0_S_PRE的预置值到发送移位缓冲区，所以最好可以在片选
有效前向SPI0_S_PRE寄存器写入预发值，或者在主机端丢弃首个接收字节，发送时注意主机会先
取走SPI0_S_PRE里面的值产生一个S0_IF_BYTE中断。
如果片选从无效到有效，从机首先进行发送的话，最好把输出的首字节放到SPI0_S_PRE寄存器中；
如果已经处于片选有效的话，数据数据使用SPI0_DATA就可以
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/
#ifndef __CH549_SPI_H__
#define __CH549_SPI_H__

#include <CH549_sdcc.h>	 //CH549的头文件，其中定义了单片机的一些特殊功能寄存器 
#include <CH549_debug.h> //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数

#define  SPI_INTERRUPT  0                     //中断使能（仅Slave模式）0=disable 1=enable
#define  SPI_CK_SET(n) (SPI0_CK_SE = n)       //SPI时钟设置函数 

//Master相关函数
extern void  SPIMasterModeSet(UINT8 mode);    //SPI主机模式初始化
extern void  CH549SPIMasterWrite(UINT8 dat);  //CH549硬件SPI写数据，主机模式
extern UINT8 CH549SPIMasterRead(void);        //CH549硬件SPI0读数据，主机模式

//Slave相关函数
extern void  SPISlvModeSet(void);             //SPI从机模式初始化
extern void  CH549SPISlvWrite(UINT8 dat);     //CH549硬件SPI写数据，从机模式
extern UINT8 CH549SPISlvRead(void);           //CH549硬件SPI0读数据，从机模式

//中斷程式宣告
void SPIInterrupt(void) __interrupt(INT_NO_SPI0) __using(1);

#endif
