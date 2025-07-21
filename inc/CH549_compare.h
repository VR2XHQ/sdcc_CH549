/**
 * Convert WCH CH549 library for SDDC and modified by Beny on 2024-01-29.
*/

/********************************** (C) COPYRIGHT *******************************
* File Name          : Compare.H
* Author             : WCH
* Version            : V1.0
* Date               : 2018/08/09
* Description        : CH549 ADC电压比较
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/
#ifndef __CH549_COMPARE_H__
#define __CH549_COMPARE_H__

#include <CH549_sdcc.h>	 //CH549的头文件，其中定义了单片机的一些特殊功能寄存器 
#include <CH549_debug.h> //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数

//ADC中断使能开关
#ifndef EN_ADC_INT
#define EN_ADC_INT  0
#endif

/* 官方提供初始化範例 */
void CMP_Init(void);

/* 官方車提供的設定呼叫 */
void CMP_IN_Minus_Signal_SELT(UINT8 mode);
void CMP_Inter_IN_Plus_Signal_SELT(UINT8 mode);
void CMP_Ext_IN_Plus_SELT(UINT8 ch);
void CMP_Ext_IN_Plus_Float(void);

#if(EN_ADC_INT==1)
void ADC_ISR(void) __interrupt(INT_NO_ADC);
#endif

#endif
