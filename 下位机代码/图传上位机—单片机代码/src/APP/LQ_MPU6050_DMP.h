/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�F3277���İ��ĸ��
����    д������Ƽ�
��E-mail  ��chiusir@163.com
������汾��V1.0 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2021��3��31�գ��������£����ע���°棡
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��IDE��IAR7.8 KEIL5.24�����ϰ汾
��Target �� MM32F3277
��SYS PLL�� 120MHz Ƶ��̫�߿����޷�����system_mm32f327x.c
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
=================================================================
���������MPU6050DMP���ܣ�������������վ4.34��ʾ����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_MPU6050_DMP_
#define _LQ_MPU6050_DMP_




extern short   gyro[3], accel[3], sensors;

extern float Roll,Pitch,Yaw,gyro_Roll,gyro_Pitch,gyro_Yaw ;//XYZ������ת�Ƕȡ����ٶ�
unsigned short inv_row_2_scale(const signed char *row);

int LQ_DMP_Init(void);
int LQ_DMP_Read(void);
void Test_LQDMP(void);

extern int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat);
extern int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf);
//extern void delayms(u16 nms);
//extern void delayus(u16 nus);

// ��ʼ��ʹ��
int mpu_set_gyro_fsr(unsigned short fsr);
int mpu_set_accel_fsr(unsigned char fsr);
int mpu_set_lpf(unsigned short lpf);
int mpu_set_sample_rate(unsigned short rate);
int dmp_set_orientation(unsigned short orient);
int mpu_write_mem(unsigned short mem_addr, unsigned short length, unsigned char *lqda);
int mpu_read_mem(unsigned short mem_addr, unsigned short length, unsigned char *lqda);
int mpu_reset_fifo_in_dmp_enable_feature(void);
int mpu_reset_fifo_in_mpu_set_dmp_state(void);

// ��ֵʹ��
int dmp_read_fifo(short *gyro, short *accel, long *quat, short *sensors, unsigned char *more);

#endif
