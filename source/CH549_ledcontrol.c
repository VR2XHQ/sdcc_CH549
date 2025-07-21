/**
 * File Name    : CH549_ledcontrol.c
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
#include <CH549_ledcontrol.h> //程式庫控制8x8LED陣矩

void Write_Max7219_byte(UINT8 DATA);
void spiTransfer(UINT8 addr, UINT8 opcode, UINT8 data);

void Write_Max7219_byte(UINT8 DATA)         
{
  for(UINT8 i=8;i>=1;i--)
  {		  
    Max7219_pinCLK=0;
    Max7219_pinDIN=DATA&0x80; //MSBfirst
    DATA=DATA<<1; //Left shift a bit
    Max7219_pinCLK=1;
  }
}

void Init_MAX7219(void)
{
  /* set PIN mode */
  GPIO_Init(PORT2,PIN4,MODE1);
  GPIO_Init(PORT2,PIN6,MODE1);
  GPIO_Init(PORT2,PIN7,MODE1);

  for(UINT8 i=0;i<64;i++)
    status[i]=0x00;

  for(UINT8 i=0;i<=maxDevices;i++)
  {
    testDisplay(i,0);
    setScanLimit(i,7);
    setDecode(i,0);
    clearDisplay(i,0);
    setIntensity(i,2);
    shutdown(i,1);
  }
}

void spiTransfer(UINT8 addr, UINT8 opcode, UINT8 data) {
  //Create an array with the data to shift out
  UINT8 offset=addr*2;
  UINT8 maxbytes=maxDevices*2;
  UINT8 i;
  for(i=0;i<maxbytes;i++) spidata[i]=OP_NOOP;
  //put our device data into the array
  spidata[offset+1]=opcode;
  spidata[offset]=data;
  //enable the line 
  Max7219_pinCS=0;
  //Now shift out the data 
  for(i=maxbytes;i>0;i--)
    Write_Max7219_byte(spidata[i-1]);
  //latch the data onto the display
  Max7219_pinCS=1;
}

void setLed(UINT8 addr, UINT8 row, UINT8 column, UINT8 state) {
  UINT8 offset;
  UINT8 val=0x00;
  if(addr<0 || addr>=maxDevices)
    return;
  if(row<0 || row>7 || column<0 || column>7)
    return;
  offset=addr*8;
  val=0b10000000 >> column;
  if(state==1)
    status[offset+row]=status[offset+row]|val;
  else {
    val=~val;
    status[offset+row]=status[offset+row]&val;
  }
  spiTransfer(addr, row+1, status[offset+row]);
}

void setRow(UINT8 addr, UINT8 row, UINT8 value) {
  UINT8 offset;
  if(addr<0 || addr>=maxDevices)
    return;
  if(row<0 || row>7)
    return;
  offset=addr*8;
  status[offset+row]=value;
  spiTransfer(addr, row+1, status[offset+row]);
}

void setColumn(UINT8 addr, UINT8 col, UINT8 value) {
  UINT8 val=0x00;
  if(addr<0 || addr>=maxDevices)
    return;
  if(col<0 || col>7) 
    return;
  for(UINT8 row=0;row<8;row++) {
    val=value >> (7-row);
    val=val & 0x01;
    setLed(addr, row, col, val);
  }
}

void printDEC(UINT8 addr, UINT8 digit, UINT8 value_dec) {
  if(addr<0 || addr>=maxDevices)
    return;
  if(digit<0 || digit>9 || value_dec>9)
    return;
  spiTransfer(addr, digit+1, value_dec);
}

void setDigit(UINT8 addr, UINT8 digit, UINT8 value, UINT8 dp) {
  UINT8 offset,v;
  if(addr<0 || addr>=maxDevices)
      return;
  if(digit<0 || digit>7 || value>15)
      return;
  offset=addr*8;
  v=charTable[value]; 
  if(dp==TRUE)
      v|=0b10000000;
  status[offset+digit]=v;
  spiTransfer(addr, digit+1, v);
}

void setChar(UINT8 addr, UINT8 digit, UINT8 value, UINT8 dp) {
    UINT8 offset,index,v;
    if(addr<0 || addr>=maxDevices)
        return;
    if(digit<0 || digit>7)
        return;
    offset=addr*8;
    index=value;
    if(index >127) {
      index=32; //no defined beyond index 127, so we use the space char
    }
    v=charTable[index];
    if(dp==TRUE)
      v|=0b10000000;
    status[offset+digit]=v;
    spiTransfer(addr, digit+1, v);
}

void shutdown(UINT8 addr, UINT8 status) {
  if(addr<0 || addr>=maxDevices)
      return;
  (status==1) ? (status=0x00) : (status=0x01);
      spiTransfer(addr, OP_SHUTDOWN, status);
}

void setScanLimit(UINT8 addr, UINT8 limit) {
  if(addr<0 || addr>=maxDevices)
    return;
  if(limit>=0 && limit<8)
    spiTransfer(addr, OP_SCANLIMIT, limit);
}

void setIntensity(UINT8 addr, UINT8 intensity) {
  if(addr<0 || addr>=maxDevices)
    return;
  if(intensity>=0 && intensity<16)	
    spiTransfer(addr, OP_INTENSITY, intensity);
}

void setDecode(UINT8 addr, UINT8 decode) {
  if(addr<0 || addr>=maxDevices)
    return;
  (decode==1) ? (decode=0xFF) : (decode=0x00);
    spiTransfer(addr, OP_DECODEMODE, decode);
}

void testDisplay(UINT8 addr, UINT8 test) {
  if(addr<0 || addr>=maxDevices)
    return;
  (test==1) ? (test=0x01) : (test=0x00);
    spiTransfer(addr, OP_DISPLAYTEST, test);
}

void clearDisplay(UINT8 addr, UINT8 BCD) {
  UINT8 offset;
  (BCD==1) ? (BCD=0x0F) : (BCD=0x00);
  if(addr<0 || addr>=maxDevices)
    return;
  offset=addr*8;
  for(UINT8 i=0;i<8;i++) {
    status[offset+i]=BCD;
    spiTransfer(addr, i+1, status[offset+i]);
  }
}

/* read bit from array of bytes and set relevant led */
void displayImage8(UINT8 addr, const PUINT8 image, UINT8 direction) {
  UINT8 d;
  for (int i = 0; i < 8; i++) { //set row
    for (int j = 0; j < 8; j++) { //set column
      (direction==1) ? (d=j) : (d=7-j); // direction: 0=normal, 1=reverse
      setLed(addr, i, j, readbit(image[i], d));
    }
  }
}

/* read bit from array of 64-bit long integer and set relevant led */
void displayImage64(UINT8 addr, unsigned long long image, UINT8 direction) {
  UINT8 d;
  for (int i = 0; i < 8; i++) { //set row
    UINT8 row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) { //set column
      (direction==1) ? (d=7-j) : (d=j); // direction: 0=normal, 1=reverse
      setLed(addr, i, j, readbit(row, d));
    }
  }
}

