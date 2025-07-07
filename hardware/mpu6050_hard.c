#include "stm32f10x.h"                  // Device header

void mpu6050_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 50000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);
}

void mpu6050_hard_sendbyte(uint8_t Reg_Add, uint8_t byte){
	
	I2C_GenerateSTART(I2C2, ENABLE);
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
	
	I2C_Send7bitAddress(I2C2, 0xD0, I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
	
	I2C_SendData(I2C2, Reg_Add);
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);	
	
	I2C_SendData(I2C2, byte);
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);
	
	I2C_GenerateSTOP(I2C2, ENABLE);
	
}
	

