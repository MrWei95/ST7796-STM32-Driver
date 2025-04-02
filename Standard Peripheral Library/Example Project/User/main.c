/**** 
	* 本程序由MrWei95开源共享
	*/

#include "main.h"
#include "ST7796.h"
#include "lcdPicture.h"

int main(void)
{
	ST7796_Init();
	ST7796_Fill(0, 0, LCD_W, LCD_H, BLACK);
	ST7796_ShowChinese(0, 0, "屏幕中文测试", GREEN, BLACK, 12, 0);
	ST7796_ShowChinese(0, 16, "屏幕中文测试", GREEN, BLACK, 16, 0);
	ST7796_ShowChinese(0, 34, "屏幕中文测试", GREEN, BLACK, 24, 0);
	ST7796_ShowChinese(0, 60, "屏幕中文测试", GREEN, BLACK, 32, 0);
	ST7796_ShowString(0, 100, "Smart enough to know,", GREEN, BLACK, 24, 0);
	ST7796_ShowString(0, 126, "Stupid enough to do.", GREEN, BLACK, 24, 0);
	ST7796_ShowPicture(300, 50, 72, 72, Image_Stewie);
	Delay_s(5);
	while(1)
	{
		ST7796_Fill(0,0,LCD_W,LCD_H,GREEN);
		ST7796_Fill(0,0,LCD_W,LCD_H,BLUE);
		ST7796_Fill(0,0,LCD_W,LCD_H,YELLOW);
		ST7796_Fill(0,0,LCD_W,LCD_H,RED);
	}
}
