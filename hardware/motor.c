#include "motor.h"

uint16_t step_count_x,step_count_y;



void Motor_PWM_Init(u16 arr,u16 psc)        
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;                          
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);   //使能定时器2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);   //使能定时器3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //使能GPIOA的时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_6;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_5;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M		
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStruct.TIM_Period = arr;              //设定计数器自动重装值 
	TIM_TimeBaseInitStruct.TIM_Prescaler  = psc;          //设定预分频器
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;         //设置时钟分割
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);       //初始化定时器
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);       //初始化定时器
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);                       //初始化输出比较参数
	TIM_OC2Init(TIM2,&TIM_OCInitStruct);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_UpdateRequestConfig(TIM3,TIM_UpdateSource_Regular);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_UpdateRequestConfig(TIM2,TIM_UpdateSource_Regular);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);   //CH1使能预装载寄存器
	//TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);   //CH4使能预装载寄存器
	//TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);   //CH4使能预装载寄存器
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);                //使能TIM1在ARR上的预装载寄存器
	TIM_ARRPreloadConfig(TIM3, ENABLE);                //使能TIM1在ARR上的预装载寄存器
	
	//TIM_CtrlPWMOutputs(TIM8,ENABLE);
	

}
void TIM2_IRQHandler(void)   //X轴
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		step_count_y++;
		if(step_count_y>=pwm_y)
		{
			TIM_Cmd(TIM2,DISABLE);                              //使能定时器1
			step_count_y=0;
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
void TIM3_IRQHandler(void)   //Y轴
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		step_count_x++;
		if(step_count_x>=pwm_x)
		{
			TIM_Cmd(TIM3,DISABLE);                              //使能定时器1
			step_count_x=0;
		}
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

double get_pwm_count(double angle)
{
	double count=angle/360.0f*6400.0f;
	return count;
}



