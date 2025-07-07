#include "motor.h"

uint16_t step_count_x,step_count_y;



void Motor_PWM_Init(u16 arr,u16 psc)        
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;                          
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);   //ʹ�ܶ�ʱ��2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);   //ʹ�ܶ�ʱ��3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //ʹ��GPIOA��ʱ��
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_6;	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_5;	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M		
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStruct.TIM_Period = arr;              //�趨�������Զ���װֵ 
	TIM_TimeBaseInitStruct.TIM_Prescaler  = psc;          //�趨Ԥ��Ƶ��
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;         //����ʱ�ӷָ�
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);       //��ʼ����ʱ��
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);       //��ʼ����ʱ��
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��PWM1ģʽ
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStruct.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //�����������
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);                       //��ʼ������Ƚϲ���
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
	//TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);   //CH1ʹ��Ԥװ�ؼĴ���
	//TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);   //CH4ʹ��Ԥװ�ؼĴ���
	//TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);   //CH4ʹ��Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);                //ʹ��TIM1��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM3, ENABLE);                //ʹ��TIM1��ARR�ϵ�Ԥװ�ؼĴ���
	
	//TIM_CtrlPWMOutputs(TIM8,ENABLE);
	

}
void TIM2_IRQHandler(void)   //X��
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		step_count_y++;
		if(step_count_y>=pwm_y)
		{
			TIM_Cmd(TIM2,DISABLE);                              //ʹ�ܶ�ʱ��1
			step_count_y=0;
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
void TIM3_IRQHandler(void)   //Y��
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		step_count_x++;
		if(step_count_x>=pwm_x)
		{
			TIM_Cmd(TIM3,DISABLE);                              //ʹ�ܶ�ʱ��1
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



