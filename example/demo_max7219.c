/**
 * File Name    : demo_max7219.c
 * Author       : Beny
 * Version      : V0.1
 * Date         : 2024/01/15
 * Description  :
 *  用於 Verimake CH549 開發板, 展示使用 MAX7219 驅動的數碼管或led陣矩.
 */
#include <CH549_ledcontrol.h> //程式庫控制8x8LED陣矩
#include <CH549_matrix.h>

UINT8 i=0, j=0; //初始化變量用於 demo_6 及 demo_7

/* 7-segment display by array pattern, no BCD decode */
void demo_1(void)
{
  setDecode(0,FALSE);
  clearDisplay(0,0);
  setChar(1,7,'d',FALSE);
  setChar(0,6,'p',FALSE);
  setChar(0,4,'A',FALSE);
  setChar(0,1,'-',FALSE);
  _delay_ms(1000);
  clearDisplay(0,0);

  for(UINT8 j=0;j<8;j++)
  {
    setDigit(0,j,j,FALSE);
  }
  _delay_ms(1000);
  clearDisplay(0,0);

  for(UINT8 j=0;j<8;j++)
  {
    setChar(0,j,j+64,FALSE);
  }
  _delay_ms(1000);
  clearDisplay(0,0);
}

/* 7-segment display by BCD decode */
void demo_2(void)
{
  setDecode(0,TRUE);
  clearDisplay(0,1);
  for(UINT8 i=0;i<8;i++)
  {
    printDEC(0,i,i+6);
  }
  _delay_ms(1000);
  clearDisplay(0,1);
}

/* test 7-segment display */
void demo_3(void)
{
  setDecode(0,TRUE);
  clearDisplay(0,1);
  setRow(0, 0, 0);
  setRow(0, 1, 1);
  _delay_ms(1000);
  clearDisplay(0,1);
  _delay_ms(1000);
  testDisplay(0,TRUE);
  _delay_ms(1000);
  setRow(0, 2, 2);
  setRow(0, 3, 3);
  testDisplay(0,FALSE);
  _delay_ms(1000);
  clearDisplay(0,1);
  setRow(0, 4, 4);
  setRow(0, 5, 5);
  _delay_ms(1000);
  shutdown(0,TRUE);
  _delay_ms(1000);
  shutdown(0,FALSE);
  setRow(0, 6, 6);
  setRow(0, 7, 7);
  _delay_ms(1000);
  setDecode(0,FALSE);
  _delay_ms(1000);
  setDecode(0,TRUE);
  _delay_ms(1000);
  clearDisplay(0,1);
  _delay_ms(1000);
}

/* matrix display from array of bytes */
void demo_4(void)
{
  setDecode(0,FALSE);
  setDecode(1,FALSE);
  clearDisplay(0,0);
  clearDisplay(1,0);

  for(UINT8 k = 0; k < DEC_8_LEN; k++)
  {
    for(UINT8 i = 0; i < 8; i++)
    {
      setRow(0, i, DEC_8[k][i]);
    }
    for(UINT8 i = 0; i < 8; i++)
    {
      setColumn(1, i, reverse(DEC_8[k][i]));
    }
    _delay_ms(500);
  }
  clearDisplay(0,0);
  clearDisplay(1,0);

  for(UINT8 k = 0; k < chinese_LEN; k++)
  {
    for(UINT8 i = 0; i < 8; i++)
    {
      setRow(0, i, chinese[k][i]);
    }
    for(UINT8 i = 0; i < 8; i++)
    {
      setColumn(1, i, reverse(chinese[k][i]));
    }
    _delay_ms(500);
  }
  clearDisplay(0,0);
  clearDisplay(1,0);
}

/* matrix display from array of 64-bit long integer */
void demo_5(void)
{
  setDecode(0,FALSE);
  clearDisplay(0,0);

  for (int k = 0; k < DEC_64_LEN; k++) {
    unsigned long long image = DEC_64[k];
    for (int j = 0; j < 8; j++) {
      UINT8 character = (image >> (8 * j)) & 0xFF;
      // 陣矩字元順序不同, 使用這子程序時, 要將字完倒返 
      setRow(0,j,reverse(character));
    }
    _delay_ms(500);
  }
  clearDisplay(0,0);
}

/* other method for matrix display from array of bytes */
void demo_6(void)
{
  setDecode(1,FALSE);
  clearDisplay(1,0);

  for (int k = 0; k < DEC_8_LEN; k++) {
    displayImage8(1, DEC_8[DEC_8_LEN-1-i], 0);
    if (++i >= DEC_8_LEN) {
      i = 0;
    }
    _delay_ms(500);
  }
  clearDisplay(1,0);
}

/* other method for matrix display from array of 64-bit long integer */
void demo_7(void)
{
  setDecode(0,FALSE);
  clearDisplay(0,0);
  setDecode(1,FALSE);
  clearDisplay(1,0);

  for (int k = 0; k < ARROW_LEFT_LEN; k++) {
    displayImage64(0, ARROW_RIGHT[i], 0);
    if (++i >= ARROW_RIGHT_LEN) {
      i = 0;
    }
    displayImage64(1, ARROW_LEFT[j], 0);
    if (++j >= ARROW_LEFT_LEN) {
      j = 0;
    }
    _delay_ms(100);
  }
  for (int k = 0; k < ARROW_LEFT_LEN; k++) {
    displayImage64(0, ARROW_RIGHT[i], 1);
    if (++i >= ARROW_RIGHT_LEN) {
      i = 0;
    }
    displayImage64(1, ARROW_LEFT[j], 1);
    if (++j >= ARROW_LEFT_LEN) {
      j = 0;
    }
    _delay_ms(100);
  }
  clearDisplay(0,0);
  clearDisplay(1,0);
}

/* Letter display */
void demo_8(void)
{
  setDecode(0,FALSE);
  clearDisplay(0,0);

  for (int k = 0; k < LETTER_LEN; k++) {
    displayImage64(0, LETTER[i], 0);
    if (++i >= LETTER_LEN) {
      i = 0;
    }
    _delay_ms(200);
  }
  clearDisplay(0,0);
}

void main(void)
{
  CfgFsys();
  _delay_ms(20);
  mInitSTDIO();
  Init_MAX7219();
  printf("MAX7219 matrix Demo start ...\n");
  Debug_LED();


  shutdown(0,FALSE);
  shutdown(1,FALSE);

  while(1)
  {
    // demo_1();
    // demo_2();
    // demo_3();
    demo_4();
    demo_5();
    demo_6();
    for(UINT8 k=0; k<5; k++) demo_7();
    demo_8();
  } 
}





