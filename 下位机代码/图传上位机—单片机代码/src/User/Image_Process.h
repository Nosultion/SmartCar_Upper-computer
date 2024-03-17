/*
 * Image_Process.h
 *
 *  Created on: 2023年4月26日
 *      Author: HSC
 */
#include <stdbool.h>
#include <stdint.h>

#ifndef SRC_USER_IMAGE_PROCESS_H_
#define SRC_USER_IMAGE_PROCESS_H_

#define IMG_WIDTH 160
#define IMG_HEIGHT 120
#define MAX_LINES 100

#define WIDTH 160
#define HEIGHT 120
#define WINDOW_WIDTH 20
#define WINDOW_HEIGHT 10
#define STEP 5

#define WIDTH 160
#define HEIGHT 120
#define RHO_MAX (int)(sqrt(HEIGHT * HEIGHT + WIDTH * WIDTH))
#define THETA_STEPS 180
//#define THRESHOLD 50
#define LOCAL_THRESHOLD 20
#define M_PI 3.14

// 双阈值
#define HIGH_THRESHOLD 80
#define LOW_THRESHOLD 20

//void sobel_operator(unsigned char Image_Use[IMG_HEIGHT][IMG_WIDTH], unsigned char Binary_Image[IMG_HEIGHT][IMG_WIDTH]);

//void sliding_window(unsigned char Bin_Image[HEIGHT][WIDTH], uint8_t ImageSide[HEIGHT][2]);

//void hufu(unsigned char Bin_Image[HEIGHT][WIDTH], uint8_t ImageSide[HEIGHT][2]);

void detect_lane_edges(unsigned char src[HEIGHT][WIDTH], uint8_t Imageside[][2]) ;

#endif /* SRC_USER_IMAGE_PROCESS_H_ */
