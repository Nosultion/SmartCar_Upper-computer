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
#ifndef SRC_APPSW_TRICORE_USER_LQ_IMAGEPROCESS_H_
#define SRC_APPSW_TRICORE_USER_LQ_IMAGEPROCESS_H_
#include "include.h"
#include <Platform_Types.h>
#include <stdio.h>
#include <stdint.h>
#include "LQ_CAMERA.h"


extern uint8_t UpdowmSide[2][LCDW];
extern uint8_t ImageSide[LCDH][2];
extern uint8_t ImageSide_last[LCDH][2];
extern uint8_t Roadwide[LCDH];
extern uint8_t OpeningRoad_Flag ; //��·��־λ


void CameraCar(void);//��⺯��

//���ұ��߻����ж�
uint8_t RoundaboutGetArc(uint8_t imageSide[LCDH][2], uint8_t status, uint8_t num,uint8_t* index);

//���ߴ���
void RoadNoSideProcess(uint8_t imageInput[LCDH][LCDW], uint8_t imageOut[LCDH][2], uint8_t mode, uint8_t lineIndex);

//��ʾ
void TFT_Show_Camera_Info(void);

//��ȡ���±���
uint8_t UpdownSideGet(uint8_t imageInput[LCDH][LCDW], uint8_t imageOut[2][LCDW]);

//�������±���
void TFTSPI_BinRoad_UpdownSide(uint8_t imageOut[2][LCDW]);

//��ȡ���ұ���
uint8_t ImageGetSide(uint8_t imageInput[LCDH][LCDW], uint8_t imageOut[LCDH][2], uint8_t imageOut_last[LCDH][2]);

//�������ұ���
void TFTSPI_BinRoadSide(uint8_t imageOut[LCDH][2]);

//��ȡ�������
uint8_t GetRoadWide(uint8_t imageInput[LCDH][2] ,uint8_t imageOut[LCDH]);

//���»�ȡ��/�ұ���
void RoundaboutGetSide(uint8_t imageInput[LCDH][LCDW], uint8_t imageSide[LCDH][2], uint8_t status);

//���»�ȡ��/�±���
void Roundabout_Get_UpDowmSide(uint8_t imageInput[LCDH][LCDW], uint8_t imageSide[2][LCDW], uint8_t status);

//���ߴ���
void ImageAddingLine(uint8_t imageSide[LCDH][2], uint8_t status, uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY);

void Round_Process(uint8_t *L_Flag,uint8_t *R_Flag,uint8_t *M_Flag,uint8_t imageSide[LCDH][2],uint8_t Roadwide[120],uint8_t UpImageSide[2][160],uint8_t *miss_Flag);

//��ȡ����ƫ��
int16_t RoadGetSteeringError(uint8_t imageSide[LCDH][2], uint8_t lineIndex);

void Change_V(void);
//��ȡ�ϵ�
void X_Find_Point(uint8_t start_point,uint8_t end_point,uint8_t UpdowmSide[2][LCDW], uint8_t pointup[2]);
void X_Find_Point_1(uint8_t start_point,uint8_t end_point,uint8_t UpdowmSide[2][LCDW], uint8_t pointup[2]);
void Y_Find_Point(uint8_t start_point,uint8_t end_point,uint8_t ImageSide[LCDH][2], uint8_t pointup[2]);
void Y_Find_Point_1(uint8_t start_point,uint8_t end_point,uint8_t ImageSide[LCDH][2], uint8_t pointup[2]);

//��ȡ�յ�
void Find_Point_Left_1(uint8_t start_point,uint8_t end_point,uint8_t ImageSide[LCDH][2], uint8_t pointup[2]);
void Find_Point_Left_2(uint8_t start_point,uint8_t end_point,uint8_t ImageSide[LCDH][2], uint8_t pointup[2]);
void Find_Point_Right_1(uint8_t start_point,uint8_t end_point,uint8_t ImageSide[LCDH][2], uint8_t pointup[2]);
void Find_Point_Right_2(uint8_t start_point,uint8_t end_point,uint8_t ImageSide[LCDH][2], uint8_t pointup[2]);

void Crossroad_Find(uint8_t UpdowmSide [2][LCDW],uint8_t imageSide[LCDH][2], uint8_t Roadwide[LCDH],uint8_t * Flag);

#endif /* SRC_APPSW_TRICORE_USER_LQ_IMAGEPROCESS_H_ */
