#ifndef __MYI2C_H
#define __MYI2C_H

void MyI2C_Init(void);
void My_I2C_Start(void);
void MyI2CStop(void);
void MyI2CSendByte(uint8_t Byte);
uint8_t MyI2CReceiveByte(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(void);


#endif
