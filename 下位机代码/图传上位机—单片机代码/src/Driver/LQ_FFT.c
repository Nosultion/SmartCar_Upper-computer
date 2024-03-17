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
ע��  Ӳ��FFT��ҪTC264 DA��оƬ���д˹���
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/


#include "LQ_FFT.h"

#include <CompilerTasking.h>




/**
 * FFT ���
 */
IfxFft_Fft g_Fft;
IfxFft_Fft_Job g_FftJob;
IfxFft_Fft_JobConfig fftJobConfig;
/***********************************************************************************************/
/********************************FFT�ж�  ������******************************************/
/***********************************************************************************************/

/*************************************************************************
*  �������ƣ�void FFT_INTRA_IRQHandler(void)
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��30��
*  ��    ע��FFT�жϷ�����
*************************************************************************/
IFX_INTERRUPT(FFT_INTRA_IRQHandler, FFT_VECTABNUM, FFT_INTRA_PRIORITY)
{
	/* ����CPU�ж�  �����жϲ���Ƕ�� */
//	IfxCpu_enableInterrupts();

    IfxFft_Fft_isrIntra(&g_Fft);
}

/*************************************************************************
*  �������ƣ�void FFT_INTRA_IRQHandler(void)
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��30��
*  ��    ע��FFT�жϷ�����
*************************************************************************/
IFX_INTERRUPT(FFT_INPUT_IRQHandler, FFT_VECTABNUM, FFT_INPUT_PRIORITY)
{
	/* ����CPU�ж�  �����жϲ���Ƕ�� */
//	IfxCpu_enableInterrupts();

    IfxFft_Fft_isrInput(&g_Fft);
}

/*************************************************************************
*  �������ƣ�void FFT_INTRA_IRQHandler(void)
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��30��
*  ��    ע��FFT�жϷ�����
*************************************************************************/
IFX_INTERRUPT(FFT_OUTPUT_IRQHandler, FFT_VECTABNUM, FFT_OUTPUT_PRIORITY)
{
	/* ����CPU�ж�  �����жϲ���Ƕ�� */
//	IfxCpu_enableInterrupts();

    IfxFft_Fft_isrOutput(&g_Fft);

}






/*************************************************************************
*  �������ƣ�void FFT_InitConfig(uint32 InDataAddr, uint32 OutDataAddr, IfxFft_Length len)
*  ����˵����FFT��ʼ������
*  ����˵����InDataAddr  : FFT�������ݵ�ַ
*  ����˵����OutDataAddr : FFT������ݵ�ַ
*  ����˵����len         : FFT����  IfxFft_Length_256  ��   IfxFft_Length_512  ��  IfxFft_Length_1024
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��       ע��  Ӳ��FFT��ҪTC264 DA��оƬ���д˹���  ע��FFT�����ʵ�ʽ����� ��Ŵ�һ������
*
*************************************************************************/
void FFT_InitConfig(uint32 InDataAddr, uint32 OutDataAddr, IfxFft_Length len)
{


	//�½�FFTģ������
	IfxFft_Fft_Config fftConfig;

	//�ر�CPU���ж�
	IfxCpu_disableInterrupts();



	IfxFft_Fft_initModuleConfig(&fftConfig, &MODULE_FFT, &MODULE_LMU);

	//�ж�����
	fftConfig.inputPriority = FFT_INPUT_PRIORITY;
	fftConfig.intraPriority = FFT_INTRA_PRIORITY;//���ڲ����䡱�ж����ȼ�
	fftConfig.outputPriority = FFT_OUTPUT_PRIORITY;
	fftConfig.typeOfService = FFT_VECTABNUM;     //����CPU�ķ�������

	//ʹ��DMAͨ������input/output�任
	fftConfig.inputDmaChannelId = IfxDma_ChannelId_4;  //����DMAͨ������(IFXFFT_FFT_DMA_SUPPORT != 0)ʱ��ʹ����
	fftConfig.outputDmaChannelId = IfxDma_ChannelId_5; //���DMAͨ������(IFXFFT_FFT_DMA_SUPPORT != 0)ʱ��ʹ����
	fftConfig.discardDmaChannelId = IfxDma_ChannelId_6;//����δʹ��������DMAͨ������(IFXFFT_FFT_DMA_SUPPORT != 0)ʱ��ʹ����

	//��װinterrupt handlers ISR_FFT_INTRA
	IfxCpu_Irq_installInterruptHandler(&FFT_INPUT_IRQHandler, FFT_INPUT_PRIORITY);
	IfxCpu_Irq_installInterruptHandler(&FFT_INTRA_IRQHandler, FFT_INTRA_PRIORITY);
	IfxCpu_Irq_installInterruptHandler(&FFT_OUTPUT_IRQHandler, FFT_OUTPUT_PRIORITY);
	IfxCpu_enableInterrupts();//���ж�ʹ��



	//FFT��ʼ��
	IfxFft_Fft_initModule(&g_Fft, &fftConfig);


	//��ҵ����
	IfxFft_Fft_initJobConfig(&fftJobConfig, &g_Fft);

	fftJobConfig.operation = IfxFft_Operation_fft;             //FFT�任   ����IFFT�任

	//�Զ�����ҵ
	fftJobConfig.fftLength = len;                              //FFT�任�ĳ���

	fftJobConfig.inputFormat = IfxFft_Input_realSInt16;        //�����ʽ��16λ��
	fftJobConfig.inputPtr = (void *)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), (uint32)InDataAddr);//��������ָ��
	fftJobConfig.inputLength = (uint16)(pow(2, len)+0.90f);     //�������ݳ���

	fftJobConfig.outputFormat = IfxFft_Output_complexSInt32;   //�����ʽ������32λ
	fftJobConfig.outputPtr = (void *)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), (uint32)OutDataAddr);//�������ָ��
	fftJobConfig.outputLength = (uint16)(pow(2, len)+0.90f);    //������ݳ���

	//��ʼ����ҵ����
	IfxFft_Fft_initJob(&g_FftJob, &fftJobConfig);
}

/*************************************************************************
*  �������ƣ�void IFFT_InitConfig(uint32 InDataAddr, uint32 OutDataAddr, IfxFft_Length len)
*  ����˵����IFFT��ʼ������
*  ����˵����InDataAddr  : IFFT�������ݵ�ַ
*  ����˵����OutDataAddr : IFFT������ݵ�ַ
*  ����˵����len         : IFFT����  IfxFft_Length_256  ��   IfxFft_Length_512  ��  IfxFft_Length_1024
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��       ע��  Ӳ��FFT��ҪTC264 DA��оƬ���д˹���
*
*************************************************************************/
void IFFT_InitConfig(uint32 InDataAddr, uint32 OutDataAddr, IfxFft_Length len)
{
	//�½�FFTģ������
	IfxFft_Fft_Config fftConfig;

	//�ر�CPU���ж�
	IfxCpu_disableInterrupts();



	IfxFft_Fft_initModuleConfig(&fftConfig, &MODULE_FFT, &MODULE_LMU);

	//�ж�����
	fftConfig.inputPriority = FFT_INPUT_PRIORITY;
	fftConfig.intraPriority = FFT_INTRA_PRIORITY;//���ڲ����䡱�ж����ȼ�
	fftConfig.outputPriority = FFT_OUTPUT_PRIORITY;
	fftConfig.typeOfService = FFT_VECTABNUM;     //����CPU�ķ�������

	//ʹ��DMAͨ������input/output�任
	fftConfig.inputDmaChannelId = IfxDma_ChannelId_4;  //����DMAͨ������(IFXFFT_FFT_DMA_SUPPORT != 0)ʱ��ʹ����
	fftConfig.outputDmaChannelId = IfxDma_ChannelId_5; //���DMAͨ������(IFXFFT_FFT_DMA_SUPPORT != 0)ʱ��ʹ����
	fftConfig.discardDmaChannelId = IfxDma_ChannelId_6;//����δʹ��������DMAͨ������(IFXFFT_FFT_DMA_SUPPORT != 0)ʱ��ʹ����

	//��װinterrupt handlers ISR_FFT_INTRA
	IfxCpu_Irq_installInterruptHandler(&FFT_INPUT_IRQHandler, FFT_INPUT_PRIORITY);
	IfxCpu_Irq_installInterruptHandler(&FFT_INTRA_IRQHandler, FFT_INTRA_PRIORITY);
	IfxCpu_Irq_installInterruptHandler(&FFT_OUTPUT_IRQHandler, FFT_OUTPUT_PRIORITY);
	IfxCpu_enableInterrupts();//���ж�ʹ��

	//FFT��ʼ��
	IfxFft_Fft_initModule(&g_Fft, &fftConfig);



	//��ҵ����
	IfxFft_Fft_initJobConfig(&fftJobConfig, &g_Fft);

	fftJobConfig.operation = IfxFft_Operation_ifft;            //IFFT�任

	//�Զ�����ҵ
	fftJobConfig.fftLength = len;                              //FFT�任�ĳ���

	fftJobConfig.inputFormat = IfxFft_Input_complexSInt32;     //�����ʽ
	fftJobConfig.inputPtr = (void *)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), (uint32)InDataAddr);//��������ָ��
	fftJobConfig.inputLength = (uint16)(pow(2, len)+0.90f);    //�������ݳ���

	fftJobConfig.outputFormat = IfxFft_Output_complexSInt32;    //�����ʽ
	fftJobConfig.outputPtr = (void *)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), (uint32)OutDataAddr);//�������ָ��
	fftJobConfig.outputLength = (uint16)(pow(2, len)+0.90f);    //������ݳ���

	//��ʼ����ҵ����
	IfxFft_Fft_initJob(&g_FftJob, &fftJobConfig);
}

/*************************************************************************
*  �������ƣ�void FFT_Start(void)
*  ����˵����FFT�任����
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��     ע��  Ӳ��FFT��ҪTC264 DA��оƬ���д˹���
*************************************************************************/
void FFT_Start(void)
{
	//��ʼת����ֱ��ת����������TRUE.
	while (IfxFft_Fft_startJob(&g_FftJob) == FALSE);

	/* Ӳ��FFT��������ͨ��DMA��������䵽���������   ��һ����ʱ   �ȴ�DMA�������  */
	for (volatile int i = 0; i < 1000; ++i)
	{
		__asm("NOP"); /* delay */
	}
}



