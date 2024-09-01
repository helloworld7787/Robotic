#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "kalman_cal.h"
#include "Servo.h"


int main(void)
{
	OLED_Init();
	MPU6050_Init();
	servo_init();
	
	servo1_set_angle(0);
	servo2_set_angle(90);
	
	while(1)
	{	
		Angle_Cal();
		servo1_set_angle(roll_kalman+90);
		servo2_set_angle(yaw/5+90);
		OLED_ShowSignedNum(2,1,yaw/5,5);
        OLED_ShowSignedNum(3, 1, pitch_kalman, 5);
		OLED_ShowSignedNum(4, 1, roll_kalman, 5);
	}
}
