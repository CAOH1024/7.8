#ifndef __I2C_H
#define __I2C_H

void SoftI2C_Init(void);
void SoftI2C_Start(void);
void SoftI2C_Stop(void);
void SoftI2C_Sendbyte(uint8_t DATA);
uint8_t SoftI2C_Receivebyte(void);

void SoftI2C_SendAck(BitAction(bit));     
uint8_t SoftI2C_ReceiveAck(void);
//回0说明继续读， 回1说明不继续读了

#endif
