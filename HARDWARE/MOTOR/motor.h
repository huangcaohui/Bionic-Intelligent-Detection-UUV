#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"
#include "timer.h"

#define MIN(x,y) ((x)>(y)?(y):(x))
#define MAX(x,y) ((x)>(y)?(x):(y))

#define STOP_SPEED	1.5e3

#define LEFT_MAX_SPEED	1.8e3
#define LEFT_MIN_SPEED	1.2e3

#define RIGHT_MAX_SPEED	1.8e3
#define RIGHT_MIN_SPEED	1.2e3

#define BACK_MAX_SPEED	1.65e3
#define BACK_MIN_SPEED	1.35e3

#define BACK_FORWARD_TURN_SPEED	1.6e3	//后电机前转弯速度
#define BACK_BACK_TURN_SPEED	1.35e3	//后电机后转弯速度

#define STEER_LEFT_ANGLE	1.1e3	//舵机左转角度上限
#define STEER_ZERO_ANGLE	1.5e3	//舵机零角度
#define STEER_RIGHT_ANGLE	1.9e3	//舵机右转角度上限

#define LEFT_ACCE_STEP	20	//左电机加速步长
#define LEFT_SLOW_STEP	50	//左电机减速步长

#define RIGHT_ACCE_STEP	20	//右电机加速步长
#define RIGHT_SLOW_STEP	50	//右电机减速步长

#define BACK_ACCE_STEP	5	//后电机加速步长
#define BACK_SLOW_STEP	50	//后电机减速步长

#define STEER_ACCE_STEP	30	//舵机加速步长
#define STEER_SLOW_STEP	30	//舵机减速步长

extern u16 left_speed;	//左电机
extern u16 right_speed;	//右电机
extern u16 back_speed;	//后电机
extern u16 steer_angle;	//舵机

void go_up(void);
void go_dive(void);
void hover(void);
void roll(void);
void auto_voyage(void);
void artifical(u8 action1, u8 action2);

void go_forward(void);
void go_back(void);
void go_forward_left(void);
void go_forward_right(void);
void go_back_left(void);
void go_back_right(void);
void roll_left(void);
void roll_right(void);
void back_stop(void);
void sides_stop(void);

#endif
