#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"

float angle;

int main(void)
{
	OLED_Init();
	servo_init();
	
	OLED_ShowString(1,1,"Angle:");
	
	while(1)
	{
		for(angle = 0;angle<=180;angle++)
		{
			servo_set_angle(angle);
			OLED_ShowNum(1,7,angle,3);
		}
		Delay_ms(500);
		for(angle = 180;angle>=0;angle--)
		{
			servo_set_angle(angle);
			OLED_ShowNum(1,7,angle,3);
		}
		Delay_ms(500);
	}
}
