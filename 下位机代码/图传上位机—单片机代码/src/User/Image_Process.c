/*
 * Image_Process.c
 *
 *  Created on: 2023年4月26日
 *      Author: HSC
 */
#include "Image_Process.h"
#include <Platform_Types.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <time.h>

unsigned short THRESHOLD=100;
//void sobel_operator(unsigned char Image_Use[IMG_HEIGHT][IMG_WIDTH], unsigned char Binary_Image[IMG_HEIGHT][IMG_WIDTH]) {
//    int gx[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
//    int gy[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
//
//    for (int y = 0; y < IMG_HEIGHT; y++) {
//        for (int x = 0; x < IMG_WIDTH; x++) {
//            int gradient_x = 0, gradient_y = 0;
//
//            if (y == 0 || y == IMG_HEIGHT - 1 || x == 0 || x == IMG_WIDTH - 1) {
//                Binary_Image[y][x] = 0;
//            } else {
//                for (int k1 = -1; k1 <= 1; k1++) {
//                    for (int k2 = -1; k2 <= 1; k2++) {
//                        gradient_x += Image_Use[y + k1][x + k2] * gx[k1 + 1][k2 + 1];
//                        gradient_y += Image_Use[y + k1][x + k2] * gy[k1 + 1][k2 + 1];
//                    }
//                }
//
//                int gradient_magnitude = sqrt(gradient_x * gradient_x + gradient_y * gradient_y);
//                Binary_Image[y][x] = (gradient_magnitude > THRESHOLD) ? 0 : 255;
//            }
//        }
//    }
//}
//
//
////基于滑动窗口的方法进行边缘检测
//void sliding_window(unsigned char Bin_Image[HEIGHT][WIDTH], uint8_t ImageSide[HEIGHT][2]) {
//    int y = HEIGHT - 1 - WINDOW_HEIGHT;
//
//    // 1. 在图像底部选择一个窗口的初始位置
//    int left_x = 0;
//    int right_x = WIDTH - 1 - WINDOW_WIDTH;
//
//    while (y >= 0) {
//        int left_sum = 0;
//        int right_sum = 0;
//        int max_left_sum = 0;
//        int max_right_sum = 0;
//        int max_left_x = left_x;
//        int max_right_x = right_x;
//
//        // 2. 在窗口内部查找边界
//        for (int x = 0; x <= (WIDTH - WINDOW_WIDTH); x++) {
//            left_sum = 0;
//            right_sum = 0;
//            for (int yy = y; yy < y + WINDOW_HEIGHT; yy++) {
//                for (int xx = x; xx < x + WINDOW_WIDTH; xx++) {
//                    left_sum += Bin_Image[yy][xx];
//                    right_sum += Bin_Image[yy][WIDTH - 1 - xx];
//                }
//            }
//
//            if (left_sum > max_left_sum) {
//                max_left_sum = left_sum;
//                max_left_x = x;
//            }
//
//            if (right_sum > max_right_sum) {
//                max_right_sum = right_sum;
//                max_right_x = WIDTH - 1 - x;
//            }
//        }
//
//        // 3. 将找到的边界点存储在ImageSide数组中
//        for (int yy = y; yy < y + WINDOW_HEIGHT; yy++) {
//            ImageSide[yy][0] = max_left_x;
//            ImageSide[yy][1] = max_right_x;
//        }
//
//        // 4. 根据找到的边界点移动窗口
//        left_x = max_left_x;
//        right_x = max_right_x;
//        y -= STEP;
//
//        // 5. 如果窗口到达图像顶部，结束处理
//        if (y < 0) {
//            break;
//        }
//    }
//}
//
//
////基于局部搜索的霍夫变换进行边缘检测
//extern uint8_t hough_space[100][45];
//void hufu(unsigned char Bin_Image[HEIGHT][WIDTH], uint8_t ImageSide[HEIGHT][2]){
//    printf("1");
//   int step_size = 5; // 可根据需要调整
//   for (int y = 0; y < HEIGHT; y += step_size) {
//       for (int x = 0; x < WIDTH; x += step_size) {
//
//           // 检查局部区域的边缘强度
//           int edge_sum = 0;
//           for (int dy = 0; dy < step_size; dy++) {
//               for (int dx = 0; dx < step_size; dx++) {
//                   edge_sum += Bin_Image[y + dy][x + dx];
//               }
//           }
//
//           // 2. 对边缘强度较高的区域进行霍夫变换
//           if (edge_sum > THRESHOLD) {
//               // 在此区域中执行霍夫变换并找到左右边界
//               for (int yy = y; yy < y + step_size; yy++) {
//                   for (int xx = x; xx < x + step_size; xx++) {
//                       if (Bin_Image[yy][xx] > LOCAL_THRESHOLD) {
//                           for (int theta = 0; theta < THETA_STEPS; theta++) {
//                               double theta_rad = (double)theta / THETA_STEPS * M_PI;
//                               int rho = (int)(xx * cos(theta_rad) + yy * sin(theta_rad));
//                               hough_space[rho + RHO_MAX][theta]++;
//                           }
//                       }
//                   }
//               }
//           }
//       }
//   }
//
//   // 3. 根据霍夫空间中的局部最大值找到赛道的左右边界
//   int threshold_votes = 10; // 可根据需要调整
//   for (int y = 0; y < HEIGHT; y++) {
//       int max_left = 0;
//       int max_right = 0;
//       int max_votes_left = threshold_votes;
//       int max_votes_right = threshold_votes;
//
//       for (int x = 0; x < WIDTH / 2; x++) {
//           for (int theta = 0; theta < THETA_STEPS; theta++) {
//               double theta_rad = (double)theta / THETA_STEPS * M_PI;
//               int rho = (int)(x * cos(theta_rad) + y * sin(theta_rad));
//               if (hough_space[rho + RHO_MAX][theta] > max_votes_left) {
//                   max_votes_left = hough_space[rho + RHO_MAX][theta];
//                     max_left = x;
//                 }
//             }
//         }
//
//         for (int x = WIDTH / 2; x < WIDTH; x++) {
//             for (int theta = 0; theta < THETA_STEPS; theta++) {
//                 double theta_rad = (double)theta / THETA_STEPS * M_PI;
//                 int rho = (int)(x * cos(theta_rad) + y * sin(theta_rad));
//                 if (hough_space[rho + RHO_MAX][theta] > max_votes_right) {
//                     max_votes_right = hough_space[rho + RHO_MAX][theta];
//                     max_right = x;
//                 }
//             }
//         }
//
//         // 4. 将左右边界的点存储在ImageSide数组中
//         ImageSide[y][0] = max_left;
//         ImageSide[y][1] = max_right;
//     }
//}

//Canny边缘检测

#define SQRT_LUT_SIZE 255
#define ATAN2_LUT_SIZE 125
#define ATAN2_SCALE_FACTOR 25

char sqrt_lut[SQRT_LUT_SIZE];
char atan2_lut[ATAN2_LUT_SIZE][ATAN2_LUT_SIZE];

void generate_sqrt_lut() {
    for (char i = 0; i < SQRT_LUT_SIZE; i++) {
        sqrt_lut[i] = sqrt(i);
    }
}

void generate_atan2_lut() {
    for (char y = 0; y < ATAN2_LUT_SIZE; y++) {
        for (char x = 0; x < ATAN2_LUT_SIZE; x++) {
            atan2_lut[y][x] = atan2(y, x) * 180.0 / M_PI;
        }
    }
}

char lut_sqrt(char x) {
    if (x < 0 || x >= SQRT_LUT_SIZE) {
        return -1;
    }

    return sqrt_lut[x];
}

char lut_atan2(char y, char x) {
    char scaled_y = y * ATAN2_SCALE_FACTOR / (abs(y) + abs(x));
    char scaled_x = x * ATAN2_SCALE_FACTOR / (abs(y) + abs(x));

    if (scaled_y < 0) {
        scaled_y += ATAN2_LUT_SIZE - 1;
    }
    if (scaled_x < 0) {
        scaled_x += ATAN2_LUT_SIZE - 1;
    }

    return atan2_lut[scaled_y][scaled_x];
}
//计算图像梯度和方向
void gradient_and_direction_lut(unsigned char src[HEIGHT][WIDTH],char gradient[120][WIDTH], char direction[120][WIDTH]) {
    char gx, gy;

    for (int y = 1; y < HEIGHT - 1; y++) {
        for (int x = 1; x < WIDTH - 1; x++) {
            gx = src[y - 1][x + 1] + 2 * src[y][x + 1] + src[y + 1][x + 1] -
                 src[y - 1][x - 1] - 2 * src[y][x - 1] - src[y + 1][x - 1];
            gy = src[y + 1][x - 1] + 2 * src[y + 1][x] + src[y + 1][x + 1] -
                 src[y - 1][x - 1] - 2 * src[y - 1][x] - src[y - 1][x + 1];

            gradient[y][x] = lut_sqrt(gx * gx + gy * gy);
            direction[y][x] = lut_atan2(gy, gx);
        }
    }
}


//非极大值抑制
void non_max_suppression(float gradient[][WIDTH], float direction[][WIDTH], unsigned char dst[][WIDTH]) {
    for (int y = 1; y < HEIGHT - 1; y++) {
        for (int x = 1; x < WIDTH - 1; x++) {
            float angle = direction[y][x];
            float q = 0, r = 0;

            if ((angle > -22.5 && angle <= 22.5) || (angle > 157.5 || angle <= -157.5)) {
                q = gradient[y][x - 1];
                r = gradient[y][x + 1];
            } else if ((angle > 22.5 && angle <= 67.5) || (angle > -157.5 && angle <= -112.5)) {
                q = gradient[y - 1][x + 1];
                r = gradient[y + 1][x - 1];
            } else if ((angle > 67.5 && angle <= 112.5) || (angle > -112.5 && angle <= -67.5)) {
                q = gradient[y - 1][x];
                r = gradient[y + 1][x];
            } else if ((angle > 112.5 && angle <= 157.5) || (angle > -67.5 && angle <= -22.5)) {
                q = gradient[y - 1][x - 1];
                r = gradient[y + 1][x + 1];
            }

            if (gradient[y][x] >= q && gradient[y][x] >= r) {
                dst[y][x] = (unsigned char)(gradient[y][x]);
            } else {
                dst[y][x] = 0;
            }
        }
    }
}
//执行边缘连接
void edge_linking(int x, int y, unsigned char src[HEIGHT][WIDTH], unsigned char dst[][WIDTH]) {
    int neighbor[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    dst[y][x] = 255;

    for (int i = 0; i < 8; i++) {
        int nx = x + neighbor[i][1];
        int ny = y + neighbor[i][0];

        if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && src[ny][nx] != 255 && src[ny][nx] > LOW_THRESHOLD) {
            edge_linking(nx, ny, src, dst);
        }
    }
}
//执行双阈值连接
void double_threshold(unsigned char src[HEIGHT][WIDTH], unsigned char dst[][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (src[y][x] >= HIGH_THRESHOLD) {
                edge_linking(x, y, src, dst);
            } else {
                dst[y][x] = 0;
            }
        }
    }
}
//Canny边缘检测
void detect_lane_edges(unsigned char src[HEIGHT][WIDTH], uint8_t Imageside[][2]) {
    //float gaussian_filtered[HEIGHT][WIDTH];
    char gradient[HEIGHT][WIDTH];
    char direction[HEIGHT][WIDTH];
    //unsigned char non_max_suppressed[HEIGHT][WIDTH];
    unsigned char thresholded[HEIGHT][WIDTH];
    printf("0");
    // 高斯滤波
    //gaussian_filter(src, gaussian_filtered);

//    // 计算梯度幅值与方向
    gradient_and_direction_lut(src,gradient,direction);
    printf("1");
//    // 非极大值抑制
//    non_max_suppression(gradient, direction, non_max_suppressed);
//
//    // 双阈值连接
//    double_threshold(non_max_suppressed, thresholded);

    // 检测左右边线
    for (int row = 0; row < HEIGHT / 2; row++) {
        Imageside[row][0] = 0;
        Imageside[row][1] = 159;
        for (int col = 0; col < WIDTH; col++) {
            if (!thresholded[row][col]) {
                if (Imageside[row][0] == 0) {
                    Imageside[row][0] =(uint8_t) col;
                }
                else if(Imageside[row][1] == 159)
                {
                    Imageside[row][1] =(uint8_t) col;
                }
            }
        }
    }
}
