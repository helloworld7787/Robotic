#include "stm32f10x.h"                  // Device header
#include <math.h>
#include <stdio.h>

#define RAD_TO_DEG 57.295779513082320876798154814105 //弧度转角度，因为涉及到math中的一些函数
#define ALPHA 0.4 //互补滤波系数，不断调整后得到0.4比较合适


//向量归一化
void normalize(float* x, float* y, float* z) {
    float norm = sqrt((*x) * (*x) + (*y) * (*y) + (*z) * (*z));
    *x /= norm;
    *y /= norm;
    *z /= norm;
}

//计算姿态角
void computeAngles(float ax, float ay, float az, float gx, float gy, float gz, float* roll, float* pitch, float* yaw) {
    //加速度归一化
    normalize(&ax, &ay, &az);

    //利用加速度计的数据，计算rollAcc和pitchAcc
    float rollAcc = atan2f(ay, az) * RAD_TO_DEG;
    float pitchAcc = atan2f(-ax, sqrtf(ay * ay + az * az)) * RAD_TO_DEG;

    //对角速度积分（取时间间隔为0.01）
    float dt = 0.01;
    *roll += gx * dt;
    *pitch += gy * dt;
    *yaw += gz * dt;

    //互补滤波融合pitch、roll
    *roll = ALPHA * (*roll) + (1.0 - ALPHA) * rollAcc;
    *pitch = ALPHA * (*pitch) + (1.0 - ALPHA) * pitchAcc;
}
