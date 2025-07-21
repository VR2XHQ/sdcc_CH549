/**
 * Convert WCH CH549 library for SDDC and modified by Beny on 2024-01-29.
*/

/********************************** (C) COPYRIGHT *******************************
* File Name          : TouchKey.H
* Author             : WCH
* Version            : V1.0
* Date               : 2019/07/22
* Description        : CH549 TouchKey触摸按键
*                      CH0~CH15 分别对应引脚 P1.0~P1.7  P0.0~P0.7
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/

#ifndef __CH549_TOUCHKEY_H__
#define __CH549_TOUCHKEY_H__

#include <CH549_sdcc.h>	 //CH549的头文件，其中定义了单片机的一些特殊功能寄存器 
#include <CH549_debug.h> //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数

//ADC中断使能开关
#ifndef EN_ADC_INT
#define EN_ADC_INT  0
#endif

#define   DOWM_THRESHOLD_VALUE        1000                      //按下阈值定义
#define   UP_THRESHOLD_VALUE          50                        //抬起阈值
#define   KEY_BUF_LEN                  10                       //每个KEY按键对应的滤波缓冲区大小（库修改无效）20
#define   KEY_BUF_LOST                  2                       //排序后前后分别丢弃的字节数（库修改无效） 5

// extern UINT16 __xdata KeyBuf[16][KEY_BUF_LEN];

void TouchKey_Init(void);
UINT16 TouchKeySelect(UINT8 ch, UINT8 cpw);
UINT16 Default_TouchKey(UINT8 ch, UINT8 cpw);

#endif
