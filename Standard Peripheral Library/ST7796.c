/**** 
	* 本程序由MrWei95开源共享
	* 基于中景园例程
	*/

#include "ST7796.h"
#include "lcdfont.h"
#include "Delay.h"

/**** 
	* 函    数：在指定区域填充颜色
	* 参    数：xsta,ysta	起始坐标
				xend,yend	终止坐标
				color		要填充的颜色
	* 返 回 值：无
	*/
void ST7796_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	ST7796_Address_Set(xsta,ysta,xend-1, yend-1);	// 设置显示范围
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			ST7796_WR_DATA(color);
		}
	}
}


/**** 
	* 函    数：在指定位置画点
	* 参    数：x,y 画点坐标
				color 点的颜色
	* 返 回 值：无
	*/
void ST7796_DrawPoint(u16 x,u16 y,u16 color)
{
	ST7796_Address_Set(x,y,x,y);		// 设置光标位置 
	ST7796_WR_DATA(color);
} 


/**** 
	* 函    数：画线
	* 参    数：x1,y1   起始坐标
				x2,y2   终止坐标
				color   线的颜色
	* 返 回 值：无
	*/
void ST7796_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t;
	int xerr=0, yerr=0, delta_x, delta_y, distance;
	int incx,incy, uRow, uCol;
	delta_x = x2 - x1;							// 计算坐标增量 
	delta_y = y2 - y1;
	uRow = x1;									// 画线起点坐标
	uCol = y1;
	if (delta_x>0) incx = 1;					// 设置单步方向 
	else if (delta_x==0) incx = 0;				// 垂直线 
	else {incx=-1; delta_x=-delta_x;}
	if (delta_y>0)incy=1;
	else if (delta_y == 0) incy = 0;			// 水平线 
	else {incy=-1;delta_y=-delta_y;}
	if (delta_x>delta_y) distance = delta_x;	// 选取基本增量坐标轴 
	else distance = delta_y;
	for (t = 0; t < distance+1; t++)
	{
		ST7796_DrawPoint(uRow,uCol,color);			// 画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/**** 
	* 函    数：画矩形
	* 参    数：x1,y1   起始坐标
				x2,y2   终止坐标
				color   矩形的颜色
	* 返 回 值：无
	*/
void ST7796_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	ST7796_DrawLine(x1,y1,x2,y1,color);
	ST7796_DrawLine(x1,y1,x1,y2,color);
	ST7796_DrawLine(x1,y2,x2,y2,color);
	ST7796_DrawLine(x2,y1,x2,y2,color);
}


/**** 
	* 函    数：画圆
	* 参    数：x0, y0	圆心坐标
				r 		半径
				color	圆的颜色
	* 返 回 值：无
	*/
void ST7796_Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	a = 0; b = r;	  
	while(a <= b)
	{
		ST7796_DrawPoint(x0-b,y0-a,color);             //	3           
		ST7796_DrawPoint(x0+b,y0-a,color);             //	0           
		ST7796_DrawPoint(x0-a,y0+b,color);             //	1                
		ST7796_DrawPoint(x0-a,y0-b,color);             //	2             
		ST7796_DrawPoint(x0+b,y0+a,color);             //	4               
		ST7796_DrawPoint(x0+a,y0-b,color);             //	5
		ST7796_DrawPoint(x0+a,y0+b,color);             //	6 
		ST7796_DrawPoint(x0-b,y0+a,color);             //	7
		a++;
		if((a*a+b*b)>(r*r))		// 判断要画的点是否过远
		{
			b--;
		}
	}
}


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
    u8 i, j, m = 0;
    u16 k;
    u16 HZnum;			// 汉字数目
    u16 TypefaceNum;	// 一个字符所占字节大小
    u16 x0 = x;
    
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont12) / sizeof(typFNT_UTF_8_12);		// 统计汉字数目
    
    for (k = 0; k < HZnum; k++)
	{
        if ((tfont12[k].Index[0] == s[0]) && (tfont12[k].Index[1] == s[1]) && (tfont12[k].Index[2] == s[2]))
		{
            ST7796_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++)
			{
                for (j = 0; j < 8; j++)
				{
					// 非叠加方式
                    if (!mode)
					{
                        if (tfont12[k].Msk[i] & (0x01 << j))
                            ST7796_WR_DATA(fc);
                        else
                            ST7796_WR_DATA(bc);
                        m++;
                        if (m % sizey == 0)
						{
                            m = 0;
                            break;
                        }
                    }
					// 叠加方式
					else
					{
                        if (tfont12[k].Msk[i] & (0x01 << j))
                            ST7796_DrawPoint(x, y, fc);
                        x++;
                        if ((x - x0) == sizey)
						{
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
            break;		// 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
        }
    }
}


/**** 
	* 函    数：显示单个16x16汉字
	* 参    数：x,y显示坐标
				*s 要显示的汉字
				fc 字的颜色
				bc 字的背景色
				sizey 字号
				mode：0非叠加模式	1叠加模式
	* 返 回 值：无
	* 说    明：最终使用LCD_ShowChinese()显示
	*/
void ST7796_ShowChinese16x16(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    u8 i, j, m = 0;
    u16 k;
    u16 HZnum;			// 汉字数目
    u16 TypefaceNum;	// 一个字符所占字节大小
    u16 x0 = x;
    
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont16) / sizeof(typFNT_UTF_8_16);		// 统计汉字数目
    
    for (k = 0; k < HZnum; k++) 
    {
        if ((tfont16[k].Index[0] == s[0]) && (tfont16[k].Index[1] == s[1]) && (tfont16[k].Index[2] == s[2]))
        {    
            ST7796_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {    
					// 非叠加方式
                    if (!mode)
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j))
                            ST7796_WR_DATA(fc);
                        else 
                            ST7796_WR_DATA(bc);
                        
                        m++;
                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
					// 叠加方式
                    else
                    {
						if (tfont16[k].Msk[i] & (0x01 << j))
							ST7796_DrawPoint(x, y, fc);
						x++;
                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
            break;		// 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
        }
    }
}


/**** 
	* 函    数：显示单个24x24汉字
	* 参    数：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
				mode	0非叠加模式	1叠加模式
	* 返 回 值：无
	* 说    明：最终使用LCD_ShowChinese()显示
	*/
void ST7796_ShowChinese24x24(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    u8 i, j, m = 0;
    u16 k;
    u16 HZnum;			// 汉字数目
    u16 TypefaceNum;	// 一个字符所占字节大小
    u16 x0 = x;
    
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont24) / sizeof(typFNT_UTF_8_24);		// 统计汉字数目
    
    for (k = 0; k < HZnum; k++) 
    {
        if ((tfont24[k].Index[0] == s[0]) && (tfont24[k].Index[1] == s[1]) && (tfont24[k].Index[2] == s[2]))
        {    
            ST7796_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
					// 非叠加方式
                    if (!mode)
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j))
                            ST7796_WR_DATA(fc);
                        else 
                            ST7796_WR_DATA(bc);
                        
                        m++;
                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
					// 叠加方式
                    else
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j))
                            ST7796_DrawPoint(x, y, fc);
                        x++;
                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
            break;		// 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
        }
    }
}


/**** 
	* 函    数：显示单个32x32汉字
	* 参    数：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
				mode	0非叠加模式	1叠加模式
	* 返 回 值：无
	* 说    明：最终使用LCD_ShowChinese()显示
	*/
void ST7796_ShowChinese32x32(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    u8 i, j, m = 0;
    u16 k;
    u16 HZnum;			// 汉字数目
    u16 TypefaceNum;	// 一个字符所占字节大小
    u16 x0 = x;
    
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont32) / sizeof(typFNT_UTF_8_32);		// 统计汉字数目
    
    for (k = 0; k < HZnum; k++) 
    {
        if ((tfont32[k].Index[0] == s[0]) && (tfont32[k].Index[1] == s[1]) && (tfont32[k].Index[2] == s[2]))
        {    
            ST7796_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
					// 非叠加方式
                    if (!mode)
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j))
                            ST7796_WR_DATA(fc);
                        else 
                            ST7796_WR_DATA(bc);
                        
                        m++;
                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
					// 叠加方式
                    else
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j))
                            ST7796_DrawPoint(x, y, fc);
                        x++;
                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
            break;		// 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
        }
    }
}


/**** 
	* 函    数：显示单个字符
	* 参    数：x,y显示坐标
				num 要显示的字符
				fc 字的颜色
				bc 字的背景色
				sizey 字号
				mode	0非叠加模式	1叠加模式
	* 返 回 值：无
	*/
void ST7796_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp,sizex,t,m=0;
	u16 i,TypefaceNum;	// 一个字符所占字节大小
	u16 x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';		// 得到偏移后的值
	ST7796_Address_Set(x,y,x+sizex-1,y+sizey-1);			// 设置光标位置 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];			// 调用6x12字体
		else if(sizey==16)temp=ascii_1608[num][i];		// 调用8x16字体
		else if(sizey==24)temp=ascii_2412[num][i];		// 调用12x24字体
		else if(sizey==32)temp=ascii_3216[num][i];		// 调用16x32字体
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)	// 非叠加模式
			{
				if(temp&(0x01<<t)) ST7796_WR_DATA(fc);
				else ST7796_WR_DATA(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else	// 叠加模式
			{
				if(temp&(0x01<<t)) ST7796_DrawPoint(x,y,fc);	// 画一个点
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}


/**** 
	* 函    数：显示字符串
	* 参    数：x,y显示坐标
				*p 要显示的字符串
				fc 字的颜色
				bc 字的背景色
				sizey 字号
				mode	0非叠加模式	1叠加模式
	* 返 回 值：无
	*/
void ST7796_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{         
	while(*p!='\0')
	{       
		ST7796_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


/**** 
	* 函    数：显示数字
	* 参    数：m底数，n指数
	* 返 回 值：无
	*/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}


/**** 
	* 函    数：显示整数变量
	* 参    数：x,y显示坐标
				num 要显示小数变量
				len 要显示的位数
				fc 字的颜色
				bc 字的背景色
				sizey 字号
	* 返 回 值：无
	*/
void ST7796_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp;
	u8 enshow=0;
	u8 sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				ST7796_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	ST7796_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
} 


/**** 
	* 函    数：显示两位小数变量
	* 参    数：x,y显示坐标
				num 要显示小数变量
				len 要显示的位数
				fc 字的颜色
				bc 字的背景色
				sizey 字号
	* 返 回 值：无
	*/
void ST7796_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey)
{
	u8 t,temp,sizex;
	u16 num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			ST7796_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	ST7796_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}


/**** 
	* 函    数：显示图片
	* 参    数：x,y起点坐标
				length 图片长度
				width  图片宽度
				pic[]  图片数组   
	* 返 回 值：无
	*/
void ST7796_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[])
{
	u16 i,j;
	u32 k=0;
	ST7796_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			ST7796_WR_DATA8(pic[k*2]);
			ST7796_WR_DATA8(pic[k*2+1]);
			k++;
		}
	}
}


/**** 
	* 函    数：设置起始和结束地址
	* 参    数：x1,x2 设置列的起始和结束地址
				y1,y2 设置行的起始和结束地址
	* 返 回 值：无
	*/
void ST7796_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	ST7796_WR_REG(0x2a);	// 列地址设置
	ST7796_WR_DATA(x1);
	ST7796_WR_DATA(x2);
	ST7796_WR_REG(0x2b);	// 行地址设置
	ST7796_WR_DATA(y1);
	ST7796_WR_DATA(y2);
	ST7796_WR_REG(0x2c);	// 储存器写
}


/**** 
	* 函    数：LCD初始化
	* 参    数：无
	* 返 回 值：无
	* 说    明：宏定义ST7796_SCREEN_POSITION控制屏幕方向
	*/
void ST7796_Init(void)
{
	ST7796_GPIO_Init();		// 初始化GPIO
	
	ST7796_RES_Clr();		// 复位
	Delay_ms(100);
	ST7796_RES_Set();
	Delay_ms(200);
	
	ST7796_BLK_Set();		// 打开背光
	Delay_ms(100);
	
	ST7796_WR_REG(0x11);	// 唤醒
	Delay_ms(120);
	
	ST7796_WR_REG(0Xf0);
	ST7796_WR_DATA8(0xc3);
	ST7796_WR_REG(0Xf0);
	ST7796_WR_DATA8(0x96);
	ST7796_WR_REG(0x36);	// 内存访问控制
	
	// 设置屏幕方向
	if (ST7796_SCREEN_POSITION == 0) ST7796_WR_DATA8(0x48);
	else if (ST7796_SCREEN_POSITION == 1) ST7796_WR_DATA8(0x88);
	else if (ST7796_SCREEN_POSITION == 2) ST7796_WR_DATA8(0x28);
	else ST7796_WR_DATA8(0xE8);
	
	ST7796_WR_REG(0x3A);
	ST7796_WR_DATA8(0x05);
	
	ST7796_WR_REG(0Xe8);
	ST7796_WR_DATA8(0x40);
	ST7796_WR_DATA8(0x82);
	ST7796_WR_DATA8(0x07);
	ST7796_WR_DATA8(0x18);
	ST7796_WR_DATA8(0x27);
	ST7796_WR_DATA8(0x0a);
	ST7796_WR_DATA8(0xb6);
	ST7796_WR_DATA8(0x33);
	
	ST7796_WR_REG(0Xc5);
	ST7796_WR_DATA8(0x27);
	
	ST7796_WR_REG(0Xc2);
	ST7796_WR_DATA8(0xa7);
	
	ST7796_WR_REG(0Xe0);
	ST7796_WR_DATA8(0xf0);
	ST7796_WR_DATA8(0x01);
	ST7796_WR_DATA8(0x06);
	ST7796_WR_DATA8(0x0f);
	ST7796_WR_DATA8(0x12);
	ST7796_WR_DATA8(0x1d);
	ST7796_WR_DATA8(0x36);
	ST7796_WR_DATA8(0x54);
	ST7796_WR_DATA8(0x44);
	ST7796_WR_DATA8(0x0c);
	ST7796_WR_DATA8(0x18);
	ST7796_WR_DATA8(0x16);
	ST7796_WR_DATA8(0x13);
	ST7796_WR_DATA8(0x15);
	
	ST7796_WR_REG(0Xe1);
	ST7796_WR_DATA8(0xf0);
	ST7796_WR_DATA8(0x01);
	ST7796_WR_DATA8(0x05);
	ST7796_WR_DATA8(0x0a);
	ST7796_WR_DATA8(0x0b);
	ST7796_WR_DATA8(0x07);
	ST7796_WR_DATA8(0x32);
	ST7796_WR_DATA8(0x44);
	ST7796_WR_DATA8(0x44);
	ST7796_WR_DATA8(0x0c);
	ST7796_WR_DATA8(0x18);
	ST7796_WR_DATA8(0x17);
	ST7796_WR_DATA8(0x13);
	ST7796_WR_DATA8(0x16);
	
	ST7796_WR_REG(0Xf0);
	ST7796_WR_DATA8(0x3c);
	
	ST7796_WR_REG(0Xf0);
	ST7796_WR_DATA8(0x69);
	
	ST7796_WR_REG(0X29);	 
}


/**** 
	* 函    数：LCD串行数据写入函数
	* 参    数：dat  要写入的串行数据
	* 返 回 值：无
	*/
void ST7796_Writ_Bus(u8 dat) 
{	
	GPIO_ResetBits(ST7796_OTC_PORT, ST7796_CS_PIN);	// 选中LCD

    while (SPI_I2S_GetFlagStatus(ST7796_SPI_PORT, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(ST7796_SPI_PORT, dat);
    while (SPI_I2S_GetFlagStatus(ST7796_SPI_PORT, SPI_I2S_FLAG_BSY) == SET);

    GPIO_SetBits(ST7796_OTC_PORT, ST7796_CS_PIN);	// 取消选中LCD
}

/**** 
	* 函    数：LCD写入数据
	* 参    数：dat 写入的数据
	* 返 回 值：无
	*/
void ST7796_WR_DATA8(u8 dat)
{
	GPIO_SetBits(ST7796_OTC_PORT, ST7796_DC_PIN);
    ST7796_Writ_Bus(dat);
}

/**** 
	* 函    数：LCD写入数据
	* 参    数：dat 写入的数据
	* 返 回 值：无
	*/
void ST7796_WR_DATA(u16 dat)
{
	GPIO_SetBits(ST7796_OTC_PORT, ST7796_DC_PIN);
    ST7796_Writ_Bus(dat >> 8);
    ST7796_Writ_Bus(dat);
}

/**** 
	* 函    数：LCD写入命令
	* 参    数：dat 写入的命令
	* 返 回 值：无
	*/
void ST7796_WR_REG(u8 dat)
{
	GPIO_ResetBits(ST7796_OTC_PORT, ST7796_DC_PIN);
    ST7796_Writ_Bus(dat);
}


/**** 
	* 函    数：LCD GPIO 初始化
	* 参    数：无
	* 返 回 值：无
	* 说    明：使用硬件SPI通讯
	*/
void ST7796_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    
    // 使能SPI相关的GPIO时钟
    RCC_APB2PeriphClockCmd(ST7796_SPI_RCC | ST7796_RCC, ENABLE);
    
    // 配置SPI SCK，MOSI
    GPIO_InitStructure.GPIO_Pin = ST7796_SCL_PIN | ST7796_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ST7796_GPIO_PORT, &GPIO_InitStructure);
    
	// 使能屏幕相关其他的GPIO时钟
	RCC_APB2PeriphClockCmd(ST7796_OTC_RCC, ENABLE);
	
    // 配置CS，DC，RES，BLK为推挽输出
    GPIO_InitStructure.GPIO_Pin = ST7796_CS_PIN | ST7796_DC_PIN | ST7796_RES_PIN | ST7796_BLK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(ST7796_OTC_PORT, &GPIO_InitStructure);
    
    // 设置默认电平
    GPIO_SetBits(ST7796_OTC_PORT, ST7796_CS_PIN);
    GPIO_SetBits(ST7796_OTC_PORT, ST7796_DC_PIN);
    GPIO_SetBits(ST7796_OTC_PORT, ST7796_RES_PIN);
    GPIO_SetBits(ST7796_OTC_PORT, ST7796_BLK_PIN);
    
    // SPI配置
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		// 更快
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(ST7796_SPI_PORT, &SPI_InitStructure);
    
    SPI_Cmd(ST7796_SPI_PORT, ENABLE);
}
