/**
 * File Name    : CH549_ledcontrol.h
 * Author       : Beny
 * Version      : V0.1
 * Date         : 2024/01/15
 * Description  :
 *  用於 Verimake CH549 開發板
 *  參照以下的Arduino庫, 移植至C51環境, 採用 WCH CH549. 
 *  LedControl.h - A library for controling Leds with a MAX7219/MAX7221
 *  Copyright (c) 2007 Eberhard Fahle
 *  https://github.com/wayoda/LedControl
 */

#ifndef __CH549_LEDCONTROL_H__
#define __CH549_LEDCONTROL_H__

#include <CH549_sdcc.h> //CH549的头文件，其中定义了单片机的一些特殊功能寄存器
#include <CH549_debug.h> //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数
#include <CH549_gpio.h> //GPIO设置函数定义

#pragma less_pedantic

//the opcodes for the MAX7221 and MAX7219
#define OP_NOOP   0x00
#define OP_DIGIT0 0x01
#define OP_DIGIT1 0x02
#define OP_DIGIT2 0x03
#define OP_DIGIT3 0x04
#define OP_DIGIT4 0x05
#define OP_DIGIT5 0x06
#define OP_DIGIT6 0x07
#define OP_DIGIT7 0x08
#define OP_DECODEMODE  0x09
#define OP_INTENSITY   0x0A
#define OP_SCANLIMIT   0x0B
#define OP_SHUTDOWN    0x0C
#define OP_DISPLAYTEST 0x0F

//定义Max7219端口
#define Max7219_pinCLK P2_4
#define Max7219_pinCS  P2_6
#define Max7219_pinDIN P2_7

#define maxDevices 2 //設定連接裝置數量

static UINT8 __xdata spidata[16]={0x00};
static UINT8 __xdata status[64]={0x00};

/*
 * Segments to be switched on for characters and digits on
 * 7-Segment Displays
 */
static const UINT8 __xdata charTable[] = {
    0b01111110,0b00110000,0b01101101,0b01111001,0b00110011,0b01011011,0b01011111,0b01110000,
    0b01111111,0b01111011,0b01110111,0b00011111,0b00001101,0b00111101,0b01001111,0b01000111,
    0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
    0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
    0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
    0b00000000,0b00000000,0b00000000,0b00000000,0b10000000,0b00000001,0b10000000,0b00000000,
    0b01111110,0b00110000,0b01101101,0b01111001,0b00110011,0b01011011,0b01011111,0b01110000,
    0b01111111,0b01111011,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
    0b00000000,0b01110111,0b00011111,0b00001101,0b00111101,0b01001111,0b01000111,0b00000000,
    0b00110111,0b00000000,0b00000000,0b00000000,0b00001110,0b00000000,0b00000000,0b00000000,
    0b01100111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
    0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001000,
    0b00000000,0b01110111,0b00011111,0b00001101,0b00111101,0b01001111,0b01000111,0b00000000,
    0b00110111,0b00000000,0b00000000,0b00000000,0b00001110,0b00000000,0b00010101,0b00011101,
    0b01100111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
    0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000
};

/* Initial the device */
void Init_MAX7219(void);

/**
 * Functions to operate MAX7219 devices
 * Params :
 *  addr = The address of the display to control
 *  limit = number of digits to be displayed (1..8)
 *  status = If true the device goes into power-down mode. Set to false for normal operation.
 * 	intensity = the brightness of the display. (0..15)
 *  decode = set true for normal matrix and false for BCD code B mode
 * 	test = set true for display testing and false for nomal display
 * 	BCD = set true for clearDisplay when in BCD code B mode
 *  ---- led matrix operation ----
 *		row = the row of the Led (0..7)
 *		col = the column of the Led (0..7)
 *  	value = each bit set to 1 will light up the corresponding Led
 * 		state = If true the led is switched on, if false it is switched off.
 *	---- 7-Segment led operation ----
 *		digit = the position of the digit on the display (0..7)
 *		value = the value to be displayed. (0x00..0x0F)
 *		value_dec = decimal value to be displayed
 *		dp = sets the decimal point.
*/
void shutdown(UINT8 addr, UINT8 status);
void setScanLimit(UINT8 addr, UINT8 limit);
void setIntensity(UINT8 addr, UINT8 intensity);
void setDecode(UINT8 addr, UINT8 decode);
void testDisplay(UINT8 addr, UINT8 test);
void clearDisplay(UINT8 addr, UINT8 BCD); //all 8x8 SRAM is clear to zero
void setLed(UINT8 addr, UINT8 row, UINT8 col, UINT8 state); //set each led matrix on/off
void setRow(UINT8 addr, UINT8 row, UINT8 value); //set row value of led matrix
void setColumn(UINT8 addr, UINT8 col, UINT8 value); //set coloumn value of led matrix
void printDEC(UINT8 addr, UINT8 digit, UINT8 value_dec); //use BCD code B mode to print 7-Segment
void setDigit(UINT8 addr, UINT8 digit, UINT8 value, UINT8 dp); //set 7-Segment displays
void setChar(UINT8 addr, UINT8 digit, UINT8 value, UINT8 dp); //set 7-Segment displays

/**
 * function to manage images from array of bytes or 64-bit long integer
 * 參照 LED Matrix Editor 的例程 https://xantorohara.github.io/led-matrix-editor/#
*/
void displayImage8(UINT8 addr, const PUINT8 image, UINT8 direction);
void displayImage64(UINT8 addr, unsigned long long image, UINT8 direction);

#endif
