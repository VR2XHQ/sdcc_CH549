/**
 * WCH CH549 code example, modified by Beny on 2024-01-29.
 * File Name : demo_adc_compare.c
*/

/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.0
* Date               : 2018/08/09
* Description        : CH549 外部通道和内部参考电压比较；外部通道和外部通道比较；
                       内部参考电压和外部通道比较比较器使能，通道选择，中断函数
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/
#include <CH549_sdcc.h>	 //CH549的头文件，其中定义了单片机的一些特殊功能寄存器 
#include <CH549_debug.h> //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数
#include <CH549_compare.h> //比较器使能，通道选择，中断函数

void main(void)
{
    CfgFsys(); //CH549时钟选择配置
    _delay_ms(20);
    // mInitSTDIO(); //串口0初始化
    CH549UART1Init();
    CH549UART1Alter();
    printf("CMP demo start ...\n");
    // CMP_Init(); //比较器通道初始化
    CMP_Ext_IN_Plus_SELT(0);
    CMP_IN_Minus_Signal_SELT(2);
    CMP_Inter_IN_Plus_Signal_SELT(3);

    while(1)
    {
#if (EN_ADC_INT==0)
        if(ADC_CTRL&bCMPO)
        {
            printf("+ > -\n");
        }
        else
        {
            printf("+ < -\n");
        }
        _delay_ms(1000);
#endif
    }
}
