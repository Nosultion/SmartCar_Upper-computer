/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 ��ƽ    ̨�������������ܿƼ�TC264DA���İ�
 ����    д��chiusir
 ��E-mail��chiusir@163.com
 ������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
 �������¡�2020��10��28��
 �������Ϣ�ο����е�ַ��
 ����    վ��http:// www.lqist.cn
 ���Ա����̡�http:// longqiu.taobao.com
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
 =================================================================
 ����������Ƶ��ַ��https:// space.bilibili.com/95313236
 =================================================================
 ʹ��˵����
 ����ѧ��ʾ���������ڵ�����ֻ������ֳ���
 ������ԴΪ��
 ģ�飺����TC264DA���İ塢����ĸ�塢˫·ȫ�ŵ��������˫������512������TFT1.8��Ļ�����������·���ģ�飻
 ��ģ�����ֻ������־��ɣ�
 ��зֲ���
 ||----------------------��-----------------��----------------------||
 ||--------------------��---------------------��--------------------||
 ||------------------��-------------------------��------------------||
 ||-����1�����------2--------- ------------- ----2------�Ҳ��1�����-||
 ||---------------��-------------------------------��---------------||
 ||-------------��-----------------------------------��-------------||
 ||-----------��---------------------------------------��-----------||
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "LQ_Inductor.h"

#include <Platform_Types.h>
#include <stdio.h>

#include "../APP/LQ_GPIO_KEY.h"
#include "../APP/LQ_TFT18.h"
#include "../Driver/LQ_ADC.h"
#include "../Driver/LQ_CCU6.h"
#include "../Driver/LQ_STM.h"
#include "LQ_MotorServo.h"


sint16 TempAngle = 0;        // ���ݵ��ƫ�������������ʱ���ֵ
sint16 LastAngle = 0;        // ����������ǰ����Чƫ�Ʒ���

sint16 Lleft1 = 0, Lleft2 = 0, Lright2 = 0, Lright1 = 0;  // ���ƫ����
sint16 LnowADC[6];           // ��е�ǰADC��ֵ

sint16 ad_max[6] = {2500, 2300, 999, 999, 2500, 2500}; // �°��ӷŵ������궨���ֵ,�ᱻ����ˢ��
sint16 ad_min[6] = {120, 120, 999, 999, 120, 120}; // �°��Ӿ���Ҫ�궨��Сֵ,�ᱻ����ˢ��

uint8 CircleNumber = 1;    // �뻷������0������Ĭ��1�� ;���ĸ���һ���ڱ���ǰ��������ʱ��֪����
uint8 TangentPoint = 1;    // �е��ж�   0�е������Ĭ��1�����뻷����ȡ����Ϊ�뻷׼��
uint8 EnterCircle = 0;     // �������  Ĭ�� 0���ɽ�����1���Խ���
uint8 OutCircle = 0;       // �������   Ĭ��0���ɳ�����1���Գ���
uint8 LeftRightCircle = 0; // ��໷�����Ҳ໷ Ĭ��0ԭʼ��1�󻷣�2�һ�

sint32 TangentPointpulse = 0; // ���������ʱ����1
sint32 EnterCirclePulse = 0;  // ���������ʱ����2
sint32 OutCirclePulse = 0;    // ���������ʱ����3
sint32 EnterCircleOKPulse = 0;// ���������ʱ����4
sint16 LowSpeed = 0;          // Բ��/ʮ�ֿڼ���

uint16 MagneticField = 0;     // �ų�ǿ��    magnetic field intensity,�ж�Բ���Ƿ����

sint16 OffsetDelta = 0;

/*************************************************************************
 *  �������ƣ�void InductorInit (void)
 *  ����˵�����ĸ����ADC��ʼ��������
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��11��18��
 *  ��    ע��
 *************************************************************************/
void InductorInit (void)
{
    ADC_InitConfig(ADC0, 100000); // ��ʼ��,������100kHz
    ADC_InitConfig(ADC1, 100000); // ��ʼ��
    ADC_InitConfig(ADC2, 100000); // ��ʼ��
    ADC_InitConfig(ADC3, 100000); // ��ʼ��
}
/*************************************************************************
 *  �������ƣ�void InductorNormal(void)
 *  ����˵�����ɼ���е�ѹ����һ����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��11��18��
 *  ��    ע��   ע��Ҫ�ȱ궨�˷ŵķŴ����������ĸ�һ�»��߽ӽ�
 *************************************************************************/
void InductorNormal (void)
{
    LnowADC[0] = ADC_Read(ADC0);  // ����1����У��������н�Լ30�ȣ��ɼ�������е�ADֵ
    LnowADC[1] = ADC_Read(ADC1);  // ����2����У���ֱ������
    LnowADC[4] = ADC_Read(ADC2);  // �Ҳ��2����У���ֱ������
    LnowADC[5] = ADC_Read(ADC3);  // �Ҳ��1����У��������н�Լ30��

    if (LnowADC[0] < ad_min[0])
        ad_min[0] = LnowADC[0];     // ˢ����Сֵ
    else if (LnowADC[0] > ad_max[0])
        ad_max[0] = LnowADC[0];     // ˢ�����ֵ
    if (LnowADC[1] < ad_min[1])
        ad_min[1] = LnowADC[1];
    else if (LnowADC[1] > ad_max[1])
        ad_max[1] = LnowADC[1];
    if (LnowADC[4] < ad_min[4])
        ad_min[4] = LnowADC[4];
    else if (LnowADC[4] > ad_max[4])
        ad_max[4] = LnowADC[4];
    if (LnowADC[5] < ad_min[5])
        ad_min[5] = LnowADC[5];
    else if (LnowADC[5] > ad_max[5])
        ad_max[5] = LnowADC[5];

    Lleft1 = (LnowADC[0] - ad_min[0]) * 100 / (ad_max[0] - ad_min[0]);     // ��ƫ������һ����0--100����
    Lleft2 = (LnowADC[1] - ad_min[1]) * 100 / (ad_max[1] - ad_min[1]);
    Lright2 = (LnowADC[4] - ad_min[4]) * 100 / (ad_max[4] - ad_min[4]);
    Lright1 = (LnowADC[5] - ad_min[5]) * 100 / (ad_max[5] - ad_min[5]);

    MagneticField = Lleft1 + Lleft2 + Lright2 + Lright1;// �ų�����ǿ��

    if(Lleft2 + Lright2<40)      // ����ֵ��С��20����Ÿ�Զ������������ֱ����ͼ���
    {
        if (Lleft1 + Lleft2 > Lright2 + Lright1)
        {
            TempAngle = Servo_Left_Max;
        }
        else
        {
            LastAngle =TempAngle = Servo_Right_Min;
        }
        LastAngle =TempAngle;// ������Ч����������ƫ�Ʒ���
    }
    else if ((Lleft2 > 18) && (Lright2 > 18))   // С���������������м�
    {
        TempAngle = Servo_Center_Mid + (Lleft2 - Lright2) * 2; //  ����ƫ������ֵС���ȴ�ǣ���ֱֹ��ҡ��
    }
    else                                        //  С���������������������һ��һС����Ҫ�ϴ�̶ȿ���ת��
    {
        TempAngle = Servo_Center_Mid + (Lleft2 - Lright2) * 7; //  һ�����ת����ƣ���ֵԽ��ת��Խ��
    }
}

/*************************************************************************
 *  �������ƣ�void CircleDetect void
 *  ����˵����ʶ�𲢽���Բ���ĸ�����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��11��18��
 *  ��    ע��
 *************************************************************************/
void CircleDetect (void)
{
    if (CircleNumber) // �뻷������0������Ĭ��2��
    {
        // �����е������ж�
        if (MagneticField > 220) // ֱ�����룬��ֵ����̫�����׶�ʧ�е�
        {
            if (TangentPoint)
            {
                TangentPointpulse = RAllPulse; // ��ȡ��ǰ������ֵ
                TangentPoint = 0;             // ��ֹ�ٴζ�ȡ��ǰ������ֵ
            }
            else if (LeftRightCircle == 0)
            {                                 // ���ǰ������Ϊ220���˴�Ӧ�ô�������������һЩ��̫����߹��е���Ҳ���У�
                if (RAllPulse > TangentPointpulse + 3500)   // �����е����ǰ��3000���壬��Լ50cm��
                {
                    EnterCircle = 1;      // ͨ���е����򣬿����뻷
                }
            }
        }

        // ��Լ1.2���������Ч������Ҫ����ʶ���е�
        if ((LeftRightCircle == 0) && (RAllPulse > TangentPointpulse + 8000)) // Լ1.2���������Ч
        {
            EnterCircle = 0;   // Լ1.2���������Ч
            TangentPoint = 1;  // ����ʶ���е�
        }
        if ((EnterCircle) && (MagneticField > 260)) // Լ1.2�����ٴη���ǿ�ų����뻷
        {
            LowSpeed = 500;    // ����
            // ������뻷����
            if (Lleft1 + Lleft2 > Lright2 + Lright1)     // ����뻷���������У�����
            {
                LeftRightCircle = 1;  // ��໷Ϊ1
                EnterCircle = 0;      // �뻷���ֹ�ٴ��뻷
                EnterCirclePulse = RAllPulse;
                ServoCtrl(Servo_Left_Max);     // ���PWM�����ת������ǿ���
                while (RAllPulse < EnterCirclePulse + 2800)
                {
                    delayms(1);       // ���Ƕ�ǰ��1200���壬Լ20cm������512�����������1��5790������
                }
            }
            else // �Ҳ��뻷����
            {
                LeftRightCircle = 2;  // �Ҳ໷Ϊ2
                EnterCircle = 0;  // �뻷���ֹ�ٴ��뻷
                EnterCirclePulse = RAllPulse;
                ServoCtrl(Servo_Right_Min);        // ���PWM�����ת������ǿ���
                while (RAllPulse < EnterCirclePulse + 1800) // �õ����Ҳ�ı�������ʵ���ߵľ����һ���
                {
                    delayms(1);   // ���Ƕ�ǰ��1200���壬Լ20cm������512�����������1��5790������
                }
            }
            EnterCircleOKPulse = RAllPulse; // ������
        }
        // ��������
        if ((LeftRightCircle > 0) && (RAllPulse > EnterCircleOKPulse + 3000))
        {
            EnterCircleOKPulse = 10000000; //��ֹ�ٴγ���ʹ��
            OutCircle = 1;  // ��������Գ���
        }
        if ((OutCircle) && (MagneticField > 220)) // �뻷��־Ϊ������뻷
        {
            LowSpeed = 500;        // ����
            // ����������
            if (LeftRightCircle == 1)   //����뻷
            {
                //OutCircle = 0;    // �뻷���ֹ�ٴ��뻷
                OutCirclePulse = RAllPulse;
                ServoCtrl(Servo_Left_Max);     // ���PWM�����ת������ǿ���
                while (RAllPulse < OutCirclePulse + 2800)
                {
                    delayms(1); // ���Ƕ�ǰ��1200���壬Լ20cm������512�����������1��5790������
                }
                OutCirclePulse = RAllPulse;
                ServoCtrl(Servo_Center_Mid-Servo_Delta*2/3);     // ���PWM�����������
                while (RAllPulse < OutCirclePulse + 700)
                {
                    delayms(1);         // ���Ƕ�ǰ��600���壬Լ10cm������512�����������1��5790������
                }
                CircleNumber--;         // ������
                TangentPoint = 1;       // �е��ж�   0�е������Ĭ��1�����뻷����ȡ����Ϊ�뻷׼��
                EnterCircle = 0;        // �������  Ĭ�� 0���ɽ�����1���Խ���
                OutCircle = 0;          // �������   Ĭ��0���ɳ�����1���Գ���
                LeftRightCircle = 0;    // ��໷�����Ҳ໷ Ĭ��0ԭʼ��1�󻷣�2�һ�
                LowSpeed = 0;           // �ָ��ٶ�
                Reed_Init();            // �ɻɹ�GPIO���жϳ�ʼ������,Ϊͣ�������׼��
            }
            // �Ҳ��������
            else if (LeftRightCircle == 2)  //�ұ��뻷
            {
                //OutCircle = 0;     // �뻷���ֹ�ٴ��뻷
                OutCirclePulse = RAllPulse;
                ServoCtrl(Servo_Right_Min);     // ���PWM�����ת������ǿ���
                while (RAllPulse < OutCirclePulse + 2500)
                {
                    delayms(1);   // ���Ƕ�ǰ��1200���壬Լ20cm������512�����������1��5790������
                }
                OutCirclePulse = RAllPulse;
                ServoCtrl(Servo_Center_Mid+Servo_Delta*2/3);     // ���PWM�����������
                while (RAllPulse < OutCirclePulse + 1400)
                {
                    delayms(1);         // ���Ƕ�ǰ��600���壬Լ10cm������512�����������1��5790������
                }
                CircleNumber--;         // ������
                TangentPoint = 1;       // �е��ж�   0�е������Ĭ��1�����뻷����ȡ����Ϊ�뻷׼��
                EnterCircle = 0;        // �������  Ĭ�� 0���ɽ�����1���Խ���
                OutCircle = 0;          // �������   Ĭ��0���ɳ�����1���Գ���
                LeftRightCircle = 0;    // ��໷�����Ҳ໷ Ĭ��0ԭʼ��1�󻷣�2�һ�
                LowSpeed = 0;           // �ָ��ٶ�
                Reed_Init();            // �ɻɹ�GPIO���жϳ�ʼ������,Ϊͣ�������׼��
            }
        }
    }
}

/*************************************************************************
 *  �������ƣ�void TFT_Show_EleMag_Info(void)
 *  ����˵������ʾ����������Ϣ
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��11��18��
 *  ��    ע��
 *************************************************************************/
void TFT_Show_EleMag_Info(void)
{
    char txt[16] = "X:";

    sint16 mps = 0, dmm = 0;    // �ٶȣ�m/s,������ֵ
    sint16 pulse100 = 0;


    dmm = (sint16) (RAllPulse * 100 / 579);           // ����512�����������1��5790�����壬��ֵ̫�󣬳���100
    pulse100 = (sint16) (RAllPulse / 100);
    sprintf(txt, "AP:%05d00", pulse100);              //
    TFTSPI_P8X16Str(3, 1, txt, u16RED, u16BLACK);     // ��ʾ����ƫ�����

    NowTime = (STM_GetNowUs(STM0) - NowTime) / 1000;  // ��ȡSTM0 ��ǰʱ�䣬�õ�����
    mps = (sint16) (dmm / (NowTime / 1000));          // �����ٶ�mm/s
    // ������Ϣ
    sprintf(txt, "%04d %04d %04d ", TempAngle, ECPULSE1, ECPULSE2);   // ��ʾ����Ƕ���ֵ�����ռ�ձ���ֵ����������ֵ
    TFTSPI_P8X16Str(1, 0, txt, u16WHITE, u16BLACK);      // �ַ�����ʾ
    //��ʾ����й�һ�����ƫ����  ��ǰ����е�ѹֵ ����п�������ʷ��Сֵ ����п�������ʷ���ֵ
    sprintf(txt, "0:%04d %04d %04d ", Lleft1, LnowADC[0], ad_max[0]);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
    sprintf(txt, "1:%04d %04d %04d ", Lleft2, LnowADC[1], ad_max[1]);
    TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
    sprintf(txt, "2:%04d %04d %04d ", Lright2, LnowADC[4], ad_max[4]);
    TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
    sprintf(txt, "3:%04d %04d %04d ", Lright1, LnowADC[5], ad_max[5]);
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);

    sprintf(txt, "Ring num: %d ", CircleNumber);
    TFTSPI_P8X16Str(2, 6, txt, u16GREEN, u16BLACK);
    sprintf(txt, "M:%03d Q:%d J:%d ", MagneticField, TangentPoint, EnterCircle);
    TFTSPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);
    if (LeftRightCircle == 1)
        TFTSPI_P8X16Str(0, 8, "Left Ring ", u16WHITE, u16BLACK);
    else if (LeftRightCircle == 2)
        TFTSPI_P8X16Str(0, 8, "Right Ring", u16WHITE, u16BLACK);
    else
        TFTSPI_P8X16Str(0, 8, "No Ring   ", u16WHITE, u16BLACK);


}
/*************************************************************************
 *  �������ƣ�void ElectroMagneticCar(void)
 *  ����˵������ų�˫������ٿ���
 -->1.�����㷨���򵥵ķֶα��������㷨����ѧ��ʾ�����㷨��
 ---2.�����㷨��PID����Ӧ�ÿ����㷨����ѧ��ʾ�����㷨��
 ---3.�߶��㷨���ݶ��Ľ�����ȺЭͬ�����㷨��
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��10��28��
 *  ��    ע������2�����
 *************************************************************************/
void ElectroMagneticCar (void)
{
    sint16 bat=0;

    CircleNumber = 1;   // �뻷������0������Ĭ��1��
    TangentPoint = 1;   // �е��ж�   0�е������Ĭ��1�����뻷����ȡ����Ϊ�뻷׼��
    EnterCircle = 0;    // �������  Ĭ�� 0���ɽ�����1���Խ���
    OutCircle = 0;      // �������   Ĭ��0���ɳ�����1���Գ���
    LeftRightCircle = 0;// ��໷�����Ҳ໷ Ĭ��0ԭʼ��1�󻷣�2�һ�
    LowSpeed = 0;       // �ٶȲ�

    // �м�CPU0,CPU1...������ͬʱ������Ļ��ʾ�������ͻ����ʾ
    mutexCpu0TFTIsOk=0;             // CPU1�� 0ռ��/1�ͷ� TFT
    CircleNumber = SetCircleNum();  // ������Ҫ����Բ���ĸ�����

    // ��������Ҫ���ó���⣬�����ǹ̶�ִ�У�
    // �������Ҫ�ɻɹܺ��ⲿ�ж����ʵ��
    // ���������У��ɻɹ���ͨ��Բ������������������ܴ����Ŀ�����
    OutInGarage(OUT_GARAGE,ReadOutInGarageMode()); // ���Գ��⣬��������������⣬��֮�Ҳ�����
    //OutInGarage(IN_GARAGE,ReadOutInGarageMode());  // �������

    TFTSPI_CLS(u16BLACK);            // ����
    // �м�CPU0,CPU1...������ͬʱ������Ļ��ʾ�������ͻ����ʾ
    mutexCpu0TFTIsOk=1;             // CPU1�� 0ռ��/1�ͷ� TFT

    RAllPulse = 0;                  // ȫ�ֱ����������������
    NowTime = STM_GetNowUs(STM0);   // ��ȡSTM0 ��ǰʱ��

    while (1)
    {
        InductorNormal();           // �ɼ���е�ѹ������һ����
        if (MagneticField > 220)    // ֱ�����룬��ֵ����̫�����׶�ʧ�е�
        {
            LowSpeed = 500;         // ����
        }
        else if (MagneticField < 200)
        {
            LowSpeed = 0; // �ָ��ٶ�
        }
        CircleDetect();             // ʶ�𲢽���Բ���ĸ�����

        ServoDuty = TempAngle;
        ServoCtrl(ServoDuty);       // ���PWM�����ת������ǿ���
        //
        OffsetDelta = (Lleft2 - Lright2);  // ֱ��ƫ��
        bat=(0 * 11 / 25-750)*Kbat;
        MotorDuty1 = MtTargetDuty-bat + ECPULSE1 * Kencoder - OffsetDelta * Koffset - LowSpeed;  // �в��ٿ��ƣ���תƫ��Ϊ����������
        MotorDuty2 = MtTargetDuty-bat - ECPULSE2 * Kencoder + OffsetDelta * Koffset - LowSpeed;  // �в��ٿ��ƣ���תƫ��Ϊ�����Ҳ����

        if (MagneticField < 30)     // �ж��Ƿ�������
        {
            MotorCtrl(0, 0);        // �������ͣ��
            delayms(200);
        }
        else
        {
            MotorCtrl(MotorDuty1, MotorDuty2);   // ����˫�������
            // MotorCtrl(MtTargetDuty-TempAngle*8/5, MtTargetDuty+TempAngle*8/5);// ���ֳ����޶��
        }

        if(Game_Over)
        {
            OutInGarage(IN_GARAGE, ReadOutInGarageMode());
        }
    }
}

