/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��zyf/chiusir
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2020��4��10��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��Hightec4.9.3/Tasking6.3�����ϰ汾
��Target �� TC264DA
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
����iLLD_1_0_1_11_0�ײ����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <LQ_STEER_PID.h>
#include "LQ_ImageProcess.h"
#include <LQ_STEER_PID.h>
#include <Platform_Types.h>
#include <stdio.h>
#include "LQ_GPT12_ENC.h"
#include "../APP/LQ_CAMERA.h"
#include "../APP/LQ_GPIO_KEY.h"
#include "../APP/LQ_GPIO_LED.h"
#include "../APP/LQ_TFT18.h"
#include "../Driver/LQ_ADC.h"
#include "../Driver/LQ_CCU6.h"
#include "../Driver/LQ_STM.h"
#include "LQ_Inductor.h"
#include "LQ_MotorServo.h"
#include "LQ_PID.h"
#include "LQ_UART.h"
#include "LQ_ADC.h"
#include "LQ_GPIO_Infrared.h"
#include "LQ_GPIO_BUZZ.h"
#include "LQ_IIC_Gyro.h"
#include "LQ_MPU6050_DMP.h"
#include "stdlib.h"
#include "LQ_CAMERA.h"
#include "TFT_Key_control.h"

/**  @brief    ת�����  */
float g_sSteeringError = 0;
float last_g_sSteeringError=0;
/**  @brief    ������  */
extern int ROAD_MAIN_ROW;

/**  @brief    ʹ����ʼ��  */
#define ROAD_START_ROW     80

/**  @brief    ʹ�ý�����  */
#define ROAD_END_ROW       10

/**  @brief    ��·��־λ  */
uint8_t OpeningRoad_Flag = 0;        //��·��־λ 0�����Ƕ�· 1����·

/*   @brief    �ϰ����־λ*/
uint8_t Barrier_Flag = 1 ;           //�ϰ����־λ 1:���ϰ��� ���������ϰ���

/**  @brief    ������־λ  */
uint8_t R_CircleFlag = 0;           //�һ�����־λ
uint8_t L_CircleFlag = 0;           //�󻷵���־λ


/** @brief  ʮ��·�ڱ�־λ  */
uint8_t Crossroad_Flag = 0;         //ʮ��·�ڱ�־λ 0:����ʮ��·�� 2-3:��ʮ��·��


/** @brief  �����־λ  */
uint8_t Carbarn_Flag = 0;           //�����־λ 0:���ǳ��� 1:�ǳ���
uint8_t Depart_Flag = 0;            //�����־λ 0�������� 1�������  2���ҳ���

/**  @brief    ���ߴ���  */
uint8_t Left_misi = 0,Right_misi = 0 ,Up_misi = 0;

uint8_t miss_Flag = 0;//���߱�־λ 1�� 2�Ҷ�

/** @brief ʮ��·�ڶϵ�����  */
int leftup[2] ;
int leftup_1[2];
int rightup[2] ;
int rightup_1[2];

uint8_t ImageSide[LCDH][2];         //���ұ�������
uint8_t ImageSide_last[LCDH][2];    //����ͼƬ���ұ�������
uint8_t UpdowmSide[2][LCDW];        //���±�������
uint8_t Roadwide[LCDH];             //�������
uint8_t Servo_P = 11;
extern unsigned char motor_flag;    //�����ͣ��־λ
extern sint16 OUT_PWM;              //ת�����PWM

extern unsigned char MAX[24];
extern int min_tag;
extern uint8_t open_flag;
extern uint8_t Obstacle_flag;
extern uint8_t slope_flag;
extern uint8_t Park_flag;
//���߹յ��־λ
uint8_t LeftFlag1 = 0;
uint8_t LeftFlag2 = 0;
uint8_t RightFlag1 = 0;
uint8_t RightFlag2 = 0;

extern int X_minL,X_minR,X_min;
extern int B_lk;
/*!
  * @brief    �����ֵ
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2022/12/2 ������
  */
float my_abs(float x)
{
    if(x >= 0) return x;
    else
        return -x;
}
/*!
  * @brief    �����ұ���
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/28 ������
  */
void TFTSPI_BinRoadSide(uint8_t imageOut[LCDH][2])
{
    uint8_t i = 0;
//    int min=0;
//    if(X_minR<X_minL)
//    {
//        min=X_minR;
//    }
//    else
//    {
//        min=X_minL;
//    }
    for(i = 115; i >X_minR; i--)
    {
        //TFTSPI_Draw_Dot(imageOut[i][0], i, u16GREEN);//����������������ɫ
        TFTSPI_Draw_Dot((imageOut[i][0]+imageOut[i][1])/2, i, u16PURPLE);
        //TFTSPI_Draw_Dot(imageOut[i][1], i, u16RED);
    }
}

/*!
  * @brief    �����±���
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/28 ������
  */
void TFTSPI_BinRoad_UpdownSide(uint8_t imageOut[2][LCDW])
{
    uint8_t i = 0;

    for(i = 0; i < 120; i++)
    {
        TFTSPI_Draw_Dot(i, imageOut[0][i], u16YELLOW);
        //TFTSPI_Draw_Dot(i, imageOut[1][i], u16RED);
    }
}

/*!
  * @brief    �ж��ϱ����Ƿ񵥵�
  * @param    X1 :��ʼX��
  * @param    X2 :��ֹX��              X1 < X2
  * @param    imageIn �� ��������
  *
  * @return   0��������or���� 1������������ 2�������ݼ�
  *
  * @note
  *
  * @see
  *
  * @date     2021/11/30 ���ڶ�
  */
uint8_t RoadUpSide_Mono(uint8_t X1, uint8_t X2, uint8_t imageIn[2][LCDW])
{
    uint8_t i = 0, num = 0;

    for(i = X1; i < X2-1; i++)
    {
        if(imageIn[0][i] >= imageIn[0][i+1])
            num++;
        else
            num = 0;
        if (num >= (X2-X1)*4/5)
            return 1;
    }
    for(i = X1; i < X2-1; i++)
    {
        if(imageIn[0][i] <= imageIn[0][i+1])
            num++;
        else
            num = 0;
        if (num >= (X2-X1)*4/5)
            return 2;
    }
    return 0;
}
/*!
  * @brief    �ж����ұ����Ƿ񵥵�
  * @param
  * @param
  * @param    imageIn �� ��������    Flag 0:����� 1�ұ���
  *
  * @return   0�������� 1������
  *
  * @note
  *
  * @see
  *
  * @date     2021/11/30 ���ڶ�
  */
uint8_t RoadImageSide_Mono(uint8_t imageSide[LCDH][2],uint8_t Flag)
{
    uint8_t i = 0;
    uint8_t State;
    State = 0;
        /* ������Ƿ񵥵� */
        if(Flag == 0)
        {
        for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
        {
            if((my_abs(imageSide[i][0] - imageSide[i+1][0]) <= 1) && (imageSide[i][0] > 10))
            {
                State++;
            }
        }
        }
        /* �ұ����Ƿ񵥵� */
        if(Flag == 1)
        {
        for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
        {
            if((my_abs(imageSide[i][1] - imageSide[i+1][1]) <= 1) && (imageSide[i][1] < 155))
            {
                State++;
            }
        }
        }
        if(State >(ROAD_START_ROW-ROAD_END_ROW)*0.7 )
            return 1;
        else
            return 0;
}
/*!
  * @brief    �������Ҷ��ߴ���
  * @param
  * @param
  * @param    imageIn �� ���ұ�������    Flag 0:����� 1�ұ���
  *
  * @return   ���ߴ���
  *
  * @note
  *
  * @see
  *
  * @date     2022/11/30 ������
  */
uint8_t line_miss(uint8_t imageIn[LCDH][2],uint8_t UpdowmSide[2][LCDW],uint8_t Flag)
{
    uint8_t i = 0;
    uint8_t misi_number;
    misi_number = 0;
    if(Flag == 0)
    {
        for(i = 0;i < LCDH; i++)
        {
            if(imageIn[i][0] < 5) misi_number++;
        }
    }
    if(Flag == 1)
    {
        for(i = 0;i < LCDH; i++)
        {
            if(imageIn[i][1] >155) misi_number++;
        }
    }
    if(Flag == 2)
    {
        for(i = 0;i <= LCDW;i++)
        {
            if(UpdowmSide[0][i] > 40)misi_number++;
        }
    }
    return misi_number;
}

uint8_t leftState = 0, rightState = 0;
/*!
  * @brief    �ж��Ƿ���ֱ��
  *
  * @param    image �� ���ұ���
  *
  * @return   0������ֱ���� 1��ֱ��
  *
  * @note     ˼·�����߱��߶�����
  *
  * @see
  *
  * @date     2022/11/29 ���ڶ�
  */
uint8_t RoadIsStraight(uint8_t imageSide[LCDH][2])
{
    uint8_t i = 0;
    leftState = 0;
    rightState = 0;
    /* ������Ƿ񵥵� */
    for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
    {
        if((imageSide[i][0] >= imageSide[i+1][0]) && (imageSide[i][0] > 10))
        {
            leftState++;
        }
    }
    /* �ұ����Ƿ񵥵� */
    for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
    {
        if((imageSide[i][1] <= imageSide[i+1][1]) && (imageSide[i][1] < 155))
        {
            rightState++;
        }
    }
    if(leftState >40 && rightState >45)
    {
        return 1;
    }
    return 0;
}
/*!
  * @brief    ���ߴ���
  *
  * @param    imageSide  : ����
  * @param    status     : 1������߲���   2���ұ��߲���
  * @param    startX     : ��ʼ�� ����
  * @param    startY     : ��ʼ�� ����
  * @param    endX       : ������ ����
  * @param    endY       : ������ ����
  *
  * @return
  *
  * @note     endY һ��Ҫ���� startY
  *
  * @see
  *
  * @date     2022/12/13
  */
void ImageAddingLine(uint8_t imageSide[LCDH][2], uint8_t status, uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY)
{
    int i = 0;
    /* ֱ�� x = ky + b*/
    float k = 0.0f, b = 0.0f;
    switch(status)
    {
      case 1://����
        {
            k = (float)((float)endX - (float)startX) / (float)((float)endY - (float)startY);
            b = (float)startX - (float)startY * k;

            for(i = startY; i < endY; i++)
            {
                imageSide[i][0] = (uint8_t)(k * i + b);
            }
            break;
        }
      case 2://�Ҳ���
        {
            k = (float)((float)endX - (float)startX) / (float)((float)endY - (float)startY);
            b = (float)startX - (float)startY * k;

            for(i = startY; i < endY; i++)
            {
                imageSide[i][1] = (uint8_t)(k * i + b);
            }
            break;
        }
    }
}



/*!
  * @brief    �ж��Ƿ��ǰ�����
  *
  * @param    image �� ��ֵͼ����Ϣ
  *
  * @return   0�����ǣ� 1����
  *
  * @note     ˼·��
  *
  * @see
  *
  * @date     2020/6/23 ���ڶ�
  */
uint8_t RoadIsZebra(uint8_t image[LCDH][LCDW], uint8_t *flag)
{
    int i = 0, j = 0;
    int count = 0;
    for(i = ROAD_MAIN_ROW - 30; i >  ROAD_MAIN_ROW + 30; i++)
    {
        for(j = 1; j < LCDW; j++)
        {
            if(image[i][j] == 1 && image[i][j-1] == 0)
            {
                count ++;
            }
        }
        if(count > 5)
        {
            *flag = 1;
            return 1;
        }
    }
    return 0;
}

/*!
  * @brief    �ж����·
  *
  * @param    Upimage[2][LCDW]���±�������    RoadwideInput[LCDH]�������  Flag ��־λ
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2022/11/29 ���ڶ�
  */
void InputOpening(uint8_t Upimage[2][LCDW] ,uint8_t RoadwideInput[LCDH],uint8_t *Flag)
{
    if((Upimage[0][100] < Upimage[0][120])&&(Upimage[0][60]<Upimage[0][40])&&Upimage[0][100]>25&&Upimage[0][60]>25 && motor_flag == 1 && Depart_Flag == 0)
    {
        *Flag = 1;
    }
}
/*!
  * @brief    �жϳ���·
  *
  * @param  imageSideinput[LCDH][2]���ұ�������   RoadwideInput[LCDH]������� Flag��־λ
  *
  * @return
  *
  * @note       ���ǳ���·��תΪ����ͷ�ģ����ǵȳ����ȶ������������Ƚ�ֱ��ʱ���л�����ͷ
  *
  * @see
  *
  * @date     2022/11/29 ���ڶ�
  */
void OutOpening(uint8_t Upimage[2][LCDW], uint8_t imageSideinput[LCDH][2],uint8_t RoadwideInput[LCDH],uint8_t *Flag)
{
    uint8_t flag = 0;
    if(RoadwideInput[40] > RoadwideInput[35] && RoadwideInput[35] > RoadwideInput[30] && RoadwideInput[30] > RoadwideInput[20]
       && RoadwideInput[20] > RoadwideInput[15] && RoadwideInput[45] >100 && RoadwideInput[45] <= 159 )//&& RoadwideInput[15] > 25)
    {
        flag = 1;
    }
    if(flag == 1 && imageSideinput[40][0] > 5 && imageSideinput[40][1] < 159 && imageSideinput[30][0] > 5 && imageSideinput[30][1] < 158 &&
            imageSideinput[110][0] < 5 && imageSideinput[110][1] > 155 && imageSideinput[105][0] < 5 && imageSideinput[105][1] > 155)
    {
        *Flag = 0;
    }
}
/*!
  * @brief    ��·������
  *
  * @param     Upimage[2][LCDW]���±�������   imageSideinput[LCDH][2]���ұ�������   RoadwideInput[LCDH]������� Flag ��־λ
  *
  * @return     ��
  *
  * @note
  *
  * @see
  *
  * @date     2022/11/30 ������
  */
void OpeningProcess(uint8_t Upimage[2][LCDW],uint8_t imageSideinput[LCDH][2],uint8_t RoadwideInput[LCDH],uint8_t *Flag)
{
    if(L_CircleFlag == 0 && R_CircleFlag == 0 && Barrier_Flag == 1)      //��ֹ����
    {
       switch(*Flag)
       {
       case 0:InputOpening(Upimage,RoadwideInput,&*Flag);break;     //���·
       case 1:OutOpening(Upimage,imageSideinput,RoadwideInput,&*Flag);break;        //����·
       default:break;
       }
    }
}
/*!
  * @brief    �µ�������
  *
  * @param     Upimage[2][LCDW]���±�������   imageSideinput[LCDH][2]���ұ�������   RoadwideInput[LCDH]������� Flag ��־λ
  *
  * @return     ��
  *
  * @note
  *
  * @see    ʶ���µ���ʱ��ת��Ϊ���ѭ���������������Ӳ�����ֲ�ת�����
  *
  * @date     2022/11/30 ������
  */
//���뻷��ǰ�Ĳ���
float P_L = 0,P_R = 0;
sint16 V_L = 0, V_R = 0;
/*!
  * @brief    �һ���������--���Բ��������ֹ���������кͳ�Բ��ʱ��һ���ط��������ʱ
  *
  * @param     Upimage[2][LCDW]���±�������   imageSideinput[LCDH][2]���ұ�������   RoadwideInput[LCDH]������� Flag �һ�����־λ
  *
  * @return     ��
  *
  * @note
  *
  * @see
  *
  * @date     2022/11/30 ������
  */
void R_CircleProcess(uint8_t imageSideinput[LCDH][2],uint8_t RoadwideInput[LCDH],uint8_t *Flag)
{
    uint8_t flag = 0;
    if(RoadwideInput[50] > RoadwideInput[40] && RoadwideInput[40] > RoadwideInput[30] && RoadwideInput[30] > RoadwideInput[20]
        && RoadwideInput[20] > RoadwideInput[15] && RoadwideInput[50] >110 && RoadwideInput[50] < 159 && RoadwideInput[15] > 35)
    {
         flag = 1;
    }
    if(*Flag >= 1 && flag == 1 && imageSideinput[50][0] > 5 && imageSideinput[50][1] < 159 && imageSideinput[40][0] > 5 && imageSideinput[40][1] < 158 &&
            imageSideinput[110][0] < 5 && imageSideinput[110][1] > 155 && imageSideinput[105][0] < 5 && imageSideinput[105][1] > 155)
    {
         *Flag = 0;
         Angle_kp = P_L;
    }
}
extern int ADC_Value[4];
uint8_t time = 90;
uint8_t K = 18;
uint32_t R2_Value = 0;
uint8_t time2 = 40;
/*************************************************************************
*  �������ƣ�void Round_Process(uint8_t *L_Flag,uint8_t *R_Flag,uint8_t *M_Flag,uint8_t Roadwide[120],uint8_t UpImageSide[2][160],uint8_t OpeningRoad_Flag,uint8_t Barrier_Flag)
*  ����˵����Բ�����������õ�ż��Բ������⵽��������ͷ�������ܺͳ�Բ��
*  ����˵����*L_Flag:��Բ����־λ      *R_Flag:��Բ����־λ     *M_Flag:�����ͣ��־λ
*  ����˵��:imageSideinput[LCDH][2]���ұ�������       Roadwide[LCDH] �������     UpImageSide[2][LCDW]���±�������      *miss_Flag:���߱�־λ
*  �������أ���
*  �޸�ʱ�䣺2022��11��29�����ڶ�
*  ��    ע��
*************************************************************************/
void Round_Process(uint8_t *L_Flag,uint8_t *R_Flag,uint8_t *M_Flag,uint8_t imageSide[LCDH][2],uint8_t Roadwide[LCDH],uint8_t UpImageSide[2][LCDW],uint8_t *miss_Flag)
{
    static uint8_t i =0;
    /*************************���ߵ�ʶ��*****************************************/
   if(ALL_AD < 8 && Barrier_Flag == 1 && Depart_Flag == 0 )
   {
       *M_Flag = 0;
   }
   else if(ALL_AD < 100 && OpeningRoad_Flag ==1)
   {
       if(left_V + left_X <= right_V + right_X)
       {
           *miss_Flag = 2;
       }
       else
       {
           *miss_Flag = 1;
       }
   }
   else if(ALL_AD > 150)
   {
       *miss_Flag = 0;
   }

    /*************************Բ��ʶ������*****************************************/
    switch(*R_Flag)
    {
        case 0:
        {
            i = 0;
            P_L =  Angle_kp;
            if(((ADC_Value[2] > 1500 && ADC_Value[3] > 1050 && ADC_Value[0] > 700 && ADC_Value[1] > 900 ) || (left_V > 80 && right_V < 30 && ALL_AD > 215))
                    && UpImageSide[0][80] < 10 && Roadwide[90] > 125 && OpeningRoad_Flag == 0 )
            {
                i = 2;
            }
            if(i == 2 && imageSide[50][1] > 157 && imageSide[55][1] > 157 && imageSide[60][1] > 157 && imageSide[65][1] > 157
                    && imageSide[70][1] > 157 && imageSide[75][1] > 157 && imageSide[80][1] > 157 && imageSide[85][1] > 157
                    && imageSide[90][1] > 157 && imageSide[95][1] > 157 && imageSide[100][1] > 157)
            {
                i = 0;
                *R_Flag = 1;
            }
            break;
        }
        case 1:
        {
            R2_Value = 0;
            ImageAddingLine(ImageSide, 1,90, 5,5, 110);
            for(i = 10;i <= 150; i++)
            {
                if(UpImageSide[0][i] > UpImageSide[0][i+2])
                {
                    R2_Value ++;
                }
            }
            if(R2_Value >= time)
            {
                R2_Value = 0;
                *R_Flag = 2;
                Angle_kp = 27;
            }
            break;
        }
        case 2:
        {
            R2_Value = 0;
            if(++i >= 10)
            {
                i = 20;
                for(i = 5;i < 155;i++)
                {
                    R2_Value = R2_Value + (UpImageSide[0][i] - UpImageSide[0][i+1]);
                }
                if(R2_Value < 10 && imageSide[110][0] < 5 &&  imageSide[100][0] < 5 && imageSide[90][0] < 5 && imageSide[80][0] < 5 && UpImageSide[0][10] < 60)
                {
                    R2_Value = 0;
                    *R_Flag = 3;
                    i = 0;
                }
            }
            break;
        }
        case 3:
        {
            ImageAddingLine(ImageSide, 1, 90, 5,5, 110);
//            if(imageSide[110][1] < 156 || imageSide[100][1] < 156 || imageSide[90][1] < 156 || imageSide[80][1] < 156 || imageSide[70][1] < 156)
            if(++i == 30)
            {
                i = 0;
                *R_Flag = 4;
            }
            break;
        }
        case 4:
        {
            ImageAddingLine(ImageSide, 2, 130, 5,155, 110);
            if(++i == 40)
            {
                i = 0;
                *R_Flag = 0;
                Angle_kp = P_L;
            }
        }
    }
}
/*!
  * @brief    �󻷵�������--���Բ��������ֹ���������кͳ�Բ������һ���ط��������ʱ
  *
  * @param     Upimage[2][LCDW]���±�������   imageSideinput[LCDH][2]���ұ�������   RoadwideInput[LCDH]������� Flag �һ�����־λ
  *
  * @return     ��
  *
  * @note
  *
  * @see
  *
  * @date     2022/12/2 ������
  */
void L_CircleProcess(uint8_t imageSideinput[LCDH][2],uint8_t RoadwideInput[LCDH],uint8_t *Flag)
{
    uint8_t flag = 0;
    if(RoadwideInput[50] > RoadwideInput[40] && RoadwideInput[40] > RoadwideInput[30] && RoadwideInput[30] > RoadwideInput[20]
        && RoadwideInput[20] > RoadwideInput[15] && RoadwideInput[50] >110 && RoadwideInput[50] < 140 && RoadwideInput[15] > 50)
    {
         flag = 1;
    }
    if(flag == 1 && imageSideinput[50][0] > 5 && imageSideinput[50][1] < 155 && imageSideinput[40][0] > 5 && imageSideinput[40][1] < 155)
    {
        Angle_kp = P_L;
        *Flag = 0;
    }
}
sint16 RAllPulse1 = 0 ,RAllPulse2 = 0;
/*!
  * @brief    �ϰ��ﴦ����
  *
  * @param    Flag �ϰ�����λ
  *
  * @return     ��
  *
  * @note
  *
  * @see    ִ��˳��1-0-2-3-4-5
  *
  * @date     2022/12/2 ������
  */
void BarrierProcess(uint8_t imageSideinput[LCDH][2],uint8_t *Flag)
{
    static uint8_t i = 0;
    switch(*Flag)
    {
        case 0:                 //���ֳ�ͨ�����������ͨ�����˴�ǿ��ͣ��
            i++;
            if(i > 10)           //ɲ��ʱ�䣨10*10ms��
            {
                i = 0;
                *Flag = 2;
            }
            g_sSteeringError = 0;
            MotorCtrl4w(0, 0,-3000, -3000);
            break;
        case 1:                 //ʶ���ϰ���
            if(OpeningRoad_Flag == 0 && R_CircleFlag == 0 &&  L_CircleFlag == 0 && motor_flag == 1)
            {
                if(Infrared_Read() == 0)
                {
                    i = 0;
                    *Flag = 0;
                    Target_Speed1 = 0;
                    Target_Speed2 = 0;
                }
            }
            break;
        case 2:             //��ʼת��
            Target_Speed1 = -20;
            Target_Speed2 = 20;
            g_sSteeringError = 0;
            if(Infrared_Read())//ת��ʶ�𲻵��ϰ���
            {
                *Flag = 3;
            }
            break;
        case 3:             //�ƹ��ϰ���
            Target_Speed1 = 40;
            Target_Speed2 = 40;
            g_sSteeringError = 0;
            RAllPulse1 += ECPULSE1;
            if(RAllPulse1 > 3300)//��������¼3300������
            {
                RAllPulse1 = 0;
                RAllPulse2 = 0;
                *Flag = 4;
            }
            break;
        case 4:             //��������
            Target_Speed1 = 20;
            Target_Speed2 = -20;
            g_sSteeringError = 0;
            RAllPulse1 += ECPULSE1;
            if(RAllPulse1 > 1100) //��������¼1100������
            {
               RAllPulse1 = 0;
               *Flag = 5;
            }
            break;
        case 5:         //ʻ������
            Target_Speed1 = 50;
            Target_Speed2 = 50;
            g_sSteeringError = 0;
            if(ALL_AD > 120)     //��⵽���˵����������
            {
                *Flag = 1;
            }
            break;
    }
}
/*!
 * @brief    Ѱ�Ҷϵ�
 *
 * @param    start_point �� ��ʼ��
 * @param    end_point   �� ������
 * @param    UpdowmSide  �� ��������
 * @param    *pointup[2] �� �ϵ���������
 *
 * @return   void
 *
 * @note
 *
 * @see         ��ʼ��С�ڽ�����
 *
 * @date     2022/12/13
 */
void X_Find_Point(uint8_t start_point,uint8_t end_point,uint8_t UpdowmSide[2][LCDW], uint8_t pointup[2])
{
    uint8_t i = 0;
    for(i = start_point;i <= end_point ;i++)
    {
        if((UpdowmSide[0][i-4]-UpdowmSide[0][i-2])>0&&(UpdowmSide[0][i-6]-UpdowmSide[0][i-4])>0&&(UpdowmSide[0][i-8]-UpdowmSide[0][i-6])>0
         //&&(UpdowmSide[0][i-3]-UpdowmSide[0][i-1])>0&&(UpdowmSide[0][i-7]-UpdowmSide[0][i-6])>0&&(UpdowmSide[0][i-8]-UpdowmSide[0][i-7])>0
         )
        {
//            open_flag=1;
            open_flag=2;
//            pointup[0] = i;
//            pointup[1] = UpdowmSide[0][i];
            break;
        }
//        else
//        {
//            open_flag=0;
////            pointup[0] = 0;
////            pointup[1] = 0;
//        }
    }
}

/*!
 * @brief    Ѱ�Ҷϵ�
 *
 * @param    start_point �� ��ʼ��
 * @param    end_point   �� ������
 * @param    UpdowmSide  �� ��������
 * @param    *pointup[2] �� �ϵ���������
 *
 * @return   void
 *
 * @note
 *
 * @see         ��ʼ��С�ڽ�����
 *
 * @date     2022/12/13
 */
void X_Find_Point_1(uint8_t start_point,uint8_t end_point,uint8_t UpdowmSide[2][LCDW], uint8_t pointup[2])
{
    uint8_t i = 0;
    for(i = start_point;i <= end_point ;i++)
    {
        if((UpdowmSide[0][i+4]-UpdowmSide[0][i+2])>0&&(UpdowmSide[0][i+6]-UpdowmSide[0][i+4])>0&&(UpdowmSide[0][i+8]-UpdowmSide[0][i+6])>0
         //&&(UpdowmSide[0][i+6]-UpdowmSide[0][i+5])>0&&(UpdowmSide[0][i+7]-UpdowmSide[0][i+6])>0&&(UpdowmSide[0][i+8]-UpdowmSide[0][i+7])>0
         //&&open_flag==2
         )
        {
            open_flag=3;
//            pointup[0] = i;
//            pointup[1] = UpdowmSide[0][i];
            break;
        }
        else
        {
            open_flag=0;
//            pointup[0] = 0;
//            pointup[1] = 0;
        }
    }
}

/*!
 * @brief    Ѱ�Ҷϵ�
 *
 * @param    start_point �� ��ʼ��
 * @param    end_point   �� ������
 * @param    UpdowmSide  �� ��������
 * @param    *pointup[2] �� �ϵ���������
 *
 * @return   void
 *
 * @note
 *
 * @see         ��ʼ��С�ڽ�����
 *
 * @date     2022/12/13
 */

void Y_Find_Point(uint8_t start_point,uint8_t end_point,uint8_t ImageSide[LCDH][2], uint8_t pointup[4])
{
//    uint8_t i = 0;
//    uint8_t point_num=0;
//    for(i = start_point;i <= end_point ;i--)
//    {
//        if(abs(ImageSide[i][0] - ImageSide[i-1][0] )>8 && abs(ImageSide[i][0] - ImageSide[i+1][0]) < 3 &&ImageSide[i][0] > ImageSide[i+3][0]&&abs(ImageSide[i][0] - ImageSide[i-2][0]) > 8)
//        {
////            left_point[0] = (uint8_t)ImageSide[i][0];//��ߵ�һ���ϵ�����
////            left_point[1] = (uint8_t)i;//��ߵ�һ���ϵ�����
//            //point_leftnum=1;
//            //break;
//        }
//        if(abs(ImageSide[i][1] - ImageSide[i-1][1] )>8 && abs(ImageSide[i][1] - ImageSide[i+1][1]) < 3 &&ImageSide[i][1] < ImageSide[i+3][1] &&abs(ImageSide[i][1] - ImageSide[i-2][1]) > 8)
//        {
////            right_point[0] = (uint8_t)ImageSide[i][0];//��ߵڶ����ϵ�����
////            right_point[1] = (uint8_t)i;//��ߵڶ����ϵ�����
//            //point_rightnum=1;
//        }
//
//        for(int i=0;i<4;i++)
//        {
//            printf("left_point[%d]=%d\n",i,(int)left_point[i]);
//        }
//
////        else
////        {
////            pointup[0] = 0;
////            pointup[1] = 0;
////            pointup[2] = 0;
////            pointup[3] = 0;
////        }
//    }
}

/*!
 * @brief    Ѱ�Ҷϵ�
 *
 * @param    start_point �� ��ʼ��
 * @param    end_point   �� ������
 * @param    UpdowmSide  �� ��������
 * @param    *pointup[2] �� �ϵ���������
 *
 * @return   void
 *
 * @note
 *
 * @see         ��ʼ��С�ڽ�����
 *
 * @date     2022/12/13
 */
void Y_Find_Point_1(uint8_t start_point,uint8_t end_point,uint8_t ImageSide[LCDH][2], uint8_t pointup[2])
{
    uint8_t i = 0;
    for(i = start_point;i <= end_point ;i++)
    {
        if(abs(ImageSide[i+2][1] - ImageSide[i-2][1] )> 5 && abs(ImageSide[i][1] - ImageSide[i+1][1]) < 3&&abs(ImageSide[i][1] - ImageSide[i+1][1]) > 0)
        {
            pointup[0] = ImageSide[i][1];
            pointup[1] = i;
            break;
        }
        else
        {
            pointup[0] = 0;
            pointup[1] = 0;
        }
    }
}

/*!
 * @brief    Ѱ�ҹյ�
 *
 * @param    start_point �� ��ʼ��
 * @param    end_point   �� ������
 * @param    ImageSide  ��  ���ұ�������
 * @param    *pointup[2] �� �ϵ���������
 *
 * @return   void
 *
 * @note
 *
 * @see         ��ʼ��С�ڽ�����
 *
 * @date     2022/12/13
 */
//����ߵ�һ���յ㣨��ʼ�������ָ����������
void Find_Point_Left_1(uint8_t start_point,uint8_t end_point,uint8_t ImageSide[LCDH][2], uint8_t pointup[2])
{
    uint8_t i = 0;
    for(i = start_point;i <= end_point ;i++)
    {
        if(abs(ImageSide[i+1][0] - ImageSide[i-1][0] )> 10 && abs(ImageSide[i][0] - ImageSide[i+1][0]) >10 )
        {
            pointup[0] = ImageSide[i][0];
            pointup[1] = i;
            LeftFlag1=1;
            break;
        }
        else
        {
            pointup[0] = 0;
            pointup[1] = 0;
            LeftFlag1=0;
        }
    }
}
//����ߵڶ����յ�
void Find_Point_Left_2(uint8_t start_point,uint8_t end_point,uint8_t ImageSide[LCDH][2], uint8_t pointup[2])
{
    uint8_t i = 0;
    for(i = start_point;i <= end_point ;i++)
    {
        if(abs(ImageSide[i+1][0] - ImageSide[i-1][0] )> 10 && abs(ImageSide[i][0] - ImageSide[i+1][0]) >10 )
        {
            pointup[0] = ImageSide[i][0];
            pointup[1] = i;
            LeftFlag2=1;
            break;
        }
        else
        {
            pointup[0] = 0;
            pointup[1] = 0;
            LeftFlag2=0;
        }
    }
}
//�ұ��ߵ�һ���յ�
void Find_Point_Right_1(uint8_t start_point,uint8_t end_point,uint8_t ImageSide[LCDH][2], uint8_t pointup[2])
{

    uint8_t i = 0;
    for(i = start_point;i <= end_point ;i++)
    {
        if(abs(ImageSide[i+1][1] - ImageSide[i-1][1] )> 10 && abs(ImageSide[i][1] - ImageSide[i+1][1]) >10 )
        {
            pointup[0] = ImageSide[i][1];
            pointup[1] = i;
            RightFlag1=1;
            break;
        }
        else
        {
            pointup[0] = 0;
            pointup[1] = 0;
            RightFlag1=0;
        }
    }
}

//�ұ��ߵڶ����յ�
void Find_Point_Right_2(uint8_t start_point,uint8_t end_point,uint8_t ImageSide[LCDH][2], uint8_t pointup[2])
{

    uint8_t i = 0;
    for(i = start_point;i <= end_point ;i++)
    {
        if(abs(ImageSide[i+1][1] - ImageSide[i-1][1] )> 10 && abs(ImageSide[i][1] - ImageSide[i+1][1]) >10 )
        {
            pointup[0] = ImageSide[i][1];
            pointup[1] = i;
            RightFlag2=1;
            break;
        }
        else
        {
            pointup[0] = 0;
            pointup[1] = 0;
            RightFlag2=0;
        }
    }
}


/*!
  * @brief    �ж��Ƿ���ʮ��
  *
  * @param    imageInput ��
  * @param    imageOut   ��
  * @param    lineIndex  ��
  *
  * @return
  *
  * @note
  *
  * @see     �ϱ��������δ�����������ȼ�С��������������������仯����Ȼȫ��Ϊ���ߣ�Ȼ���ٱ��������
  *
  * @date     2022/12/12
  */
void Crossroad_Find(uint8_t UpdowmSide [2][LCDW],uint8_t imageSide[LCDH][2], uint8_t Roadwide[LCDH],uint8_t * Flag)
{
//    uint8_t i,j,flag,flag_number;
//    flag = 0;
//    flag_number = 0;
//    X_Find_Point(10,70,UpdowmSide,leftup);
//    X_Find_Point(90,158,UpdowmSide,rightup);
//    for(i = 5 ;i < 100;i++)//
//    {
//        if((UpdowmSide [0][i] - UpdowmSide [0][i+4]) > 8 && UpdowmSide [0][i] < 80)
//        {
//           for(j = i;j < 155; j++)
//           {
//               if((UpdowmSide [0][j] - UpdowmSide [0][j+4]) < -8)
//               {
//                   flag = 1;
//                   break;
//               }
//           }
//        }
//        if(flag == 1)break;
//    }
//    for(i = 100;i >= 40;i--)
//    {
//        if(imageSide[i][0] < 3 && imageSide[i][1] > 157)//���ұ��ߴ������ת
//        {
//            flag_number++;
//        }
//        if(flag_number > 50 && flag == 0)//�ж�Ϊ��߱��ߴ������ת
//        {
//            flag =2;
//            break;
//        }
//        else if(flag_number > 50 && flag == 1)
//        {
//            flag =3;
//            break;
//        }
//    }
//    if(flag == 1 && motor_flag == 1)
//    {
//        *Flag = 1;
//    }
//    if(flag == 2 && motor_flag == 1)
//    {
//        *Flag = 2;
//        if(leftup[0] != 0 && rightup[0] != 0)
//        {
//            ImageAddingLine(imageSide,1, leftup[0], leftup[1], 5,110);
//            ImageAddingLine(imageSide,2, rightup[0], rightup[1], 155,110);
//        }
//        else
//        {
//            ImageAddingLine(imageSide,1, 80, 10, 5,110);
//            ImageAddingLine(imageSide,2, 80, 10, 155,110);
//        }
//
//    }
//    if(flag == 3 && motor_flag == 1)
//    {
//        *Flag = 3;
//        if(leftup[0] != 0 && rightup[0] != 0)
//        {
//            ImageAddingLine(imageSide,1, leftup[0], leftup[1], 5,110);
//            ImageAddingLine(imageSide,2, rightup[0], rightup[1], 155,110);
//        }
//        else
//        {
//            ImageAddingLine(imageSide,1, 80, 10, 5,110);
//            ImageAddingLine(imageSide,2, 80, 10, 155,110);
//        }
//
//    }
//    if(flag == 0)
//    {
//        *Flag = 0;
//    }

//    Find_Point_Left_1(0,40,imageSide,leftup);//���Ϲյ�
//    Find_Point_Left_2(40,80,imageSide,leftup_1);//���¹յ�
//    Find_Point_Right_1(0,40,imageSide,rightup);//���Ϲյ�
//    Find_Point_Right_2(40,80,imageSide,rightup_1);//���¹յ�
    if(LeftFlag1==1 && LeftFlag2==1 && RightFlag1==1 && RightFlag2==1)//����ĸ��յ㶼���ҵ�
    {
        *Flag=1;
//        TFTSPI_Draw_Dot(leftup[1],leftup[0],u16YELLOW);
//        TFTSPI_Draw_Dot(leftup_1[1],leftup_1[0],u16YELLOW);
//        TFTSPI_Draw_Dot(rightup[1],rightup[0],u16YELLOW);
//        TFTSPI_Draw_Dot(rightup_1[1],rightup_1[0],u16YELLOW);
//        ImageAddingLine(imageSide,1, leftup[0], leftup[1], 5,78);
//        ImageAddingLine(imageSide,2, rightup[0], rightup[1], 115,78);
        LeftFlag1=0;
        LeftFlag2=0;
        RightFlag1=0;
        RightFlag2=0;



    }
    else
    {
        *Flag=0;
    }


}


/*!
  * @brief    ��ȡ���±���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageSide  �� ��������
  * @param    status     �� 1���ϱ���  2���±���
  *
  * @return   void
  *
  * @note
  *
  * @see
  *
  * @date     2022/11/23
  */
void Roundabout_Get_UpDowmSide(uint8_t imageInput[LCDH][LCDW], uint8_t imageSide[2][LCDW], uint8_t status)
{
    uint8_t i = 0, j = 0;

    switch(status)
    {
      case 1:
        {
            /* ����ȷ���ϱ߽� */
            for(i = 159; i > 0; i--)
            {
                for(j = LCDH/2; j > 0; j--)
                {
                    if(!imageInput[j][i])
                    {
                        imageSide[0][i] = j;
                        break;
                    }
                }
            }
            break;
        }
      case 2:
        {
            /* ����ȷ���±߽� */
            for(i = 159; i > 0; i--)
            {
                for(j = LCDH/2; j < LCDH; j++)
                {
                    if(!imageInput[j][i])
                    {
                        imageSide[1][i] = j;
                        break;
                    }
                }
            }
            break;
        }
    }
}
/*!
  * @brief    �жϱ����Ƿ���ڻ���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  * @param    status     �� 1�������  2���ұ���
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/23 ���ڶ�
  */
uint8_t RoundaboutGetArc(uint8_t imageSide[LCDH][2], uint8_t status, uint8_t num,uint8_t* index)
{
    int i = 0;
    uint8_t inc = 0, dec = 0, n = 0;
    switch(status)
    {
      case 1:
        for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
        {
            if(imageSide[i][0] != 0 && imageSide[i+1][0] != 0)
            {
                if(imageSide[i][0] == imageSide[i+1][0]){
                    n++;
                    continue;
                }
                if(imageSide[i][0] > imageSide[i+1][0])
                {
                    inc++;
                    inc+=n;
                    n=0;
                }
                else
                {
                    dec++;
                    dec+=n;
                    n=0;
                }
                /* �л��� */
                if(inc > num && dec > num)
                {
                    *index = i + num;
                    return 1;
                }
            }
            else
            {
                inc = 0;
                dec = 0;n=0;
            }
        }
        break;

      case 2:
        for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
        {
            if(imageSide[i][1] != 159 && imageSide[i+1][1] != 159)
            {
                if(imageSide[i][1] == imageSide[i+1][1])
                {
                    n++;
                    continue;
                }
                if(imageSide[i][1] > imageSide[i+1][1])
                {
                    inc++;
                    inc+=n;
                    n = 0;
                }
                else
                {
                    dec++;
                    dec+=n;
                    n=0;
                }
                /* �л��� */
                if(inc > num && dec > num)
                {
                    *index = i + num;
                    return 1;
                }
            }
            else
            {
                inc = 0;
                dec = 0;n=0;
            }
        }
        break;
    }
    return 0;
}

/*!
  * @brief    �жϱ����Ƿ���ڻ���
  *
  * @param    SideInput �� �ϱ�������
  * @param    num       �� ������
  * @param    index     �� ��͵�
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2021/12/01 ������
  */
uint8_t UpSideErr(uint8_t SideInput[2][LCDW],uint8_t status ,uint8_t num, uint8_t * index)
{
    uint8_t dec = 0, inc = 0, i;
    //�����Ƿ���ͻ��
    switch(status)
    {
        case 1:
            for(i = 159-1; i > 0; i--)
                {
                  if(UpdowmSide[0][i] > 1 && UpdowmSide[0][i+1] > 1)
                  {
                      if(UpdowmSide[0][i] >= UpdowmSide[0][i+1])
                          inc++;
                      else
                          dec++;
                      /* �л��� */
                      if(inc > num && dec > num)
                      {
                          * index = i + num;
                          return 1;
                      }
                  }
                  else
                  {
                      inc = 0;
                      dec = 0;
                  }
                }
            break;
        //�±���
        case 2:
            for(i = 159-1; i > 0; i--)
                {
                  if(UpdowmSide[1][i] != 1 && UpdowmSide[1][i+1] != 1)
                  {
                      if(UpdowmSide[1][i] >= UpdowmSide[1][i+1])
                          inc++;
                      else
                          dec++;
                      /* �л��� */
                      if(inc > num && dec > num)
                      {
                          * index = i + num;
                          return 1;
                      }
                  }
                  else
                  {
                      inc = 0;
                      dec = 0;
                  }
                }
            break;
    }
    return 0;
}
/*!
  * @brief    ���⺯��
  * @param    Flag        : �����־λ
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2022/12/26
  */
void Depart(uint8_t *Flag)
{
    static volatile sint16 ECPULSE1_value = 0,ECPULSE2_value = 0;
    switch (*Flag)
    {
        case 1:
            g_sSteeringError = 0;
            Target_Speed1 = 20;
            Target_Speed2 = 40;
            if(motor_flag == 1)
            {
                ECPULSE2_value += ECPULSE2;
                if(ECPULSE2_value > 1500)
                {
                   ECPULSE2_value = 0;
                   *Flag = 0;
                }
            }
            break;
        case 2:
            g_sSteeringError = 0;
            Target_Speed1 = 40;
            Target_Speed2 = 20;
            ECPULSE1_value += ECPULSE1;
            if(ECPULSE1_value > 1500)
            {
                ECPULSE1_value = 0;
                *Flag = 0;
            }
            break;
    }
}
/*!
  * @brief    �����ʶ��
  *
  * @param    imageInput  �� ��ֵͼ��
  * @param    Flag        �� �����־λ
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2022/12/16
  */
void FindCarbarn(uint8_t imageInput[LCDH][LCDW],uint8_t * Flag)
{
    uint8_t i,Flag_number;
    Flag_number = 0;
    for(i = 5;i <= LCDW-5;i++)
    {
        if(imageInput[50][i] != imageInput[50][i+1])
        {
            Flag_number ++;
        }
    }
    if(Flag_number > 10)
    {
        *Flag = 1;
    }
}
/*!
  * @brief    ͣ���ߴ���
  *
  * @param    imageSide  �� ��������
  * @param    state      �� ͣ��״̬  1�����������   2���������Ҳ�
  * @param    speed      �� �ٶ�
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2022/12/26
  */
void ZebraProcess(uint8_t imageSide[LCDH][2], uint8_t state, uint8_t* motor)
{
    static uint16_t count = 0;
    count++;
    if(state == 1)
    {
        imageSide[ROAD_MAIN_ROW][0] = 0;
        imageSide[ROAD_MAIN_ROW][1] = LCDW/2;
    }
    else
    {
        imageSide[ROAD_MAIN_ROW][0] = LCDW/2;
        imageSide[ROAD_MAIN_ROW][1] = LCDW - 1;
    }
    if(count > 100)
    {
        * motor = 0;
        while(1);
    }
}
/*!
  * @brief    ͣ������
  * @param    imageInput  �� ��ֵͼ��
  * @param    imageSide   �� ��������
  * @param    motor_flag  : �����־λ
  * @param    Flag        : �����־λ
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2022/12/26
  */
void Carbarn(uint8_t imageInput[LCDH][LCDW],uint8_t imageSide[LCDH][2],uint8_t *motor_flag,uint8_t *Flag)
{
    switch(*Flag)
    {
        case 0:
            FindCarbarn(imageInput,&*Flag);              //����ʶ��
            break;
        case 1:
            ZebraProcess(imageSide,1, &*motor_flag);     //���복��
            break;
    }
}

/*!
  * @brief    ���������У���ȡ���ƫ��
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @see    �󼸸�ƫ��ֵ��Ȼ����ƽ��ֵ
  *
  * @date     2022/11/25
  */
volatile sint16 Error_high=188;
volatile sint16 high=1;
int16_t RoadGetSteeringError(uint8_t imageSide[LCDH][2], uint8_t lineIndex)
{
    int16_t sum = 0;
    if(lineIndex<X_min)
    {
        lineIndex=X_min+1;
        //regression(X_min,X_min+30);///б�ʼ���18  47
    }
//    else
//    {
//        regression(28,58);///б�ʼ���18  47
//    }
//    if(B_lk>19)B_lk=19;
//    if(B_lk<-19)B_lk=-19;
//

    sum =imageSide[lineIndex][0] + imageSide[lineIndex][1] - Error_high;
    //sum=(1.6*sum-0.3*B_lk);
//        sum = (imageSide[lineIndex][0] + imageSide[lineIndex][1] - Error_high) +
//              (imageSide[lineIndex + high][0] + imageSide[lineIndex +  high][1] - Error_high) +
//              (imageSide[lineIndex + 2*high][0] + imageSide[lineIndex +  2*high][1] - Error_high) +
//              (imageSide[lineIndex + 3*high][0] + imageSide[lineIndex + 3*high][1] - Error_high) +
//              (imageSide[lineIndex + 4*high][0] + imageSide[lineIndex + 4*high][1] - Error_high);
//
//         sum = sum / 5;

    return sum;
}
/*!
  * @brief    �ж��Ƿ���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  * @param    lineIndex  �� ��
  *
  * @return   0��û�ж���   1:��߶���  2���ұ߶���  3�� ���Ҷ�����   4������
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/24 ������
  */
uint8_t RoadIsNoSide(uint8_t imageInput[LCDH][LCDW], uint8_t imageOut[LCDH][2], uint8_t lineIndex)
{
    uint8_t state = 0;
    uint8_t i = 0;
    static uint8_t last = 78;
    imageOut[lineIndex][0] = 0;
    imageOut[lineIndex][1] = 159;
    /* �þ���С���ȽϽ����� �ж��Ƿ��� */
    for(i = last; i > 1; i--)
    {
        if(imageInput[lineIndex][i])
        {
            imageOut[lineIndex][0] = i;
            break;
        }
    }
    if(i == 1)
    {
        /* ��߽綪�� */
        state = 1;
    }

    for(i = last; i < 159; i++)
    {
        if(imageInput[lineIndex][i])
        {
            imageOut[lineIndex][1] = i;
            break;
        }
    }
    if(i == 159)
    {
        /* ���ұ߽綪�� */
        if(state == 1)
        {
            state = 3;
        }
        /* �ұ߽綪�� */
        else
        {
            state = 2;
        }
    }
    if(imageOut[lineIndex][1] <= imageOut[lineIndex][0])
    {
        state = 4;
    }
    return state;
}

/*!
  * @brief    ���ߴ���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  * @param    mode       �� �Ǳ߶��ߣ�   1����߶���  2���ұ߶���
  * @param    lineIndex  �� ��������
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/24 ������
  */
void RoadNoSideProcess(uint8_t imageInput[LCDH][LCDW], uint8_t imageOut[LCDH][2], uint8_t mode, uint8_t lineIndex)
{
    uint8_t i = 0, j = 0, count = 0;
    switch(mode)
    {
      case 1:
        for(i = imageOut[lineIndex][1]; i > 1; i--)
        {
            count++;
            for(j = lineIndex; j > ROAD_END_ROW && lineIndex > count; j--)
            {
                if(imageInput[j][i])
                {
                    imageOut[lineIndex - count][0] = 0;
                    imageOut[lineIndex - count][1] = i;
                    break;
                }
            }
        }
        break;
      case 2:
        for(i = imageOut[lineIndex][0]; i < 159; i++)
        {
            count++;
            for(j = lineIndex; j > ROAD_END_ROW && lineIndex > count; j--)
            {
                if(imageInput[j][i])
                {
                    imageOut[lineIndex - count][0] = i;
                    imageOut[lineIndex - count][1] = 159;
                    break;
                }
            }
        }
        break;

    }
}
/*!
  * @brief    ��ȡ�������
  *
  * @param    imageInput �� ���ұ�������
  * @param    imageOut   �� ������ȱ�������
  *
  * @return   �Ƿ���
  *
  * @note     ˼·���ұ��������ȥ���������
  *
  * @see
  *
  * @date     2022/11/29 ���ڶ�
  */
uint8_t GetRoadWide(uint8_t imageInput[LCDH][2] ,uint8_t imageOut[LCDH])
{
    uint8_t i = 0;
    for(i = 10;i <= LCDH - 5; i++)
    {
        imageOut[i] = 0;
        if(imageInput[i][1] > imageInput[i][0])
        {
            imageOut[i] = imageInput[i][1] - imageInput[i][0];
        }
        else
        {
            imageOut[i] = 160;
        }
    }
    return 0;
}

/*!
  * @brief    ��ȡ���ұ���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  * @param    imageOut_last   �� ��һ��ͼƬ��������
  * @return   �Ƿ���
  *
  * @note     ˼·���Ӿ��복ͷ�Ͻ����п�ʼ���м�����������
  *
  * @see
  *
  * @date     2020/6/23 ���ڶ�
  */
uint8_t ImageGetSide(uint8_t imageInput[LCDH][LCDW], uint8_t imageOut[LCDH][2], uint8_t imageOut_last[LCDH][2])
{
    uint8_t i = 0, j = 0;

//    RoadIsNoSide(imageInput, imageOut, ROAD_START_ROW);
    for(i = 0 ;i <= LCDH;i++)
    {
        imageOut_last[i][0] = imageOut[i][0];
        imageOut_last[i][1] = imageOut[i][1];
    }

    /* �복ͷ����40�� Ѱ�ұ��� */
    for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
    {
        imageOut[i][0] = 0;
        imageOut[i][1] = 159;

        /* ���ݱ߽��������� Ѱ�ұ߽� */
        //Ѱ�������
        for(j = 60; j > 0; j--)
        {
            if(!imageInput[i][j])//0�Ǻ�ɫ
            {
                imageOut[i][0] = j ;
                break;
            }
        }
        //Ѱ���ұ���
        for(j = 60; j < 160; j++)
        {
            if(!imageInput[i][j])
            {
                imageOut[i][1] = j;
                break;
            }
        }
        /* �����߽� ������������ �����Ƿ��Ҷ��� */
        if(imageOut[i][0] > (LCDW/2 - 10) && imageOut[i][1] >  (LCDW - 5))
        {
            /* �Ҷ��ߴ��� */
            RoadNoSideProcess(imageInput, imageOut, 2, i);

            if(i > 70)
            {
                imageOut[i][0] += 50;
            }
            return 1;
        }

//        /* ����ұ߽� ������������ �����Ƿ����� */
//        if(imageOut[i][1] < (LCDW/2 + 10) && imageOut[i][0] <  (5))
//        {
//            /* ���ߴ��� */
//            RoadNoSideProcess(imageInput, imageOut, 1, i);
//
//            if(i > 70)
//            {
//                imageOut[i][1] -= 50;
//            }
//            return 2;
//
//        }
//        /*�������鷢��ͻ��*/
//        if((imageOut_last[110][0] - imageOut[110][0]) > 20 && (imageOut_last[110][0] - imageOut[110][0]) < -20)
//        {
//            for(i = 0;i < LCDH;i++)
//            {
//                imageOut[i][0] = imageOut_last[i][0];
//                imageOut[i][1] = 158;
//            }
//        }
//        if(imageOut_last[110][1] - imageOut[110][1] > 20 && imageOut_last[110][1] - imageOut[110][1] < -20)
//        {
//            for(i = 0;i <= LCDH;i++)
//            {
//                imageOut[i][1] = imageOut_last[i][1];
//                imageOut[i][0] = 1;
//            }
//        }
    }
    return 0;
}

/*!
  * @brief    ��ȡ���±���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  *
  * @return   �Ƿ���
  *
  * @note     ˼·���Ӿ��복ͷ�Ͻ����п�ʼ���м�����������
  *
  * @see
  *
  * @date     2021/11/30 ���ڶ�
  */
uint8_t UpdownSideGet(uint8_t imageInput[LCDH][LCDW], uint8_t imageOut[2][LCDW])
{
    uint8_t i = 0, j = 0;
    uint8_t last = 60;

    imageOut[0][159] = 0;
    imageOut[1][159] = 119;
    /* �����߱ȽϽ����� �ж��Ƿ��� */
    for(i = last; i >= 0; i--)
    {
        if(!imageInput[i][80])
        {
            imageOut[0][80] = i;
            break;
        }
    }

    for(i = last; i < 120; i++)
    {
        if(!imageInput[i][80])
        {
            imageOut[1][80] = i;
            break;
        }
    }

    /* �������� Ѱ�ұ��� */
    for(i = 80-1; i > 0; i--)
    {
        imageOut[0][i] = 0;
        imageOut[1][i] = 119;

        /* ���ݱ߽��������� Ѱ�ұ߽� */
        for(j = imageOut[0][i+1] + 10; j > 0; j--)
        {
            if(!imageInput[j][i])
            {
                imageOut[0][i] = j;
                break;
            }
        }
        for(j = imageOut[1][i+1] - 10; j < 120; j++)
        {
            if(!imageInput[j][i])
            {
                imageOut[1][i] = j;
                break;
            }
        }
    }
    /*�������� Ѱ�ұ���*/
    for(i = 80+1; i < 159; i++)
        {
            imageOut[0][i] = 0;
            imageOut[1][i] = 119;

            /* ���ݱ߽��������� Ѱ�ұ߽� */
            for(j = imageOut[0][i-1] + 10; j > 0; j--)
            {
                if(!imageInput[j][i])
                {
                    imageOut[0][i] = j;
                    break;
                }
            }
            for(j = imageOut[1][i-1] - 10; j < 120; j++)
            {
                if(!imageInput[j][i])
                {
                    imageOut[1][i] = j;
                    break;
                }
            }
        }
    return 0;
}

/*!
  * @brief    ����һ�����
  *
  * @param
  *
  * @return
  *
  * @note     ˼·�� �����������ڵ�9���㣬�����������ֵ�������õ�
  *
  * @see
  *
  * @date     2020/6/24 ������
  */
void ImagePortFilter(uint8_t imageInput[LCDH][LCDW], uint8_t imageOut[LCDH][LCDW])
{
    uint8_t temp = 0;

    for(int i = 1; i < LCDH - 1; i++)
    {
        for(int j = 1; j < LCDW - 1; j++)
        {
            temp = imageInput[i-1][j-1] + imageInput[i-1][j] + imageInput[i-1][j+1] +
                   imageInput[i  ][j-1] + imageInput[i  ][j] + imageInput[i  ][j+1] +
                   imageInput[i+1][j-1] + imageInput[i+1][j] + imageInput[i+1][j+1];

            /* ������5�����Ǳ��� �����õ� ���Ե��������Ż��˲�Ч�� */
            if(temp > 4)
            {
                imageOut[i][j] = 1;
            }
            else
            {
                imageOut[i][j] = 0;
            }
        }
    }
}
/*************************************************************************
 *  �������ƣ�void Get_Errand(void)
 *  ����˵�������ͼ��ƫ��
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2022��12��21��
 *  ��   ע��
 *************************************************************************/
void Get_Errand(void)
{
    if(OpeningRoad_Flag == 0 )
       {
           if(Barrier_Flag == 1 && miss_Flag ==0 && Depart_Flag ==0)
           {
               g_sSteeringError = RoadGetSteeringError(ImageSide, (uint8_t)ROAD_MAIN_ROW);
           }
        //ƫ��Ŵ�
        if((g_sSteeringError < 60) && (g_sSteeringError > -60))
        {
            if((g_sSteeringError < 20) && (g_sSteeringError > -20))
            {
                Servo_P = 11;
            }
            else
            {
                Servo_P = 14;
            }
        }
        else
        {
            Servo_P = 11;
        }
        ServoDuty = g_sSteeringError * Servo_P/10;
        //ƫ���޷�
        if(ServoDuty>100) ServoDuty=100;
        if(ServoDuty<-100) ServoDuty=-100;
       }
}

/*************************************************************************
 *  �������ƣ�void Change_V(void)
 *  ����˵��������ƫ����ٶ�
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2022��12��6��
 *  ��        ע��
 *************************************************************************/
static uint8_t i = 0;
void Change_V(void)
{

//    if(g_sSteeringError <= 15 && g_sSteeringError >= -15 && OpeningRoad_Flag == 0
//            && Barrier_Flag == 1 && R_CircleFlag == 0)
//    {
//       if(++i == 4)
//       {
//          Target_Speed1 = 100;
//          Target_Speed2 = 100;
//        }
//    }
    if(g_sSteeringError <= 3 && g_sSteeringError >= -3)
    {
       if(++i == 35)
       {
           if(start_V_L<=12&&start_V_R<=12)
           {
               start_V_L = start_V_L+1;
               start_V_R = start_V_R+1;
           }
           i=0;
        }
    }
    else
    {
        if(++i == 5)
        {
            if(start_V_L>5&&start_V_R>5)
            {
                start_V_L = start_V_L-1;
                start_V_R = start_V_R-1;
            }
            i=0;
         }
//        if(R_CircleFlag >= 1)
//        {
//            Target_Speed1 = 60;
//            Target_Speed2 = 60;
//        }
//        else
//        {
//            Target_Speed1 = 80;
//            Target_Speed2 = 80;
//        }
    }
}
extern float Gro3_p;
/*!
  * @brief    ��Ϣ��Ļ��ʾ
  *
  * @param      void
  *
  * @return     void
  *
  * @note
  *
  * @see
  *
  * @date     2022/1/05 ������
  */
void TFT_Show_Camera_Info(void)
{
    static char KEY_Value = 0;
    char txt[30];
    //��������
    if(KEY_Read(KEY2)==0)
    {
        KEY_Value = 3;
    }
    else if(KEY_Read(KEY1)==0)
    {
        KEY_Value = 2;
    }
    else if(KEY_Read(DSW0)==0)
    {
        KEY_Value = 4;
    }
    else if(KEY_Read(KEY0)==0)
    {
        TFTSPI_BinRoad(0, 0, LCDH, LCDW, (unsigned char *) Bin_Image);  //��ʾ��Ե��ȡͼ��
        TFTSPI_Draw_Line(80, 0, 80, 119, u16RED);                       //��ʾ����

        TFTSPI_BinRoadSide(ImageSide);          //���ұ���
        TFTSPI_BinRoad_UpdownSide(UpdowmSide);  //���±���
        KEY_Value = 1;
    }
    //��������
    if(KEY_Value == 1 && KEY_Read(KEY0) != 0)
    {
//        TFTSPI_Draw_Dot(leftup[0], leftup[1], u16GREEN);
//        TFTSPI_Draw_Dot(leftup[0]+1, leftup[1], u16GREEN);
//        TFTSPI_Draw_Dot(leftup[0]-1, leftup[1], u16GREEN);
//        TFTSPI_Draw_Dot(leftup[0], leftup[1]+1, u16GREEN);
//        TFTSPI_Draw_Dot(leftup[0], leftup[1]-1, u16GREEN);
        KEY_Value = 0;
        TFTSPI_CLS(u16BLACK);                   // ����
        motor_flag = 1;                         //�����־λ����ʼ����
    }
    else if(KEY_Value == 2 && KEY_Read(KEY1) != 0)
    {
        value = value +0.1;
        KEY_Value = 0;
    }
    else if(KEY_Value == 3 && KEY_Read(KEY2) != 0)
    {
        value = value -0.1;
        KEY_Value = 0;
    }
    else if(KEY_Value == 4 && KEY_Read(DSW0) != 0)
    {
        KEY_Value = 0;
        TFTSPI_Init(0);               // TFT2.0��ʼ��0:����  1������
        TFTSPI_CLS(u16BLACK);         // ����
    }

    //һ�� Ԫ�ر�־λ
    sprintf(txt,"%-2d",OpeningRoad_Flag);       //��·��־λ
    TFTSPI_P6X8Str(0, 0, txt, u16RED, u16BLUE);
    sprintf(txt,"L%-2d",L_CircleFlag);          //�󻷵���־λ
    TFTSPI_P6X8Str(4, 0, txt, u16RED, u16BLUE);
    sprintf(txt,"R%-2d",R_CircleFlag);          //�һ�����־λ
    TFTSPI_P6X8Str(8, 0, txt, u16RED, u16BLUE);
    sprintf(txt,"%-2d",Barrier_Flag);           //��·��־λ
    TFTSPI_P6X8Str(11, 0, txt, u16RED, u16BLUE);
    sprintf(txt,"%-2d",Crossroad_Flag);         //ʮ��·�ڱ�־λ
    TFTSPI_P6X8Str(15, 0, txt, u16RED, u16BLUE);
    sprintf(txt,"%-2d",Depart_Flag);            //������λ
    TFTSPI_P6X8Str(19, 0, txt, u16RED, u16BLUE);

    //���� ����ר��
    sprintf(txt,"G_P%-4.3f",Gro3_p);
    TFTSPI_P6X8Str(0, 1, txt, u16RED, u16BLUE);
    sprintf(txt,"G_Z%-4d",gyro[2]);         //�����ǽ��ٶ�Z��
    TFTSPI_P6X8Str(8, 1, txt, u16RED, u16BLUE);
    sprintf(txt,"P%.1f",Angle_kp);          //ѭ��PID
    TFTSPI_P6X8Str(15, 1, txt, u16RED, u16BLUE);
    sprintf(txt,"D%.2f",Angle_kd);
    TFTSPI_P6X8Str(21, 1, txt, u16RED, u16BLUE);

    //������
    sprintf(txt,"L:AD0:%-3d",left_V);               //�����ֵ
    TFTSPI_P6X8Str(0, 4, txt, u16RED, u16BLUE);
    sprintf(txt,"AD1:%-3d",left_X);                 //�����ֵ
    TFTSPI_P6X8Str(10, 4, txt, u16RED, u16BLUE);
    sprintf(txt,"R:AD2:%-3d",right_X);              //�Ҳ���ֵ
    TFTSPI_P6X8Str(0, 5, txt, u16RED, u16BLUE);
    sprintf(txt,"AD3:%-3d",right_V);                //�Ҳ���ֵ
    TFTSPI_P6X8Str(10, 5, txt, u16RED, u16BLUE);
    sprintf(txt,"ALL:%-3f",ALL_AD);                 //ȫ�����ֵ
    TFTSPI_P6X8Str(18, 5, txt, u16RED, u16BLUE);
    sprintf(txt,"PWM%4d",OUT_PWM);                  //ת��PWM
    TFTSPI_P6X8Str(4, 10, txt, u16RED, u16BLUE);

    //ʮ����
//    sprintf(txt, "V_L:%d", Target_Speed1);             //����Ŀ���ٶ�
//    TFTSPI_P6X8Str(0, 12, txt, u16RED, u16BLUE);
//    sprintf(txt, "V_R:%d", Target_Speed2);             //����Ŀ���ٶ�
//    TFTSPI_P6X8Str(8, 12, txt, u16RED, u16BLUE);

    //ʮ��ʮ����
    sprintf(txt, "L%-4d", ECPULSE1);             //���ֱ�����
    TFTSPI_P6X8Str(0, 13, txt, u16RED, u16BLUE);
    sprintf(txt, "R%-4d", ECPULSE2);             //���ֱ�����
    TFTSPI_P6X8Str(0, 14, txt, u16RED, u16BLUE);
    sprintf(txt, "%-5d", MotorDuty_L);           //����PWMֵ
    TFTSPI_P6X8Str(8, 13, txt, u16RED, u16BLUE);
    sprintf(txt, "%-5d", MotorDuty_R);           //����PWMֵ
    TFTSPI_P6X8Str(8, 14, txt, u16RED, u16BLUE);

   //ʮ����
    sprintf(txt, "%.2f", g_sSteeringError);      //���ֵ
    TFTSPI_P6X8Str(0, 15, txt, u16RED, u16BLUE);
}

uint8 LED1_Time = 0;
/*************************************************************************
 *  �������ƣ�void CameraCar(void)
 *  ����˵����ͼ����ͼ����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2022��11��29��
 *  ��        ע��
 *************************************************************************/
void CameraCar (void)
{
    if(++LED1_Time == 50)   //10*50 = 500ms
    {
        LED1_Time = 0;
        LED_Ctrl(LED1,RVS);     // LED��˸ ָʾ��������״̬
    }
    /*************************��̬�ٶ�********************************************/
    Change_V();

    /*************************�ϰ��ﴦ����***************************************/
    BarrierProcess(ImageSide,&Barrier_Flag);

    /*************************ʮ�ִ�����*****************************************/
    Crossroad_Find(UpdowmSide,ImageSide,Roadwide,&Crossroad_Flag);

    /*************************��·ʶ������**************************************/
    OpeningProcess(UpdowmSide,ImageSide,Roadwide,&OpeningRoad_Flag);

    /*************************Բ��������******************************************/
    Round_Process(&L_CircleFlag,&R_CircleFlag,&motor_flag,ImageSide,Roadwide,UpdowmSide,&miss_Flag);       //Բ����ʶ����
    if((OpeningRoad_Flag == 0) && ((L_CircleFlag >= 1) || (R_CircleFlag >= 1))) //������
    {
        R_CircleProcess(ImageSide,Roadwide,&R_CircleFlag);        // ����⻷��
    }

    /*************************���⴦����******************************************/
    Depart(&Depart_Flag);       //������
//    Carbarn(Bin_Image,ImageSide,&motor_flag,&Carbarn_Flag);       //�복��

    /*************************���������У���ȡƫ�� ***********************************/
    Get_Errand();
}

