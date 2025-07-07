#include "stm32f10x.h"                  // Device header

uint32_t LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	return 0;
}

uint32_t LED1_turn(void)
{
	uint16_t bit1;
	bit1 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
	if(bit1)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	}
	else{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
	}
	return 0;
}

uint32_t LED2_turn(void)
{
	uint16_t bit2;
	bit2 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1);
	if(bit2)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	}
	else{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
	}
	return 0;
}
