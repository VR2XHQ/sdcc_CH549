/**
 * WCH CH549 code example, modified by Beny on 2024-01-29.
 * File Name : demo_adc_uart.c
*/

/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.0
* Date               : 2019/07/22
* Description        : CH549 ADC初始化，ADC中断和查询方式演示外部通道以及内部温度检测
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/
#include <CH549_sdcc.h> //CH549的头文件，其中定义了单片机的一些特殊功能寄存器
#include <CH549_debug.h> //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数
#include <CH549_adc.h> //CH549官方提供库的头文件，定义了一些关于ADC初始化，设置等函数

void main(void)
{
    float Voltage;
    UINT8 ch = 0;
    CfgFsys();                           //CH549时钟选择配置24MHz
    _delay_ms(5);                        //稍加延时等待延时稳定
    // mInitSTDIO();                        //串口0初始化
    CH549UART1Init();
    CH549UART1Alter();
    printf("ADC demo start ...\n");

#if 1  //設為0, 執行内部温度检测
    ADC_ExInit(3);                           //ADC初始化,选择采样时钟
    while(1)
    {
    #if (EN_ADC_INT == 0)                    //查询方式演示ADC通道切换和采集
            for(ch=8; ch!=16; ch++)
            {
                ADC_ChSelect(ch);            //选择通道
                ADC_StartSample();           //启动采样
                while((ADC_CTRL&bADC_IF) == 0)
                {
                    ;    //查询等待标志置位
                }
                ADC_CTRL = bADC_IF;                                //清标志
                printf("ch%d=%d  ",(UINT16)ch,(UINT16)ADC_DAT);    //输出ADC采样值
            }
            printf("\n");
            printf("\n");
            _delay_ms(1000);
    #else                                          //中断方式演示ADC通道切换和采集，以通道0为例
        ADC_StartSample();                         //启动采样
        _delay_ms(10);                             //模拟单片机干其他事情							
        if(GetValueFlag==1)                           //通道采集完成标志
        {
            GetValueFlag = 0;
            Voltage = (ADC_DAT / 4095.0)*5.0;
            printf_fast_f("Voltage = %.2fV\n",Voltage);    //输出ADC采样值
        }
        _delay_ms(1000);                            //模拟单片机干其他事情
    #endif
    }
#else                                             //温度检测
    ADC_InTSInit();                               //初始化温度检测
    while(1)
    {
        ADC_StartSample();                        //启动采样
#if EN_ADC_INT
        while(GetValueFlag==0)
        {
            ;
        }
        _delay_ms(20);                            //中断函数处理，也可以添加标志位
#else
        while((ADC_CTRL&bADC_IF) == 0)
        {
            ;    //查询等待标志置位
        }
        ADC_CTRL = bADC_IF;                       //清标志
#endif
        printf("TS value = %d ",(UINT16)ADC_DAT);            //输出ADC采样值
        printf("\n");
        _delay_ms(5000);
    }
#endif
}

