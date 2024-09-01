#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS		0xD0

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	My_I2C_Start();
	MyI2CSendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2CSendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2CSendByte(Data);
	MyI2C_ReceiveAck();
	MyI2CStop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	//指定地址写的时序，调整地址指针
	My_I2C_Start();
	MyI2CSendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2CSendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	//读取的时序
	My_I2C_Start();
	MyI2CSendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	uint8_t ReceiveData = MyI2CReceiveByte();
	MyI2C_SendAck(1);
	MyI2CStop();
	
	return ReceiveData;
}

void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AX,int16_t *AY, int16_t *AZ, int16_t *GX, int16_t *GY, int16_t *GZ)
{
	uint8_t H,L;
	
	H = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	L = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AX = (H<<8) | L;
	
	H = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	L = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AY = (H<<8) | L;
	
	H = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	L = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AZ = (H<<8) | L;
	
	H = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	L = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GX = (H<<8) | L;
	
	H = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	L = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GY = (H<<8) | L;
	
	H = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	L = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GZ = (H<<8) | L;
}
