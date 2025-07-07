#ifndef __I2C_USE_H
#define __I2C_USE_H

void I2C_Use_Init(void);
void I2C_USE_WriteRegister(uint8_t Sla_Address, uint8_t Reg_Address, uint8_t DATA, uint32_t Num_Of_DATA);
uint8_t I2C_USE_ReadRegister(uint8_t Sla_Address, uint8_t Reg_Address);

#endif
