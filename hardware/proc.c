#include "stm32f10x.h"                  // Device header
#include "sys.h"

float fastInverseSqrt(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = *(long *)&y;                       // ?????????????
    i  = 0x5f3759df - (i >> 1);            // ???????
    y  = *(float *)&i;
    y  = y * (threehalfs - (x2 * y * y));  // ?????
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

void line_walker(double x_step,double y_step)
{
	double x1=get_pwm_count_x(x,x+x_step);
	pwm_x=(uint16_t)get_pwm_count_x(x,x+x_step);
	error_x += (x1 - pwm_x);
	if(error_x-1>0)
	{	
		pwm_x+=1;
		error_x-=1.0;
	}	
	double y1=get_pwm_count_y(x,x+x_step,y,y+y_step);
	pwm_y=(uint16_t)get_pwm_count_y(x,x+x_step,y,y+y_step);
	error_y+=(y1-pwm_y);
	if(error_y-1>0)
	{	
		pwm_y+=1;
		error_y-=1.0;
	}
	x+=x_step;
	y+=y_step;
	if(pwm_y!=0) TIM_Cmd(TIM2,ENABLE);
	if(pwm_x!=0) TIM_Cmd(TIM3,ENABLE);
}

void move_line(double x2, double y2)
{
	double delta_x = x2 - x;
	double delta_y = y2 - y;
	double x_step = delta_x / 50.0; // 50 steps for smooth movement
	double y_step = delta_y / 50.0;
	while(1){
		if(tim4_flag == 1){
			line_walker(x_step, y_step); 
			if((x < x2 && dir_x == dir_left)||(x > x2 && dir_x == dir_right)){
				error_x = 0;
				error_y = 0;
				x = x2;
				y = y2;
				tim4_flag = 0; // Reset flag
				break;
			}
			tim4_flag = 0; // Reset flag
		}
	}

}

void move_rac(double x1, double y1, double x2, double y2, double x3, double y3)
{
	double delta_x1 = x2 - x1;
	double delta_y1 = y2 - y1;
	double delta_x2 = x3 - x1;
	double delta_y2 = y3 - y1;
	double delta_x_ori = x1;
	double delta_y_ori = y1 + 19.5; // Adjust y1 to start from -19.5
	double x1_step = delta_x1 / 50.0 ; // 50 steps for smooth movement
	double y1_step = delta_y1 / 50.0 ;
	double x2_step = delta_x2 / 50.0 ; // 50 steps for smooth movement
	double y2_step = delta_y2 / 50.0 ;
	double x_ori_step = delta_x_ori /50.0;
	double y_ori_step = delta_y_ori /50.0; 
	static int move_status = 0;
	while (1)
	{
		if(tim4_flag == 1)
		{
			switch(move_status)
			{
				case 0:
					line_walker(x_ori_step, y_ori_step);
					if(x < x1)
					{
						move_status++;
						error_x = 0;
						error_y = 0;
						x = x1;
						y = y1;
					}
					break;
				case 1:
					line_walker(x1_step, y1_step);
					if(x > x2)
					{
						move_status++;
						error_x = 0;
						error_y = 0;
						x = x2;
						y = y2;
					}
					break;
				case 2:
					line_walker(x2_step, y2_step);
					if(y > y2 + delta_y2)
					{
						move_status++;
						error_x = 0;
						error_y = 0;
						x = x3 + delta_x1;
						y = y3 + delta_y1;
					}
					break;
				case 3:
					line_walker(-x1_step, -y1_step);
					if(x < x3)
					{
						move_status++;
						error_x = 0;
						error_y = 0;
						x = x3;
						y = y3;
					}
					break;
				case 4:
					line_walker(-x2_step, -y2_step);
					if(y < y1)
					{
						move_status++;
						error_x = 0;
						error_y = 0;
						x = x1;
						y = y1;
					}
					break;
				case 5:
					line_walker(-x_ori_step, -y_ori_step);
					if(x > 0 || y > -19.5)
					{
						move_status ++; // Reset to start
						error_x = 0;
						error_y = 0;
						x = 0;
						y = -19.5; // Reset position
					}
					break;
			}
			tim4_flag = 0; // Reset flag
		}
		if(move_status == 6) // If we have completed the movement
			{
				TIM_Cmd(TIM2, DISABLE); // Disable PWM for x
				TIM_Cmd(TIM3, DISABLE); // Disable PWM for y
				move_status = 0; // Reset move status
				break; // Exit the loop
			}	
	}
}
