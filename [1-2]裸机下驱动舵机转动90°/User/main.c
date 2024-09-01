#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "Key.h"

uint8_t KeyNum;
uint8_t flag = 1;
float angle;

int main(void)
{
	OLED_Init();
	servo_init();
	Key_Init();
	
	OLED_ShowString(1,1,"Angle:");
	
	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			if(flag==1)
			{
				angle+=90;
			}
			else
			{
				angle-=90;
			}
			
			if(angle>180)
			{
				angle = 90;
				flag = -flag;
			}
			if(angle<0)
			{
				angle = 90;
				flag = -flag;
			}
		}
		servo_set_angle(angle);
		OLED_ShowNum(1,7,angle,3);
	}
}
