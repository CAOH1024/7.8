#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define I2C_SCL_Write(bit) {GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)bit);Delay_us(10);}
#define I2C_SDA_Write(bit) {GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)bit);Delay_us(10);}

uint8_t I2C_SDA_Read(void){
	uint8_t DATA_Bit;
	DATA_Bit = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	return DATA_Bit;
}

void SoftI2C_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_SDA_Write(1);
	I2C_SCL_Write(1);	
}

void SoftI2C_Start(void) {
	I2C_SDA_Write(1);
	I2C_SCL_Write(1);
	I2C_SDA_Write(0);
	I2C_SCL_Write(0);
}

void SoftI2C_Stop(void) {
	I2C_SDA_Write(0);
	I2C_SCL_Write(1);
	I2C_SDA_Write(1);
}

void SoftI2C_Sendbyte(uint8_t byTe){
	for(uint8_t i = 0; i < 8; i++){
		I2C_SDA_Write(byTe & (0x80 >> i));
		I2C_SCL_Write(1);
		I2C_SCL_Write(0);
	}
}

uint8_t SoftI2C_Receivebyte(void){
	uint8_t DATA_Received = 0x00;
	I2C_SDA_Write(1);
	for(uint8_t i = 0; i < 8; i++){
		I2C_SCL_Write(1);
		DATA_Received |= (I2C_SDA_Read() << (7 - i));
		I2C_SCL_Write(0);
	}
	return DATA_Received;
}

void SoftI2C_SendAck(uint8_t bit){
		I2C_SDA_Write(bit);
		I2C_SCL_Write(1);
		I2C_SCL_Write(0);
}

uint8_t SoftI2C_ReceiveAck(void){
	uint8_t Ack;
	I2C_SDA_Write(1);
	I2C_SCL_Write(1);
	Ack =  I2C_SDA_Read();
	I2C_SCL_Write(0);	
	return Ack;
}
