#include "stm32f10x.h"                  // Device header
#include "sys.h"
double point[3][2] = {{0, 0}, {0, 0}, {0, 0}}; // Define points for movement
//void move_up_down()
int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断分组为2
	Motor_PWM_Init(5000,71);  	    //=====初始化PWM 10KHZ，用于驱动电机 	
	TIM3->CCR1=500;
	TIM2->CCR2=500;
	Timer4_Init(999,7199);	     //=====初始化定时器4 10HZ，用于定时
	Key_Init();
	uart_init(9600);                //=====初始化串口1	
	Delay_s(2);
	
	TIM_Cmd(TIM4,ENABLE);
	x=0;
	y=-19.5;
	task_status = 0 ; // Initialize task status
	while(1)
	{
		switch (task_status) // Switch case for task status
		{
			case 0:               //待机
				break;
			case 1:
				usart1_sendpackage(0x00);
				while(Serial_RxFlag == 0); // Wait for data to be received
				Data_Process(&x, &y);
				move_line(0, -19.5);
				task_status = 0;
				break;
			case 2:
				move_rac(-25, -44.5, 25, -44.5, -25, 5.5);
				task_status = 1;
				break;
			case 3:
				usart1_sendpackage(0x01);
				while(Serial_RxFlag == 0); // Wait for data to be received
				Data_Process(&point[0][0], &point[0][1]);
				usart1_sendpackage(0x02);
				while(Serial_RxFlag == 0); // Wait for data to be received
				Data_Process(&point[1][0], &point[1][1]);
				usart1_sendpackage(0x03);
				while(Serial_RxFlag == 0); // Wait for data to be received
				Data_Process(&point[2][0], &point[2][1]);			
				move_rac(point[0][0],point[0][1],point[1][0], point[1][1], point[2][0], point[2][1]);
				task_status = 1; // Reset task status after completion
		}
	}
			
}
