#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"

int16_t AX,AY,AZ,GX,GY,GZ;//接收MPU6050数据的变量

int main(void)
{
	//初始化
	MPU6050_Init();
	OLED_Init();
	
	//显示数据类型
	OLED_ShowString(1,1,"Accel: Gyro:");
	
	while(1)
	{
		//获取并显示数据
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);	
	    OLED_ShowSignedNum(2, 1, AX, 5);
		OLED_ShowSignedNum(2, 8, GX, 5);		
        OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
        OLED_ShowSignedNum(4, 1, AZ, 5);
        OLED_ShowSignedNum(4, 8, GZ, 5);
	}
}
