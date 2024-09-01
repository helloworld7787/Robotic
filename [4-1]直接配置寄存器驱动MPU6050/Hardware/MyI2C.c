#include "stm32f10x.h"                  // Device header
#include "Delay.h"


void MyI2C_Init(void)
{
	*(uint32_t*)0x40021018 |= 0x00000008; //����RCC_APB2ERN�Ĵ���������GPIOB��ʱ��
	*(uint32_t*)0x40010C04 &= 0xFFFF00FF;//����GPIOB_CRH�Ĵ��������PB10��PB11�ſ�ԭ�������ã�����������С��򡱲�����ʱ�����
	*(uint32_t*)0x40010C04 |=  (0x00000700 | 0x00007000);//����GPIOB_CRH�Ĵ���������PB10��PB11�ſ�Ϊͨ�ÿ�©���ģʽ������ٶ�50MHz
	*(uint32_t*)0x40010C0C |= (0x00000400 | 0x00000800);//����GPIOB_ODR�Ĵ�����PB10��PB11�ͷ�����
}

void MyI2C_W_SCL(uint8_t BitValue)
{
	if(BitValue)
    {
        *(uint32_t*)0x40010C10 |= 0x00000400;//����GPIOB_BSRR�Ĵ�������PB10λΪ1
    }
    else
    {
        *(uint32_t*)0x40010C14 |= 0x00000400;//����GPIOB_BRR�Ĵ�������PB10λΪ0
    }
    Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	if(BitValue)
	{
		*(uint32_t*)0x40010C10 |= 0x00000800;//����GPIOB_BSRR�Ĵ�������PB11λΪ1
	}
	else
	{
		*(uint32_t*)0x40010C14 |= 0x00000800;//����GPIOB_BRR�Ĵ�������PB11λΪ0
	}
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = (*(uint32_t*)0x40010C08 & 0x0800) ? 1 : 0;//��ȡGPIOB_IDR�Ĵ�����PB11λ
	Delay_us(10);
	return BitValue;
}

void My_I2C_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2CStop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2CSendByte(uint8_t Byte)
{
	uint8_t i;
	for(i = 0;i<8;i++)
	{
		MyI2C_W_SDA(Byte & (0x80>>i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

uint8_t MyI2CReceiveByte(void)
{
	uint8_t i,Byte = 0x00;
	MyI2C_W_SDA(1);
	for(i = 0;i<8;i++)
	{
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA()==1){Byte |= (0x80>>i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);		
    MyI2C_W_SCL(0);	
}	

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);		
    MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}
