/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��chiusir
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

#include "LQ_PID.h"
extern float Angularz_speed;//ˮƽ������ٶ�
/*************************************************************************
 *  �������ƣ�float constrain_float(float amt, float low, float high)
 *  ����˵�����޷�����
 *  ����˵����
  * @param    amt   �� ����
  * @param    low   �� ���ֵ
  * @param    high  �� ���ֵ
 *  �������أ���
 *  �޸�ʱ�䣺2020��4��1��
 *  ��    ע��
 *************************************************************************/
float constrain_float(float amt, float low, float high)
{
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}
// pid����ֵ�޸�
pid_param_t LSpeed_PID;
pid_param_t RSpeed_PID;
pid_param_t ESpeed_PID;
void Pid_Value(void)
{
    //�ٶ�V                  //����100ת��    50(/7)      60(/7)       70(/7)
    //�ٶȻ����ֵ��PID����
    LSpeed_PID.kp = 170;    //170
    LSpeed_PID.ki = 15;      //15
    LSpeed_PID.kd = 0;      //15
    LSpeed_PID.imax = 5000;
    //����ͷ����PID����
    RSpeed_PID.kp = 40;    //45  95
    RSpeed_PID.ki = 0;   //4   5.6
    RSpeed_PID.kd = 0;     //0
    //��ŷ���PID����
    ESpeed_PID.kp = 75;    //75  //30        205             215
    ESpeed_PID.ki = 6;    //0      1.0         1.0             1.0
    ESpeed_PID.kd = 0;    //0       0.5         0.5             0.5
}
// pid������ʼ������
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
 *  �������ƣ�float constrain_float(float amt, float low, float high)
 *  ����˵����pidλ��ʽ���������
 *  ����˵����
  * @param    pid     pid����
  * @param    error   pid�������
 *  �������أ�PID������
 *  �޸�ʱ�䣺2020��4��1��
 *  ��    ע��ֻ���p d
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

    /* �ۻ���� */
    pid->integrator += error;

    /* ����޷� */
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
 *  �������ƣ�float constrain_float(float amt, float low, float high)
 *  ����˵����pid����ʽ���������
 *  ����˵����
  * @param    pid     pid����
  * @param    error   pid�������
 *  �������أ�PID������   ע���������Ѿ��������ϴν��
 *  �޸�ʱ�䣺2020��4��1��
 *  ��    ע��ֻ��� i p
 *************************************************************************/
float PidIncCtrl(pid_param_t * pid, float error)
{
//ϵ��������
//    float w=0.0005;//ϵ��w��0.3-3
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

