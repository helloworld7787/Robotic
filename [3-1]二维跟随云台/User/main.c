#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "sys.h"
#include "Servo.h"

float pitch,roll,yaw;

int main(void)
{
//	OLED_Init();
	mpu_dmp_init();
	servo_init();
	
//	OLED_ShowString(1,1,"pitch:");
//	OLED_ShowString(2,1,"roll:");
//	OLED_ShowString(3,1,"yaw:");
	
	servo1_set_angle(0);
	servo2_set_angle(90);
	
	delay_ms(1000);
	while(1)
	{
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		servo1_set_angle(roll+90);
		servo2_set_angle(yaw+90);
//		OLED_ShowSignedNum(1,7,pitch,5);
//		OLED_ShowSignedNum(2,7,roll,5);
//		OLED_ShowSignedNum(3,7,yaw,5);
		
	}
}
