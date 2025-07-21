/**
 * Convert WCH CH549 library for SDDC and modified by Beny on 2023-12-25.
 */

/********************************** (C) COPYRIGHT *******************************
* File Name          : DEBUG.C
* Author             : WCH
* Version            : V1.5
* Date               : 2023/05/31
* Description        : CH5XX DEBUG Interface
					 (1)、主频设置;
					 (2)、us\ms基本延时函数;
					 (3)、串口0输出打印信息，波特率可调;
					 (4)、看门狗初始化和赋值函数;
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/
#include <CH549_debug.h> //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数
// #pragma less_pedantic
// #pragma disable_warning 110

/*******************************************************************************
* Function Name  : CfgFsys( )
* Description    : CH5XX时钟选择和配置函数,默认使用内部晶振24MHz，如果定义了FREQ_SYS可以
				   根据PLL_CFG和CLOCK_CFG配置得到，公式如下：
				   Fsys = (Fosc * ( PLL_CFG & MASK_PLL_MULT ))/(CLOCK_CFG & MASK_SYS_CK_DIV);
				   具体时钟需要自己配置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CfgFsys(void)
{
#if OSC_EN_XT
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	CLOCK_CFG |= bOSC_EN_XT; // 使能外部晶振
	SAFE_MOD = 0x00;
	mDelaymS(10);
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	CLOCK_CFG &= ~bOSC_EN_INT; // 关闭内部晶振
	SAFE_MOD = 0x00;
#else
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	CLOCK_CFG |= bOSC_EN_INT; // 使能内部晶振
	CLOCK_CFG &= ~bOSC_EN_XT; // 关闭外部晶振
	SAFE_MOD = 0x00;
#endif

	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
#if FREQ_SYS == 48000000
	CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x07; // 48MHz
#endif
#if FREQ_SYS == 32000000
	CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x06; // 32MHz
#endif
#if FREQ_SYS == 24000000
	CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x05; // 24MHz
#endif
#if FREQ_SYS == 16000000
	CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x04; // 16MHz
#endif
#if FREQ_SYS == 12000000
	CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x03; // 12MHz
#endif
#if FREQ_SYS == 3000000
	CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x02; // 3MHz
#endif
#if FREQ_SYS == 750000
	CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x01; // 750KHz
#endif
#if FREQ_SYS == 187500
	CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x00; // 187.5KHz
#endif
	SAFE_MOD = 0x00;

#if MCU_WORK_AT_3V3
	if ((POWER_CFG & 0x07) >= 2)
	{
		SAFE_MOD = 0x55;
		SAFE_MOD = 0xAA;
		POWER_CFG -= 2;
		SAFE_MOD = 0x00;
	}
	else if ((POWER_CFG & 0x07) == 1)
	{
		SAFE_MOD = 0x55;
		SAFE_MOD = 0xAA;
		POWER_CFG &= 0xF8;
		SAFE_MOD = 0x00;
	}
#endif
}

/* Beny: For Keil, not suitable for SDCC compiler. */
/*******************************************************************************
 * Function Name  : mDelayus(UNIT16 n)
 * Description    : us延时函数
 * Input          : UNIT16 n
 * Output         : None
 * Return         : None
 *******************************************************************************/
void mDelayuS(UINT16 n) // 以uS为单位延时
{
	while (n)
	{				// total = 12~13 Fsys cycles, 1uS @Fsys=12MHz
		++SAFE_MOD; // 2 Fsys cycles, for higher Fsys, add operation here
#ifdef FREQ_SYS
#if FREQ_SYS >= 14000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 16000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 18000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 20000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 22000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 24000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 26000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 28000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 30000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 32000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 34000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 36000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 38000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 40000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 42000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 44000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 46000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 48000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 50000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 52000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 54000000
		++SAFE_MOD;
#endif
#if FREQ_SYS >= 56000000
		++SAFE_MOD;
#endif
#endif
		--n;
	}
}

/* Beny: For Keil, not suitable for SDCC compiler. */
/*******************************************************************************
 * Function Name  : mDelayms(UNIT16 n)
 * Description    : ms延时函数
 * Input          : UNIT16 n
 * Output         : None
 * Return         : None
 *******************************************************************************/
void mDelaymS(UINT16 n) // 以mS为单位延时
{
	while (n)
	{
		mDelayuS(1000);
		--n;
	}
}

/********************************************************************
 * Function Name  : _delay_ms(UNIT32 i)
 * Description    : ms delay based on Fsys for SDCC compiler
 * Input          : UNIT32 n
 * Output         : None
 * Return         : None
 ********************************************************************/
void _delay_ms(UINT32 i)
{
	UINT32 j;

#if FREQ_SYS == 48000000
	j = i * (4000000 / 1900); // 48MHz
#elif FREQ_SYS == 32000000
	j = i * (2666000 / 1900); // 32MHz
#elif FREQ_SYS == 24000000
	j = i * (2000000 / 1900); // 24MHz
#elif FREQ_SYS == 16000000
	j = i * (1333000 / 1900); // 16MHz
#elif FREQ_SYS == 12000000
	j = i * (1000000 / 1900); // 12MHz
#elif FREQ_SYS == 3000000
	j = i * (250000 / 1900); // 3MHz
#elif FREQ_SYS == 750000
	j = i * (62500 / 1900); // 750KHz
#elif FREQ_SYS == 187500
	j = i * (15625 / 1900); // 187.5KHz
#endif

	while (j--);
}

/*******************************************************************************
 * Function Name  : CH549UART0Alter()
 * Description    : CH549串口0引脚映射,串口映射到P0.2(R)和P0.3(T)
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH549UART0Alter(void)
{
	P0_MOD_OC |= (3 << 2); // 准双向模式
	P0_DIR_PU |= (3 << 2);
	PIN_FUNC |= bUART0_PIN_X; // 开启引脚复用功能
}

/*******************************************************************************
* Function Name  : mInitSTDIO()
* Description    : CH549串口0初始化,默认使用T1作UART0的波特率发生器,也可以使用T2
				   作为波特率发生器, P3_0 RXD, P3_1 TXD
* Input          : None
* Output         : None+
* Return         : None
*******************************************************************************/
void mInitSTDIO(void)
{
	UINT32 x;
	volatile UINT8 x2; // 袛設這為 volatile, 解決 warning 110, 生成碼亦不增加太多. Beny on 2023-12-08
	/**
	 * 問 Bing AI 結果:
	 * 警告 110 表示编译器优化器更改了条件流程，因为它发现可以将测试 1 的代码删除。这是一个严格的警告，
	 * 但是您可以使用以下方法之一来消除它：
	 * 使用 --less-pedantic 或 --disable-warning 110 编译选项来禁用警告。
	 * 在代码中添加 #pragma less_pedantic 或 #pragma disable_warning 110 来禁用警告.
	 * 如果您仍然遇到问题，您可以尝试使用 volatile 关键字来解决警告。
	 * 将变量 x 和 x2 声明为 volatile 可以告诉编译器，这些变量的值可能会在程序的其他部分中更改，因此不应进行优化.
	*/

	SM0 = 0;
	SM1 = 1;
	SM2 = 0; //串口0使用模式1
	/* 使用Timer1作为波特率发生器 */
	RCLK = 0; //UART0接收时钟
	TCLK = 0; //UART0发送时钟
	PCON |= SMOD; //設定為快速波特率
	x = 10 * FREQ_SYS / 16 / UART0_BAUD; //如果更改主频，注意x的值不要溢出
	x2 = x % 10;
	x /= 10;
	if (x2 >= 5)
		x++; // 四舍五入

	TMOD = TMOD & ~bT1_GATE & ~bT1_CT & ~MASK_T1_MOD | bT1_M1; //0X20，Timer1作为8位自动重载定时器
	T2MOD = T2MOD | bTMR_CLK | bT1_CLK; //Timer1时钟选择
	TH1 = 0 - (UINT8)x; //12MHz晶振,buad/12为实际需设置波特率 <<== 官方說法不明
	TR1 = 1; //启动定时器1
	TI = 1;
	REN = 1; //串口0接收使能
#if UART0_INTERRUPT //开启中断使能
    ES = 1;
    EA = 1;
#endif
}

/*******************************************************************************
 * Function Name  : CH549SoftReset()
 * Description    : CH549软复位
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH549SoftReset(void)
{
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	GLOBAL_CFG |= bSW_RESET;
}

/*******************************************************************************
* Function Name  : CH549WDTModeSelect(UINT8 mode)
* Description    : CH549看门狗模式选择
*                  8位计数器，溢出周期（秒）： (131072/FREQ_SYS)*（256-WDOG_COUNT）
* Input          : UINT8 mode
				   0  timer
				   1  watchDog
* Output         : None
* Return         : None
*******************************************************************************/
void CH549WDTModeSelect(UINT8 mode)
{
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xaa; //进入安全模式
	if (mode)
	{
		GLOBAL_CFG |= bWDOG_EN; //启动看门狗复位
	}
	else
		GLOBAL_CFG &= ~bWDOG_EN; //启动看门狗仅仅作为定时器
	SAFE_MOD = 0x00; //退出安全模式
	WDOG_COUNT = 0;	//看门狗赋初值
}

/*******************************************************************************
* Function Name  : CH549WDTFeed(UINT8 tim)
* Description    : CH549看门狗定时时间设置
* Input          : UINT8 tim 看门狗复位时间设置
				   00H(12MHz)=2.8s     00H(24MHz)=1.4s
				   80H(12MHz)=1.4s     80H(24MHz)=0.7s
* Output         : None
* Return         : None
*******************************************************************************/
void CH549WDTFeed(UINT8 tim)
{
	WDOG_COUNT = tim; // 看门狗计数器赋值
}

UINT8 CH549UART0RcvByte(void)
{
	while(RI == 0)
    {
        ;    //查询接收
    }
    RI = 0;  //清除接收中断
	return SBUF;
}

void CH549UART0SendByte(UINT8 SendDat)
{
	TI = 0;
	SBUF = SendDat;
	while(TI == 0)
    {
        ;    //查询等待发送完成
    }
    TI = 0;  //清除发送完成中断
}

/* 使用中斷回傳由PC發出的訊息 */
#if UART0_INTERRUPT
void UART0Interrupt(void) __interrupt(INT_NO_UART0)
{
	UINT8 dat;
    if(RI == 1)
    {
        RI = 0;  //清除接收完中断
        dat = SBUF;
        CH549UART0SendByte(dat);
    }
}
#endif

/* Keil 使用UART0輸出printf, 不需設定putchar */
/********************************************************************
 * 函 数 名       : putchar
 * 函数功能       : 調試時設定如何顯示輸出
 * 输    入      : 字符串
 * 输    出    	: 字符串
 ********************************************************************/
#if DE_PRINTF == 0
int putchar(int a)
{
	// SBUF = a; //直接寫入UART0寄存器不作檢測
	CH549UART0SendByte(a);
	return (a);
}
#endif

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

/*******************************************************************************
* Function Name  : CH549UART1Init
* Description    : UART1初始化函数,串口默认引脚 P26 P27
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH549UART1Init(void)
{
    UINT8 baud = FREQ_SYS/16/UART1_BAUD;         //增加這轉換使SDCC編譯不報錯
    SCON1 &= ~bU1SM0;                            //选择8位数据通讯
    SCON1 |= bU1SMOD;                            //快速模式
    SCON1 |= bU1REN;                             //使能接收
    SBAUD1 = 256 - baud;                         //波特率配置
    SIF1 = bU1TI;                                //清空发送完成标志
#if UART1_INTERRUPT                           //开启中断使能
    IE_UART1 = 1;
    EA = 1;
#endif
}

/*******************************************************************************
* Function Name  : CH549UART1Alter()
* Description    : CH549串口1引脚映射,串口映射到 P16 P17
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH549UART1Alter(void)
{
    P1_MOD_OC |= (3<<6);                                                   //准双向模式
    P1_DIR_PU |= (3<<6);
    PIN_FUNC |= bUART1_PIN_X;                                              //开启引脚复用功能
}

/*******************************************************************************
* Function Name  : CH549UART1RcvByte()
* Description    : CH549UART1接收一个字节
* Input          : None
* Output         : None
* Return         : SBUF
*******************************************************************************/
UINT8 CH549UART1RcvByte(void)
{
    while((SIF1&bU1RI) == 0)
    {
        ;    //查询接收
    }
    SIF1 = bU1RI;  //清除接收中断
    return SBUF1;
}

/*******************************************************************************
* Function Name  : CH549UART1SendByte(UINT8 SendDat)
* Description    : CH549UART1发送一个字节
* Input          : UINT8 SendDat；要发送的数据
* Output         : None
* Return         : None
*******************************************************************************/
void CH549UART1SendByte(UINT8 SendDat)
{
    SBUF1 = SendDat;
    while((SIF1&bU1TI) == 0)
    {
        ;    //查询等待发送完成
    }
    SIF1 = bU1TI;  //清除发送完成中断
}

#if UART1_INTERRUPT
/*******************************************************************************
* Function Name  : UART1Interrupt(void)
* Description    : UART1 中断服务程序
*******************************************************************************/
void UART1Interrupt(void) __interrupt(INT_NO_UART1) __using(1)  //串口1中断服务程序,使用寄存器组1
{
    UINT8 dat;
    if(SIF1&bU1RI)
    {
        SIF1 = bU1RI;  //清除接收完中断
        dat = SBUF1;
        CH549UART1SendByte(dat);
    }
}
#endif

/*******************************************************************************
* Function Name  : CH549UART2Init
* Description    : UART2初始化函数,串口默认引脚 P04 P05
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH549UART2Init(void)
{
    UINT8 baud = FREQ_SYS/16/UART2_BAUD;         //增加這轉換使SDCC編譯不報錯
    SCON2 &= ~bU2SM0;                            //选择8位数据通讯
    SCON2 |= bU2SMOD;                            //快速模式
    SCON2 |= bU2REN;                             //使能接收
    SBAUD2 = 256 - baud;                         //波特率配置
    SIF2 = bU2TI;                                //清空发送完成标志
#if UART2_INTERRUPT                           //开启中断使能
    SCON2 |= bU2IE;                              //开启UART2中断，关闭ADC中断
    IE_UART2 = 1;
    EA = 1;
#endif
}

/*******************************************************************************
* Function Name  : CH549UART2RcvByte()
* Description    : CH549UART2接收一个字节
* Input          : None
* Output         : None
* Return         : SBUF
*******************************************************************************/
UINT8 CH549UART2RcvByte(void)
{
    while((SIF2&bU2RI) == 0)
    {
        ;    //查询接收
    }
    SIF2 = bU2RI;  //清除接收中断
    return SBUF2;
}

/*******************************************************************************
* Function Name  : CH549UART2SendByte(UINT8 SendDat)
* Description    : CH549UART2发送一个字节
* Input          : UINT8 SendDat；要发送的数据
* Output         : None
* Return         : None
*******************************************************************************/
void CH549UART2SendByte(UINT8 SendDat)
{
    SBUF2 = SendDat;
    while((SIF2&bU2TI) == 0)
    {
        ;    //查询等待发送完成
    }
    SIF2 = bU2TI;  //清除发送完成中断
}

#if UART2_INTERRUPT
/*******************************************************************************
* Function Name  : UART1Interrupt(void)
* Description    : UART1 中断服务程序
*******************************************************************************/
void UART2Interrupt(void) __interrupt(INT_NO_UART2) __using(1)  //串口2中断服务程序,使用寄存器组1
{
    UINT8 dat;
    if(SIF2&bU2RI)
    {
        SIF2 = bU2RI;  //清除接收完中断
        dat = SBUF2;
        CH549UART2SendByte(dat);
    }
}
#endif

/*******************************************************************************
* Function Name  : CH549UART3Init
* Description    : UART3初始化函数,串口默认引脚 P06 P07
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH549UART3Init(void)
{
    UINT8 baud = FREQ_SYS/16/UART3_BAUD;         //增加這轉換使SDCC編譯不報錯
    SCON3 &= ~bU3SM0;                            //选择8位数据通讯
    SCON3 |= bU3SMOD;                            //快速模式
    SCON3 |= bU3REN;                             //使能接收
    SBAUD3 = 256 - baud;                         //波特率配置
    SIF3 = bU3TI;                                //清空发送完成标志
#if UART3_INTERRUPT                           //开启中断使能
    SCON3 |= bU3IE;                              //开启UART3中断，关闭PWM中断
    IE_UART3 = 1;
    EA = 1;
#endif
}

/*******************************************************************************
* Function Name  : CH549UART3RcvByte()
* Description    : CH549UART3接收一个字节
* Input          : None
* Output         : None
* Return         : SBUF
*******************************************************************************/
UINT8 CH549UART3RcvByte(void)
{
    while((SIF3&bU3RI) == 0)
    {
        ;    //查询接收
    }
    SIF3 = bU3RI;  //清除接收中断
    return SBUF3;
}

/*******************************************************************************
* Function Name  : CH549UART3SendByte(UINT8 SendDat)
* Description    : CH549UART3发送一个字节
* Input          : UINT8 SendDat；要发送的数据
* Output         : None
* Return         : None
*******************************************************************************/
void CH549UART3SendByte(UINT8 SendDat)
{
    SBUF3 = SendDat;
    while((SIF3&bU3TI) == 0)
    {
        ;    //查询等待发送完成
    }
    SIF3 = bU3TI;  //清除发送完成中断
}

#if UART3_INTERRUPT
/*******************************************************************************
* Function Name  : UART1Interrupt(void)
* Description    : UART1 中断服务程序
*******************************************************************************/
void UART3Interrupt(void) __interrupt(INT_NO_UART3) __using(1)  //串口3中断服务程序,使用寄存器组1
{
    UINT8 dat;
    if(SIF3&bU3RI)
    {
        SIF3 = bU3RI;  //清除接收完中断
        dat = SBUF3;
        CH549UART3SendByte(dat);
    }
}
#endif

/********************************************************************
* 函 数 名       : putchar
* 函数功能       : 将printf映射到串口123通讯
* 输    入      : 字符串
* 输    出    	: 字符串
********************************************************************/
#if DE_PRINTF != 0  //Use when not in debug mode
int putchar(int a)
{
    #if DE_PRINTF == 1
    CH549UART1SendByte(a);       //printf映射到串口1输出
    #endif
    #if DE_PRINTF == 2
    CH549UART2SendByte(a);       //printf映射到串口2输出
    #endif
    #if DE_PRINTF == 3
    CH549UART3SendByte(a);       //printf映射到串口3输出
    #endif
    return(a);
}
#endif

#if T0_LED_BLINK != 0
void Debug_LED(void)
{
	mTimer0Clk12DivFsys(); //T0定时器时钟设置 FREQ_SYS/12
    mTimer_x_ModInit(0,1); //T0 定时器模式设置 模式1 16位定时器
    mTimer_x_SetData_16bit(0,2000); //T0定时器赋值 24MHZ 1MS中断
    mTimer0RunCTL(1); //T0定时器启动
    ET0 = 1; //T0定时器中断开启
    EA = 1;
}

void Timer0(void) __interrupt(INT_NO_TMR0)
{
    mTimer_x_SetData(0,2000); //T0定时器赋值 24MHZ 1MS中断
	// SCK = !SCK; //大约1ms
	static int i; //设置局部变量 i
	i++;
	if(i==BLINK_SPEED) //每进入一次中断则是1ms, 1000次则是1s.
	{
		i = 0;
		LED_BUILTIN = !LED_BUILTIN;
	}	
}
#endif

/* reverse the bit order of a byte */
UINT8 reverse(UINT8 MSB) {
    UINT8 LSB = 0;
    for (int i = 0; i < 8; i++) {
        LSB = LSB << 1;
        if (MSB & 1) {
            LSB = LSB | 1;
        }
        MSB = MSB >> 1;
    }
    return LSB;
}

/* read a bit from a byte */
int readbit(UINT8 byte, int bit) {
    return (byte >> bit) & 1;
}
