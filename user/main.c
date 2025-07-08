#include "stm32f10x.h"                  // Device header
#include "sys.h"

//void move_up_down()
int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断分组为2
	Motor_PWM_Init(5000,71);  	    //=====初始化PWM 10KHZ，用于驱动电机 	
	TIM3->CCR1=500;
	TIM2->CCR2=500;
	Timer4_Init(999,7199);	     //=====初始化定时器4 10HZ，用于定时
	
	uart_init(9600);                //=====初始化串口1	
	Delay_s(2);
	
	TIM_Cmd(TIM4,ENABLE);
	x=0;
	y=-19.5;
	task_status = 2 ; // Initialize task status
	while(1)
	{
		switch (task_status) // Switch case for task status
		{
			case 0:               //待机
				break;
			case 1:
				move_line(25, -44.5);
				task_status = 0;
				break;
			case 2:
				move_rac(-25, -44.5, 25, -44.5, -25, 5.5);
				task_status = 2;
				break;
		}
	}
			
}
