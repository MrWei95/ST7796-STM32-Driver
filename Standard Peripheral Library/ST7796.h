/**** 
	* 本程序由MrWei95开源共享
	*/

#ifndef __LCD_H
#define __LCD_H

#include "main.h"                  // Device header

// SPI配置
#define		ST7796_GPIO_PORT	GPIOA
#define		ST7796_RCC			RCC_APB2Periph_GPIOA
#define		ST7796_SPI_RCC		RCC_APB2Periph_SPI1
#define		ST7796_SPI_PORT		SPI1
#define		ST7796_SCL_PIN		GPIO_Pin_5
#define		ST7796_SDA_PIN		GPIO_Pin_7

// 屏幕相关其他GPIO配置
#define		ST7796_OTC_PORT		GPIOA
#define		ST7796_OTC_RCC		RCC_APB2Periph_GPIOA
#define		ST7796_RES_PIN		GPIO_Pin_1
#define		ST7796_DC_PIN		GPIO_Pin_2
#define		ST7796_CS_PIN		GPIO_Pin_3
#define		ST7796_BLK_PIN		GPIO_Pin_4

// 屏幕分辨率，根据实际情况修改
#define		Pixel_W				480
#define		Pixel_H				320

//设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏
#define		ST7796_SCREEN_POSITION		2
#if ST7796_SCREEN_POSITION == 0 || ST7796_SCREEN_POSITION == 1
#define		LCD_W				Pixel_H
#define		LCD_H				Pixel_W
#else
#define		LCD_W				Pixel_W
#define		LCD_H				Pixel_H
#endif


// 预设画笔颜色
#define		WHITE			0xFFFF
#define		BLACK			0x0000	  
#define		BLUE			0x001F  
#define		BRED			0XF81F
#define		GRED			0XFFE0
#define		GBLUE			0X07FF
#define		RED				0xF800
#define		MAGENTA			0xF81F
#define		GREEN			0x07E0
#define		CYAN			0x7FFF
#define		YELLOW			0xFFE0
#define		BROWN			0XBC40		// 棕色
#define		BRRED			0XFC07		// 棕红色
#define		GRAY			0X8430		// 灰色
#define		DARKBLUE		0X01CF		// 深蓝色
#define		LIGHTBLUE		0X7D7C		// 浅蓝色  
#define		GRAYBLUE		0X5458		// 灰蓝色
#define		LIGHTGREEN		0X841F		// 浅绿色
#define		LGRAY			0XC618		// 浅灰色(窗体背景色)
#define		LGRAYBLUE		0XA651		// 浅灰蓝色(中间层颜色)
#define		LBBLUE			0X2B12		// 浅棕蓝色(选择条目的反色)


void ST7796_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);		// 指定区域填充颜色
void ST7796_DrawPoint(u16 x,u16 y,u16 color);							// 在指定位置画一个点
void ST7796_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);			// 在指定位置画一条线
void ST7796_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);	// 在指定位置画一个矩形
void ST7796_Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);						// 在指定位置画一个圆

void ST7796_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);			// 显示汉字串
void ST7796_ShowChinese12x12(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);		// 显示单个12x12汉字
void ST7796_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);		// 显示单个16x16汉字
void ST7796_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);		// 显示单个24x24汉字
void ST7796_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);		// 显示单个32x32汉字

void ST7796_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode);			// 显示一个字符
void ST7796_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode);		// 显示字符串

u32 mypow(u8 m,u8 n);																//求幂

void ST7796_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey);			// 显示整数变量
void ST7796_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey);		// 显示两位小数变量

void ST7796_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]);			// 显示图片

void ST7796_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);								// 设置坐标函数
void ST7796_Init(void);																// 屏幕初始化


#define		ST7796_RES_Clr()	GPIO_ResetBits(ST7796_OTC_PORT, ST7796_RES_PIN)
#define		ST7796_RES_Set()	GPIO_SetBits(ST7796_OTC_PORT, ST7796_RES_PIN)

#define		ST7796_DC_Clr()		GPIO_ResetBits(ST7796_OTC_PORT, ST7796_DC_PIN)
#define		ST7796_DC_Set()		GPIO_SetBits(ST7796_OTC_PORT, ST7796_DC_PIN)

#define		ST7796_CS_Clr()		GPIO_ResetBits(ST7796_OTC_PORT, ST7796_CS_PIN)
#define		ST7796_CS_Set()		GPIO_SetBits(ST7796_OTC_PORT, LCD_CS_PIN)

#define		ST7796_BLK_Clr()	GPIO_ResetBits(ST7796_OTC_PORT, ST7796_BLK_PIN)
#define		ST7796_BLK_Set()	GPIO_SetBits(ST7796_OTC_PORT, ST7796_BLK_PIN)


void ST7796_Writ_Bus(u8 dat);	// SPI时序
void ST7796_WR_DATA8(u8 dat);	// 写入一个字节
void ST7796_WR_DATA(u16 dat);	// 写入两个字节
void ST7796_WR_REG(u8 dat);		// 写入一个指令
void ST7796_GPIO_Init(void);	// 初始化GPIO

#endif
