#include "LQ_STEER_PID.h"
#include "LQ_GPIO_BUZZ.h"
#include "LQ_MotorServo.h"
#include <Platform_Types.h>
#include "LQ_ImageProcess.h"
#include "typeinfo.h"
#include "stdint.h"
#include "LQ_MPU6050_DMP.h"


sint16 OUT_PWM=0;       //转向输出PWM                                                                       90                 95       93       80
float Angle_kp=31.3;    //转向环PID（外环）32.4     31.5    32.1     31.9   31.7    31.3    32.2    30.5    29.5    29.5        30       31.2      31.3
float value = 3;     //循迹P的变化大小                                              5       4     3.5       3     2.4         2.3        3         3
float Angle_ki=0;
float Angle_kd=0;
float Gro3_p = 0.3;   //陀螺仪的乘积系数//0.19     0.75    0.75    0.585 0.545    0.445   0.415     0.4    0.31    0.15       0.21      0.35       0.3
uint8_t Number=0;       //内外环计数器
float Angle=0;          //目标角度
float Angle_last=0;
int ServoDuty_Last=0;
int Servoduty=0;        //外环偏差
uint16_t i = 0;
extern sint16 ServoDuty;
/*************************************************************************
*  函数名称：void AngleControl(void)
*  功能说明：转向环控制
*  参数说明：无
*  函数返回：无
*  修改时间：2022年11月25日
*  备    注：转向外环15ms,转向内环5ms
*************************************************************************/
void AngleControl(void)
{
  Servoduty = (int)ServoDuty; //传递偏差
//三次内环，执行一次内环
  if(Number==3){
    Number=0;
    Angle_Get();
  }
  Number++;
  Angle=Angle_last+(Angle-Angle_last)*Number/3;
  Angle_out();
}
/*************************************************************************
*  函数名称：void Angle_Get(void)
*  功能说明：转向外环控制
*  参数说明：无
*  函数返回：无
*  修改时间：2022年11月25日
*  备    注：内环阻尼作用，为了防止车行驶时左摇右摆
*************************************************************************/
void Angle_Get(void)  //外环
{
    if(Servoduty >= -10 && Servoduty <= 10 && OpeningRoad_Flag ==0)
    {
        Angle=(Angle_kp - 2 * value)*Servoduty+Angle_kd*(Servoduty-ServoDuty_Last);

    }
    else if(((Servoduty > 10 && Servoduty <= 35) || (Servoduty >= -35 && Servoduty < -10)) && OpeningRoad_Flag == 0)
    {
        Angle=(Angle_kp - value)*Servoduty+Angle_kd*(Servoduty-ServoDuty_Last);
    }
    else if(OpeningRoad_Flag == 1)      //断路时，转向P要大
    {
        if(++i >= 300)
        {
            i = 300;
            Angle=Angle_kp *Servoduty+3*(Servoduty-ServoDuty_Last);
        }
        else
        {
            Angle=(Angle_kp + 5)*Servoduty+5*(Servoduty-ServoDuty_Last);
        }
    }
    else
    {
        Angle=Angle_kp*Servoduty+Angle_kd*(Servoduty-ServoDuty_Last);
    }

//    Angle=0; //开启这句话，表转向环只有内环，外环输出为0
     ServoDuty_Last=Servoduty;
     Angle_last=Angle;
}

/*************************************************************************
*  函数名称：void Angle_out(void)
*  功能说明：转向内环控制
*  参数说明：无
*  函数返回：无
*  修改时间：2022年11月25日
*  备    注：外环转向作用，可以使得车沿着赛道跑
*************************************************************************/
void Angle_out(void)
{
  float error;
  static  float last_error;
  float price_Differention;
  float price_Proportion;
  static float Pre1_Error[4];
  float Direct_Parameter;
  if(gyro[2] > 2500) gyro[2] = 2500;
  if(gyro[2] < -2500) gyro[2] = -2500;
  error = Angle*8.0 + gyro[2]*Gro3_p;  //转向内环参数，一般不用改，改变后面的参数，改变阻尼大小
  price_Proportion=0.35 * error;
  price_Differention= 0.35 * (error - last_error);

  Direct_Parameter =price_Proportion  + price_Differention ;//

  last_error =  error;
  Pre1_Error[3]=Pre1_Error[2];
  Pre1_Error[2]=Pre1_Error[1];
  Pre1_Error[1]=Pre1_Error[0];
  Pre1_Error[0]=Direct_Parameter;
  Direct_Parameter=Pre1_Error[0]*0.8+Pre1_Error[1]*0.1+Pre1_Error[2]*0.06+Pre1_Error[3]*0.04 ;
  OUT_PWM=(sint16)(Direct_Parameter);
}
