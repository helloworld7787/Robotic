#include "stm32f10x.h"                  // Device header
#include "kalman_cal.h"
#include "MPU6050.h"

#define RAD_TO_DEG 57.295779513082320876798154814105 //弧度转角度，因为涉及到math中的一些函数

float yaw; 					//yaw
float pitch_kalman; 	 	//pitch滤波后数据
float roll_kalman; 		 	//roll滤波后数据					

//向量归一化
void normalize(float* x, float* y, float* z) {
    float norm = sqrt((*x) * (*x) + (*y) * (*y) + (*z) * (*z));
    *x /= norm;
    *y /= norm;
    *z /= norm;
}

//获取最终角度
void Angle_Cal(void)
{
	//获取原始数据
	int16_t AX, AY, AZ, GX, GY, GZ;
	MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);	
	//数据类型转换
	float accel_x = (float)AX;
	float accel_y = (float)AY;
	float accel_z = (float)AZ;
	float gyro_x  = (float)GX;
	float gyro_y  = (float)GY;
	float gyro_z  = (float)GZ;

	//根据加速度计得到角度值
	normalize(&accel_x, &accel_y, &accel_z);
	float rollAcc = atan2f(accel_y, accel_z) * RAD_TO_DEG;
    float pitchAcc = atan2f(-accel_x, sqrtf(accel_y * accel_y + accel_z * accel_z)) * RAD_TO_DEG;
	
	//得到真实角速度值
	//陀螺仪配置量程为±2000deg/s，16位，65536
	//32768对应着0，所以这样进行正负判断
	if(gyro_x<32768) gyro_x=-(gyro_x/16.4);
	if(gyro_x>32768) gyro_x=+(65535-gyro_x)/16.4;
	if(gyro_y<32768) gyro_y=-(gyro_y/16.4);
	if(gyro_y>32768) gyro_y=+(65535-gyro_y)/16.4;
	if(gyro_z<32768) gyro_z=-(gyro_z/16.4);
	if(gyro_z>32768) gyro_z=+(65535-gyro_z)/16.4;
	
	//使用卡尔曼滤波
	Kalman_Cal_Pitch(pitchAcc,gyro_x);
	Kalman_Cal_Roll(rollAcc,gyro_y);
	
	//MPU6050没有磁力计，无法对yaw轴运用卡尔曼滤波算法
	yaw += GZ*0.01;
	yaw+=0.051;
	
}
 
//卡尔曼参数		
static float Q_angle = 0.001;		//角度噪声的协方差
static float Q_gyro  = 0.003;		//角速度噪声的协方差  
static float R_angle = 0.5;			//加速度计测量噪声的协方差
static float dt      = 0.01;		//采样周期，10ms

//卡尔曼滤波pitch轴计算		
void Kalman_Cal_Pitch(float acc,float gyro)
{
	static float Q_bias;	//陀螺仪的偏差
	static float K_0, K_1;	//卡尔曼增益，系数
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };//协方差矩阵P

	//1. 先验估计
	pitch_kalman += (gyro - Q_bias) * dt;
	//2. 预测协方差矩阵
	PP[0][0] = PP[0][0] + Q_angle - (PP[0][1] + PP[1][0])*dt;
	PP[0][1] = PP[0][1] - PP[1][1]*dt;
	PP[1][0] = PP[1][0] - PP[1][1]*dt;
	PP[1][1] = PP[1][1] + Q_gyro;
	//3. 计算卡尔曼增益
	K_0 = PP[0][0] / (PP[0][0] + R_angle);
	K_1 = PP[1][0] / (PP[0][0] + R_angle);
	//4. 计算当前最优化估计值
	pitch_kalman = pitch_kalman + K_0 * (acc - pitch_kalman);
	Q_bias = Q_bias + K_1 * (acc - pitch_kalman);
	//5. 更新协方差矩阵
	PP[0][0] = PP[0][0] - K_0 * PP[0][0];
	PP[0][1] = PP[0][1] - K_0 * PP[0][1];
	PP[1][0] = PP[1][0] - K_1 * PP[0][0];
	PP[1][1] = PP[1][1] - K_1 * PP[0][1];

}

//卡尔曼滤波roll轴计算	
void Kalman_Cal_Roll(float acc,float gyro) 			
{
	static float Q_bias;	//陀螺仪的偏差
	static float K_0, K_1;	//卡尔曼增益，系数
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };//协方差矩阵P
	
	//1. 先验估计
	roll_kalman += (gyro - Q_bias) * dt;
	//2. 预测协方差矩阵
	PP[0][0] = PP[0][0] + Q_angle - (PP[0][1] + PP[1][0])*dt;
	PP[0][1] = PP[0][1] - PP[1][1]*dt;
	PP[1][0] = PP[1][0] - PP[1][1]*dt;
	PP[1][1] = PP[1][1] + Q_gyro;
	//3. 计算卡尔曼增益
	K_0 = PP[0][0] / (PP[0][0] + R_angle);
	K_1 = PP[1][0] / (PP[0][0] + R_angle);
	//4. 计算当前最优化估计值
	roll_kalman = roll_kalman + K_0 * (acc - roll_kalman);
	Q_bias = Q_bias + K_1 * (acc - roll_kalman);
	//5. 更新协方差矩阵
	PP[0][0] = PP[0][0] - K_0 * PP[0][0];
	PP[0][1] = PP[0][1] - K_0 * PP[0][1];
	PP[1][0] = PP[1][0] - K_1 * PP[0][0];
	PP[1][1] = PP[1][1] - K_1 * PP[0][1];
}

