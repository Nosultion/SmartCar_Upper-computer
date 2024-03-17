#ifndef __LQ_INFRARED_H_
#define __LQ_INFRARED_H_

#define INFRARED P33_13

void GPIO_Infrared_Init(void);
unsigned char Infrared_Read(void);

#endif
