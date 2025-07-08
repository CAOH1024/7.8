#include "stm32f10x.h"                  // Device header
#include "sys.h"

void Key_Init(void){

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);         //打开AFIO引脚映射寄存器的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);        //打开GPIOA的时钟
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);  //将GPIO口的引脚映射到中断线上
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);  //虽然函数名为GPIO开头,实际上属于AFIO
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);  //将GPIO口的引脚映射到中断线上
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);  //虽然函数名为GPIO开头,实际上属于AFIO
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);  //将GPIO口的引脚映射到中断线上

    //将GPIOA的6号引脚配置为下拉输入模式,默认读取的电平为低电平(逻辑0)
    GPIO_InitTypeDef gitd;
    gitd.GPIO_Mode=GPIO_Mode_IPU;
    gitd.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_7; //配置GPIOB的6,5,8,9号引脚
    gitd.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gitd);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //配置中断通道的优先级
    NVIC_InitTypeDef nitd;
    nitd.NVIC_IRQChannel=EXTI9_5_IRQn;
    nitd.NVIC_IRQChannelCmd=ENABLE;
    nitd.NVIC_IRQChannelPreemptionPriority=1;   //抢占优先级
    nitd.NVIC_IRQChannelSubPriority=1;          //响应优先级
    NVIC_Init(&nitd);
    //配置外部中断

    EXTI_InitTypeDef eitd;
    eitd.EXTI_Line=EXTI_Line6;              //GPIOA的6号引脚对应6号中断线
    eitd.EXTI_LineCmd=ENABLE;
    eitd.EXTI_Mode=EXTI_Mode_Interrupt;     //选择中断模式
    eitd.EXTI_Trigger=EXTI_Trigger_Falling;  //上升沿触发,因为GPIO口配置为了下拉输入
    EXTI_Init(&eitd);
    
    eitd.EXTI_Line=EXTI_Line7;              //GPIOA的7号引脚对应7号中断线
    EXTI_Init(&eitd);
	eitd.EXTI_Line=EXTI_Line5;              //GPIOA的7号引脚对应7号中断线
    EXTI_Init(&eitd);
	eitd.EXTI_Line=EXTI_Line8;              //GPIOA的7号引脚对应7号中断线
    EXTI_Init(&eitd);
	eitd.EXTI_Line=EXTI_Line9;              //GPIOA的7号引脚对应7号中断线
    EXTI_Init(&eitd);

}

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


void EXTI9_5_IRQHandler(void){
	if(EXTI_GetFlagStatus(EXTI_Line5)){     //因为两条中断线用的都是同一个函数,因此通过获取中断标志位来判断是谁触发的中断
        task_status = 1; //设置任务状态为1,表示任务已完成
        Delay_ms(10);                       //消除机械按键的抖动
        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==1);
        Delay_ms(10);
        EXTI_ClearITPendingBit(EXTI_Line5); //需要手动清除中断标志位,否则会一直重复触发.
	}else if(EXTI_GetFlagStatus(EXTI_Line6)){     //因为两条中断线用的都是同一个函数,因此通过获取中断标志位来判断是谁触发的中断
        task_status = 2; //设置任务状态为1,表示任务已完成
        Delay_ms(10);                       //消除机械按键的抖动
        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1);
        Delay_ms(10);
        EXTI_ClearITPendingBit(EXTI_Line6); //需要手动清除中断标志位,否则会一直重复触发.
    }else if(EXTI_GetFlagStatus(EXTI_Line7)){
        task_status = 3;
        Delay_ms(10);
        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0);
        Delay_ms(10);
        EXTI_ClearITPendingBit(EXTI_Line7);
    }else if(EXTI_GetFlagStatus(EXTI_Line8)){
        task_status = 4;
        Delay_ms(10);
        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0);
        Delay_ms(10);
        EXTI_ClearITPendingBit(EXTI_Line8);
    }else if(EXTI_GetFlagStatus(EXTI_Line9)){
        task_status = 5;
        Delay_ms(10);
        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==0);
        Delay_ms(10);
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}
