/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��ZYF/chiusir
��E-mail  ��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.2.2�����ϰ汾
��Target �� TC264DA/TC264D
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,

ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
����CIFΪTC264DA�����⣬�����Ĵ������TC264D
����Ĭ�ϳ�ʼ����EMEM��512K������û�ʹ��TC264D��ע�͵�EMEM_InitConfig()��ʼ��������
������\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c��164�����ҡ�
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_TFT2_TFT2SPI_H__
#define __LQ_TFT2_TFT2SPI_H__

#include "stdint.h"

/*******************�ӿڶ���******************************/
#define TFT20W        220
#define TFT20H        176

#define u16RED		0xf800
#define u16GREEN	0x07e0
#define u16BLUE	    0x001f
#define u16PURPLE	0xf81f
#define u16YELLOW	0xffe0
#define u16CYAN	    0x07ff 		//����ɫ
#define u16ORANGE	0xfc08
#define u16BLACK	0x0000
#define u16WHITE	0xffff

/*!
  * @brief    TFT2��ʼ��
  *
  * @param    type �� 0:����  1������
  *
  * @return   ��
  *
  * @note     ����޸Ĺܽ� ��Ҫ�޸ĳ�ʼ���Ĺܽ�
  *
  * @see      TFT2SPI_Init(1);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Init(unsigned char type);

/*!
  * @brief    д����
  *
  * @param    cmd ������
  *
  * @return   ��
  *
  * @note     �ڲ�����
  *
  * @see      TFT2SPI_Write_Cmd(0xb7); //LCD Driveing control
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Write_Cmd(unsigned char cmd);

/*!
  * @brief    д�ֽ�
  *
  * @param    dat ������
  *
  * @return   ��
  *
  * @note     �ڲ�����
  *
  * @see      TFT2SPI_Write_Byte(0x00);	//CRL=0
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Write_Byte(unsigned char dat);


/*!
  * @brief    д����
  *
  * @param    dat ������
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      TFT2SPI_Write_Word(0xFFFF);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Write_Word(unsigned short dat);


/*!
  * @brief    ���¶�λ������Ϣλ��
  *
  * @param    xs ����ʼx
  * @param    ys ����ʼy
  * @param    xe ������x
  * @param    ys ������y
  *
  * @return   ��
  *
  * @note     �ڲ�����
  *
  * @see      TFT2SPI_Set_Pos(10, 20, 30, 40);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Set_Pos(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye);

/*!
  * @brief    ���ָ������
  *
  * @param    xs ����ʼx
  * @param    ys ����ʼy
  * @param    xe ������x
  * @param    ys ������y
  * @param    color ��������ɫ
  *
  * @return   ��
  *
  * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
  *
  * @see      TFT2SPI_Fill_Area(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Fill_Area(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color);

/*!
  * @brief    ȫ����ʾ��ɫ����
  *
  * @param    color ��������ɫ
  *
  * @return   ��
  *
  * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
  *
  * @see      TFT2SPI_CLS(u16YELLOW);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_CLS(unsigned short color);


/*!
  * @brief    ����������
  *
  * @param    xs ����ʼx
  * @param    ys ����ʼy
  * @param    xe ������x
  * @param    ys ������y
  * @param    color_dat ��������ɫ
  *
  * @return   ��
  *
  * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
  *
  * @see      TFT2SPI_Draw_Part(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Draw_Part(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);

/*!
  * @brief    �����α߿�
  *
  * @param    xs ����ʼx
  * @param    ys ����ʼy
  * @param    xe ������x
  * @param    ys ������y
  * @param    color_dat ����ɫ
  *
  * @return   ��
  *
  * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
  *
  * @see      TFT2SPI_Draw_Rectangle(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Draw_Rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);

/*!
  * @brief    ��Բ
  *
  * @param    x ��Բ��x   (0-127)
  * @param    y ��Բ��y   (0-159)
  * @param    r ���뾶    (0-128)
  * @param    color_dat ����ɫ
  *
  * @return   ��
  *
  * @note     Բ�����겻Ҫ������Ļ��Χ
  *
  * @see      TFT2SPI_Draw_Circle(50, 50, 30, u16YELLOW);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Draw_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned short color_dat);

/*!
  * @brief    ����
  *
  * @param    xs ����ʼx
  * @param    ys ����ʼy
  * @param    xe ������x
  * @param    ys ������y
  * @param    color_dat ����ɫ
  *
  * @return   ��
  *
  * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
  *
  * @see      TFT2SPI_Draw_Line(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Draw_Line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);

/*!
  * @brief    ����
  *
  * @param    x ��x
  * @param    y ��y
  * @param    color_dat ����ɫ
  *
  * @return   ��
  *
  * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
  *
  * @see      TFT2SPI_Draw_Dot(10, 20, u16YELLOW);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Draw_Dot(unsigned char x,unsigned char y,unsigned short color_dat);

/*!
  * @brief    ��ʾͼƬ
  *
  * @param    xs ����ʼx
  * @param    ys ����ʼy
  * @param    xe ������x
  * @param    ys ������y
  * @param    ppic ��ͼƬ����
  *
  * @return   ��
  *
  * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
  *
  * @see
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Show_Pic(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,const unsigned char *ppic);

/*!
  * @brief    Һ���ַ������(6*8����)
  *
  * @param    x: 0 - 20 (��)
  * @param    y: 0 - 19 (��)
  * @param    word_color: ������ɫ
  * @param    back_color: ������ɫ
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      TFT2SPI_P6X8Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_P6X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);

/*!
  * @brief    Һ���ַ������(8*8����)
  *
  * @param    x:0 - 15 (��)
  * @param    y:0 - 19 (��)
  * @param    word_color: ������ɫ
  * @param    back_color: ������ɫ
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      TFT2SPI_P8X8Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_P8X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);

/*!
  * @brief    Һ���ַ������(8*16����)
  *
  * @param    x: x: 0 -15   (��)
  * @param    y: y: 0 -9  	 (��)
  * @param    word_color: ������ɫ
  * @param    back_color: ������ɫ
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      TFT2SPI_P8X16Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_P8X16Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);


/*!
  * @brief    Һ�������ַ������(16*16����)
  *
  * @param    x: 0 - 7	(��)
  * @param    y: 0 - 9	(��)
  * @param    word_color: ������ɫ
  * @param    back_color: ������ɫ
  *
  * @return   ��
  *
  * @note     ����ֻ�����ֿ���� �ֿ�û�е���Ҫ�������
  *
  * @see      TFT2SPI_P16x16Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_P16x16Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned short word_color,unsigned short back_color);


/*!
  * @brief    ��ʾͼƬ
  *
  * @param    xs ����ʼx
  * @param    ys ����ʼy
  * @param    xe ������x
  * @param    ys ������y
  * @param    ppic ��ͼƬ����
  *
  * @return   ��
  *
  * @note     ��ʼ����ֹ������(0-127)��������(0-159),��ʾ��ɫuint16
  *
  * @see
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Show_Pic2(unsigned char xs,unsigned char ys,unsigned char w,unsigned char h,const unsigned char *ppic) ;


/*!
  * @brief    ��ʾ���ͼ��
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      TFT2SPI_Show_Battery_Icon();
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Show_Battery_Icon(void);

/*!
  * @brief    ��ʾ��ǩ
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      TFT2SPI_Show_Title();
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Show_Title(void);

/*!
  * @brief    ��ʾ����LOG
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      TFT2SPI_Show_Logo();
  *
  * @date     2019/6/13 ������
  */
void TFT2SPI_Show_Logo(unsigned char xs,unsigned char ys);

/*!
  * @brief    TFT2�� unsigned char �Ҷ�������ʾ
  *
  * @param    high_start �� ��ʾͼ��ʼλ��
  * @param    wide_start �� ��ʾͼ��ʼλ��
  * @param    high �� ��ʾͼ��߶�
  * @param    wide �� ��ʾͼ����
  * @param    Pixle�� ��ʾͼ�����ݵ�ַ
  *
  * @return   ��
  *
  * @note     ע�� ��Ļ����Ϊ ��0��0��
  *
  * @see
  *
  * @date     2019/12/3 ���ڶ�
  */
void TFT2SPI_Road(unsigned char high_start, unsigned char wide_start, unsigned char high, unsigned char wide, unsigned char *Pixle);

/*!
  * @brief    TFT2�� unsigned char ��ֵ��������ʾ
  *
  * @param    high_start �� ��ʾͼ��ʼλ��
  * @param    wide_start �� ��ʾͼ��ʼλ��
  * @param    high �� ��ʾͼ��߶�
  * @param    wide �� ��ʾͼ����
  * @param    Pixle�� ��ʾͼ�����ݵ�ַ
  *
  * @return   ��
  *
  * @note     ע�� ��Ļ����Ϊ ��0��0��
  *
  * @see
  *
  * @date     2019/12/3 ���ڶ�
  */
void TFT2SPI_BinRoad(unsigned char high_start, unsigned char wide_start, unsigned char high, unsigned char wide, unsigned char *Pixle);
void write_command(unsigned char LCD_Reg,unsigned int LCD_RegValue) ;

void Test_TFT2(void);


#endif /*SGP18T_ILI9163B.h*/
