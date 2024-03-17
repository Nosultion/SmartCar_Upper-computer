#ifndef __LQ_BUZZ_H_
#define __LQ_BUZZ_H_


#define BUZZ P33_8

typedef enum
{
    BUZZ0=0,  //核心板上BUZZ

} BUZZn_e;

typedef enum
{
    BUZZ_ON=0,  //响
    BUZZ_OFF=1, //不响
    BUZZ_RVS=2, //反转
}BUZZs_e;
void GPIO_BUZZ_Init(void);
void BUZZ_Ctrl(BUZZn_e BUZZno, BUZZs_e sta);


#endif
