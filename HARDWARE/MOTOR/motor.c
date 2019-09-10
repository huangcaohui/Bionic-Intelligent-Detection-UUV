#include "motor.h"

u16 left_speed = 1.5e3;	//左电机
u16 right_speed = 1.5e3;	//右电机
u16 back_speed = 1.5e3;	//后电机
u16 steer_angle = 1.5e3;	//舵机

//上浮
void go_up(void)
{
	if(left_speed < STOP_SPEED)	//左电机
	{
		left_speed += LEFT_SLOW_STEP;
		left_speed = MIN(left_speed, STOP_SPEED);
	}
	else if(left_speed < LEFT_MAX_SPEED)	//左电机
	{
		left_speed += LEFT_ACCE_STEP;
		left_speed = MIN(left_speed, LEFT_MAX_SPEED);
	}
	
	if(right_speed < STOP_SPEED)	//右电机
	{
		right_speed += RIGHT_SLOW_STEP;
		right_speed = MIN(right_speed, STOP_SPEED);
	}
	else if(right_speed < RIGHT_MAX_SPEED)	//右电机
	{
		right_speed += RIGHT_ACCE_STEP;
		right_speed = MIN(right_speed, RIGHT_MAX_SPEED);
	}
}
	
//下潜
void go_dive(void)
{
	if(left_speed > STOP_SPEED)	//左电机
	{
		left_speed -= LEFT_SLOW_STEP;
		left_speed = MAX(left_speed, STOP_SPEED);
	}
	else if(left_speed > LEFT_MIN_SPEED)	//左电机
	{
		left_speed -= LEFT_ACCE_STEP;
		left_speed = MAX(left_speed, LEFT_MIN_SPEED);
	}
	
	if(right_speed > STOP_SPEED)	//右电机
	{
		right_speed -= RIGHT_SLOW_STEP;
		right_speed = MAX(right_speed, STOP_SPEED);
	}
	else if(right_speed > RIGHT_MIN_SPEED)	//右电机
	{
		right_speed -= RIGHT_ACCE_STEP;
		right_speed = MAX(right_speed, RIGHT_MIN_SPEED);
	}
}
	
//悬停
void hover(void)
{
	if(left_speed > STOP_SPEED)	//左电机
	{
		left_speed -= LEFT_SLOW_STEP;
		left_speed = MAX(left_speed, STOP_SPEED);
	}
	if(left_speed < STOP_SPEED)
	{
		left_speed += LEFT_SLOW_STEP;
		left_speed = MIN(left_speed, STOP_SPEED);
	}
	
	if(right_speed > STOP_SPEED)	//右电机
	{
		right_speed -= RIGHT_SLOW_STEP;
		right_speed = MAX(right_speed, STOP_SPEED);
	}
	if(right_speed < STOP_SPEED)
	{
		right_speed += RIGHT_SLOW_STEP;
		right_speed = MIN(right_speed, STOP_SPEED);
	}
	
	if(back_speed > STOP_SPEED)	//后电机
	{
		back_speed -= BACK_SLOW_STEP;
		back_speed = MAX(back_speed, STOP_SPEED);
	}
	if(back_speed < STOP_SPEED)
	{
		back_speed += BACK_SLOW_STEP;
		back_speed = MIN(back_speed, STOP_SPEED);
	}
	
	if(steer_angle > STEER_ZERO_ANGLE)	//舵机
	{
		steer_angle -= STEER_SLOW_STEP;
		steer_angle = MAX(steer_angle, STEER_ZERO_ANGLE);
	}
	if(steer_angle < STEER_ZERO_ANGLE)
	{
		steer_angle += STEER_SLOW_STEP;
		steer_angle = MIN(steer_angle, STEER_ZERO_ANGLE);
	}
}
	
//翻滚
void roll(void)
{
	roll_left();
}
	
//自主航行
void auto_voyage(void)
{}
	
//人工
void artifical(u8 action1, u8 action2)
{
	//左键盘动作
	switch(action1)
	{
		case 0x57: go_forward(); break;	//前进
		case 0x53: go_back(); break;	//后退
		case 0x51: go_forward_left(); break;	//左前转
		case 0x45: go_forward_right(); break;	//右前转
		case 0x41: go_back_left(); break;	//左后转
		case 0x44: go_back_right(); break;	//右后转
		case 0x50: back_stop(); break;	//后电机和舵机停止
	}
	
	//右键盘动作
	switch(action2)
	{
		case 0x49: go_up(); break;	//上浮
		case 0x4B: go_dive(); break;	//下潜
		case 0x4A: roll_left(); break;	//左翻滚
		case 0x4C: roll_right(); break;	//右翻滚
		case 0x50: sides_stop(); break;	//侧面电机停止
	}
}

//前进
void go_forward(void)
{
	if(back_speed < STOP_SPEED)
	{
		back_speed += BACK_SLOW_STEP;
		back_speed = MIN(back_speed, STOP_SPEED);
	}
	else if(back_speed < BACK_MAX_SPEED)	//后电机
	{
		back_speed += BACK_ACCE_STEP;
		back_speed = MIN(back_speed, BACK_MAX_SPEED);
	}
	
	if(steer_angle > STEER_ZERO_ANGLE)	//舵机
	{
		steer_angle -= STEER_SLOW_STEP;
		steer_angle = MAX(steer_angle, STEER_ZERO_ANGLE);
	}
	if(steer_angle < STEER_ZERO_ANGLE)
	{
		steer_angle += STEER_SLOW_STEP;
		steer_angle = MIN(steer_angle, STEER_ZERO_ANGLE);
	}
}
	
//后退
void go_back(void)
{
	if(back_speed > STOP_SPEED)	//后电机
	{
		back_speed -= BACK_SLOW_STEP;
		back_speed = MAX(back_speed, STOP_SPEED);
	}
	else if(back_speed > BACK_MIN_SPEED)	//后电机
	{
		back_speed -= BACK_ACCE_STEP;
		back_speed = MAX(back_speed, BACK_MIN_SPEED);
	}
	
	if(steer_angle > STEER_ZERO_ANGLE)	//舵机
	{
		steer_angle -= STEER_SLOW_STEP;
		steer_angle = MAX(steer_angle, STEER_ZERO_ANGLE);
	}
	if(steer_angle < STEER_ZERO_ANGLE)
	{
		steer_angle += STEER_SLOW_STEP;
		steer_angle = MIN(steer_angle, STEER_ZERO_ANGLE);
	}
}

//左前转
void go_forward_left(void)
{
	if(back_speed < STOP_SPEED)
	{
		back_speed += BACK_SLOW_STEP;
		back_speed = MIN(back_speed, STOP_SPEED);
	}
	else if(back_speed < BACK_FORWARD_TURN_SPEED)	//后电机
	{
		back_speed += BACK_ACCE_STEP;
		back_speed = MIN(back_speed, BACK_FORWARD_TURN_SPEED);
	}
	else if(back_speed > BACK_FORWARD_TURN_SPEED)
	{
		back_speed -= BACK_ACCE_STEP;
		back_speed = MAX(back_speed, BACK_FORWARD_TURN_SPEED);
	}
	
	if(steer_angle > STEER_LEFT_ANGLE)	//舵机
	{
		steer_angle -= STEER_ACCE_STEP;
		steer_angle = MAX(steer_angle, STEER_LEFT_ANGLE);
	}
}

//右前转
void go_forward_right(void)
{
	if(back_speed < STOP_SPEED)
	{
		back_speed += BACK_SLOW_STEP;
		back_speed = MIN(back_speed, STOP_SPEED);
	}
	else if(back_speed < BACK_FORWARD_TURN_SPEED)	//后电机
	{
		back_speed += BACK_ACCE_STEP;
		back_speed = MIN(back_speed, BACK_FORWARD_TURN_SPEED);
	}
	else if(back_speed > BACK_FORWARD_TURN_SPEED)
	{
		back_speed -= BACK_ACCE_STEP;
		back_speed = MAX(back_speed, BACK_FORWARD_TURN_SPEED);
	}
	
	if(steer_angle < STEER_RIGHT_ANGLE)	//舵机
	{
		steer_angle += STEER_ACCE_STEP;
		steer_angle = MIN(steer_angle, STEER_RIGHT_ANGLE);
	}
}

//左后转
void go_back_left(void)
{
	if(back_speed > STOP_SPEED)
	{
		back_speed -= BACK_SLOW_STEP;
		back_speed = MAX(back_speed, STOP_SPEED);
	}
	else if(back_speed > BACK_BACK_TURN_SPEED)	//后电机
	{
		back_speed -= BACK_ACCE_STEP;
		back_speed = MAX(back_speed, BACK_BACK_TURN_SPEED);
	}
	else if(back_speed < BACK_BACK_TURN_SPEED)
	{
		back_speed += BACK_ACCE_STEP;
		back_speed = MIN(back_speed, BACK_BACK_TURN_SPEED);
	}
	
	if(steer_angle > STEER_LEFT_ANGLE)	//舵机
	{
		steer_angle -= STEER_ACCE_STEP;
		steer_angle = MAX(steer_angle, STEER_LEFT_ANGLE);
	}
}

//右后转
void go_back_right(void)
{
	if(back_speed > STOP_SPEED)
	{
		back_speed -= BACK_SLOW_STEP;
		back_speed = MAX(back_speed, STOP_SPEED);
	}
	else if(back_speed > BACK_BACK_TURN_SPEED)	//后电机
	{
		back_speed -= BACK_ACCE_STEP;
		back_speed = MAX(back_speed, BACK_BACK_TURN_SPEED);
	}
	else if(back_speed < BACK_BACK_TURN_SPEED)
	{
		back_speed += BACK_ACCE_STEP;
		back_speed = MIN(back_speed, BACK_BACK_TURN_SPEED);
	}
	
	if(steer_angle < STEER_RIGHT_ANGLE)	//舵机
	{
		steer_angle += STEER_ACCE_STEP;
		steer_angle = MIN(steer_angle, STEER_RIGHT_ANGLE);
	}
}

//左翻滚	
void roll_left(void)
{
	if(left_speed > STOP_SPEED)	//左电机
	{
		left_speed -= LEFT_SLOW_STEP;
		left_speed = MAX(left_speed, STOP_SPEED);
	}
	else if(left_speed > LEFT_MIN_SPEED)	//左电机
	{
		left_speed -= LEFT_ACCE_STEP;
		left_speed = MAX(left_speed, LEFT_MIN_SPEED);
	}
	
	if(right_speed < STOP_SPEED)
	{
		right_speed += RIGHT_SLOW_STEP;
		right_speed = MIN(right_speed, STOP_SPEED);
	}
	else if(right_speed < RIGHT_MAX_SPEED)	//右电机
	{
		right_speed += RIGHT_ACCE_STEP;
		right_speed = MIN(right_speed, RIGHT_MAX_SPEED);
	}
}
	
//右翻滚
void roll_right(void)
{
	if(left_speed < STOP_SPEED)
	{
		left_speed += LEFT_SLOW_STEP;
		left_speed = MIN(left_speed, STOP_SPEED);
	}
	else if(left_speed < LEFT_MAX_SPEED)	//左电机
	{
		left_speed += LEFT_ACCE_STEP;
		left_speed = MIN(left_speed, LEFT_MAX_SPEED);
	}
	
	if(right_speed > STOP_SPEED)	//右电机
	{
		right_speed -= RIGHT_SLOW_STEP;
		right_speed = MAX(right_speed, STOP_SPEED);
	}
	else if(right_speed > RIGHT_MIN_SPEED)	//右电机
	{
		right_speed -= RIGHT_ACCE_STEP;
		right_speed = MAX(right_speed, RIGHT_MIN_SPEED);
	}
}

//后电机和舵机停止
void back_stop(void)
{
	if(back_speed > STOP_SPEED)	//后电机
	{
		back_speed -= BACK_SLOW_STEP;
		back_speed = MAX(back_speed, STOP_SPEED);
		TIM_SetCompare3(TIM8, back_speed);
	}
	if(back_speed < STOP_SPEED)
	{
		back_speed += BACK_SLOW_STEP;
		back_speed = MIN(back_speed, STOP_SPEED);
	}
	
	if(steer_angle > STEER_ZERO_ANGLE)	//舵机
	{
		steer_angle -= STEER_SLOW_STEP;
		steer_angle = MAX(steer_angle, STEER_ZERO_ANGLE);
	}
	if(steer_angle < STEER_ZERO_ANGLE)
	{
		steer_angle += STEER_SLOW_STEP;
		steer_angle = MIN(steer_angle, STEER_ZERO_ANGLE);
	}
}

//左右电机停止
void sides_stop(void)
{
	if(left_speed > STOP_SPEED)	//左电机
	{
		left_speed -= LEFT_SLOW_STEP;
		left_speed = MAX(left_speed, STOP_SPEED);
	}
	if(left_speed < STOP_SPEED)
	{
		left_speed += LEFT_SLOW_STEP;
		left_speed = MIN(left_speed, STOP_SPEED);
	}
	
	if(right_speed > STOP_SPEED)	//右电机
	{
		right_speed -= RIGHT_SLOW_STEP;
		right_speed = MAX(right_speed, STOP_SPEED);
	}
	if(right_speed < STOP_SPEED)
	{
		right_speed += RIGHT_SLOW_STEP;
		right_speed = MIN(right_speed, STOP_SPEED);
	}
}
