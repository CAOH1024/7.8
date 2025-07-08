#include "stm32f10x.h"                  // Device header
#include "sys.h"

//void move_up_down()
int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断分组为2
	//pwm_x=get_pwm_count(14.0);
	//pwm_y=get_pwm_count(12.3);
	Motor_PWM_Init(5000,71);  	    //=====初始化PWM 10KHZ，用于驱动电机 	
	TIM3->CCR1=500;
	TIM2->CCR2=500;
	Timer4_Init(999,7199);	        //5ms定时
	
	uart_init(9600);                //=====初始化串口1	
	Delay_s(2);
	
	TIM_Cmd(TIM4,ENABLE);
	x=0;
	y=-19.5;
	while(1)
	{
		if(tim4_flag)
		{
			switch(task_status)
			{
				case 0:
					switch(move_status)
					{
						case 0:
							line_walker(-0.5,1);
							if(x<-25)
							{
								move_status++;
								error_x=0;
								error_y=0;
								x=-25;
								y=-44.5;
							}
							break;
						case 1:
							line_walker(1,0);
							if(x>=25)
							{
								move_status++;
								error_x=0;
								error_y=0;
								x=25;
								y=-44.5;
							}
							break;
						case 2:
							line_walker(0.01,50);
							if(y>5.5)
							{
								move_status++;
								error_x=0;
								error_y=0;
								x=25;
								y=5.5;
							}
							break;
						case 3:
							line_walker(-1,0);
							if(x<-25)
							{
								move_status++;
								error_x=0;
								error_y=0;
								x=-25;
								y=5.5;
							}
							break;
						case 4:
							line_walker(-0.01,50);
							if(y<-44.5)
							{
								move_status=1;
								error_x=0;
								error_y=0;
								x=-25;
								y=-44.5;
							}
							break;
					}
					break;
				case 1:
					switch(move_status)
					{
						case 0:
							line_walker(-0.1,0.688);
							if(x<-13.95)
							{
								move_status++;
								error_x=0;
								error_y=0;
								x=-13.95;
								y=-29.1;
							}
							break;
						case 1:
							line_walker(0.3,0);
							if(x>=13.95)
							{
								move_status++;
								error_x=0;
								error_y=0;
								x=13.95;
								y=-29.1;
							}
							break;
						case 2:
							line_walker(0.01,50);
							if(y>-9.9)
							{
								move_status++;
								error_x=0;
								error_y=0;
								x=13.95;
								y=-9.9;
							}
							break;
						case 3:
							line_walker(-0.3,0);
							if(x<-13.95)
							{
								move_status++;
								error_x=0;
								error_y=0;
								x=-13.95;
								y=-9.9;
							}
							break;
						case 4:
							line_walker(-0.01,50);
							if(y<-29.1)
							{
								move_status=1;
								error_x=0;
								error_y=0;
								x=-13.95;
								y=-29.1;
							}
							break;
					}
					break;
			}
			//line_walker(-0.05,10);
			tim4_flag=0;
		}
		
	}
}
