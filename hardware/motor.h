#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h" 
#include "stm32f10x.h"                  // Device header

extern uint16_t step_count_x,step_count_y;
void Motor_PWM_Init(u16 arr,u16 psc);

double get_pwm_count(double angle);
#endif
