#ifndef __KALMAN_CAL_H
#define __KALMAN_CAL_H	 
#include "math.h"


void Angle_Cal(void);
void Kalman_Cal_Pitch(float acc,float gyro);
void Kalman_Cal_Roll(float acc,float gyro);
	

extern float pitch_kalman; 	   //pitch滤波后数据
extern float roll_kalman; 		 //roll滤波后数据	
extern float yaw; 	 		//yaw


#endif
