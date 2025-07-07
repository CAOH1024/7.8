#include "stm32f10x.h"                  // Device header
#include "sys.h"

#define pi 3.14159
double x,y;
double error_x, error_y;
uint16_t move_status,task_status=1;
float fastInverseSqrt(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = *(long *)&y;                       // 将浮点数的位模式解释为整数
    i  = 0x5f3759df - (i >> 1);            // 魔数和位移操作
    y  = *(float *)&i;
    y  = y * (threehalfs - (x2 * y * y));  // 牛顿迭代法
    return y;
}

double get_pwm_count_x(double x,double x_goal)
{
	double angle_x=atan(x/100.0);
	double angle_x_goal=atan(x_goal/100.0);
	if(angle_x_goal-angle_x>=0) 
	{
		dir_x=dir_right;
		return  get_pwm_count((angle_x_goal-angle_x)/pi*180.0);
		
	}
	else 
	{
		dir_x=dir_left;
		return  get_pwm_count((angle_x-angle_x_goal)/pi*180.0);
	}
}

double get_pwm_count_y(double x,double x_goal,double y,double y_goal)
{
	
	double x_temp=fastInverseSqrt(10000.0+x*x);
	double angle_y=atan(y* x_temp);
	x_temp=fastInverseSqrt(10000.0+x_goal*x_goal);
	double angle_y_goal=atan(y_goal* x_temp);
	if(angle_y_goal-angle_y>=0) 
	{
		dir_y=dir_down;
		return get_pwm_count((angle_y_goal-angle_y)/pi*180.0);
	}
	else 
	{
		dir_y=dir_up;
		return get_pwm_count((angle_y-angle_y_goal)/pi*180.0);
	}
}
void line_walker(double x_step,double k)
{
	double x1=get_pwm_count_x(x,x+x_step);
	pwm_x=(uint16_t)get_pwm_count_x(x,x+x_step);
	error_x += (x1 - pwm_x);
	if(error_x-1>0)
	{	
		pwm_x+=1;
		error_x-=1.0;
	}	
	double y1=get_pwm_count_y(x,x+x_step,y,y+k*x_step);
	pwm_y=(uint16_t)get_pwm_count_y(x,x+x_step,y,y+k*x_step);
	error_y+=(y1-pwm_y);
	if(error_y-1>0)
	{	
		pwm_y+=1;
		error_y-=1.0;
	}
	x+=x_step;
	y+=k*x_step;
	if(pwm_y!=0) TIM_Cmd(TIM2,ENABLE);
	if(pwm_x!=0) TIM_Cmd(TIM3,ENABLE);
}
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
