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

#include "../Driver/LQ_FFT.h"

#include <Ifx_FftF32.h>
#include <IfxFft.h>
#include <math.h>
#include <Platform_Types.h>

#include "LQ_GPIO_LED.h"



/* matlab ����  ���ԶԱ�fft�Ľ��
Fs = 1024;            % Sampling frequency
T = 1/Fs;             % Sampling period
L = 1024;             % Length of signal
t = (0:L-1)*T;        % Time vector
S = 70*sin(2*pi*50*t) + 100*sin(2*pi*120*t);
f = Fs*(0:(L/2))/L;
Y = fft(S);
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
hold off
plot(f,P1)
title('Single-Sided Amplitude Spectrum of S(t)')
xlabel('f (Hz)')
ylabel('|P1(f)|')
hold on
plot(1000*t(1:50),S(1:50))
title('Signal Corrupted with Zero-Mean Random Noise')
xlabel('t (milliseconds)')
ylabel('X(t)')
 */

/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////���²���Ϊ���ܲ���/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////



//IFFT�任���������256λ����
IFX_ALIGN(256)  csint32 ifftOut[1024];

//Դ���Σ�����256λ����
IFX_ALIGN(256)  sint16 fftIn[1024];

//FFT�任���������256λ����
IFX_ALIGN(256)  csint32 fftOut[1024];

/*************************************************************************
*  �������ƣ�void Test_FFT(void)
*  ����˵��������FFT�任
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע������FFT�任��IFFT�任    ע��  Ӳ��FFT��ҪTC264 DA��оƬ���д˹���
*************************************************************************/
void Test_FFT(void)
{
	unsigned short Fs = 1024;    //�ź�Ƶ��

	float    T  = 1.0f/Fs; //�ź�����

	unsigned short L  = 1024;    //����

	/* ��һ��50Hz��120Hz�����������ź�  */
	int i = 0;
	for(i = 0; i < L; i++)
	{
		fftIn[i] = 70*sin(2*IFX_PI*50*T*i) + 100*sin(2*IFX_PI*120*T*i);
	}


	while(1)
	{
		/* ��ʼ��Ϊ FFT  ע�� ����FFT��������ʵ�ʽ����64��  */
		FFT_InitConfig((uint32)fftIn, (uint32)fftOut, IfxFft_Length_1024);

		FFT_Start();

		/* ��ʼ��Ϊ IFFT  */
		IFFT_InitConfig((uint32)fftOut, (uint32)ifftOut, IfxFft_Length_1024);

		FFT_Start();

		LED_Ctrl(LED0,RVS);               //��ƽ��ת,LED��˸

	}
}


//Դ���Σ�256λ����
IFX_ALIGN(256) cfloat32 sfftIn[1024];

//IFFT�任�����256λ����
IFX_ALIGN(256) cfloat32 sifftOut[1024];

//FFT�任�����256λ����
IFX_ALIGN(256) cfloat32 sfftOut[1024];

/*************************************************************************
*  �������ƣ�void Test_SoftFft(void)
*  ����˵�����������FFT�任
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��4��20��
*  ��    ע������FFT�任��IFFT�任    ע��  ����TC264 оƬ������
*************************************************************************/
void Test_SoftFft(void)
{
	unsigned short Fs = 1024;    //�ź�Ƶ��

	float    T  = 1.0f/Fs; //�ź�����

	unsigned short L  = 1024;    //����

	/* ��һ��50Hz��120Hz�����������ź�  */
	int i = 0;
	for(i = 0; i < L; i++)
	{
		sfftIn[i].real = 70*sin(2*IFX_PI*50*T*i) + 100*sin(2*IFX_PI*120*T*i);
		sfftIn[i].imag = 0;
	}

	while(1)
	{
		//FFT
		Ifx_FftF32_radix2(sfftOut, sfftIn, 1024);

		//IFFT  ע�� ����IFFT��������ʵ�ʽ����1024��
		Ifx_FftF32_radix2I(sifftOut, sfftOut, 1024);

		LED_Ctrl(LED0,RVS);               //��ƽ��ת,LED��˸

	}
}
