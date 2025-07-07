#include "stm32f10x.h"                  // Device header
#include "I2C.h"                  // Device header

#define I2C_USE_Read 0x01
#define I2C_USE_Write 0x00

void I2C_Use_Init(void){
	SoftI2C_Init();
}
	
void I2C_USE_WriteRegister(uint8_t Sla_Address, uint8_t Reg_Address, uint8_t DATA, uint32_t Num_Of_DATA){
	SoftI2C_Start();
	SoftI2C_Sendbyte(Sla_Address | I2C_USE_Write);
	SoftI2C_ReceiveAck();
	SoftI2C_Sendbyte(Reg_Address);
	SoftI2C_ReceiveAck();
	for(uint8_t i = 0; i < Num_Of_DATA; i++){
		SoftI2C_Sendbyte(DATA);
		SoftI2C_ReceiveAck();
	}
	SoftI2C_Stop();
}

uint8_t I2C_USE_ReadRegister(uint8_t Sla_Address, uint8_t Reg_Address){
	uint8_t DATA = 0x00;
	SoftI2C_Start();
	SoftI2C_Sendbyte(Sla_Address | I2C_USE_Write);
	SoftI2C_ReceiveAck();
	SoftI2C_Sendbyte(Reg_Address);	
	SoftI2C_ReceiveAck();
	SoftI2C_Start();
	SoftI2C_Sendbyte(Sla_Address | I2C_USE_Read);	
	SoftI2C_ReceiveAck();
	DATA = SoftI2C_Receivebyte();
	SoftI2C_SendAck(1);
	SoftI2C_Stop();
	return DATA;
}
