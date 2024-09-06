#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "sys.h"
#include "Servo.h"
#include "Serial.h"

float pitch,roll,yaw;
uint8_t RxData;

int main(void)
{
	mpu_dmp_init();
	servo_init();
	Serial_Init();
	
	servo1_set_angle(0);
	servo2_set_angle(90);
	
	delay_ms(1000);
	
	
	while(1)
	{
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		RxData = Serial_GetRxData();
		if(RxData == 1)
		{
			servo1_set_angle(roll+90);
			servo2_set_angle(yaw+90);
		}
		else if(RxData == 2)
		{
			servo1_set_angle(90-roll-30);
			servo2_set_angle(90-yaw);
		}
		else
		{
			servo1_set_angle(roll+90);
			servo2_set_angle(yaw+90);
		}

	}
}
