#ifndef __LQ_BUZZ_H_
#define __LQ_BUZZ_H_


#define BUZZ P33_8

typedef enum
{
    BUZZ0=0,  //���İ���BUZZ

} BUZZn_e;

typedef enum
{
    BUZZ_ON=0,  //��
    BUZZ_OFF=1, //����
    BUZZ_RVS=2, //��ת
}BUZZs_e;
void GPIO_BUZZ_Init(void);
void BUZZ_Ctrl(BUZZn_e BUZZno, BUZZs_e sta);


#endif
