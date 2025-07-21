/**
 * library provided by Verimake modified by Beny on 2023-12-08.
*/

#ifndef __CH549_OLED_H
#define __CH549_OLED_H

#include <CH549_spi.h> //CH549官方提供库的头文件，定义了一些关于SPI初始化，传输数据等函数
#include <CH549_debug.h> //CH549官方提供库的头文件，定义了一些关于主频，延时，串口设置，看门口，赋值设置等基础函数

//定义屏幕尺寸
#define SSD1306_LCDWIDTH   128
#define SSD1306_LCDHEIGHT  64

//For Page addressing mode
#define Max_Column	128
#define Max_Row		8

//定义SSD1306指令
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//SSD1306指令集
//referenced from Adafruit SSD1306 lib
#define SSD1306_SETLOWCOLUMN 0x00        //--set low column start address for Page addressing mode
#define SSD1306_SETHIGHCOLUMN 0x00       //--set high column start address for Page addressing mode
#define SSD1306_SETSTARTLINE 0x40        //--set Mapping RAM Display Start Line (0x00~0x3F)
#define SSD1306_MEMORYMODE 0x20          //--Set Memory Addressing Mode (0x00/0x01/0x02)
#define SSD1306_COLUMNADDR 0x21          //--Set Column Address for Horizontal/Vertical Addressing mode
#define SSD1306_PAGEADDR 0x22            //--Set Page Address for Horizontal/Vertical Addressing mode
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init right scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range

#define SSD1306_SETCONTRAST 0x81         //--set contrast control register
#define SSD1306_CHARGEPUMP 0x8D          //--set Charge Pump enable/disable
#define SSD1306_SEGREMAP 0xA1            //--Set SEG/Column Mapping  0xA0左右反置 0xA1正常
#define SSD1306_DISPLAYALLON_RESUME 0xA4 //--Entire Display On with RAM content
#define SSD1306_DISPLAYALLON 0xA5        //--Entire Display on without RAM content
#define SSD1306_NORMALDISPLAY 0xA6       //--Normal display
#define SSD1306_INVERTDISPLAY 0xA7       //--Inverse display
#define SSD1306_SETMULTIPLEX 0xA8        //--set multiplex ratio(1 to 64)
#define SSD1306_DISPLAYOFF 0xAE          //--turn off oled panel
#define SSD1306_DISPLAYON 0xAF           //--turn on oled panel
// #define SSD1306_COMSCANINC 0xC0          //--Set COM/Row Scan Direction
// #define SSD1306_COMSCANDEC 0xC8          //--Set COM/Row Scan Direction
#define SSD1306_SET_COMSCAN_DIR 0xC8     //--Set COM/Row Scan Direction   0xC0上下反置 0xC8正常
#define SSD1306_SETDISPLAYOFFSET 0xD3    //--set display offset	Shift Mapping RAM Counter (0x00~0x3F)
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  //--set display clock divide ratio/oscillator frequency
#define SSD1306_SETPRECHARGE 0xD9        //--set pre-charge period
#define SSD1306_SETCOMPINS 0xDA          //--set com pins hardware configuration
#define SSD1306_SETVCOMDETECT 0xDB       //--set vcomh

// #define SSD1306_EXTERNALVCC 0x01                          ///< External display voltage source
// #define SSD1306_SWITCHCAPVCC 0x02                         ///< Gen. display voltage from 3.3V

//SSD1306的SPI接口定义
#define OLED_CS     P1_4 //片选
#define OLED_RST    P3_5 //复位
#define OLED_DC     P2_7 //数据/命令控制
#define OLED_SCL    P1_7 //D0（SCLK) 时钟 
#define OLED_SDIN   P1_5 //D1（MOSI) 数据

#define OLED_SELECT()        OLED_CS=0   //设备选择
#define OLED_DESELECT()      OLED_CS=1   //取消设备选择
#define OLED_RST_Clr()       OLED_RST=0  //清除复位
#define OLED_RST_Set()       OLED_RST=1  //激活复位
#define OLED_MODE_COMMAND()  OLED_DC=0   //SSD1306设置为命令模式
#define OLED_MODE_DATA()     OLED_DC=1   //SSD1306设置为数据模式
#define OLED_SCLK_Clr()      OLED_SCL=0 
#define OLED_SCLK_Set()      OLED_SCL=1
// #define OLED_SDIN_Clr()      OLED_SDIN=0
// #define OLED_SDIN_Set()      OLED_SDIN=1

void OLED_Init(void);                    //OLED初始化
void OLED_Display_On(void);              //显示
void OLED_Display_Off(void);             //关闭显示
void OLED_Clear(void);                   //清屏函数

void OLED_ShowChar(UINT8 fontSize, UINT8 x, UINT8 y, UINT8 chr);
void OLED_ShowString(UINT8 fontSize, UINT8 x, UINT8 y, UINT8 *ptr);  //显示字符串
void OLED_ShowCHinese(UINT8 x, UINT8 y, UINT8 no);                   //显示汉字
void OLED_DrawBMP(UINT8 x0, UINT8 y0, UINT8 x1, UINT8 y1, UINT8 BMP[]);

#endif
