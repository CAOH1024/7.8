#ifndef __MPU6050_SOFT_H
#define __MPU6050_SOFT_H

uint8_t mpu6050_Getid(void);
void mpu6050_GetData(int16_t* ACCEL_X, int16_t* ACCEL_Y, int16_t* ACCEL_Z,
				int16_t* TEMP, int16_t* GYRO_X, int16_t* GYRO_Y, int16_t* GYRO_Z);
void mpu6050_WriteRegister(uint8_t Reg_Address, uint8_t DATA, uint32_t Num_Of_DATA);
uint8_t mpu6050_ReadRegister(uint8_t Reg_Address);
void mpu6050_Init(void);

#endif
