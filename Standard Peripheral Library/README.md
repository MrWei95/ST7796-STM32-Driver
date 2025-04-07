# 标准库
## 例程说明
* 例程使用STM32F103ZET6 + 3.5寸LCD屏幕(480x320)验证可行性。
* 为了方便在更加常用的C8T6上使用，已将引脚全部配置到PA口。

## 驱动说明
绝大部分自定义设置我都放在了ST7796.h中。

### 引脚相关配置
```C
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
```
### 屏幕相关设置
我使用的屏幕分辨率是480x320，如果需要修改，注意：
```C
// 屏幕分辨率，根据实际情况修改
#define		Pixel_W				480
#define		Pixel_H				320
```
```Pixel_W```是屏幕长边，```Pixel_H```是屏幕短边，屏幕方向由宏定义```ST7796_SCREEN_POSITION```控制，实现代码：
```C
#if ST7796_SCREEN_POSITION == 0 || ST7796_SCREEN_POSITION == 1
#define		LCD_W				Pixel_H
#define		LCD_H				Pixel_W
#else
#define		LCD_W				Pixel_W
#define		LCD_H				Pixel_H
#endif


/**** 
	* 函    数：LCD初始化
	* 参    数：无
	* 返 回 值：无
	* 说    明：宏定义ST7796_SCREEN_POSITION控制屏幕方向
	*/
void ST7796_Init(void)
{
	...
	
	// 设置屏幕方向
	if (ST7796_SCREEN_POSITION == 0) ST7796_WR_DATA8(0x48);
	else if (ST7796_SCREEN_POSITION == 1) ST7796_WR_DATA8(0x88);
	else if (ST7796_SCREEN_POSITION == 2) ST7796_WR_DATA8(0x28);
	else ST7796_WR_DATA8(0xE8);
	
	...
```

### 通讯相关
引脚的拉低拉高函数直接在.h中声明了。
```C
#define		ST7796_RES_Clr()	GPIO_ResetBits(ST7796_OTC_PORT, ST7796_RES_PIN)
#define		ST7796_RES_Set()	GPIO_SetBits(ST7796_OTC_PORT, ST7796_RES_PIN)

#define		ST7796_DC_Clr()		GPIO_ResetBits(ST7796_OTC_PORT, ST7796_DC_PIN)
#define		ST7796_DC_Set()		GPIO_SetBits(ST7796_OTC_PORT, ST7796_DC_PIN)

#define		ST7796_CS_Clr()		GPIO_ResetBits(ST7796_OTC_PORT, ST7796_CS_PIN)
#define		ST7796_CS_Set()		GPIO_SetBits(ST7796_OTC_PORT, LCD_CS_PIN)

#define		ST7796_BLK_Clr()	GPIO_ResetBits(ST7796_OTC_PORT, ST7796_BLK_PIN)
#define		ST7796_BLK_Set()	GPIO_SetBits(ST7796_OTC_PORT, ST7796_BLK_PIN)
```

### 关于中文字库
字库结构体：
```C
typedef struct 
{
	unsigned char Index[3];	
	unsigned char Msk[24];
}typFNT_UTF_8_12; 

typedef struct 
{
	unsigned char Index[3];	
	unsigned char Msk[32];
}typFNT_UTF_8_16;

typedef struct 
{
	unsigned char Index[3];	
	unsigned char Msk[72];
}typFNT_UTF_8_24;

typedef struct 
{
	unsigned char Index[3];	
	unsigned char Msk[128];
}typFNT_UTF_8_32;
```
中景园原例程使用GB2312编码，```Index```的值是2，如果有转码的需求，请修改结构体的同时修改两处中文显示函数。
第一处：
```C
/**** 
	* 函    数：显示汉字
	* 参    数：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
				mode	0非叠加模式	1叠加模式
	* 返 回 值：无
	* 说    明：最终显示中文的函数
	*/
void ST7796_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	while(*s!=0)
	{
		if(sizey==12) ST7796_ShowChinese12x12(x,y,s,fc,bc,sizey,mode);
		else if(sizey==16) ST7796_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) ST7796_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) ST7796_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=3;
		x+=sizey;
	}
}
```
将步长```s+=3```修改为```s+=2```。
第二处：
```C
/**** 
	* 函    数：显示单个12x12汉字
	* 参    数：x,y显示坐标
				*s 要显示的汉字
				fc 字的颜色
				bc 字的背景色
				sizey 字号
				mode：0非叠加模式	1叠加模式
	* 返 回 值：无
	* 说    明：最终使用LCD_ShowChinese()显示
	*/
void ST7796_ShowChinese12x12(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    ...
    
    for (k = 0; k < HZnum; k++)
	{
        if ((tfont12[k].Index[0] == s[0]) && (tfont12[k].Index[1] == s[1]) && (tfont12[k].Index[2] == s[2]))
        
		...
		
    }
    
    ...
    
}
```
12x12、16x16、24x24、32x32函数的Index都要修改，参考中景园原函数：
```C
/******************************************************************************
      函数说明：显示单个12x12汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese12x12(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	                         
	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 
```
其他的想到再写吧，有问题可以联系[我B站](https://space.bilibili.com/2004315025)或者发邮件到mrwei95@outlook.com。
