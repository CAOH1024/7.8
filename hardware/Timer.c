#include "stm32f10x.h"                  // Device header
#include "Timer.h"
 #include "sys.h"
 
void Timer4_Init(u16 arr,u16 psc)  
{  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_TimeBaseInitStruct.TIM_Period = arr;               //重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;            //预分频系数
	TIM_TimeBaseInitStruct.TIM_ClockDivision =0;           //时钟分割
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);               //使能定时器中断
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;        //使能按键所在的外部中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           //使能外部中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;        //响应优先级3
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

