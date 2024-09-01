#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "sys.h"
#include "Servo.h"

float pitch,roll,yaw;

float kp = 0.6, ki = 0.2, kd = 0.05; //比例系数、积分系数、微分系数

float roll_target_position = 0;
float roll_current_position = 0;
float roll_integral_error = 0; //积分误差
float roll_last_error = 0; //上一次误差值

float yaw_current_position = 0;
float yaw_target_position = 0;
float yaw_integral_error = 0; //积分误差
float yaw_last_error = 0; //上一次误差值


int main(void)
{
	mpu_dmp_init();
	servo_init();
	

	servo1_set_angle(0);
	servo2_set_angle(90);
	
	delay_ms(1000);
	while(1)
	{
		mpu_dmp_get_data(&pitch,&roll,&yaw);

		
		//roll角度PID控制
		roll_target_position = roll;
		float roll_error = roll_target_position - roll_current_position;
		roll_integral_error += roll_error;//积分误差
		float roll_derivative_error = roll_error - roll_last_error;//微分误差
		float roll_control = kp * roll_error + ki * roll_integral_error + kd * roll_derivative_error;//PID控制量
		roll_current_position = roll_control;
		roll_last_error = roll_error;
		
		//yaw角度PID控制
		yaw_target_position = yaw;
		float yaw_error = yaw_target_position - yaw_current_position;
		yaw_integral_error += yaw_error;//积分误差
		float yaw_derivative_error = yaw_error - yaw_last_error;//微分误差
		float yaw_control = kp * yaw_error + ki * yaw_integral_error + kd * yaw_derivative_error;//PID控制量
		yaw_current_position = yaw_control;
		yaw_last_error = yaw_error;
		
		//角度设置
		servo1_set_angle(roll_control+90);
		servo2_set_angle(yaw_control+90);
		
	}
}
