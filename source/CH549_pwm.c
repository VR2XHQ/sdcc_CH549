/**
 * Convert WCH CH549 library for SDDC and modified by Beny on 2024-01-24.
*/

/********************************** (C) COPYRIGHT *******************************
* tips               ：官方pwm库
* File Name          : PWM.C
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
*******************************************************************************/
#include <CH549_pwm.h> //PWM初始化、设置函数定义

/*******************************************************************************
* Function Name  : PWM_CFG_CHANNEL()
* Description    : PWM通道输出使能,包含IO端口配置
* Input          : Channel：通道号，位域表示
*                  NewState：0:关闭通道  1:开启通道
* Output         : None
* Return         : None
*******************************************************************************/
void PWM_SEL_CHANNEL(UINT8 Channel, UINT8 NewState)
{
    UINT8 i;
    /* 通道配置 */
    if(NewState == Enable) //输出开启
    {
        PWM_CTRL &= ~bPWM_CLR_ALL;
        if(Channel&ch0)
        {
            PWM_CTRL |= bPWM0_OUT_EN;
        }
        if(Channel&ch1)
        {
            PWM_CTRL |= bPWM1_OUT_EN;
        }
        PWM_CTRL2 |= (Channel>>2); //Beny: 官方用了 '=', 應用 '|=', 否則ch3-7不能獨立設定.

        /* 对应的GPIO口配置成推挽模式 */
        for(i=0; i!=6; i++)
        {
            if(Channel & (1<<i))
            {
                P2_MOD_OC &= ~(1<<(5-i));
                P2_DIR_PU |= (1<<(5-i));
            }
        }
        if(Channel&ch6)
        {
            P2_MOD_OC &= ~ch6;
            P2_DIR_PU |= ch6;
        }
        if(Channel&ch7)
        {
            P2_MOD_OC &= ~ch7;
            P2_DIR_PU |= ch7;
        }
    }
    else //输出关闭
    {
        if(Channel&ch0)
        {
            PWM_CTRL &= ~bPWM0_OUT_EN;
        }
        if(Channel&ch1)
        {
            PWM_CTRL &= ~bPWM1_OUT_EN;
        }
        PWM_CTRL2 &= ~(Channel>>2);
    }
}

#ifdef PWM_INTERRUPT
/*******************************************************************************
* Function Name  : PWMInterruptEnable()
* Description    : PWM中断使能
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PWMInterruptEnable(void)
{
    PWM_CTRL |= bPWM_IF_END; //清除PWM中断
    SCON3 |= ~bU3IE; //开启PWM中断，关闭UART3中断
    IE_PWMX = 1;
    EA = 1;
}

/*******************************************************************************
* Function Name  : PWMInterrupt(void)
* Description    : PWM中断服务程序 FREQ_SYS/n/64或FREQ_SYS/n/256 进中断
*******************************************************************************/
void PWMInterrupt(void) __interrupt(INT_NO_PWMX) __using(1) //PWM0~8中断服务程序,使用寄存器组1
{
    static UINT8 duty = 0;
    static int i = 0;
    PWM_CTRL |= bPWM_IF_END; //清除PWM中断
    /* 利用中斷修改占空比令led灯从灭逐步到亮 */
    if(i == 100)
    {
        i = 0;
        /* 可选的,重新配置占空比 */
        SetPWM3Dat(256-duty);
        duty++;
    }
    i++;
}
#endif
