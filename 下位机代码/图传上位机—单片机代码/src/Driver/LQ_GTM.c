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
ͨ�ö�ʱ��ģ�飨GTM��
��4��ATOM��ģ�飬ÿ����ģ����8·�ɶ������õ�ͨ��
��3��TIM��ģ�飬ÿ����ģ����8·�ɶ������õ�ͨ��
��2��TOM��ģ�飬ÿ����ģ����16·�ɶ������õ�ͨ��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/


#include "LQ_GTM.h"

#include <IfxGtm_Atom.h>
#include <IfxGtm_regdef.h>

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////ATOM PWM���ܺ���    //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

//PWM ���ýṹ��
IfxGtm_Atom_Pwm_Driver g_AtomDriverPWM[32];
IfxGtm_Atom_Pwm_Config g_atomConfig;

/*************************************************************************
*  �������ƣ�void ATOM_PWM_InitConfig(IfxGtm_Atom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz)
*  ����˵������ʼ��ATOMΪPWM�������
*  ����˵����
  * @param    pin            ��    PWMͨ��  IfxGtm_PinMap.h�� �Ѿ������˶�Ӧ��PWM�ܽ�
  * @param    duty           ��    ռ�ձ� * ATOM_PWM_MAX
  * @param    pwmFreq_Hz     ��    Ƶ��
*  �������أ���
*  �޸�ʱ�䣺2020��4��1��
*  ��    ע��PWM_InitConfig(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 5000, 100);//��ʼ��P21_2 ��ΪPWM����� Ƶ��100Hz ռ�ձ� �ٷ�֮(5000/ATOM_PWM_MAX)*100
*************************************************************************/
void ATOM_PWM_InitConfig(IfxGtm_Atom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz)
{
	static uint8 count = 0;

	if(count == 0)
	{
		IfxGtm_enable(&MODULE_GTM); /* ʹ�� GTM */

		IfxGtm_Cmu_setClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Clk_0, ATOM_PWM_CLK); //���� CMU clock 100M Ƶ��
		IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_CLK0);             //ʹ�� CMU clock 0

		count ++;
	}

	IfxGtm_Atom_Pwm_initConfig(&g_atomConfig, &MODULE_GTM);

	g_atomConfig.atom = pin.atom;                                //ѡ��PWM����ܽ�
	g_atomConfig.atomChannel = pin.channel;                      //ѡ��PWM����ܽŶ�Ӧ��ͨ��
	g_atomConfig.pin.outputPin = &pin;                           //��������ܽ�
	g_atomConfig.period = ATOM_PWM_CLK / pwmFreq_Hz;             //�����������
	g_atomConfig.dutyCycle = (unsigned long)(duty * ((float)g_atomConfig.period / ATOM_PWM_MAX));//����ռ�ձ�
	g_atomConfig.synchronousUpdateEnabled = TRUE;                        //ʹ��PWMͬ������

	uint8 tempNum = (uint8)pin.atom * 8 + (uint8)pin.channel;

	IfxGtm_Atom_Pwm_init(&g_AtomDriverPWM[tempNum], &g_atomConfig);      //ATOM_PWM��ʼ��
	IfxGtm_Atom_Pwm_start(&g_AtomDriverPWM[tempNum], TRUE);              //��ʼ���PWM


}

/*************************************************************************
*  �������ƣ�void ATOM_PWM_SetDuty(IfxGtm_Atom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz)
*  ����˵��������PWMƵ�ʺ�ռ�ձ�
*  ����˵����
   pin            ��PWMͨ�� IfxGtm_PinMap.h�� �Ѿ������˶�Ӧ��PWM�ܽ�
   duty           ��ռ�ձ� * ATOM_PWM_MAX
   pwmFreq_Hz     : PWMƵ��    ע��˺�������������Ƶ��  ���Ҫ����Ƶ��    �������Ƶ�ʴ���ע��
*  �������أ���
*  �޸�ʱ�䣺2020��4��1��
*  ��    ע��ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 5000, 1000);//����P21_2  Ƶ�� 1000hz ռ�ձ� �ٷ�֮(5000/ATOM_PWM_MAX)*100
*************************************************************************/
void ATOM_PWM_SetDuty(IfxGtm_Atom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz)
{
	unsigned long period = ATOM_PWM_CLK / pwmFreq_Hz;                         //�����������
	unsigned long dutyCycle = (unsigned long)(duty * ((float)period / ATOM_PWM_MAX));//����ռ�ձ�

	Ifx_GTM_ATOM     *atomSFR = &MODULE_GTM.ATOM[pin.atom];

	//����Ƶ��
//	IfxGtm_Atom_Ch_setCompareZeroShadow(atomSFR, pin.channel, period);

	//����ռ�ձ�
	IfxGtm_Atom_Ch_setCompareOneShadow(atomSFR, pin.channel, dutyCycle);
}






/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////TOM PWM���ܺ���    //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


//PWM ���ýṹ��
IfxGtm_Tom_Pwm_Config tomConfig;
IfxGtm_Tom_Pwm_Driver tomDriverPWM[32];
/*************************************************************************
*  �������ƣ�void TOM_PWM_InitConfig(IfxGtm_Atom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz)
*  ����˵������ʼ��ATOMΪPWM�������
*  ����˵����
  * @param    pin            ��    PWMͨ��  IfxGtm_PinMap.h�� �Ѿ������˶�Ӧ��PWM�ܽ�
  * @param    duty           ��    ռ�ձ� * TOM_PWM_MAX
  * @param    pwmFreq_Hz     ��    Ƶ��
*  �������أ���
*  �޸�ʱ�䣺2020��4��1��
*  ��    ע��TOM_PWM_InitConfig(IfxGtm_TOM0_11_TOUT63_P20_7_OUT, 5000, 125);//��ʼ��P20_7 ��ΪPWM����� Ƶ��100Hz ռ�ձ� �ٷ�֮(5000/TOM_PWM_MAX)*100
*************************************************************************/
void TOM_PWM_InitConfig(IfxGtm_Tom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz)
{
	static uint8 count = 0;

	if(count == 0)
	{
		IfxGtm_enable(&MODULE_GTM); /* ʹ�� GTM */

		IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);       /* Enable the FXU clocks               */

		count ++;
	}


    IfxGtm_Tom_Pwm_initConfig(&tomConfig, &MODULE_GTM);                 /* Initialize default parameters            */

	tomConfig.tom = pin.tom;                                 //ѡ��PWM����ܽ�
	tomConfig.tomChannel = pin.channel;                      //ѡ��PWM����ܽŶ�Ӧ��ͨ��
	tomConfig.pin.outputPin = &pin;                           //��������ܽ�
	tomConfig.period = TOM_PWM_CLK / pwmFreq_Hz;              //�����������
	tomConfig.dutyCycle = (unsigned long)(duty * ((float)tomConfig.period / TOM_PWM_MAX));//����ռ�ձ�
    tomConfig.synchronousUpdateEnabled = TRUE;                        //ʹ��PWMͬ������
	tomConfig.clock = IfxGtm_Tom_Ch_ClkSrc_cmuFxclk1;                   /* Select the FXU clock 1              */

    uint8 tempNum = (uint8)pin.tom * 16 + (uint8)pin.channel;
    IfxGtm_Tom_Pwm_init(&tomDriverPWM[tempNum], &tomConfig);         /* Initialize the module                    */
    IfxGtm_Tom_Pwm_start(&tomDriverPWM[tempNum], TRUE);              /* Start the generation of the PWM signal   */

}
/*************************************************************************
*  �������ƣ�void TOM_PWM_SetDuty(IfxGtm_Tom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz)
*  ����˵��������PWMƵ�ʺ�ռ�ձ�
*  ����˵����
   pin            ��PWMͨ�� IfxGtm_PinMap.h�� �Ѿ������˶�Ӧ��PWM�ܽ�
   duty           ��ռ�ձ� * TOM_PWM_MAX
   pwmFreq_Hz     : PWMƵ��   ע��˺�������������Ƶ��  ���Ҫ����Ƶ��    �������Ƶ�ʴ���ע��
*  �������أ���
*  �޸�ʱ�䣺2020��4��1��
*  ��    ע��TOM_PWM_SetDuty(IfxGtm_TOM0_11_TOUT63_P20_7_OUT, 5000, 1000);//����P20_7  Ƶ�� 1000hz ռ�ձ� �ٷ�֮(5000/TOM_PWM_MAX)*100
*************************************************************************/
void TOM_PWM_SetDuty(IfxGtm_Tom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz)
{
	unsigned long period = TOM_PWM_CLK / pwmFreq_Hz;                         //�����������
	unsigned long dutyCycle = (unsigned long)(duty * ((float)period / TOM_PWM_MAX));//����ռ�ձ�

	Ifx_GTM_TOM     *atomSFR = &MODULE_GTM.TOM[pin.tom];

	//����Ƶ��
//	IfxGtm_Tom_Ch_setCompareZeroShadow(atomSFR, pin.channel, period);

	//����ռ�ձ�
	IfxGtm_Tom_Ch_setCompareOneShadow(atomSFR, pin.channel, dutyCycle);
}





/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////TIM ���岶׽���ܺ���    ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IfxGtm_Tim_In g_driverTIM[24];                          /* TIM driver structure                                     */
IfxGtm_Tim_In_Config configTIM;


/*************************************************************************
*  �������ƣ�void TIM_InitConfig(void)
*  ����˵����TIM��ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��22��
*  ��    ע��P33.12��ΪTIM�����
*************************************************************************/
void TIM_InitConfig(IfxGtm_Tim_TinMap pin)
{
	static uint8 count = 0;

	if(count == 0)
	{
		IfxGtm_enable(&MODULE_GTM);                                         /* Enable the GTM                           */
		IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_CLK0);        // Enable the CMU clock 0

		count++;
	}


    uint8 tempNum = (uint8)pin.tim * 8 + (uint8)pin.channel;

    IfxGtm_Tim_In_initConfig(&configTIM, &MODULE_GTM);                  /* Initialize default parameters            */
    configTIM.filter.inputPin = &pin;                                   /* Select input port pin                    */
    configTIM.filter.inputPinMode = IfxPort_InputMode_pullDown;         /* Select input port pin mode               */
    IfxGtm_Tim_In_init(&g_driverTIM[tempNum], &configTIM);                       /* Initialize the TIM                       */
}


/*************************************************************************
*  �������ƣ�void TIM_GetPwm(void)
*  ����˵������ȡTIM�����źŵ����ں�ռ�ձ�
*  ����˵����pin        TIM�ܽ�
*  ����˵����Period     ����
*  ����˵����Duty       ռ�ձ�
*  �������أ���
*  �޸�ʱ�䣺2020��3��22��
*  ��    ע��
*************************************************************************/
void TIM_GetPwm(IfxGtm_Tim_TinMap pin, float32 *PwmFreq_Hz, float32 *Duty)
{
	uint8  g_dataCoherent = FALSE;
	uint8 tempNum = (uint8)pin.tim * 8 + (uint8)pin.channel;
    IfxGtm_Tim_In_update(&g_driverTIM[tempNum]);                                         /* Update the measured data         */
    *PwmFreq_Hz = 1.0f/IfxGtm_Tim_In_getPeriodSecond(&g_driverTIM[tempNum]);             /* Get the period of the PWM signal */
    *Duty = IfxGtm_Tim_In_getDutyPercent(&g_driverTIM[tempNum], &g_dataCoherent);        /* Get the duty cycle     */
}






/////////////////////////////////////////////////////////////////////////////////////

