#ifndef __PROC_H
#define __PROC_H

void line_walker(double x_step,double k);
double get_pwm_count_y(double x,double x_goal,double y,double y_goal);
double get_pwm_count_x(double x,double x_goal);
float fastInverseSqrt(float number);
void move_rac(double x1, double y1, double x2, double y2, double x3, double y3);
void move_line(double x2, double y2);

#endif

