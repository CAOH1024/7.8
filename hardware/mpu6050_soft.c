#include "stm32f10x.h"                  // Device header
#include "I2C_USE.h"
#include "mpu6050_register.h"
#define mpu6050 0xD0

uint8_t mpu6050_ReadRegister(uint8_t Reg_Address){
	return I2C_USE_ReadRegister(mpu6050, Reg_Address);
}

void mpu6050_WriteRegister(uint8_t Reg_Address, uint8_t DATA, uint32_t Num_Of_DATA){
	I2C_USE_WriteRegister(mpu6050, Reg_Address, DATA, Num_Of_DATA);
}

void mpu6050_Init(void){
	I2C_Use_Init();
	mpu6050_WriteRegister(PWR_MGMT_1, 0x01, 1);
	mpu6050_WriteRegister(PWR_MGMT_2, 0x00, 1);
	mpu6050_WriteRegister(0x19, 0x09, 1);
	mpu6050_WriteRegister(0x1A, 0x06, 1);
	mpu6050_WriteRegister(0x1B, 0x18, 1);
	mpu6050_WriteRegister(0x1C, 0x18, 1);
}

uint8_t mpu6050_Getid(void){
	uint8_t id = 0x00;
	id = mpu6050_ReadRegister(WHO_AM_I);
	return id;
}

void mpu6050_GetData(int16_t* ACCEL_X, int16_t* ACCEL_Y, int16_t* ACCEL_Z,
				int16_t* TEMP, int16_t* GYRO_X, int16_t* GYRO_Y, int16_t* GYRO_Z)
{
	uint8_t data_h, data_l;
	
	data_h = mpu6050_ReadRegister(ACCEL_XOUT_H);
	data_l = mpu6050_ReadRegister(ACCEL_XOUT_L);
	*ACCEL_X = (data_h << 8) | data_l;
	data_h = mpu6050_ReadRegister(ACCEL_YOUT_H);
	data_l = mpu6050_ReadRegister(ACCEL_YOUT_L);
	*ACCEL_Y = (data_h << 8) | data_l;
	data_h = mpu6050_ReadRegister(ACCEL_ZOUT_H);
	data_l = mpu6050_ReadRegister(ACCEL_ZOUT_L);
	*ACCEL_Z = (data_h << 8) | data_l;	
	
	data_h = mpu6050_ReadRegister(TEMP_OUT_H);
	data_l = mpu6050_ReadRegister(TEMP_OUT_L);
	*TEMP = (data_h << 8) | data_l;	
	
	data_h = mpu6050_ReadRegister(GYRO_XOUT_H);
	data_l = mpu6050_ReadRegister(GYRO_XOUT_L);
	*GYRO_X = (data_h << 8) | data_l;
	data_h = mpu6050_ReadRegister(GYRO_YOUT_H);
	data_l = mpu6050_ReadRegister(GYRO_YOUT_L);
	*GYRO_Y = (data_h << 8) | data_l;
	data_h = mpu6050_ReadRegister(GYRO_ZOUT_H);
	data_l = mpu6050_ReadRegister(GYRO_ZOUT_L);
	*GYRO_Z = (data_h << 8) | data_l;	
}



