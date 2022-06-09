#include "motor.h"
#include "pwm.h"
#include "can.h"
#include "FreeRTOS.h"
#include "remote_control.h"
#include "task.h"
#include "timer.h"
#include "stdlib.h"
#include "pid.h"
#include "arm_math.h"
#include "eng_control.h"
//1:3500 2760
//2:3320 2620


Motor_Msg motor_msg[16];
                   /* can1[0]~[3] 	底盘电机	
                          [4]~[7]		抬升		  
                      can2[8]~[9]	  翻转	
													[10]~[11] 转矿
                          [12]			地上矿夹子旋转
													[13]			地上矿夹子加紧
													[14]      地上矿夹子移动
													[15]			救援卡
                   */


void Motor_Msg_Init(void)
{
	uint8_t i;
	for(i=0; i<16; i++)
	{
		motor_msg[i].speed_actual=0;
		motor_msg[i].speed_last=0;
		motor_msg[i].speed_desired=0;
		motor_msg[i].angle_actual=0;
		motor_msg[i].angle_desired=0;
		motor_msg[i].angle_last=0;
		motor_msg[i].angle=0;
		motor_msg[i].given_current=0;
		motor_msg[i].real_current=0;
		motor_msg[i].turns=0;
		motor_msg[i].original_position=0;
		motor_msg[i].first_run = true;
		motor_msg[i].whe_use_pid=true;
		motor_msg[i].time.time_now=TIM2->CNT;
		motor_msg[i].time.time_last=motor_msg[i].time.time_now;
		motor_msg[i].time.dt=0;
    desired_angle[i] = 0;
	}
}

void Stop_Motor(Motor_Msg *msg)
{
	msg->speed_desired = 0;
	msg->given_current = 0;
}

void Stop_AllMotor(void)
{
	uint8_t i;
	for(i=0; i<14; i++)
	{
		motor_msg[i].given_current=0;
	}
  CAN1_Set_Current();
  CAN2_Set_Current();
}

void Set_MotorZeropoint(Motor_Msg *msg)	//设置零点
{
  msg->first_run = true;
}

void SingleMotoTest(int32_t left,int32_t right)  //left- back right+ back
{
  motor_msg[0].angle_desired = left;
  motor_msg[1].angle_desired = right;
}
