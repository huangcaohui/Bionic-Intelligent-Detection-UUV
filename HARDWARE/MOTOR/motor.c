#include "motor.h"

u16 left_speed = 1.5e3;	//����
u16 right_speed = 1.5e3;	//�ҵ��
u16 back_speed = 1.5e3;	//����
u16 steer_angle = 1.5e3;	//���

//�ϸ�
void go_up(void)
{
	if(left_speed < STOP_SPEED)	//����
	{
		left_speed += LEFT_SLOW_STEP;
		left_speed = MIN(left_speed, STOP_SPEED);
	}
	else if(left_speed < LEFT_MAX_SPEED)	//����
	{
		left_speed += LEFT_ACCE_STEP;
		left_speed = MIN(left_speed, LEFT_MAX_SPEED);
	}
	
	if(right_speed < STOP_SPEED)	//�ҵ��
	{
		right_speed += RIGHT_SLOW_STEP;
		right_speed = MIN(right_speed, STOP_SPEED);
	}
	else if(right_speed < RIGHT_MAX_SPEED)	//�ҵ��
	{
		right_speed += RIGHT_ACCE_STEP;
		right_speed = MIN(right_speed, RIGHT_MAX_SPEED);
	}
}
	
//��Ǳ
void go_dive(void)
{
	if(left_speed > STOP_SPEED)	//����
	{
		left_speed -= LEFT_SLOW_STEP;
		left_speed = MAX(left_speed, STOP_SPEED);
	}
	else if(left_speed > LEFT_MIN_SPEED)	//����
	{
		left_speed -= LEFT_ACCE_STEP;
		left_speed = MAX(left_speed, LEFT_MIN_SPEED);
	}
	
	if(right_speed > STOP_SPEED)	//�ҵ��
	{
		right_speed -= RIGHT_SLOW_STEP;
		right_speed = MAX(right_speed, STOP_SPEED);
	}
	else if(right_speed > RIGHT_MIN_SPEED)	//�ҵ��
	{
		right_speed -= RIGHT_ACCE_STEP;
		right_speed = MAX(right_speed, RIGHT_MIN_SPEED);
	}
}
	
//��ͣ
void hover(void)
{
	if(left_speed > STOP_SPEED)	//����
	{
		left_speed -= LEFT_SLOW_STEP;
		left_speed = MAX(left_speed, STOP_SPEED);
	}
	if(left_speed < STOP_SPEED)
	{
		left_speed += LEFT_SLOW_STEP;
		left_speed = MIN(left_speed, STOP_SPEED);
	}
	
	if(right_speed > STOP_SPEED)	//�ҵ��
	{
		right_speed -= RIGHT_SLOW_STEP;
		right_speed = MAX(right_speed, STOP_SPEED);
	}
	if(right_speed < STOP_SPEED)
	{
		right_speed += RIGHT_SLOW_STEP;
		right_speed = MIN(right_speed, STOP_SPEED);
	}
	
	if(back_speed > STOP_SPEED)	//����
	{
		back_speed -= BACK_SLOW_STEP;
		back_speed = MAX(back_speed, STOP_SPEED);
	}
	if(back_speed < STOP_SPEED)
	{
		back_speed += BACK_SLOW_STEP;
		back_speed = MIN(back_speed, STOP_SPEED);
	}
	
	if(steer_angle > STEER_ZERO_ANGLE)	//���
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
	
//����
void roll(void)
{
	roll_left();
}
	
//��������
void auto_voyage(void)
{}
	
//�˹�
void artifical(u8 action1, u8 action2)
{
	//����̶���
	switch(action1)
	{
		case 0x57: go_forward(); break;	//ǰ��
		case 0x53: go_back(); break;	//����
		case 0x51: go_forward_left(); break;	//��ǰת
		case 0x45: go_forward_right(); break;	//��ǰת
		case 0x41: go_back_left(); break;	//���ת
		case 0x44: go_back_right(); break;	//�Һ�ת
		case 0x50: back_stop(); break;	//�����Ͷ��ֹͣ
	}
	
	//�Ҽ��̶���
	switch(action2)
	{
		case 0x49: go_up(); break;	//�ϸ�
		case 0x4B: go_dive(); break;	//��Ǳ
		case 0x4A: roll_left(); break;	//�󷭹�
		case 0x4C: roll_right(); break;	//�ҷ���
		case 0x50: sides_stop(); break;	//������ֹͣ
	}
}

//ǰ��
void go_forward(void)
{
	if(back_speed < STOP_SPEED)
	{
		back_speed += BACK_SLOW_STEP;
		back_speed = MIN(back_speed, STOP_SPEED);
	}
	else if(back_speed < BACK_MAX_SPEED)	//����
	{
		back_speed += BACK_ACCE_STEP;
		back_speed = MIN(back_speed, BACK_MAX_SPEED);
	}
	
	if(steer_angle > STEER_ZERO_ANGLE)	//���
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
	
//����
void go_back(void)
{
	if(back_speed > STOP_SPEED)	//����
	{
		back_speed -= BACK_SLOW_STEP;
		back_speed = MAX(back_speed, STOP_SPEED);
	}
	else if(back_speed > BACK_MIN_SPEED)	//����
	{
		back_speed -= BACK_ACCE_STEP;
		back_speed = MAX(back_speed, BACK_MIN_SPEED);
	}
	
	if(steer_angle > STEER_ZERO_ANGLE)	//���
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

//��ǰת
void go_forward_left(void)
{
	if(back_speed < STOP_SPEED)
	{
		back_speed += BACK_SLOW_STEP;
		back_speed = MIN(back_speed, STOP_SPEED);
	}
	else if(back_speed < BACK_FORWARD_TURN_SPEED)	//����
	{
		back_speed += BACK_ACCE_STEP;
		back_speed = MIN(back_speed, BACK_FORWARD_TURN_SPEED);
	}
	else if(back_speed > BACK_FORWARD_TURN_SPEED)
	{
		back_speed -= BACK_ACCE_STEP;
		back_speed = MAX(back_speed, BACK_FORWARD_TURN_SPEED);
	}
	
	if(steer_angle > STEER_LEFT_ANGLE)	//���
	{
		steer_angle -= STEER_ACCE_STEP;
		steer_angle = MAX(steer_angle, STEER_LEFT_ANGLE);
	}
}

//��ǰת
void go_forward_right(void)
{
	if(back_speed < STOP_SPEED)
	{
		back_speed += BACK_SLOW_STEP;
		back_speed = MIN(back_speed, STOP_SPEED);
	}
	else if(back_speed < BACK_FORWARD_TURN_SPEED)	//����
	{
		back_speed += BACK_ACCE_STEP;
		back_speed = MIN(back_speed, BACK_FORWARD_TURN_SPEED);
	}
	else if(back_speed > BACK_FORWARD_TURN_SPEED)
	{
		back_speed -= BACK_ACCE_STEP;
		back_speed = MAX(back_speed, BACK_FORWARD_TURN_SPEED);
	}
	
	if(steer_angle < STEER_RIGHT_ANGLE)	//���
	{
		steer_angle += STEER_ACCE_STEP;
		steer_angle = MIN(steer_angle, STEER_RIGHT_ANGLE);
	}
}

//���ת
void go_back_left(void)
{
	if(back_speed > STOP_SPEED)
	{
		back_speed -= BACK_SLOW_STEP;
		back_speed = MAX(back_speed, STOP_SPEED);
	}
	else if(back_speed > BACK_BACK_TURN_SPEED)	//����
	{
		back_speed -= BACK_ACCE_STEP;
		back_speed = MAX(back_speed, BACK_BACK_TURN_SPEED);
	}
	else if(back_speed < BACK_BACK_TURN_SPEED)
	{
		back_speed += BACK_ACCE_STEP;
		back_speed = MIN(back_speed, BACK_BACK_TURN_SPEED);
	}
	
	if(steer_angle > STEER_LEFT_ANGLE)	//���
	{
		steer_angle -= STEER_ACCE_STEP;
		steer_angle = MAX(steer_angle, STEER_LEFT_ANGLE);
	}
}

//�Һ�ת
void go_back_right(void)
{
	if(back_speed > STOP_SPEED)
	{
		back_speed -= BACK_SLOW_STEP;
		back_speed = MAX(back_speed, STOP_SPEED);
	}
	else if(back_speed > BACK_BACK_TURN_SPEED)	//����
	{
		back_speed -= BACK_ACCE_STEP;
		back_speed = MAX(back_speed, BACK_BACK_TURN_SPEED);
	}
	else if(back_speed < BACK_BACK_TURN_SPEED)
	{
		back_speed += BACK_ACCE_STEP;
		back_speed = MIN(back_speed, BACK_BACK_TURN_SPEED);
	}
	
	if(steer_angle < STEER_RIGHT_ANGLE)	//���
	{
		steer_angle += STEER_ACCE_STEP;
		steer_angle = MIN(steer_angle, STEER_RIGHT_ANGLE);
	}
}

//�󷭹�	
void roll_left(void)
{
	if(left_speed > STOP_SPEED)	//����
	{
		left_speed -= LEFT_SLOW_STEP;
		left_speed = MAX(left_speed, STOP_SPEED);
	}
	else if(left_speed > LEFT_MIN_SPEED)	//����
	{
		left_speed -= LEFT_ACCE_STEP;
		left_speed = MAX(left_speed, LEFT_MIN_SPEED);
	}
	
	if(right_speed < STOP_SPEED)
	{
		right_speed += RIGHT_SLOW_STEP;
		right_speed = MIN(right_speed, STOP_SPEED);
	}
	else if(right_speed < RIGHT_MAX_SPEED)	//�ҵ��
	{
		right_speed += RIGHT_ACCE_STEP;
		right_speed = MIN(right_speed, RIGHT_MAX_SPEED);
	}
}
	
//�ҷ���
void roll_right(void)
{
	if(left_speed < STOP_SPEED)
	{
		left_speed += LEFT_SLOW_STEP;
		left_speed = MIN(left_speed, STOP_SPEED);
	}
	else if(left_speed < LEFT_MAX_SPEED)	//����
	{
		left_speed += LEFT_ACCE_STEP;
		left_speed = MIN(left_speed, LEFT_MAX_SPEED);
	}
	
	if(right_speed > STOP_SPEED)	//�ҵ��
	{
		right_speed -= RIGHT_SLOW_STEP;
		right_speed = MAX(right_speed, STOP_SPEED);
	}
	else if(right_speed > RIGHT_MIN_SPEED)	//�ҵ��
	{
		right_speed -= RIGHT_ACCE_STEP;
		right_speed = MAX(right_speed, RIGHT_MIN_SPEED);
	}
}

//�����Ͷ��ֹͣ
void back_stop(void)
{
	if(back_speed > STOP_SPEED)	//����
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
	
	if(steer_angle > STEER_ZERO_ANGLE)	//���
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

//���ҵ��ֹͣ
void sides_stop(void)
{
	if(left_speed > STOP_SPEED)	//����
	{
		left_speed -= LEFT_SLOW_STEP;
		left_speed = MAX(left_speed, STOP_SPEED);
	}
	if(left_speed < STOP_SPEED)
	{
		left_speed += LEFT_SLOW_STEP;
		left_speed = MIN(left_speed, STOP_SPEED);
	}
	
	if(right_speed > STOP_SPEED)	//�ҵ��
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
