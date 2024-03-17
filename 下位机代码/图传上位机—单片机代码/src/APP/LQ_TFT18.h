/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】ZYF/chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年10月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.2.2及以上版本
【Target 】 TC264DA/TC264D
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,

使用例程的时候，建议采用没有空格的英文路径，
除了CIF为TC264DA独有外，其它的代码兼容TC264D
本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_SGP18T_TFTSPI_H__
#define __LQ_SGP18T_TFTSPI_H__	

#include "stdint.h"

/*******************接口定义******************************/

#define APP_LCD_WIDTH  LCD_WIDTH//液晶屏宽度
#define APP_LCD_HEIGHT LCD_HEIGHT//液晶屏高度

#define TFT18W        162
#define TFT18H        132

#define u16RED		0xf800
#define u16GREEN	0x07e0
#define u16BLUE	    0x001f
#define u16PURPLE	0xf81f
#define u16YELLOW	0xffe0
#define u16CYAN	    0x07ff 		//蓝绿色
#define u16ORANGE	0xfc08
#define u16BLACK	0x0000
#define u16WHITE	0xffff

void TFTSPI_Init(unsigned char type);
void TFTSPI_Write_Cmd(unsigned char cmd);
void TFTSPI_Write_Byte(unsigned char dat);
void TFTSPI_Write_Word(unsigned short dat);
void TFTSPI_Set_Pos(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye);
void TFTSPI_Fill_Area(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color);
void TFTSPI_CLS(unsigned short color);
void TFTSPI_Draw_Dot(unsigned char x,unsigned char y,unsigned short color_dat);
void TFTSPI_Draw_Part(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
void TFTSPI_Draw_Rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
void TFTSPI_Draw_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned short color_dat);
void TFTSPI_Draw_Line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
void TFTSPI_Show_Pic(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,const unsigned char *ppic);
void TFTSPI_P6X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_P8X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_P8X16Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_P16x16Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_Show_Pic2(unsigned char xs,unsigned char ys,unsigned char w,unsigned char h,const unsigned char *ppic) ;
void TFTSPI_Show_Battery_Icon(void);
void TFTSPI_Show_Title(void);
void TFTSPI_Show_Logo(unsigned char xs,unsigned char ys);
void TFTSPI_Road( unsigned char wide_start, unsigned char high_start,unsigned char high, unsigned char wide, unsigned char Pixle[120][188]);
void TFTSPI_BinRoad( unsigned char wide_start, unsigned char high_start,unsigned char high, unsigned char wide, unsigned char *Pixle);
void TFTSPI_ShowBeacon(void );
void Test_TFT18(void);


#endif /*SGP18T_ILI9163B.h*/
