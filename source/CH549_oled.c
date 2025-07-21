/**
 * library provided by Verimake and modified by Beny on 2023-12-16.
*/

/********************************** (C) COPYRIGHT *******************************
* File Name          : oled库
* Author             : 未知
* Version            : 未知
* Date               : 未知
* Description        : 驱动屏幕使用的函数
*******************************************************************************/
#include <CH549_oled.h> //程式庫驅動 128x64 oled示屏
#include <CH549_oledfont.h>

/* PROGMEM workaround for non-AVR */
#define pgm_read_byte(addr) (*(const UINT8 *)(addr))

/* OLED模組內的子程式宣告 */
void WR_Byte(UINT8 data, UINT8 mode); //写入1byte数据
void send_one_command(UINT8 cmd);
void send_commandList(const UINT8 *cmd, UINT8 n);
void Set_Pos(UINT8 x, UINT8 y);     //设置显示位置


void WR_Byte(UINT8 data, UINT8 mode)
{			
	if(mode) OLED_MODE_DATA(); 	//命令模式
	else OLED_MODE_COMMAND(); 	//数据模式
	OLED_SELECT();			    //片选设置为0,设备选择
	CH549SPIMasterWrite(data);  //使用CH549的官方函数写入8位数据
	OLED_DESELECT();			//片选设置为1,取消设备选择
	OLED_MODE_DATA();   	  	//转为数据模式
} 

void send_one_command(UINT8 cmd)
{
	WR_Byte(cmd, OLED_CMD);
}

void send_commandList(const UINT8 *cmd, UINT8 n)
{
    while (n--) 
		WR_Byte(pgm_read_byte(cmd++), OLED_CMD);
}

void Set_Pos(UINT8 col_index, UINT8 row_index) 
{
	send_one_command(0xb0+row_index);
	send_one_command(0x10|((col_index&0xf0)>>4));
	send_one_command(col_index&0x0f);
}
	  
void OLED_ShowChar(UINT8 fontSize, UINT8 col_index, UINT8 row_index, UINT8 chr)
{      	
	UINT8 char_index = 0, i = 0;	
	char_index = chr - 0x20;  //字元表由空格(0x20)起始, 設為char_index[0].
	
	if (col_index > Max_Column - 1)  //判別換行
	{
		col_index = 0;
		if (fontSize == 2) row_index = row_index + 2;
		else if (fontSize == 1) row_index = row_index + 1;
	}

	if (fontSize == 2) {
		Set_Pos(col_index, row_index);  //字符上8位顯示
		for (i = 0; i < 8; i++)
			WR_Byte(fontMatrix_8x16[char_index * 16 + i], OLED_DATA);
		Set_Pos(col_index, row_index + 1);  //字符下8位顯示
		for (i = 0; i < 8; i++)
			WR_Byte(fontMatrix_8x16[char_index * 16 + i + 8], OLED_DATA);
	}
	else if (fontSize == 1) {
		Set_Pos(col_index, row_index);
		for (i = 0; i < 6; i++)
			WR_Byte(fontMatrix_6x8[char_index][i], OLED_DATA);	
	}
}

//开启OLED显示  这里的代码待修改，完全移植的中景园
void OLED_Display_On(void)
{
	send_one_command(SSD1306_CHARGEPUMP); //SET DCDC命令
	send_one_command(0x14);	//DCDC ON
	send_one_command(SSD1306_DISPLAYON); //DISPLAY ON
}

void OLED_Display_Off(void)
{
	send_one_command(SSD1306_CHARGEPUMP); //SET DCDC命令
	send_one_command(0x10);	//DCDC OFF
	send_one_command(SSD1306_DISPLAYOFF); //DISPLAY OFF
}

void OLED_Clear(void)
{
	/* 設定 SEG0 為起始列 */
	send_one_command(0x00); //设置显示位置—列低地址
	send_one_command(0x10); //设置显示位置—列高地址
	
	UINT8 i,n;
	for(i=0;i<8;i++)  
	{  
		send_one_command(0xb0+i); //设置页地址（0~7）
		for(n=0;n<Max_Column;n++) WR_Byte(0, OLED_DATA); //每列全清,由列0~127d
	}
}

void OLED_Init(void)
{
    OLED_RST_Set();
	_delay_ms(10);
	OLED_RST_Clr();
	_delay_ms(10);  //Use delay function in ch549_debug.c
	OLED_RST_Set();
	_delay_ms(10);
    //OLED的初始化指令，这里可以通过command数组让它变得更简洁，指令有參數直接用數組.
	//順序按晶片手冊例子, 0x8D指令沒說明.
  	static const UINT8 __code init_commandList[] = {	
		SSD1306_DISPLAYOFF,
		SSD1306_SETDISPLAYCLOCKDIV, 0x80, //--set divide ratio, Set Clock as 100 Frames/Sec
		SSD1306_SETMULTIPLEX, 0x3F, //--set 1/64 duty	
		SSD1306_SETDISPLAYOFFSET, 0x00, //--default, set vertical shift by COM from 0d-63d
		SSD1306_SETSTARTLINE,
		SSD1306_CHARGEPUMP, 0x14, //--set(0x10) disable?? datasheet without info 
		SSD1306_SEGREMAP,
		SSD1306_SET_COMSCAN_DIR,
		SSD1306_SETCOMPINS, 0x12, //--value for COM Pins Hardware
		SSD1306_SETCONTRAST, 0xCF, //--value for SEG Output Current Brightness (datahseet 0x66)	
		SSD1306_SETPRECHARGE, 0xF1, //--value for Pre-Charge as 15 Clocks & Discharge as 1 Clock (datasheet 0x22)	
		SSD1306_SETVCOMDETECT, 0x30, //--value for VCOM Deselect Level
		SSD1306_MEMORYMODE, 0x02, //--default is Page Addressing Mode 
		SSD1306_DISPLAYALLON_RESUME,
		// SSD1306_DISPLAYALLON,
		SSD1306_NORMALDISPLAY,
		// SSD1306_INVERTDISPLAY,
		SSD1306_DISPLAYON
		};

  	send_commandList(init_commandList, sizeof(init_commandList));
	_delay_ms(100);
	OLED_Clear();
	Set_Pos(0,0);
}

/* 字串列印 6*8 細字体, 到尾換行. */
/* 字串列印 8*16 大字体, 到尾換行. */
void OLED_ShowString(UINT8 fontSize, UINT8 col_index, UINT8 row_index, UINT8 *ptr)
{
	UINT8 j = 0;
	UINT8 col_index_INIT = (SSD1306_SETHIGHCOLUMN<<4|SSD1306_SETLOWCOLUMN);
	col_index = col_index + col_index_INIT;
	while (ptr[j]!='\0')
	{	OLED_ShowChar(fontSize, col_index, row_index, ptr[j]);
		col_index+=8;
		if (col_index>120 && fontSize==1) {col_index=col_index_INIT; row_index+=1;}  //字完8位高, 換行加1.
		if (col_index>120 && fontSize==2) {col_index=col_index_INIT; row_index+=2;}  //字元16位高, 換行加2.
		j++;
	}
}

//显示汉字字符 目前用处比较小，因为没有中文库，每一个想要显示的中文都需要它在屏幕上匹配的坐标才能显示
void OLED_ShowCHinese(UINT8 col_index, UINT8 row_index, UINT8 no)
{      			    
	UINT8 t,adder=0;
	Set_Pos(col_index,row_index);	
    for(t=0;t<16;t++)
		{
				WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		Set_Pos(col_index,row_index+1);	
    for(t=0;t<16;t++)
			{	
				WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}

/***********功能描述：显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(UINT8 x0, UINT8 y0, UINT8 x1, UINT8 y1, UINT8 BMP[])
{ 	
	//x0和y0是显示图片的初始坐标，一般为(0,0)
	//x1 = 128; y1 = 8
	UINT16 j = 0;
	UINT8 x,y;
  
	for(y = y0; y < y1; y++) //这里的y和x是对二维数组行和列的两次for循环，遍历整个二维数组，并把每一位数据传给OLED
	
	{
		Set_Pos(x0,y);
    	for(x = x0; x < x1; x++)
	    {      
	    	WR_Byte(BMP[j++],OLED_DATA); //向OLED输入BMP中 的一位数据，并逐次递增
	    }
	}
}

/* 中景园C51例程初始化順序
OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
OLED_WR_Byte(0x00,OLED_CMD);//-not offset
OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
OLED_WR_Byte(0x12,OLED_CMD);
OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
OLED_WR_Byte(0x02,OLED_CMD);//
OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
*/
