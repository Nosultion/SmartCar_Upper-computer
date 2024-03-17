/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】chiusir
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

#include "LQ_PID.h"
extern float Angularz_speed;//水平方向角速度
/*************************************************************************
 *  函数名称：float constrain_float(float amt, float low, float high)
 *  功能说明：限幅函数
 *  参数说明：
  * @param    amt   ： 参数
  * @param    low   ： 最低值
  * @param    high  ： 最高值
 *  函数返回：无
 *  修改时间：2020年4月1日
 *  备    注：
 *************************************************************************/
float constrain_float(float amt, float low, float high)
{
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}
// pid参数值修改
pid_param_t LSpeed_PID;
pid_param_t RSpeed_PID;
pid_param_t ESpeed_PID;
void Pid_Value(void)
{
    //速度V                  //正向100转速    50(/7)      60(/7)       70(/7)
    //速度环左轮电机PID参数
    LSpeed_PID.kp = 170;    //170
    LSpeed_PID.ki = 15;      //15
    LSpeed_PID.kd = 0;      //15
    LSpeed_PID.imax = 5000;
    //摄像头方向环PID参数
    RSpeed_PID.kp = 40;    //45  95
    RSpeed_PID.ki = 0;   //4   5.6
    RSpeed_PID.kd = 0;     //0
    //电磁方向环PID参数
    ESpeed_PID.kp = 75;    //75  //30        205             215
    ESpeed_PID.ki = 6;    //0      1.0         1.0             1.0
    ESpeed_PID.kd = 0;    //0       0.5         0.5             0.5
}
// pid参数初始化函数
void PidInit(pid_param_t * pid)
{
	pid->kp        = 0;
	pid->ki        = 0;
	pid->kd        = 0;
	pid->imax      = 0;
	pid->out_p     = 0;
	pid->out_i     = 0;
	pid->out_d     = 0;
	pid->out       = 0;
	pid->integrator= 0;
	pid->last_error= 0;
	pid->last_derivative   = 0;
	pid->last_t    = 0;
}

/*************************************************************************
 *  函数名称：float constrain_float(float amt, float low, float high)
 *  功能说明：pid位置式控制器输出
 *  参数说明：
  * @param    pid     pid参数
  * @param    error   pid输入误差
 *  函数返回：PID输出结果
 *  修改时间：2020年4月1日
 *  备    注：只需调p d
 *************************************************************************/
float PidLocCtrl(pid_param_t * pid, float error)
{
//    float A=90;
//    float B=60;
//    float F=0;
//    if((error>0&&error<=B)||(error<0&&-error<=B))
//    {
//        F=1;
//    }
//    if(error>0&&B<error&&error<=A+B)
//    {
//        F=(A-error+B)/A;
//    }
//    if(error<0&&B<(-error)&&(-error)<=A+B)
//    {
//        F=(A+error+B)/A;
//    }
//    else
//    {
//        F=0;
//    }
//
//      pid->out_p = pid->kp * error;
//      pid->out_i = pid->ki * Angularz_speed * F;
//
//      pid->out_d = pid->kd * (error - pid->last_error);

    /* 累积误差 */
    pid->integrator += error;

    /* 误差限幅 */
    constrain_float(pid->integrator, -pid->imax, pid->imax);


    pid->out_p = pid->kp * error;
    //pid->out_i = pid->ki * pid->integrator;
    pid->out_i = pid->ki * Angularz_speed;
    pid->out_d = pid->kd * (error - pid->last_error);

    pid->last_error = error;

    pid->out = pid->out_p + pid->out_i + pid->out_d;

    return pid->out;
}
/*************************************************************************
 *  函数名称：float constrain_float(float amt, float low, float high)
 *  功能说明：pid增量式控制器输出
 *  参数说明：
  * @param    pid     pid参数
  * @param    error   pid输入误差
 *  函数返回：PID输出结果   注意输出结果已经包涵了上次结果
 *  修改时间：2020年4月1日
 *  备    注：只需调 i p
 *************************************************************************/
float PidIncCtrl(pid_param_t * pid, float error)
{
//系数抗饱和
//    float w=0.0005;//系数w给0.3-3
//    if((pid->out_i>0&&pid->out_i>w*pid->out_p/pid->out_i)||(pid->out_i<0&&pid->out_i<(-1)*w*pid->out_p/pid->out_i))
//    {
//        pid->out_i = 0;
//    }
//    else
//    {
//        pid->out_i = pid->ki * error;
//    }

//    float A=300;
//    float B=260;
//    float F=0;
//    if((error>0&&error<=B)||(error<0&&-error<=B))
//    {
//        F=1;
//    }
//    if(error>0&&B<error&&error<=A+B)
//    {
//        F=(A-error+B)/A;
//    }
//    if(error<0&&B<(-error)&&(-error)<=A+B)
//    {
//        F=(A+error+B)/A;
//    }
//    else
//    {
//        F=0;
//    }
//    pid->out_p = pid->kp * (error - pid->last_error);
//    pid->out_i = pid->ki * error*F;
//    pid->out_d = pid->kd * ((error - pid->last_error) - pid->last_derivative);
    pid->out_p = pid->kp * (error - pid->last_error);
    pid->out_i = pid->ki * error;
    pid->out_d = pid->kd * ((error - pid->last_error) - pid->last_derivative);

    pid->last_derivative = error - pid->last_error;
    pid->last_error = error;

    pid->out += pid->out_p + pid->out_i + pid->out_d;

    return pid->out;
}

