/*
 * TFT_Key_control.h
 *
 *  Created on: 2023��4��19��
 *      Author: HSC
 */

#ifndef SRC_USER_TFT_KEY_CONTROL_H_
#define SRC_USER_TFT_KEY_CONTROL_H_

#include "LQ_PID.h"

#define IMG_HEIGHT 120
#define IMG_WIDTH 160

#define ROWS 80
#define COLS 120
#define STEP_COL 5

void menu(void);
void show_PID(void);//��ʾPIDҳ��
void show_Error(void);//��ʾ���ҳ��
void show_Inductance(void);//��ʾ���ҳ��
void show_Sign(void); //��ʾ��־λ
void show_Dynamic(void);//��ʾ��̬��ֵ
void show_Image(void);//��ʾͼ��
void EEPROM_input(void);//EEPROM����д��
void EEPROM_output(void);//EEPROM���ݶ���
void menu_flag_function(void);//�˵���־����������һ�������ת
void arrow(int *num,int line_begin,int line_end);//��ͷ��ʾ����
void PID_Value_Change(void);//PID��������
void ChangeValue_float(float *object);//float���͵Ĳ�������
void ChangeValue_int(int *object);//int���͵Ĳ�������
void ChangeValue_thres(int *object);
void Dynamic_Value_Change(void);//��̬��ֵ��������
void Change_Obstacle_turn_flag(void);
void Change_Speed_turn_flag(void);
void ChangeValue_int_fifty(int *object);//int���͵Ĳ�������

void TFT_show(void);
void key_control(void);
void UART_Test(void);

void compressImage(unsigned char image[120][188], unsigned char image_min[60][80]);

// �����Թ�Ѳ��
void findline_lefthand_adaptive(unsigned char Image_Use[IMAGEH][IMAGEW], int block_size , int clip_value, int x, int y, unsigned char Image_Use_BLACK[IMAGEH][IMAGEW], int *num);
// �����Թ�Ѳ��
void findline_righthand_adaptive(unsigned char Image_Use[IMAGEH][IMAGEW], int block_size , int clip_value, int x, int y, unsigned char Image_Use_BLACK[IMAGEH][IMAGEW], int *num);
void process_image(unsigned char Image_Use[IMAGEH][IMAGEW]);

//���ʼ���
float computeCurvature_L(uint8_t p1, uint8_t  p2, uint8_t p3);

//ɨ�����ұ���
void findMaxDifference(unsigned char image[ROWS][COLS], unsigned char max[COLS / STEP_COL]);
void find_sideimage(unsigned char image[80][120], unsigned char max[COLS / STEP_COL],uint8_t imageOut[LCDH][2]);
//ɨ���ϱ���
void find_upsideimage(unsigned char image[80][120],uint8_t imageOut[2][160]);
//Ԫ���ж�
void judge_open(void);
void judge_Obstacle(void);
void judge_slope(void);
void judge_Park(void);
void judge_Circle(void);

void find_track_boundaries(unsigned char Image_Use[IMG_HEIGHT][IMG_WIDTH], uint8_t ImageSide[IMG_HEIGHT][2]);
void apply_gaussian_filter(unsigned char Image_Use[IMG_HEIGHT][IMG_WIDTH], unsigned char Filtered_Image[IMG_HEIGHT][IMG_WIDTH]) ;

float Get_Data(unsigned char *DataBuff);
void USART_PID_Adjust(unsigned char *DataBuff);

void Voltage_Get(void);

void Get_aac(void);

int Left_Low_Pass_Filter(short int speedL,int Feedback_Speed,long int Feedback_Speed_tmp);
int Right_Low_Pass_Filter(short int speedR,int Feedback_Speed,long int Feedback_Speed_tmp);

int updateDeviation(int* filter, int filterSize);

void lukuangudge(void);
void lkcongzhongjiansaomiao(void);
int regression(int startline,int endline);

#endif /* SRC_USER_TFT_KEY_CONTROL_H_ */
