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

#ifndef _LQ_DMA_H_
#define _LQ_DMA_H_

#include "../../Libraries/iLLD/TC26B/Tricore/Cpu/Std/IfxCpu.h"
#include "../../Libraries/iLLD/TC26B/Tricore/Dma/Dma/IfxDma_Dma.h"
#include "../../Libraries/iLLD/TC26B/Tricore/Dma/Std/IfxDma.h"
#include "../../Libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.h"
#include "../../Libraries/Infra/Sfr/TC26B/_Reg/IfxDma_reg.h"
#include "../../Libraries/Infra/Sfr/TC26B/_Reg/IfxDma_regdef.h"
#include "../APP/LQ_CAMERA.h"

/** DMA�жϷ��������ȼ�   ��Χ��1-255   ����Խ�� ���ȼ�Խ��  ע�����ȼ���Ҫ�ظ� */
#define  DMA_PRIORITY    255

/** DMA�жϷ��������ȼ� �жϹ��ĸ��ں˹��� ��Χ��0��CPU0   1��CPU1   3��DMA*/
#define  DMA_VECTABNUM    0

/**
 * ����ͷ DMA ���ýṹ��
 */
typedef struct
{
    Ifx_DMA_CH linkedList[2];               //���Ǳ���ȥ����IFX_ALIGN(256)��ʵ��û��Ӱ��
    //IFX_ALIGN(256) Ifx_DMA_CH linkedList[2]; //����洢��������256λ��ַ���룬����DMA�޷���ȡ
    IfxDma_Dma_Channel chn;                 // Dma channel handle
} Dma_Camera_t;

/** ����ͷ ���ݲɼ���ɱ�־λ */
extern unsigned char Camera_Flag;

/**
 * ����ͷ DMA ���ýṹ��
 */
extern Dma_Camera_t g_DmaCameraLinkedList;

/* DMA �жϷ�����  */
void DMA_IRQHandler (void);

/*!
 * @brief    ����ͷDMA�����ʼ��
 *
 * @param    srcStartAddr   ��Դ��ַ IO��ַ
 * @param    dstStartAddr   ��Ŀ�ĵ�ַ ����ͷ���ݻ�����
 * @param    channel        ��DMA����ͨ�� 0-47      ע�� DMA����ͨ����Ҫ��PIX�����ж����ȼ�����һ��
 *
 * @return   ��
 *
 * @note     ע�� DMA����ͨ����Ҫ��PIX�����ж����ȼ�����һ��    һ��ͼ��Ҫ��������DMA���䣬��˻ᴥ������DMA�ж�
 *
 * @see      DMA_CameraInitConfig((unsigned long)(&MODULE_P02.IN.U), (unsigned long)Image_Data, PIN_INT2_PRIORITY);
 *
 * @date     2019/10/22 ���ڶ�
 */
void DMA_CameraInitConfig (unsigned long srcStartAddr, unsigned long dstStartAddr, unsigned long channel);

/*!
 * @brief    ����ͷDMA���俪��
 *
 * @param    channel  : DMAͨ��
 *
 * @return   ��
 *
 * @note     ע��   �ڳ��ж����ٴο���DMA����
 *
 * @see      DMA_CameraIRQ(PIN_INT2_PRIORITY);
 *
 * @date     2020/4/8
 */
IFX_INLINE void DMA_CameraStart (unsigned long channel);

/*!
 * @brief    DMA����ֹͣ
 *
 * @param    channel  : DMAͨ��
 *
 * @return   ��
 *
 * @note     ע��   �ڳ��ж����ٴο���DMA����
 *
 * @see      DMA_CameraStop(PIN_INT2_PRIORITY);
 *
 * @date     2020/4/8
 */
IFX_INLINE void DMA_CameraStop (unsigned long channel);

/*!
 * @brief    ����ͷDMA�����жϴ�����
 *
 * @param    channel  : DMAͨ��
 *
 * @return   ��
 *
 * @note     ע��    һ��ͼ��Ҫ��������DMA���䣬��˻ᴥ������DMA�ж�
 *
 * @see      DMA_CameraIRQ(PIN_INT2_PRIORITY);
 *
 * @date     2020/4/8
 */
IFX_INLINE void DMA_CameraIRQ (unsigned long channel);

/*!
 * @brief    ����ͷDMA���俪��
 *
 * @param    channel  : DMAͨ��
 *
 * @return   ��
 *
 * @note     ע��   �ڳ��ж����ٴο���DMA����
 *
 * @see      DMA_CameraIRQ(PIN_INT2_PRIORITY);
 *
 * @date     2020/4/8
 */
IFX_INLINE void DMA_CameraStart (unsigned long channel)
{
    if (Camera_Flag == 0)   //����־λ�Ƿ����   ���û������򲻽����µ����ݲɼ�  ���⸲��δʹ������
    {

        /* DMA ���ô���Ŀ�ĵ�ַ  */
        IfxDma_setChannelDestinationAddress(&MODULE_DMA, channel,
                (void *) IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), Image_Data));

        /* ����DMA����  */
        IfxDma_enableChannelTransaction(&MODULE_DMA, channel);
    }
}

/*!
 * @brief    DMA����ֹͣ
 *
 * @param    channel  : DMAͨ��
 *
 * @return   ��
 *
 * @note     ע��   �ڳ��ж����ٴο���DMA����
 *
 * @see      DMA_CameraStop(PIN_INT2_PRIORITY);
 *
 * @date     2020/4/8
 */
IFX_INLINE void DMA_CameraStop (unsigned long channel)
{
    /* ����DMA���� �����־λ */
    IfxDma_disableChannelTransaction(&MODULE_DMA, channel);
    Camera_Flag = 0;
}

/*!
 * @brief    ����ͷDMA�����жϴ�����
 *
 * @param    channel  : DMAͨ��
 *
 * @return   ��
 *
 * @note     ע��    һ��ͼ��Ҫ��������DMA���䣬��˻ᴥ������DMA�ж�
 *
 * @see      DMA_CameraIRQ(PIN_INT2_PRIORITY);
 *
 * @date     2020/4/8
 */
IFX_INLINE void DMA_CameraIRQ (unsigned long channel)
{

    /* DMA ����ͷ �жϷ����� */
    Camera_Flag++;
    if (Camera_Flag == 2)
    {
        /* ����ͷһ֡���ݴ������ ֹͣdma����  �ڳ��ж����ٴο���DMA���� */
        IfxDma_disableChannelTransaction(&MODULE_DMA, channel);
    }
    else
    {
        /* ����dma��������  */
        (IfxDma_Dma_getSrcPointer(&g_DmaCameraLinkedList.chn))->B.CLRR = 1;

        /* ����DMA����Դ��ַ */
        IfxDma_setChannelDestinationAddress(&MODULE_DMA, channel,
                (void *) IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), &Image_Data[60][0]));
    }

}

#endif /* 0_APPSW_TRICORE_APP_LQ_GPIO_H_ */
