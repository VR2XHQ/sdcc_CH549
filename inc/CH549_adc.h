/**
 * Convert WCH CH549 library for SDDC and modified by Beny on 2023-12-17.
*/

/********************************** (C) COPYRIGHT *******************************
* File Name          : ADC.H
* Author             : WCH
* Version            : V1.4
* Date               : 2021/12/15
* Description        : CH549 ADC采样时钟设置，ADC通道设置函数，温度检测初始化
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/
#ifndef __CH549_ADC_H__
#define __CH549_ADC_H__

#include <CH549_sdcc.h> //CH549的头文件，其中定义了单片机的一些特殊功能寄存器
#include <CH549_debug.h> //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数

/* 配合範例程式的中斷宣告, 官方例程放主程序, SDCC編繹出錯, 需在頭文件內宣告為外部使用. */ 
extern volatile UINT8 GetValueFlag;

//ADC中断使能开关
#ifndef EN_ADC_INT
#define  EN_ADC_INT  1
#endif

//通道号选择,分别对应P1.0~P1.7, P0.0~P0.7
#define  CH0                            0
#define  CH1                            1
#define  CH2                            2
#define  CH3                            3
#define  CH4                            4
#define  CH5                            5
#define  CH6                            6
#define  CH7                            7
#define  CH8                            8
#define  CH9                            9
#define  CH10                           0x0A
#define  CH11                           0x0B
#define  CH12                           0x0C
#define  CH13                           0x0D
#define  CH14                           0x0E
#define  CH15                           0x0F
#define  CHALL                          0xFFFF

//快速调用宏定义
#define  ADC_ExChannelSelect(ch)      ADC_CHAN = ADC_CHAN & ~MASK_ADC_CHAN | ch   //外部通道取值 0~15
#define  ADC_StartSample()            ADC_CTRL = bADC_START                       //启动一次ADC采样

//ADC相关子函数
void ADC_ExInit(UINT8 AdcClk);
void ADC_ChSelect(UINT8 ch);
void ADC_InTSInit(void);
void ADC_ISR(void) __interrupt(INT_NO_ADC);

#endif
