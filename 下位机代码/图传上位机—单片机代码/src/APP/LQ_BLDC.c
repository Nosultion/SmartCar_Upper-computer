#include "LQ_BLDC.h"
#include "LQ_TFT18.h"
#include "LQ_ADC.h"
#include "LQ_GTM.h"
#include "LQ_CCU6.h"
#include "LQ_GPIO.h"
#include "LQ_GPIO_KEY.h"
#include "LQ_GPIO_LED.h"
#include "LQ_MotorServo.h"
#include "LQ_GPT12_ENC.h"
#include <stdio.h>

#define Use_IR2304        //用2304无刷驱动板 请取消此注释  用2136无刷驱动板 需注释此代码
//#define Use_IR2136        //用2136无刷驱动板 请取消此注释  用2304无刷驱动板 需注释此代码


#define BLDC_PWM_FREQUENCY    ATOM_PWM_MAX
BLDC_MANAGER g_sBLDCMag;//无刷结构管理定义
short BLDCduty=100;//无刷电机pwm
float y_vel_prev=0;//低通滤波
float Speed_KP=0;
float Speed_KI=0;
uint16_t IA = 0,IB = 0,IC = 0;//三相电流定义
int Move_distance=20;//目标速度
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void MotorInit(uint16 freq)
@功能说明：无刷电机引脚，pwm初始化
@参数说明：pwm 频率
@函数返回：无
@修改时间：2022/02/24
@调用方法：BLDC_MotorInit(1000-1);
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void BLDC_MotorInit(uint16 freq)
{
    ATOM_PWM_InitConfig(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, 0, MOTOR_FREQUENCY);
    ATOM_PWM_InitConfig(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 0, MOTOR_FREQUENCY);
    ATOM_PWM_InitConfig(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, 0, MOTOR_FREQUENCY);
//    ATOM_PWM_InitConfig(IfxGtm_ATOM0_2_TOUT55_P21_4_OUT, 0, MOTOR_FREQUENCY);

    PIN_InitConfig(P32_4, PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(P22_3, PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(P20_8, PIN_MODE_OUTPUT, 0);

    g_sBLDCMag.Electricity_flag =TRUE;  //初始化电流判断条件，用于上电检测
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void BLDC_HallInit(void);
@功能说明：初始化霍尔引脚及编码器接口
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：编码器接口同时为霍尔引脚状态读取接口
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void BLDC_HallInit(void)
{
   PIN_InitConfig(P13_0, IfxPort_Mode_inputPullUp, 1);
   ENC_InitConfig(ENC5_InPut_P10_3, ENC5_Dir_P10_1);//BC相使用
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：uint8_t Get_Hall_State(void);
@功能说明：读取霍尔状态
@参数说明：无
@函数返回：hall_state（霍尔状态）
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8_t Get_Hall_State(void)
{
  uint8_t hall_state = 0;

  if(PIN_Read(P13_0)) //F11/* 读取霍尔传感器 U 的状态 */
  {
    hall_state |= 0x01U << 0;
  }
  if(PIN_Read(P10_1)) //F12/* 读取霍尔传感器 V 的状态 */
  {
    hall_state |= 0x01U << 1;
  }
  if(PIN_Read(P10_3)) //F13 /* 读取霍尔传感器 W 的状态 */
  {
    hall_state |= 0x01U << 2;
  }
  return hall_state;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void BLDC_Motor_Hall_Run(int16_t motor_duty);
@功能说明：无刷电机驱动函数，有霍尔
@参数说明：motor_duty（电机占空比）
@函数返回：无
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void BLDC_Motor_Hall_Run(int16_t motor_duty)
{
  /* 获取霍尔传感器引脚状态,作为换相的依据 */
  g_sBLDCMag.step = Get_Hall_State();
  g_sBLDCMag.duty = motor_duty;
  PIN_Write(P20_8,1);   //打开P20.8
  PIN_Write(P32_4,1);   //打开P32.4
  PIN_Write(P22_3,1);   //打开P22.3
  if(motor_duty>0)LQ_BLDCCorotation();//逆时针方向
  else if(motor_duty==0)LQ_BLDCStop();
  else if(motor_duty<0)LQ_BLDCReversal();
}


#ifdef Use_IR2304
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCCorotation(void);
@功能说明：控制无刷电机正转
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_BLDCCorotation(void)
{
    switch(g_sBLDCMag.step) //U+ U-  V+ V-  W+ W-
    {
    case 1:
        //A→B=0   B→C=-  C→A=+
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, 0,MOTOR_FREQUENCY);              // P23.1输出0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 0,MOTOR_FREQUENCY);              // P21.2输出0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,g_sBLDCMag.duty,MOTOR_FREQUENCY); // P21.5输出PWM
      break;

    case 2:
        //A→B=-   B→C=+  C→A=0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT,0,MOTOR_FREQUENCY);               // P231输出0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,g_sBLDCMag.duty,MOTOR_FREQUENCY); // P21.2输出PWM
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,0,MOTOR_FREQUENCY);               // P21.5输出0
      break;

    case 3:
        //A→B=-   B→C=+  C→A=0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT,0 ,MOTOR_FREQUENCY);               // P231输出0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, g_sBLDCMag.duty,MOTOR_FREQUENCY); // P21.2输出PWM
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,0,MOTOR_FREQUENCY);                // P21.5输出0
      break;

    case 4:
        //A→B=+   B→C=0  C→A=-
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, g_sBLDCMag.duty,MOTOR_FREQUENCY); // P23.1输出PWM
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 0,MOTOR_FREQUENCY);               // P21.2输出0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,0,MOTOR_FREQUENCY);                // P21.5输出0
      break;

    case 5:
        //A→B=0   B→C=-  C→A=+
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, 0,MOTOR_FREQUENCY);              // P23.1输出0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 0,MOTOR_FREQUENCY);              // P21.2输出0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,g_sBLDCMag.duty,MOTOR_FREQUENCY); // P21.5输出PWM
      break;

    case 6:
        //A→B=+   B→C=0  C→A=-
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, g_sBLDCMag.duty,MOTOR_FREQUENCY); // P23.1输出PWM
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 0,MOTOR_FREQUENCY);               // P21.5输出0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, 0,MOTOR_FREQUENCY);               // P21.2输出0
      break;

    default:
      LQ_BLDCStop();
      break;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCReversal(void);
@功能说明：控制无刷电机反转
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_BLDCReversal(void)
{
    switch(g_sBLDCMag.step)
    {
      case 1:
          //A→B=+   B→C=0  C→A=-
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, 0-g_sBLDCMag.duty,MOTOR_FREQUENCY); // P23.1输出PWM
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, 0,MOTOR_FREQUENCY);                 // P21.5输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 0,MOTOR_FREQUENCY);                 // P21.2输出0
        break;

      case 2:
          //A→B=0  B→C=-  C→A=+
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, 0,MOTOR_FREQUENCY);                 // P23.1输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 0,MOTOR_FREQUENCY);                 // P21.2输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, 0-g_sBLDCMag.duty,MOTOR_FREQUENCY); // P21.5输出PWM
        break;

      case 3:
          //A→B=+   B→C=0  C→A=-
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, 0,MOTOR_FREQUENCY);                 // P21.5输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, 0-g_sBLDCMag.duty,MOTOR_FREQUENCY); // P23.1输出PWM
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 0,MOTOR_FREQUENCY);                 // P21.2输出0
        break;

      case 4:
          //A→B=-   B→C=+  C→A=0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, 0,MOTOR_FREQUENCY);                 // P21.5输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 0-g_sBLDCMag.duty,MOTOR_FREQUENCY); // P21.2输出PWM
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, 0,MOTOR_FREQUENCY);                 // P23.1输出0
        break;

      case 5:
          //A→B=-   B→C=+  C→A=0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, 0,MOTOR_FREQUENCY);                 // P21.5输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 0-g_sBLDCMag.duty,MOTOR_FREQUENCY); // P21.2输出PWM
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, 0,MOTOR_FREQUENCY);                 // P23.1输出0
        break;

      case 6:
          //A→B=0   B→C=-  C→A=+
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, 0,MOTOR_FREQUENCY);                 // P23.1输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 0,MOTOR_FREQUENCY);                 // P21.2输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, 0-g_sBLDCMag.duty,MOTOR_FREQUENCY); // P21.5输出PWM
        break;

      default:
        LQ_BLDCStop();
        break;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCStop(void);
@功能说明：关闭无刷电机，停止运行
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_BLDCStop(void)
{
    PIN_Write(P22_3,0);   //关断C-
    PIN_Write(P32_4,0);   //关断B-
    ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, 0,MOTOR_FREQUENCY);  //关断B+
    ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, 0,MOTOR_FREQUENCY);  //关断A+
    /**********guanduan********************/
    ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 0,MOTOR_FREQUENCY); //关断C+
    PIN_Write(P20_8,0);                             //关断A-

}
#else   //USE2136

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCCorotation(void);
@功能说明：控制无刷电机正转
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_BLDCCorotation(void)
{
    switch(g_sBLDCMag.step)
    {
    case 1:
       //A→B=0   B→C=-  C→A=+
       ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                 // P23.1输出0
       ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                 // P21.2输出0
       ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,MOTOR_FREQUENCY-g_sBLDCMag.duty,MOTOR_FREQUENCY);  // P21.5输出PWM
       break;

    case 2:
       //A→B=-   B→C=+  C→A=0
       ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                   // P21.5输出0
       ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, MOTOR_FREQUENCY-g_sBLDCMag.duty,MOTOR_FREQUENCY);   // P21.2输出PWM
       ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                   // P231输出0
       break;

    case 3:
        //A→B=-   B→C=+  C→A=0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                   // P21.5输出0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, MOTOR_FREQUENCY-g_sBLDCMag.duty,MOTOR_FREQUENCY);   // P21.2输出PWM
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                   // P23.1输出0
       break;

    case 4:
        //A→B=+   B→C=0  C→A=-
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, MOTOR_FREQUENCY-g_sBLDCMag.duty,MOTOR_FREQUENCY);     // P23.1输出PWM
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                     // P21.5输出0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                     // P21.2输出0
        break;

    case 5:
        //A→B=0   B→C=-  C→A=+
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                     // P23.1输出0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                     // P21.2输出0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,MOTOR_FREQUENCY-g_sBLDCMag.duty,MOTOR_FREQUENCY);      // P21.5输出PWM
        break;

    case 6:
        //A→B=+   B→C=0  C→A=-
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, MOTOR_FREQUENCY-g_sBLDCMag.duty,MOTOR_FREQUENCY);      // P23.1输出PWM
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                      // P21.5输出0
        ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                      // P21.2输出0
        PIN_Write(P32_4,1);   //打开P32.4
        break;
    default:
      LQ_BLDCStop();
      break;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCReversal(void);
@功能说明：控制无刷电机反转
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_BLDCReversal(void)
{
    switch(g_sBLDCMag.step)
    {
        case 1:
          //A→B=+   B→C=0  C→A=-
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, MOTOR_FREQUENCY+g_sBLDCMag.duty,MOTOR_FREQUENCY);  // P23.1输出PWM
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                  // P21.2输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                  // P21.5输出0
          break;

        case 2:
          //A→B=0  B→C=-  C→A=+
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, MOTOR_FREQUENCY+g_sBLDCMag.duty,MOTOR_FREQUENCY);  // P21.5输出PWM
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                  // P21.2输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                  // P23.1输出0
          break;

        case 3:
           //A→B=+   B→C=0  C→A=-
           ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                 // P21.5输出0
           ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                 // P21.2输出0
           ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, MOTOR_FREQUENCY+g_sBLDCMag.duty,MOTOR_FREQUENCY); // P23.1输出PWM
           break;

        case 4:
          //A→B=-   B→C=+  C→A=0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                 // P23.1输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                 // P21.5输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, MOTOR_FREQUENCY+g_sBLDCMag.duty,MOTOR_FREQUENCY); // P21.2输出PWM
         break;

        case 5:
          //A→B=-   B→C=+  C→A=0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                   // P23.1输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, MOTOR_FREQUENCY+g_sBLDCMag.duty,MOTOR_FREQUENCY);   // P21.2输出PWM
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,MOTOR_FREQUENCY,MOTOR_FREQUENCY);                    // P21.5输出0
          break;

        case 6:
          //A→B=0   B→C=-  C→A=+
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                  // P23.1输出0
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, MOTOR_FREQUENCY+g_sBLDCMag.duty,MOTOR_FREQUENCY);  // P21.5输出PWM
          ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);                  // P21.2输出0
         break;

        default:
          LQ_BLDCStop();
          break;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCStop(void);
@功能说明：关闭无刷电机，停止运行
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_BLDCStop(void)
{
    PIN_Write(P22_3,0);   //关断C-
    PIN_Write(P32_4,0);   //关断B-
    ATOM_PWM_SetDuty(IfxGtm_ATOM0_6_TOUT42_P23_1_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);  //关断B+
    ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY);  //关断A+
    /**********guanduan********************/
    ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, MOTOR_FREQUENCY,MOTOR_FREQUENCY); //关断C+
    PIN_Write(P20_8,0);                             //关断A-

}
#endif
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：float LPF_velocity(float x);
@功能说明：一阶低通滤波器
@参数说明：传入需滤波的值
@函数返回：返回滤波后结果
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
float LPF_velocity(float x)
{
  float y = 0.8*y_vel_prev + 0.2*x;
  y_vel_prev=y;
  return y;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：int LQ_BLDCCurrent_detection(void);
@功能说明：电流检测，保护电路
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_BLDCCurrent_detection(void)
{
    g_sBLDCMag.counts ++;
    IA = (ADC_ReadAverage(ADC0,10)-2048)*3.3*100/4095/5/0.03;   //U    扩大100倍计算公式：(IA-2048)*3.3*100/4095/5/0.03≈IA/2    //2048是参考电压1.65
    IB = (ADC_ReadAverage(ADC1,10)-2048)*3.3*100/4095/5/0.03;  //V
    IC = (ADC_ReadAverage(ADC2,10)-2048)*3.3*100/4095/5/0.03;  //W    发现A相电流最大电阻特别容易烧，B相次之，C相最小，
    IA= LPF_velocity(IA);  //一阶低通滤波
    IB= LPF_velocity(IB);
    IC= LPF_velocity(IC);
    while((g_sBLDCMag.Electricity_flag ==TRUE)&&((IA/100>5.0)||(IB/100>5.0)||(IC/100>5.0)))
    {
      LQ_BLDCStop();
      if(KEY_Read(KEY1)==0){
        g_sBLDCMag.Electricity_flag =FALSE;
        break;
      }
    }
    if(g_sBLDCMag.counts > 1000)//超过2s，视为电机启动完毕不在检测启动电流
      g_sBLDCMag.counts=0,g_sBLDCMag.Electricity_flag =FALSE;
      while((IA/100>5)&&(IB/100>5)&&(IC/100>5)&&(ECPULSE2==0))
      {
          LQ_BLDCStop();//堵转后停止输出
          if(KEY_Read(KEY1)==0)break;
      }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCShow(void);
@功能说明：无刷电机相关数据展示
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_BLDCShow(void)
{
    char txt[16];
    float Bat = 0;
    if(KEY_Read(DSW0)) TFTSPI_P8X16Str(1, 2, "BEMF Mode", u16WHITE, u16BLACK);
    else               TFTSPI_P8X16Str(1, 2, "HALL Mode", u16WHITE, u16BLACK);
    sprintf(txt, "HALL:%02X", Get_Hall_State());
    TFTSPI_P8X16Str(1, 4, txt, u16WHITE, u16BLACK);
    sprintf(txt, "PWM: %05d;", BLDCduty);
    TFTSPI_P8X16Str(1, 5, txt, u16WHITE, u16BLACK);       //字符串显示

    Bat = (float)ADC_ReadAverage(ADC7,10)*5*4/4096-0.45;  //电池电压检测，
    sprintf(txt, "Bat:%.2f ",Bat);
    TFTSPI_P8X16Str(1, 3, txt, u16WHITE, u16RED);

    sprintf(txt, "I:%02d.%02d %02d.%02d %02d.%02d", IA/100,IA%100,IB/100,IB%100,IC/100,IC%100);
    TFTSPI_P8X16Str(1, 7, txt, u16WHITE, u16BLACK);

    sprintf(txt, "speed:%02d ",ECPULSE2);
    TFTSPI_P8X16Str(1, 1, txt, u16RED, u16BLUE);

    sprintf(txt, "Servo duty:%04d ", ServoDuty);
    TFTSPI_P8X16Str(1, 6, txt, u16BLACK, u16GREEN); //显示霍尔实际脉冲数，以便灵活调整


}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TestMotorBLDC (void);
@功能说明：无刷电机测试函数
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TestMotorBLDC (void)
{
    BLDC_HallInit();//初始化霍尔引脚，读取霍尔值以及初略的编码值
    TFTSPI_CLS(u16BLUE);    //清屏
    TFTSPI_P8X16Str(2, 0, "LQ BLDC Test", u16RED, u16BLUE);
    BLDC_MotorInit(MOTOR_FREQUENCY-1);
    ADC_InitConfig(ADC7, 80000);//电池电压采集
    ADC_InitConfig(ADC0, 80000);
    ADC_InitConfig(ADC1, 80000);
    ADC_InitConfig(ADC2, 80000);
    CCU6_InitConfig(CCU60, CCU6_Channel0, 500000);  // 50ms进入一次中断
    CCU6_InitConfig(CCU60, CCU6_Channel1, 20);  // 50ms进入一次中断
    while(1){
//        ServoCtrl(ServoDuty);
        LQ_BLDCCurrent_detection();//电流检测，保护电路
        LQ_BLDCShow();
//        ATOM_PWM_SetDuty(IfxGtm_ATOM0_3_TOUT56_P21_5_OUT, 4500, 12500);
        while ((KEY_Read(KEY0) == 0))      //按下KEY0键，占空比减小
        {
          if (BLDCduty > -MOTOR_FREQUENCY)
            BLDCduty -= 100;
          while ((KEY_Read(KEY0) == 0));
        }
        while((KEY_Read(KEY2) == 0))      //按下KEY2键，占空比加大
        {
          if (BLDCduty < MOTOR_FREQUENCY)     //满占空比为12500
            BLDCduty += 100;
          while((KEY_Read(KEY2) == 0));
        }
        while(KEY_Read(KEY1) == 0)      //按下KEY1键，占空比中值
        {
          if(BLDCduty>0)BLDCduty = -3500;
          else BLDCduty = 3500;
          while(KEY_Read(KEY1) == 0);
        }
        LED_Ctrl(LED0, RVS);       //电平翻转,LED闪烁
    }
}









