/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】zyf/chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2020年4月10日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】Hightec4.9.3/Tasking6.3及以上版本
【Target 】 TC264DA
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
基于iLLD_1_0_1_11_0底层程序
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

/**  @brief    转向误差  */
float g_sSteeringError = 0;
float last_g_sSteeringError=0;
/**  @brief    主跑行  */
extern int ROAD_MAIN_ROW;

/**  @brief    使用起始行  */
#define ROAD_START_ROW     80

/**  @brief    使用结束行  */
#define ROAD_END_ROW       10

/**  @brief    断路标志位  */
uint8_t OpeningRoad_Flag = 0;        //断路标志位 0：不是断路 1：断路

/*   @brief    障碍物标志位*/
uint8_t Barrier_Flag = 1 ;           //障碍物标志位 1:无障碍物 其他：有障碍物

/**  @brief    环岛标志位  */
uint8_t R_CircleFlag = 0;           //右环岛标志位
uint8_t L_CircleFlag = 0;           //左环岛标志位


/** @brief  十字路口标志位  */
uint8_t Crossroad_Flag = 0;         //十字路口标志位 0:不是十字路口 2-3:是十字路口


/** @brief  车库标志位  */
uint8_t Carbarn_Flag = 0;           //车库标志位 0:不是车库 1:是车库
uint8_t Depart_Flag = 0;            //出库标志位 0：不出库 1：左出库  2：右出库

/**  @brief    丢线次数  */
uint8_t Left_misi = 0,Right_misi = 0 ,Up_misi = 0;

uint8_t miss_Flag = 0;//丢线标志位 1左丢 2右丢

/** @brief 十字路口断点坐标  */
int leftup[2] ;
int leftup_1[2];
int rightup[2] ;
int rightup_1[2];

uint8_t ImageSide[LCDH][2];         //左右边线数组
uint8_t ImageSide_last[LCDH][2];    //上张图片左右边线数组
uint8_t UpdowmSide[2][LCDW];        //上下边线数组
uint8_t Roadwide[LCDH];             //赛道宽度
uint8_t Servo_P = 11;
extern unsigned char motor_flag;    //电机启停标志位
extern sint16 OUT_PWM;              //转向输出PWM

extern unsigned char MAX[24];
extern int min_tag;
extern uint8_t open_flag;
extern uint8_t Obstacle_flag;
extern uint8_t slope_flag;
extern uint8_t Park_flag;
//边线拐点标志位
uint8_t LeftFlag1 = 0;
uint8_t LeftFlag2 = 0;
uint8_t RightFlag1 = 0;
uint8_t RightFlag2 = 0;

extern int X_minL,X_minR,X_min;
extern int B_lk;
/*!
  * @brief    求绝对值
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2022/12/2 星期五
  */
float my_abs(float x)
{
    if(x >= 0) return x;
    else
        return -x;
}
/*!
  * @brief    画左右边线
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/28 星期日
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
        //TFTSPI_Draw_Dot(imageOut[i][0], i, u16GREEN);//画列数、行数、颜色
        TFTSPI_Draw_Dot((imageOut[i][0]+imageOut[i][1])/2, i, u16PURPLE);
        //TFTSPI_Draw_Dot(imageOut[i][1], i, u16RED);
    }
}

/*!
  * @brief    画上下边线
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/28 星期日
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
  * @brief    判断上边线是否单调
  * @param    X1 :起始X点
  * @param    X2 :终止X点              X1 < X2
  * @param    imageIn ： 边线数组
  *
  * @return   0：不单调or错误， 1：单调递增， 2：单调递减
  *
  * @note
  *
  * @see
  *
  * @date     2021/11/30 星期二
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
  * @brief    判断左右边线是否单调
  * @param
  * @param
  * @param    imageIn ： 边线数组    Flag 0:左边线 1右边线
  *
  * @return   0：不单调 1：单调
  *
  * @note
  *
  * @see
  *
  * @date     2021/11/30 星期二
  */
uint8_t RoadImageSide_Mono(uint8_t imageSide[LCDH][2],uint8_t Flag)
{
    uint8_t i = 0;
    uint8_t State;
    State = 0;
        /* 左边线是否单调 */
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
        /* 右边线是否单调 */
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
  * @brief    计算左右丢线次数
  * @param
  * @param
  * @param    imageIn ： 左右边线数组    Flag 0:左边线 1右边线
  *
  * @return   丢线次数
  *
  * @note
  *
  * @see
  *
  * @date     2022/11/30 星期三
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
  * @brief    判断是否是直道
  *
  * @param    image ： 左右边线
  *
  * @return   0：不是直道， 1：直道
  *
  * @note     思路：两边边线都单调
  *
  * @see
  *
  * @date     2022/11/29 星期二
  */
uint8_t RoadIsStraight(uint8_t imageSide[LCDH][2])
{
    uint8_t i = 0;
    leftState = 0;
    rightState = 0;
    /* 左边线是否单调 */
    for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
    {
        if((imageSide[i][0] >= imageSide[i+1][0]) && (imageSide[i][0] > 10))
        {
            leftState++;
        }
    }
    /* 右边线是否单调 */
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
  * @brief    补线处理
  *
  * @param    imageSide  : 边线
  * @param    status     : 1：左边线补线   2：右边线补线
  * @param    startX     : 起始点 列数
  * @param    startY     : 起始点 行数
  * @param    endX       : 结束点 列数
  * @param    endY       : 结束点 行数
  *
  * @return
  *
  * @note     endY 一定要大于 startY
  *
  * @see
  *
  * @date     2022/12/13
  */
void ImageAddingLine(uint8_t imageSide[LCDH][2], uint8_t status, uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY)
{
    int i = 0;
    /* 直线 x = ky + b*/
    float k = 0.0f, b = 0.0f;
    switch(status)
    {
      case 1://左补线
        {
            k = (float)((float)endX - (float)startX) / (float)((float)endY - (float)startY);
            b = (float)startX - (float)startY * k;

            for(i = startY; i < endY; i++)
            {
                imageSide[i][0] = (uint8_t)(k * i + b);
            }
            break;
        }
      case 2://右补线
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
  * @brief    判断是否是斑马线
  *
  * @param    image ： 二值图像信息
  *
  * @return   0：不是， 1：是
  *
  * @note     思路：
  *
  * @see
  *
  * @date     2020/6/23 星期二
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
  * @brief    判断入断路
  *
  * @param    Upimage[2][LCDW]上下边线数组    RoadwideInput[LCDH]赛道宽度  Flag 标志位
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2022/11/29 星期二
  */
void InputOpening(uint8_t Upimage[2][LCDW] ,uint8_t RoadwideInput[LCDH],uint8_t *Flag)
{
    if((Upimage[0][100] < Upimage[0][120])&&(Upimage[0][60]<Upimage[0][40])&&Upimage[0][100]>25&&Upimage[0][60]>25 && motor_flag == 1 && Depart_Flag == 0)
    {
        *Flag = 1;
    }
}
/*!
  * @brief    判断出断路
  *
  * @param  imageSideinput[LCDH][2]左右边线数组   RoadwideInput[LCDH]赛道宽度 Flag标志位
  *
  * @return
  *
  * @note       不是出断路就转为摄像头的，而是等车身稳定，而且赛道比较直的时候切回摄像头
  *
  * @see
  *
  * @date     2022/11/29 星期二
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
  * @brief    断路处理函数
  *
  * @param     Upimage[2][LCDW]上下边线数组   imageSideinput[LCDH][2]左右边线数组   RoadwideInput[LCDH]赛道宽度 Flag 标志位
  *
  * @return     无
  *
  * @note
  *
  * @see
  *
  * @date     2022/11/30 星期三
  */
void OpeningProcess(uint8_t Upimage[2][LCDW],uint8_t imageSideinput[LCDH][2],uint8_t RoadwideInput[LCDH],uint8_t *Flag)
{
    if(L_CircleFlag == 0 && R_CircleFlag == 0 && Barrier_Flag == 1)      //防止误判
    {
       switch(*Flag)
       {
       case 0:InputOpening(Upimage,RoadwideInput,&*Flag);break;     //入断路
       case 1:OutOpening(Upimage,imageSideinput,RoadwideInput,&*Flag);break;        //出断路
       default:break;
       }
    }
}
/*!
  * @brief    坡道处理函数
  *
  * @param     Upimage[2][LCDW]上下边线数组   imageSideinput[LCDH][2]左右边线数组   RoadwideInput[LCDH]赛道宽度 Flag 标志位
  *
  * @return     无
  *
  * @note
  *
  * @see    识别到坡道的时候，转换为电磁循迹，而且两个轮子不会出现不转的情况
  *
  * @date     2022/11/30 星期三
  */
//进入环岛前的参数
float P_L = 0,P_R = 0;
sint16 V_L = 0, V_R = 0;
/*!
  * @brief    右环岛处理函数--清楚圆环标记物，防止赛道的误判和出圆环时另一个地方清楚不及时
  *
  * @param     Upimage[2][LCDW]上下边线数组   imageSideinput[LCDH][2]左右边线数组   RoadwideInput[LCDH]赛道宽度 Flag 右环岛标志位
  *
  * @return     无
  *
  * @note
  *
  * @see
  *
  * @date     2022/11/30 星期三
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
*  函数名称：void Round_Process(uint8_t *L_Flag,uint8_t *R_Flag,uint8_t *M_Flag,uint8_t Roadwide[120],uint8_t UpImageSide[2][160],uint8_t OpeningRoad_Flag,uint8_t Barrier_Flag)
*  功能说明：圆环处理函数，用电磁检测圆环，检测到后用摄像头在里面跑和出圆环
*  参数说明：*L_Flag:左圆环标志位      *R_Flag:右圆环标志位     *M_Flag:电机启停标志位
*  参数说明:imageSideinput[LCDH][2]左右边线数组       Roadwide[LCDH] 赛道宽度     UpImageSide[2][LCDW]上下边线数组      *miss_Flag:丢线标志位
*  函数返回：无
*  修改时间：2022年11月29日星期二
*  备    注：
*************************************************************************/
void Round_Process(uint8_t *L_Flag,uint8_t *R_Flag,uint8_t *M_Flag,uint8_t imageSide[LCDH][2],uint8_t Roadwide[LCDH],uint8_t UpImageSide[2][LCDW],uint8_t *miss_Flag)
{
    static uint8_t i =0;
    /*************************丢线的识别*****************************************/
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

    /*************************圆环识别处理函数*****************************************/
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
  * @brief    左环岛处理函数--清楚圆环标记物，防止赛道的误判和出圆环是另一个地方清楚不及时
  *
  * @param     Upimage[2][LCDW]上下边线数组   imageSideinput[LCDH][2]左右边线数组   RoadwideInput[LCDH]赛道宽度 Flag 右环岛标志位
  *
  * @return     无
  *
  * @note
  *
  * @see
  *
  * @date     2022/12/2 星期五
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
  * @brief    障碍物处理函数
  *
  * @param    Flag 障碍物标记位
  *
  * @return     无
  *
  * @note
  *
  * @see    执行顺序1-0-2-3-4-5
  *
  * @date     2022/12/2 星期五
  */
void BarrierProcess(uint8_t imageSideinput[LCDH][2],uint8_t *Flag)
{
    static uint8_t i = 0;
    switch(*Flag)
    {
        case 0:                 //三轮车通过两电机差速通过，此处强制停车
            i++;
            if(i > 10)           //刹车时间（10*10ms）
            {
                i = 0;
                *Flag = 2;
            }
            g_sSteeringError = 0;
            MotorCtrl4w(0, 0,-3000, -3000);
            break;
        case 1:                 //识别障碍物
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
        case 2:             //开始转向
            Target_Speed1 = -20;
            Target_Speed2 = 20;
            g_sSteeringError = 0;
            if(Infrared_Read())//转向到识别不到障碍物
            {
                *Flag = 3;
            }
            break;
        case 3:             //绕过障碍物
            Target_Speed1 = 40;
            Target_Speed2 = 40;
            g_sSteeringError = 0;
            RAllPulse1 += ECPULSE1;
            if(RAllPulse1 > 3300)//编码器记录3300个脉冲
            {
                RAllPulse1 = 0;
                RAllPulse2 = 0;
                *Flag = 4;
            }
            break;
        case 4:             //朝向赛道
            Target_Speed1 = 20;
            Target_Speed2 = -20;
            g_sSteeringError = 0;
            RAllPulse1 += ECPULSE1;
            if(RAllPulse1 > 1100) //编码器记录1100个脉冲
            {
               RAllPulse1 = 0;
               *Flag = 5;
            }
            break;
        case 5:         //驶向赛道
            Target_Speed1 = 50;
            Target_Speed2 = 50;
            g_sSteeringError = 0;
            if(ALL_AD > 120)     //检测到电磁说明进入赛道
            {
                *Flag = 1;
            }
            break;
    }
}
/*!
 * @brief    寻找断点
 *
 * @param    start_point ： 开始点
 * @param    end_point   ： 结束点
 * @param    UpdowmSide  ： 边线数组
 * @param    *pointup[2] ： 断点坐标数组
 *
 * @return   void
 *
 * @note
 *
 * @see         开始点小于结束点
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
 * @brief    寻找断点
 *
 * @param    start_point ： 开始点
 * @param    end_point   ： 结束点
 * @param    UpdowmSide  ： 边线数组
 * @param    *pointup[2] ： 断点坐标数组
 *
 * @return   void
 *
 * @note
 *
 * @see         开始点小于结束点
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
 * @brief    寻找断点
 *
 * @param    start_point ： 开始点
 * @param    end_point   ： 结束点
 * @param    UpdowmSide  ： 边线数组
 * @param    *pointup[2] ： 断点坐标数组
 *
 * @return   void
 *
 * @note
 *
 * @see         开始点小于结束点
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
////            left_point[0] = (uint8_t)ImageSide[i][0];//左边第一个断点列数
////            left_point[1] = (uint8_t)i;//左边第一个断点行数
//            //point_leftnum=1;
//            //break;
//        }
//        if(abs(ImageSide[i][1] - ImageSide[i-1][1] )>8 && abs(ImageSide[i][1] - ImageSide[i+1][1]) < 3 &&ImageSide[i][1] < ImageSide[i+3][1] &&abs(ImageSide[i][1] - ImageSide[i-2][1]) > 8)
//        {
////            right_point[0] = (uint8_t)ImageSide[i][0];//左边第二个断点列数
////            right_point[1] = (uint8_t)i;//左边第二个断点行数
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
 * @brief    寻找断点
 *
 * @param    start_point ： 开始点
 * @param    end_point   ： 结束点
 * @param    UpdowmSide  ： 边线数组
 * @param    *pointup[2] ： 断点坐标数组
 *
 * @return   void
 *
 * @note
 *
 * @see         开始点小于结束点
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
 * @brief    寻找拐点
 *
 * @param    start_point ： 开始点
 * @param    end_point   ： 结束点
 * @param    ImageSide  ：  左右边线数组
 * @param    *pointup[2] ： 断点坐标数组
 *
 * @return   void
 *
 * @note
 *
 * @see         开始点小于结束点
 *
 * @date     2022/12/13
 */
//左边线第一个拐点（开始点结束点指的是行数）
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
//左边线第二个拐点
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
//右边线第一个拐点
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

//右边线第二个拐点
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
  * @brief    判断是否是十字
  *
  * @param    imageInput ：
  * @param    imageOut   ：
  * @param    lineIndex  ：
  *
  * @return
  *
  * @note
  *
  * @see     上边线有两次大幅度跳到，先减小，在增大。赛道宽度正常变化，忽然全部为丢线，然后再变成正常，
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
//        if(imageSide[i][0] < 3 && imageSide[i][1] > 157)//左右边线大幅度跳转
//        {
//            flag_number++;
//        }
//        if(flag_number > 50 && flag == 0)//判断为左边边线大幅度跳转
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

//    Find_Point_Left_1(0,40,imageSide,leftup);//左上拐点
//    Find_Point_Left_2(40,80,imageSide,leftup_1);//左下拐点
//    Find_Point_Right_1(0,40,imageSide,rightup);//右上拐点
//    Find_Point_Right_2(40,80,imageSide,rightup_1);//右下拐点
    if(LeftFlag1==1 && LeftFlag2==1 && RightFlag1==1 && RightFlag2==1)//如果四个拐点都能找到
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
  * @brief    获取上下边线
  *
  * @param    imageInput ： 二值图像信息
  * @param    imageSide  ： 边线数组
  * @param    status     ： 1：上边线  2：下边线
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
            /* 重新确定上边界 */
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
            /* 重新确定下边界 */
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
  * @brief    判断边线是否存在弧形
  *
  * @param    imageInput ： 二值图像信息
  * @param    imageOut   ： 边线数组
  * @param    status     ： 1：左边线  2：右边线
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/23 星期二
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
                /* 有弧线 */
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
                /* 有弧线 */
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
  * @brief    判断边线是否存在弧形
  *
  * @param    SideInput ： 上边线数组
  * @param    num       ： 检测幅度
  * @param    index     ： 最低点
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2021/12/01 星期三
  */
uint8_t UpSideErr(uint8_t SideInput[2][LCDW],uint8_t status ,uint8_t num, uint8_t * index)
{
    uint8_t dec = 0, inc = 0, i;
    //上线是否右突起
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
                      /* 有弧线 */
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
        //下边线
        case 2:
            for(i = 159-1; i > 0; i--)
                {
                  if(UpdowmSide[1][i] != 1 && UpdowmSide[1][i+1] != 1)
                  {
                      if(UpdowmSide[1][i] >= UpdowmSide[1][i+1])
                          inc++;
                      else
                          dec++;
                      /* 有弧线 */
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
  * @brief    出库函数
  * @param    Flag        : 出库标志位
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
  * @brief    车库的识别
  *
  * @param    imageInput  ： 二值图像
  * @param    Flag        ： 车库标志位
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
  * @brief    停车线处理
  *
  * @param    imageSide  ： 边线数组
  * @param    state      ： 停车状态  1：车库在左侧   2：车库在右侧
  * @param    speed      ： 速度
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
  * @brief    停车函数
  * @param    imageInput  ： 二值图像
  * @param    imageSide   ： 边线数组
  * @param    motor_flag  : 电机标志位
  * @param    Flag        : 车库标志位
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
            FindCarbarn(imageInput,&*Flag);              //车库识别
            break;
        case 1:
            ZebraProcess(imageSide,1, &*motor_flag);     //进入车库
            break;
    }
}

/*!
  * @brief    根据主跑行，求取舵机偏差
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @see    求几个偏差值，然后求平均值
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
        //regression(X_min,X_min+30);///斜率计算18  47
    }
//    else
//    {
//        regression(28,58);///斜率计算18  47
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
  * @brief    判断是否丢线
  *
  * @param    imageInput ： 二值图像信息
  * @param    imageOut   ： 边线数组
  * @param    lineIndex  ： 行
  *
  * @return   0：没有丢线   1:左边丢线  2：右边丢线  3： 左右都丢线   4：错误
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/24 星期三
  */
uint8_t RoadIsNoSide(uint8_t imageInput[LCDH][LCDW], uint8_t imageOut[LCDH][2], uint8_t lineIndex)
{
    uint8_t state = 0;
    uint8_t i = 0;
    static uint8_t last = 78;
    imageOut[lineIndex][0] = 0;
    imageOut[lineIndex][1] = 159;
    /* 用距离小车比较近的行 判断是否丢线 */
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
        /* 左边界丢线 */
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
        /* 左右边界丢线 */
        if(state == 1)
        {
            state = 3;
        }
        /* 右边界丢线 */
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
  * @brief    丢线处理
  *
  * @param    imageInput ： 二值图像信息
  * @param    imageOut   ： 边线数组
  * @param    mode       ： 那边丢线？   1：左边丢线  2：右边丢线
  * @param    lineIndex  ： 丢线行数
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/24 星期三
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
  * @brief    获取赛道宽度
  *
  * @param    imageInput ： 左右边线数组
  * @param    imageOut   ： 赛道宽度边线数组
  *
  * @return   是否丢线
  *
  * @note     思路：右边线数组减去左边线数组
  *
  * @see
  *
  * @date     2022/11/29 星期二
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
  * @brief    获取左右边线
  *
  * @param    imageInput ： 二值图像信息
  * @param    imageOut   ： 边线数组
  * @param    imageOut_last   ： 上一张图片边线数组
  * @return   是否丢线
  *
  * @note     思路：从距离车头较近的行开始从中间向两边搜线
  *
  * @see
  *
  * @date     2020/6/23 星期二
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

    /* 离车头近的40行 寻找边线 */
    for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
    {
        imageOut[i][0] = 0;
        imageOut[i][1] = 159;

        /* 根据边界连续特性 寻找边界 */
        //寻找左边线
        for(j = 60; j > 0; j--)
        {
            if(!imageInput[i][j])//0是黑色
            {
                imageOut[i][0] = j ;
                break;
            }
        }
        //寻找右边线
        for(j = 60; j < 160; j++)
        {
            if(!imageInput[i][j])
            {
                imageOut[i][1] = j;
                break;
            }
        }
        /* 如果左边界 即将超出中线 则检查是否右丢线 */
        if(imageOut[i][0] > (LCDW/2 - 10) && imageOut[i][1] >  (LCDW - 5))
        {
            /* 右丢线处理 */
            RoadNoSideProcess(imageInput, imageOut, 2, i);

            if(i > 70)
            {
                imageOut[i][0] += 50;
            }
            return 1;
        }

//        /* 如果右边界 即将超出中线 则检查是否左丢线 */
//        if(imageOut[i][1] < (LCDW/2 + 10) && imageOut[i][0] <  (5))
//        {
//            /* 左丢线处理 */
//            RoadNoSideProcess(imageInput, imageOut, 1, i);
//
//            if(i > 70)
//            {
//                imageOut[i][1] -= 50;
//            }
//            return 2;
//
//        }
//        /*边线数组发生突变*/
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
  * @brief    获取上下边线
  *
  * @param    imageInput ： 二值图像信息
  * @param    imageOut   ： 边线数组
  *
  * @return   是否丢线
  *
  * @note     思路：从距离车头较近的行开始从中间向两边搜线
  *
  * @see
  *
  * @date     2021/11/30 星期二
  */
uint8_t UpdownSideGet(uint8_t imageInput[LCDH][LCDW], uint8_t imageOut[2][LCDW])
{
    uint8_t i = 0, j = 0;
    uint8_t last = 60;

    imageOut[0][159] = 0;
    imageOut[1][159] = 119;
    /* 用中线比较近的行 判断是否丢线 */
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

    /* 中线往左 寻找边线 */
    for(i = 80-1; i > 0; i--)
    {
        imageOut[0][i] = 0;
        imageOut[1][i] = 119;

        /* 根据边界连续特性 寻找边界 */
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
    /*中线往右 寻找边线*/
    for(i = 80+1; i < 159; i++)
        {
            imageOut[0][i] = 0;
            imageOut[1][i] = 119;

            /* 根据边界连续特性 寻找边界 */
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
  * @brief    除单一的噪点
  *
  * @param
  *
  * @return
  *
  * @note     思路： 检查边沿邻域内的9个点，如果大于设置值，则保留该点
  *
  * @see
  *
  * @date     2020/6/24 星期三
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

            /* 邻域内5个点是边沿 则保留该点 可以调节这里优化滤波效果 */
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
 *  函数名称：void Get_Errand(void)
 *  功能说明：获得图像偏差
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2022年12月21日
 *  备   注：
 *************************************************************************/
void Get_Errand(void)
{
    if(OpeningRoad_Flag == 0 )
       {
           if(Barrier_Flag == 1 && miss_Flag ==0 && Depart_Flag ==0)
           {
               g_sSteeringError = RoadGetSteeringError(ImageSide, (uint8_t)ROAD_MAIN_ROW);
           }
        //偏差放大
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
        //偏差限幅
        if(ServoDuty>100) ServoDuty=100;
        if(ServoDuty<-100) ServoDuty=-100;
       }
}

/*************************************************************************
 *  函数名称：void Change_V(void)
 *  功能说明：根据偏差改速度
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2022年12月6日
 *  备        注：
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
  * @brief    信息屏幕显示
  *
  * @param      void
  *
  * @return     void
  *
  * @note
  *
  * @see
  *
  * @date     2022/1/05 星期三
  */
void TFT_Show_Camera_Info(void)
{
    static char KEY_Value = 0;
    char txt[30];
    //按键按下
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
        TFTSPI_BinRoad(0, 0, LCDH, LCDW, (unsigned char *) Bin_Image);  //显示边缘提取图像
        TFTSPI_Draw_Line(80, 0, 80, 119, u16RED);                       //显示中线

        TFTSPI_BinRoadSide(ImageSide);          //左右边线
        TFTSPI_BinRoad_UpdownSide(UpdowmSide);  //上下边线
        KEY_Value = 1;
    }
    //按键松下
    if(KEY_Value == 1 && KEY_Read(KEY0) != 0)
    {
//        TFTSPI_Draw_Dot(leftup[0], leftup[1], u16GREEN);
//        TFTSPI_Draw_Dot(leftup[0]+1, leftup[1], u16GREEN);
//        TFTSPI_Draw_Dot(leftup[0]-1, leftup[1], u16GREEN);
//        TFTSPI_Draw_Dot(leftup[0], leftup[1]+1, u16GREEN);
//        TFTSPI_Draw_Dot(leftup[0], leftup[1]-1, u16GREEN);
        KEY_Value = 0;
        TFTSPI_CLS(u16BLACK);                   // 清屏
        motor_flag = 1;                         //电机标志位，开始发车
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
        TFTSPI_Init(0);               // TFT2.0初始化0:横屏  1：竖屏
        TFTSPI_CLS(u16BLACK);         // 清屏
    }

    //一行 元素标志位
    sprintf(txt,"%-2d",OpeningRoad_Flag);       //断路标志位
    TFTSPI_P6X8Str(0, 0, txt, u16RED, u16BLUE);
    sprintf(txt,"L%-2d",L_CircleFlag);          //左环岛标志位
    TFTSPI_P6X8Str(4, 0, txt, u16RED, u16BLUE);
    sprintf(txt,"R%-2d",R_CircleFlag);          //右环岛标志位
    TFTSPI_P6X8Str(8, 0, txt, u16RED, u16BLUE);
    sprintf(txt,"%-2d",Barrier_Flag);           //断路标志位
    TFTSPI_P6X8Str(11, 0, txt, u16RED, u16BLUE);
    sprintf(txt,"%-2d",Crossroad_Flag);         //十字路口标志位
    TFTSPI_P6X8Str(15, 0, txt, u16RED, u16BLUE);
    sprintf(txt,"%-2d",Depart_Flag);            //出库标记位
    TFTSPI_P6X8Str(19, 0, txt, u16RED, u16BLUE);

    //二行 调参专用
    sprintf(txt,"G_P%-4.3f",Gro3_p);
    TFTSPI_P6X8Str(0, 1, txt, u16RED, u16BLUE);
    sprintf(txt,"G_Z%-4d",gyro[2]);         //陀螺仪角速度Z轴
    TFTSPI_P6X8Str(8, 1, txt, u16RED, u16BLUE);
    sprintf(txt,"P%.1f",Angle_kp);          //循迹PID
    TFTSPI_P6X8Str(15, 1, txt, u16RED, u16BLUE);
    sprintf(txt,"D%.2f",Angle_kd);
    TFTSPI_P6X8Str(21, 1, txt, u16RED, u16BLUE);

    //五六行
    sprintf(txt,"L:AD0:%-3d",left_V);               //左侧电磁值
    TFTSPI_P6X8Str(0, 4, txt, u16RED, u16BLUE);
    sprintf(txt,"AD1:%-3d",left_X);                 //左侧电磁值
    TFTSPI_P6X8Str(10, 4, txt, u16RED, u16BLUE);
    sprintf(txt,"R:AD2:%-3d",right_X);              //右侧电磁值
    TFTSPI_P6X8Str(0, 5, txt, u16RED, u16BLUE);
    sprintf(txt,"AD3:%-3d",right_V);                //右侧电磁值
    TFTSPI_P6X8Str(10, 5, txt, u16RED, u16BLUE);
    sprintf(txt,"ALL:%-3f",ALL_AD);                 //全部电磁值
    TFTSPI_P6X8Str(18, 5, txt, u16RED, u16BLUE);
    sprintf(txt,"PWM%4d",OUT_PWM);                  //转向PWM
    TFTSPI_P6X8Str(4, 10, txt, u16RED, u16BLUE);

    //十二行
//    sprintf(txt, "V_L:%d", Target_Speed1);             //左轮目标速度
//    TFTSPI_P6X8Str(0, 12, txt, u16RED, u16BLUE);
//    sprintf(txt, "V_R:%d", Target_Speed2);             //右轮目标速度
//    TFTSPI_P6X8Str(8, 12, txt, u16RED, u16BLUE);

    //十三十四行
    sprintf(txt, "L%-4d", ECPULSE1);             //左轮编码器
    TFTSPI_P6X8Str(0, 13, txt, u16RED, u16BLUE);
    sprintf(txt, "R%-4d", ECPULSE2);             //右轮编码器
    TFTSPI_P6X8Str(0, 14, txt, u16RED, u16BLUE);
    sprintf(txt, "%-5d", MotorDuty_L);           //左轮PWM值
    TFTSPI_P6X8Str(8, 13, txt, u16RED, u16BLUE);
    sprintf(txt, "%-5d", MotorDuty_R);           //右轮PWM值
    TFTSPI_P6X8Str(8, 14, txt, u16RED, u16BLUE);

   //十五行
    sprintf(txt, "%.2f", g_sSteeringError);      //误差值
    TFTSPI_P6X8Str(0, 15, txt, u16RED, u16BLUE);
}

uint8 LED1_Time = 0;
/*************************************************************************
 *  函数名称：void CameraCar(void)
 *  功能说明：图像检测图像处理
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2022年11月29日
 *  备        注：
 *************************************************************************/
void CameraCar (void)
{
    if(++LED1_Time == 50)   //10*50 = 500ms
    {
        LED1_Time = 0;
        LED_Ctrl(LED1,RVS);     // LED闪烁 指示程序运行状态
    }
    /*************************动态速度********************************************/
    Change_V();

    /*************************障碍物处理函数***************************************/
    BarrierProcess(ImageSide,&Barrier_Flag);

    /*************************十字处理函数*****************************************/
    Crossroad_Find(UpdowmSide,ImageSide,Roadwide,&Crossroad_Flag);

    /*************************断路识别处理函数**************************************/
    OpeningProcess(UpdowmSide,ImageSide,Roadwide,&OpeningRoad_Flag);

    /*************************圆环处理函数******************************************/
    Round_Process(&L_CircleFlag,&R_CircleFlag,&motor_flag,ImageSide,Roadwide,UpdowmSide,&miss_Flag);       //圆环的识别处理
    if((OpeningRoad_Flag == 0) && ((L_CircleFlag >= 1) || (R_CircleFlag >= 1))) //出环岛
    {
        R_CircleProcess(ImageSide,Roadwide,&R_CircleFlag);        // 出检测环岛
    }

    /*************************车库处理函数******************************************/
    Depart(&Depart_Flag);       //出车库
//    Carbarn(Bin_Image,ImageSide,&motor_flag,&Carbarn_Flag);       //入车库

    /*************************根据主跑行，求取偏差 ***********************************/
    Get_Errand();
}

