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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ

������8��ת��������ͬ��ת����ÿ��ת��8��ͨ��������Ϊ8/10/12λ��Ĭ��Ϊ��߾���12λ��
ADC�˿�AN������Ϊ����ڣ�������Ϊ����ڣ���ARM��Ƭ����ͬ��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "LQ_ADC.h"
#include "LQ_ImageProcess.h"
#include "LQ_CCU6.h"
#include "LQ_PID.h"
#include "LQ_GPIO_BUZZ.h"
#include <LQ_Inductor.h>
/**
 * ADC�Ĵ���
 */
IfxVadc_Adc vadc;

/**
 * ADCת���ṹ��
 */
Ifx_VADC_RES conversionResult;

/**
 * ADCת����
 */
IfxVadc_Adc_Group g_AdcGroup[3];

/**
 * ADCת��ͨ�����ýṹ��
 */
IfxVadc_Adc_Channel g_AdcChannel[28];


extern float g_sSteeringError ;
extern uint8_t Servo_P;
extern sint16 ServoDuty;




/*************************************************************************
*  �������ƣ�void ADC_init(void)
*  ����˵����ADC��ʼ������
*  ����˵����channel  ADCͨ��
*  ����˵����Freq_Hz  ADCͨ������Ƶ��     ע�� ADC������ģ�� ͨ��0-13  16-25  35-49�ֱ�����������ͨ��  ����ʹ��ɨ�������ʽ��������һ��ADCģ��ͬʱ��ʼ����N��ͨ������ADCʵ�ʲ����� = Freq_Hz / N
*  �������أ���
*  �޸�ʱ�䣺2020��5��10��
*  ��    ע��     ע�� adc���������1000000
*************************************************************************/



void ADC_InitConfig(ADC_Channel_t channel, unsigned long Freq_Hz)
{
	//�½�����
	IfxVadc_Adc_Config adcConfig;
	IfxVadc_Adc_initModuleConfig(&adcConfig, &MODULE_VADC);

	adcConfig.supplyVoltage = IfxVadc_LowSupplyVoltageSelect_3V;

	//��ʼ��ģ��
	IfxVadc_Adc_initModule(&vadc, &adcConfig);

	//�½�ADC������
	IfxVadc_Adc_GroupConfig adcGroupConfig;
	IfxVadc_Adc_initGroupConfig(&adcGroupConfig, &vadc);

	if(Freq_Hz > 1000000)
	{
		Freq_Hz = 1000000;
	}

	/* ���ò���ʱ�� */
	adcGroupConfig.inputClass[0].sampleTime = 1.0f/Freq_Hz;
	adcGroupConfig.inputClass[1].sampleTime = 1.0f/Freq_Hz;

	/* ���÷ֱ��� 12λ */
	adcGroupConfig.inputClass[0].resolution = IfxVadc_ChannelResolution_12bit;
	adcGroupConfig.inputClass[1].resolution = IfxVadc_ChannelResolution_12bit;

#pragma warning 547         // ���ξ���
	//�޸���
	adcGroupConfig.groupId = ADC_GetGroup(channel);
#pragma warning default     // �򿪾���

	//��Ҫ˵��:ͨ����ͬһ������Ϊmaster!
	adcGroupConfig.master = adcGroupConfig.groupId;

	//����ȫ���ٲ�����Դ��ÿ������Դ����˳�򴥷�1��4����8·ת��
	adcGroupConfig.arbiter.requestSlotQueueEnabled          = FALSE;

	//��������ģʽ����������3������Դ�е�һ��
	adcGroupConfig.arbiter.requestSlotScanEnabled           = TRUE;

	//����ɨ��ģʽ
	adcGroupConfig.arbiter.requestSlotBackgroundScanEnabled = FALSE;

	//���ñ���ɨ��
	//ʹ�����е���Ϊ "always" ģʽ (�ޱ��ؼ��)
	adcGroupConfig.queueRequest.triggerConfig.gatingMode          = IfxVadc_GatingMode_disabled;
	adcGroupConfig.scanRequest.triggerConfig.gatingMode           = IfxVadc_GatingMode_always;
	adcGroupConfig.backgroundScanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_disabled;
	//�����Զ�ɨ��ģʽ
	adcGroupConfig.scanRequest.autoscanEnabled = TRUE;

	//��ʼ����
	IfxVadc_Adc_initGroup(&g_AdcGroup[ADC_GetGroup(channel)], &adcGroupConfig);

	//�½�ͨ������
	IfxVadc_Adc_ChannelConfig adcChannelConfig;

	IfxVadc_Adc_initChannelConfig(&adcChannelConfig, &g_AdcGroup[ADC_GetGroup(channel)]);//��ʼ��
	adcChannelConfig.channelId = (IfxVadc_ChannelId)(ADC_GetChannel(channel));           //ͨ��IDѡ��
	adcChannelConfig.resultRegister = (IfxVadc_ChannelResult)(ADC_GetChannel(channel));  //��Ӧͨ��ת����������ѡ��

	//ʹ��ר�ý���Ĵ�����ʼ��ͨ��
	IfxVadc_Adc_initChannel(&g_AdcChannel[ADC_GetIndex(channel)], &adcChannelConfig);
	//�����Զ�ɨ��
	unsigned channels = (1 << adcChannelConfig.channelId);
	unsigned mask = channels;
	IfxVadc_Adc_setScan(&g_AdcGroup[ADC_GetGroup(channel)], channels, mask);

	//��ʼ�Զ�ɨ��
	IfxVadc_Adc_startScan(&g_AdcGroup[ADC_GetGroup(channel)]);

}

/*************************************************************************
*  �������ƣ�unsigned short ADC_Read(ADC_Channel_t  chn)
*  ����˵����ADC�ɼ�
*  ����˵����chn    : ADCͨ��
*  �������أ�ADC��ֵ
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
unsigned short ADC_Read(ADC_Channel_t  chn)
{
	//�ȴ�ת�����
	do{
		conversionResult = IfxVadc_Adc_getResult(&g_AdcChannel[ADC_GetIndex(chn)]);
	} while( !conversionResult.B.VF );

	return 	conversionResult.B.RESULT;//����ת�����
}

/*************************************************************************
*  �������ƣ�unsigned short ADC_Read(ADC_Channel_t  chn)
*  ����˵����ADC��βɼ�ȡƽ��ֵ
*  ����˵����chn    : ADCͨ��
*  ����˵����count  : �ɼ�����
*  �������أ�ADC��ֵ
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
unsigned short ADC_ReadAverage(ADC_Channel_t  chn, unsigned char count)
{
	unsigned short i = 0;
	unsigned long sum = 0;
	for(i = 0; i < count; i++)
	{
		sum += ADC_Read(chn);
	}
	return 	(unsigned short)(sum/count);//����ת�����
}

/*************************************************************************
*  �������ƣ�void GetlastAdcValue(void)
*  ����˵�����ɼ�ADC��ֵ�����ҽ��й�һ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2022��11��29�����ڶ�
*  ��    ע�� ADC�ɼ���5�Σ����Ҷ����������޷� ADC�����ֵΪ2000
*************************************************************************/

int ADC_Value[4]={1000,2000,2000,1000};       //��ŵĲɼ�ֵ
short left_X = 0,left_V = 0,right_X = 0, right_V = 0;
float ALL_AD = 0;
extern float K_adjust;
float A=10,B=0,C=0,P=150;
extern uint16 MagneticField;
void GetlastAdcValue(void)
{
    ADC_Value[0] = ADC_ReadAverage(ADC3, 5);
    ADC_Value[1] = ADC_ReadAverage(ADC2, 5);
    ADC_Value[2] = ADC_ReadAverage(ADC1, 5);
    ADC_Value[3] = ADC_ReadAverage(ADC0, 5);

    if (ADC_Value[0] < 120)
        ADC_Value[0] = 120;     // ˢ����Сֵ
    else if (ADC_Value[0] > 2500)
        ADC_Value[0] = 2500;     // ˢ�����ֵ
    if (ADC_Value[1] < 120)
        ADC_Value[1] = 120;     // ˢ����Сֵ
    else if (ADC_Value[1] > 2500)
        ADC_Value[1] = 2500;     // ˢ�����ֵ
    if (ADC_Value[2] < 120)
        ADC_Value[2] = 120;     // ˢ����Сֵ
    else if (ADC_Value[2] > 2500)
        ADC_Value[2] = 2500;     // ˢ�����ֵ
    if (ADC_Value[3] < 120)
        ADC_Value[3] = 120;     // ˢ����Сֵ
    else if (ADC_Value[3] > 2500)
        ADC_Value[3] = 2500;     // ˢ�����ֵ

    left_X =  (float)(ADC_Value[0] - 120.0) / (2000.0 - 120.0) * 100.0;        // ��й�һ��
    left_V =  (float)(ADC_Value[1] - 120.0) / (2000.0 - 120.0) * 100.0;        // ��й�һ��
    right_V = (float)(ADC_Value[2] - 120.0) / (2000.0 - 120.0) * 100.0;        // ��й�һ��
    right_X = (float)(ADC_Value[3] - 120.0) / (2000.0 - 120.0) * 100.0;        // ��й�һ��

    MagneticField = ADC_Value[0] + ADC_Value[1]+ ADC_Value[2] + ADC_Value[3];// �ų�����ǿ��

    if(left_V==right_V)
    {
        ALL_AD=0;
    }
    else
    {
        ALL_AD = (-1)*(float)((left_X-right_X)*P/(left_X+right_X));
    }
}
//�������õı������Գ���û��

extern uint8_t miss_Flag;
/*************************************************************************
*  �������ƣ�void ADC_Work(void)
*  ����˵������������ADC��ֵ�����ƫ��ֵ
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2022��11��29�����ڶ�
*  ��    ע��
*************************************************************************/

void ADC_Work(uint8_t *L_Flag,uint8_t *R_Flag)
{
    if(OpeningRoad_Flag == 1 && (*R_Flag == 0 ) && miss_Flag == 0)
    {
        g_sSteeringError = (right_V - left_V );
    }
    else if(miss_Flag == 1)
    {
        g_sSteeringError = -100;
    }
    else if(miss_Flag == 2)
    {
        g_sSteeringError = 100;
    }
    //ƫ��Ŵ�
    if((g_sSteeringError < 30) && (g_sSteeringError > -30))
    {
     if((g_sSteeringError < 10) && (g_sSteeringError > -10))
       {
         Servo_P = 15;
       }
      else
       {
          Servo_P = 15;
        }
     }
     else
     {
       Servo_P = 15;
      }
     ServoDuty = g_sSteeringError * Servo_P/10;
     //ƫ���޷�
     if(ServoDuty>100) ServoDuty=100;
     if(ServoDuty<-100) ServoDuty=-100;

}

/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////��/////////////////////////////////////////////////

