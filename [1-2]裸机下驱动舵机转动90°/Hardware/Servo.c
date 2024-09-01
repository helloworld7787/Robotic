#include "stm32f10x.h"                  // Device header
#include "PWM.h"


void servo_init(void)
{
	PWM_Init();
}

void servo_set_angle(float angle)
{
	uint16_t Compare1 = angle/180*2000 + 500;
	TIM_SetCompare1(TIM2,Compare1);
}
