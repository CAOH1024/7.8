#include "stm32f10x.h"                  // Device header
#include "Timer.h"
 #include "sys.h"
 
void Timer4_Init(u16 arr,u16 psc)  
{  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_TimeBaseInitStruct.TIM_Period = arr;               //��װ��ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;            //Ԥ��Ƶϵ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision =0;           //ʱ�ӷָ�
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);               //ʹ�ܶ�ʱ���ж�
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;        //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           //ʹ���ⲿ�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;        //��Ӧ���ȼ�3
	NVIC_Init(&NVIC_InitStruct);
}  

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		tim4_flag=1;
		
		
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}

