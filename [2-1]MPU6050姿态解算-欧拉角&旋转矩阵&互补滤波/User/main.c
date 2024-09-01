#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"
#include "CalAngles.h"


int16_t AX, AY, AZ, GX, GY, GZ;			//mpu6050的原始数据
float roll,pitch,yaw;					//姿态角

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	
	while(1)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);	
		computeAngles((float)AX, (float)AY, (float)AZ, (float)GX, (float)GY, (float)GZ,&roll,&pitch,&yaw);
			
	    OLED_ShowSignedNum(2, 1, pitch, 5);				
        OLED_ShowSignedNum(3, 1, roll, 5);
		
		//尝试对yaw的漂移进行修正
        OLED_ShowSignedNum(4, 1, yaw/5, 5);//除以5后转动的角度差基本没问题
		yaw+=0.049;//减少时间累计的漂移，不断尝试后得到0.049比较合适
				   //目前大概30s偏移1°

	}
}
