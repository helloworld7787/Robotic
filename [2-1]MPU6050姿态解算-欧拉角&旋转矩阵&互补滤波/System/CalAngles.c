#include "stm32f10x.h"                  // Device header
#include <math.h>
#include <stdio.h>

#define RAD_TO_DEG 57.295779513082320876798154814105 //����ת�Ƕȣ���Ϊ�漰��math�е�һЩ����
#define ALPHA 0.4 //�����˲�ϵ�������ϵ�����õ�0.4�ȽϺ���


//������һ��
void normalize(float* x, float* y, float* z) {
    float norm = sqrt((*x) * (*x) + (*y) * (*y) + (*z) * (*z));
    *x /= norm;
    *y /= norm;
    *z /= norm;
}

//������̬��
void computeAngles(float ax, float ay, float az, float gx, float gy, float gz, float* roll, float* pitch, float* yaw) {
    //���ٶȹ�һ��
    normalize(&ax, &ay, &az);

    //���ü��ٶȼƵ����ݣ�����rollAcc��pitchAcc
    float rollAcc = atan2f(ay, az) * RAD_TO_DEG;
    float pitchAcc = atan2f(-ax, sqrtf(ay * ay + az * az)) * RAD_TO_DEG;

    //�Խ��ٶȻ��֣�ȡʱ����Ϊ0.01��
    float dt = 0.01;
    *roll += gx * dt;
    *pitch += gy * dt;
    *yaw += gz * dt;

    //�����˲��ں�pitch��roll
    *roll = ALPHA * (*roll) + (1.0 - ALPHA) * rollAcc;
    *pitch = ALPHA * (*pitch) + (1.0 - ALPHA) * pitchAcc;
}
