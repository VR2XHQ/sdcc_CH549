/**
 * Convert WCH CH549 library for SDDC and modified by Beny on 2024-01-22.
*/

/********************************** (C) COPYRIGHT *******************************
* File Name          : Timer.C
* Author             : WCH
* Version            : V1.0
* Date               : 2018/08/21
* Description        : CH549 Time 初始化、定时器、计数器赋值、T2捕捉功能开启函数等
                       定时器中断函数
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/
#include <CH549_timer.h> //Timer0&1&2定时，计数设置和捕捉函数定义

#ifdef  T2_CAP
UINT16 Cap2[2] = {0};
UINT16 Cap1[2] = {0};
UINT16 Cap0[2] = {0};
#endif

/*******************************************************************************
* Function Name  : mTimer_x_ModInit(UINT8 x, UINT8 mode)
* Description    : CH549定时计数器x模式设置
* Input          : UINT8 mode,Timer模式选择
                   0：模式0，13位定时器，TLn的高3位无效
                   1：模式1，16位定时器
                   2：模式2，8位自动重装定时器
                   3：模式3，两个8位定时器  Timer0
                   3：模式3，Timer1停止
* Output         : None
* Return         : 成功  SUCCESS
                   失败  FAIL
*******************************************************************************/
UINT8 mTimer_x_ModInit(UINT8 x, UINT8 mode)
{
    if(x == 0)
    {
        TMOD = TMOD & 0xf0 | mode;
    }
    else if(x == 1)
    {
        TMOD = TMOD & 0x0f | (mode<<4);
    }
    else if(x == 2)
    {
        RCLK = 0; //16位自动重载定时器
        TCLK = 0;
        CP_RL2 = 0;
    }
    else
    {
        return FAIL;
    }
    return SUCCESS;
}

/*******************************************************************************
* Function Name  : mTimer_x_SetData_16bit(UINT8 x, UINT16 dat)
* Description    : CH549Timer THx和TLx赋值
* Input          : UINT16 dat;定时器赋值
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 mTimer_x_SetData_16bit(UINT8 x, UINT16 dat)
{
    UINT16 tmp;
    tmp = 65536 - dat; //範圍 1-65536, 太少不宜.
    if(x == 0)
    {
        TL0 = tmp & 0xff;
        TH0 = (tmp>>8) & 0xff;
    }
    else if(x == 1)
    {
        TL1 = tmp & 0xff;
        TH1 = (tmp>>8) & 0xff;
    }
    else if(x == 2)
    {
        RCAP2L = TL2 = tmp & 0xff; //16位自动重载定时器
        RCAP2H = TH2 = (tmp>>8) & 0xff;
    }
    else
    {
        return FAIL;
    }
    return SUCCESS;    
}

/*******************************************************************************
* Function Name  : mTimer_x_SetData13bit(UINT8 x, UINT16 dat)
* Description    : CH549Timer THx和TLx赋值
* Input          : UINT16 dat;定时器赋值 (模式0,13bit,TLx高3位無效)
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 mTimer_x_SetData_13bit(UINT8 x, UINT16 dat)
{
    UINT16 tmp;
    tmp = 8192 - dat; //範圍 1-8192, 太少不宜.
    if(x == 0)
    {
        TL0 = tmp & 0x1f;
        TH0 = (tmp>>5) & 0xff;
    }
    else if(x == 1)
    {
        TL1 = tmp & 0x1f;
        TH1 = (tmp>>5) & 0xff;
    }
    else
    {
        return FAIL;
    }
    return SUCCESS;
}

/*******************************************************************************
* Function Name  : mTimer_x_SetData8bit(UINT8 x, UINT8 dat)
* Description    : CH549Timer THx和TLx赋值
* Input          : UINT16 dat;定时器赋值 (模式2,8bit,TLx=THx)
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 mTimer_x_SetData_8bit(UINT8 x, UINT8 dat)
{
    UINT8 tmp;
    tmp = 256 - dat; //範圍 1-256
    if(x == 0)
    {
        TL0 = TH0 = tmp;
    }
    else if(x == 1)
    {
        TL1 = TH1 = tmp;
    }
    else
    {
        return FAIL;
    }
    return SUCCESS;
}

/*******************************************************************************
* Function Name  : CAP2Init(UINT8 mode)
* Description    : CH549定时计数器2 T2EX引脚捕捉功能初始化（CAP2 P11）
                   UINT8 mode,边沿捕捉模式选择
                   0:T2ex从下降沿到下一个下降沿
                   1:T2ex任意边沿之间
                   3:T2ex从上升沿到下一个上升沿
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAP2Init(UINT8 mode)
{
    RCLK = 0;
    TCLK = 0;
    C_T2  = 0;
    EXEN2 = 1;
    CP_RL2 = 1; //启动T2ex的捕捉功能
    T2MOD |= mode << 2; //边沿捕捉模式选择
}

/*******************************************************************************
* Function Name  : CAP1Init(UINT8 mode)
* Description    : CH549定时计数器2 T2引脚捕捉功能初始化T2(CAP1 P10)
                   UINT8 mode,边沿捕捉模式选择
                   0:T2从下降沿到下一个下降沿
                   1:T2任意边沿之间
                   3:T2从上升沿到下一个上升沿
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void CAP1Init(UINT8 mode)
{
    RCLK = 0;
    TCLK = 0;
    CP_RL2 = 1;
    C_T2 = 0;
    T2MOD = T2MOD & ~T2OE | (mode << 2) | bT2_CAP1_EN; //使能T2引脚捕捉功能,边沿捕捉模式选择
}

/*******************************************************************************
* Function Name  : CAP0Init(UINT8 mode)
* Description    : CH549定时计数器2 CAP0引脚捕捉功能初始化(CAP0 P36)
                   UINT8 mode,边沿捕捉模式选择
                   0:CAP0从下降沿到下一个下降沿
                   1:CAP0任意边沿之间
                   3:CAP0从上升沿到下一个上升沿
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAP0Init(UINT8 mode)
{
    RCLK = 0;
    TCLK = 0;
    CP_RL2 = 1;
    C_T2 = 0;
    T2MOD |= mode << 2; //边沿捕捉模式选择
    T2CON2 = bT2_CAP0_EN;
}

#if T0_INT
/*******************************************************************************
* Function Name  : mTimer0Interrupt()
* Description    : CH549定时计数器0定时计数器中断处理函数 1ms中断
*******************************************************************************/
void mTimer0Interrupt(void) __interrupt(INT_NO_TMR0) __using(1) //timer0中断服务程序,使用寄存器组1
{
    // mTimer_x_SetData_8bit(0,100); //模式3 需重新给TH0和TL0赋值, TH0給Timer1使用
    // mTimer_x_SetData(0,2000); //模式1 需重新给TH0和TL0赋值
    mTimer_x_SetData_13bit(0,2000); //模式0 需重新给TH0和TL0赋值
    SCK = !SCK; //大约1ms (模式2/3不適用)
    static int i; //设置局部变量 i
	i++;
	if(i==BLINK_SPEED) //每进入一次中断则是1ms, 1000次则是1s.
	{
		i = 0;
		LED_BUILTIN = !LED_BUILTIN;
	}	
}
#endif

#if T1_INT
/*******************************************************************************
* Function Name  : mTimer1Interrupt()
* Description    : CH549定时计数器1定时计数器中断处理函数 100us中断
*******************************************************************************/
void mTimer1Interrupt(void) __interrupt(INT_NO_TMR1) __using(2) //timer1中断服务程序,使用寄存器组2
{
    //模式3 需重新给TH0和TL0赋值, TH0給Timer1使用
    // mTimer_x_SetData_8bit(0,100);
    
    //方式2时，Timer1自动重装
    static UINT16 tmr1 = 0;
    tmr1++;
    if(tmr1 == 2000) //100us*2000 = 200ms
    {
        SCK = !SCK;
        tmr1 = 0;
		LED_BUILTIN = !LED_BUILTIN;
    }
}
#endif

/*******************************************************************************
* Function Name  : mTimer2Interrupt()
* Description    : CH549定时计数器2定时计数器中断处理函数
*******************************************************************************/
void mTimer2Interrupt(void) __interrupt(INT_NO_TMR2) __using(3) //timer2中断服务程序,使用寄存器组3
{
#if T2_INT
    static UINT8 tmr2 = 0;
    if(TF2)
    {
        SCK = !SCK; //P17电平指示监控
        TF2 = 0; //清空定时器2溢出中断,需手动请
        tmr2++;
        if(tmr2 == 100) //200ms时间到
        {
            tmr2 = 0;
            LED_BUILTIN = !LED_BUILTIN;
        }
    }
#endif

#if T2_CAP
    if(EXF2) //T2ex电平变化中断中断标志
    {
        Cap2[0] = RCAP2; //T2EX
        printf("CAP2 %04x\n",Cap2[0]-Cap2[1]);
        Cap2[1] = Cap2[0];
        EXF2 = 0; //清空T2ex捕捉中断标志
    }
    if(CAP1F) //T2电平捕捉中断标志
    {
        Cap1[0] = T2CAP1; //T2;
        printf("CAP1 %04x\n",Cap1[0]-Cap1[1]);
        Cap1[1] = Cap1[0];
        CAP1F = 0; //清空T2捕捉中断标志
    }
    if(T2CON2&bT2_CAP0F)
    {
        Cap0[0] = T2CAP0;
        printf("CAP0 %04x\n",Cap0[0]-Cap0[1]);
        Cap0[1] = Cap0[0];
        T2CON2 &= ~bT2_CAP0F;
    }
#endif
}

