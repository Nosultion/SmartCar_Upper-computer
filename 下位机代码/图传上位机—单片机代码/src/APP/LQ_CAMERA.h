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

#ifndef __LQ_CAMERA_H
#define __LQ_CAMERA_H

#include "LQ_MT9V034.h"


#define LQMT9V034   //Ĭ��Ϊ��������ͷ
#define USETFT1_8
//#define USEOLED    //ʹ��OLED����TFT1.8ģ�飬Ĭ��ΪTFT1.8

#define IMAGEH  MT9V034_IMAGEH   /*!< ����ͷ�ɼ��߶� */
#define IMAGEW  MT9V034_IMAGEW   /*!< ����ͷ�ɼ���� */

/* ʹ�������� �޸����Ｔ�� */
#ifdef USEOLED
	#define LCDH    60  /*!< OLED��ʾ�߶ȣ��û�ʹ�ã��߶� */
	#define LCDW    80  /*!< OLED��ʾ��ȣ��û�ʹ�ã���� */
#else
	#define LCDH    120  /*!< TFT��ʾ�߶ȣ��û�ʹ�ã��߶� */
	#define LCDW    160  /*!< TFT��ʾ��ȣ��û�ʹ�ã���� */
#endif

//#define SMALLIMAGE  // ʹ��С�ߴ���ʾ60*94
#define MAX_ROW   LCDH
#define MAX_COL   LCDW

/** ͼ��ԭʼ���ݴ�� */
extern unsigned char Image_Data[IMAGEH][IMAGEW];

/** ѹ����֮�����ڴ����Ļ��ʾ����  */
extern unsigned char Image_Use[LCDH][LCDW];
extern unsigned char Image_Use_BLACK[IMAGEH][IMAGEW];
extern unsigned char image_min[60][80];
extern unsigned char Filtered_Image[LCDH][LCDW];
/** ��ֵ��������OLED��ʾ������ */
extern unsigned char Bin_Image[IMAGEH][IMAGEW];
extern unsigned char MAX[24];

/*!
  * @brief    �����ϱ���λ��
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��λ����֡ͷ������������ 
  *
  * @see      CAMERA_Reprot();
  *
  * @date     2019/9/24 ���ڶ�
  */
void CAMERA_Reprot(void);



/*!
  * @brief    ����ͷ��������
  *
  * @param    fps:  ֡�� 
  *
  * @return   ��
  *
  * @note     ����ͷ��һЩ��������LQ_MT9V034.c�еĺ궨�����޸�
  *
  * @see      CAMERA_Init(50);   //��ʼ��MT9V034  50֡ ע��ʹ�ð�ɫ������Ű�ת����
  *
  * @date     2019/10/22 ���ڶ�
  */
void CAMERA_Init(unsigned char fps);


/**
  * @brief    ��ȡ��Ҫʹ�õ�ͼ������
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      Get_Use_Image();
  *
  * @date     2019/6/25 ���ڶ�
  */
void Get_Use_Image(void);


/**
  * @brief    ��ֵ��
  *
  * @param    mode  ��  0��ʹ�ô����ֵ    1��ʹ��ƽ����ֵ
  *
  * @return   ��
  *
  * @note     ��
  *
  * @example  Get_Bin_Image(0); //ʹ�ô�򷨶�ֵ��
  *
  * @date     2019/6/25 ���ڶ�
  */
void Get_Bin_Image(unsigned char mode);



/*!
  * @brief    �������ֵ��С 
  *
  * @param    tmImage �� ͼ������
  *
  * @return   ��ֵ
  *
  * @note     �ο���https://blog.csdn.net/zyzhangyue/article/details/45841255
  * @note     https://www.cnblogs.com/moon1992/p/5092726.html
  * @note     https://www.cnblogs.com/zhonghuasong/p/7250540.html     
  * @note     Ostu������������������ͨ��ͳ������ͼ���ֱ��ͼ������ʵ��ȫ����ֵT���Զ�ѡȡ�����㷨����Ϊ��
  * @note     1) �ȼ���ͼ���ֱ��ͼ������ͼ�����е����ص㰴��0~255��256��bin��ͳ������ÿ��bin�����ص�����
  * @note     2) ��һ��ֱ��ͼ��Ҳ����ÿ��bin�����ص����������ܵ����ص�
  * @note     3) i��ʾ�������ֵ��Ҳ��һ���Ҷȼ�����0��ʼ����	1
  * @note     4) ͨ����һ����ֱ��ͼ��ͳ��0~i �Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��ı���w0����ͳ��ǰ�����ص�ƽ���Ҷ�u0��ͳ��i~255�Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ�����������) ��ռ����ͼ��ı���w1����ͳ�Ʊ������ص�ƽ���Ҷ�u1��
  * @note     5) ����ǰ�����غͱ������صķ��� g = w0*w1*(u0-u1) (u0-u1)
  * @note     6) i++��ת��4)��ֱ��iΪ256ʱ��������
  * @note     7) �����g��Ӧ��iֵ��Ϊͼ���ȫ����ֵ
  * @note     ȱ��:OSTU�㷨�ڴ�����ղ����ȵ�ͼ���ʱ��Ч�������Բ��ã���Ϊ���õ���ȫ��������Ϣ��
  * @note     ������ղ�����  https://blog.csdn.net/kk55guang2/article/details/78475414
  * @note     https://blog.csdn.net/kk55guang2/article/details/78490069
  * @note     https://wenku.baidu.com/view/84e5eb271a37f111f0855b2d.html
  *
  * @see      GetOSTU(Image_Use);//�����ֵ
  *
  * @date     2019/6/25 ���ڶ�
  */ 
short GetOSTU(unsigned char tmImage[LCDH][LCDW]);


/*!
  * @brief    ����ͷ��������
  *
  * @param
  *
  * @return
  *
  * @note     ����MT9V034  ע����Ҫʹ��  ������Ű棨��ɫ��ת����
  *
  * @example
  *
  * @date     2019/10/22 ���ڶ�
  */
void Test_CAMERA(void);


/*!
  * @brief    ����soble���ؼ�����ӵ�һ�ֱ��ؼ��
  *
  * @param    imageIn    ��������
  *           imageOut   �������      ����Ķ�ֵ����ı�����Ϣ
  *           Threshold  ��ֵ
  *
  * @return
  *
  * @note
  *
  * @example
  *
  * @date     2020/5/15
  */
void lq_sobel(unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW], unsigned char Threshold);


/*!
  * @brief    ����soble���ؼ�����ӵ�һ���Զ���ֵ���ؼ��
  *
  * @param    imageIn    ��������
  *           imageOut   �������      ����Ķ�ֵ����ı�����Ϣ
  *
  * @return
  *
  * @note
  *
  * @example
  *
  * @date     2020/5/15
  */
void lq_sobelAutoThreshold(unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW]);
void Seek_Road(void);
void Bin_Image_Filter(void);
#endif



