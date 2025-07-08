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
