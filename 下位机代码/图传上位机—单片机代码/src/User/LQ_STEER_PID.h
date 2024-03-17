#ifndef _SRC_USER_LQ_STEER_PID_H
#define _SRC_USER_LQ_STEER_PID_H


extern float Angle_kp;    //转向环PID（外环）20.5
extern float Angle_ki;
extern float Angle_kd;
extern float value;

void Angle_Get(void);
void AngleControl(void);
void Angle_out(void);

#endif


