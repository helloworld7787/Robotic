#include "stm32f10x.h"                  // Device header
#include "PWM.h"


void servo_init(void)
{
	PWM_Init();
}

void servo1_set_angle(float angle)
{
	uint16_t Compare1 = angle/180*1000 + 250;
	TIM_SetCompare1(TIM2,Compare1);
}

void servo2_set_angle(float angle)
{
	uint16_t Compare2 = angle/180*1000 + 250;
	TIM_SetCompare2(TIM2,Compare2);
}
