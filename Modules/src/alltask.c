#include "alltask.h"
#include "chassis.h"
#include "can.h"
#include "ore.h"
#include "eng_control.h"
#include "remote_control.h"
#include "Key.h"

#define CARD_POS 10000U

//�ս����񣬶һ�����
//�ֿ���̧��������λ�á�����λ�õ�����

uint8_t card = 0;
uint8_t barriar_stage = 0;

void chassis_task(void)
{
  uint8_t i;
  
  if(mode == STOP || mode == DEBUG)   //ֹͣģʽ�����ģʽ����������
    pid_chassis_spd[0].outPID = pid_chassis_spd[1].outPID = pid_chassis_spd[2].outPID = pid_chassis_spd[3].outPID = 0;
  else
  {
    chassis_control();
    for(i=0;i<4;i++)
      pid_chassis_spd[i].pid_calculate(&pid_chassis_spd[i],motor_msg[i].speed_desired,motor_msg[i].speed_actual);
  }
  CAN1_Set_AheadCur(pid_chassis_spd[0].outPID,pid_chassis_spd[1].outPID,pid_chassis_spd[2].outPID,pid_chassis_spd[3].outPID);
  
}

void remote_task(void)
{
  remote_control();
}

void ore_task(void)
{
  lift_task();			//̧��PID����
	flip_spin_task(); //���ӡ�ת��PID����
	groundore_card_task(); //����󡢾�Ԯ��PID����
}

void other_task(void)
{
	rescue_task();
	barriar_task();
	card_task();
	
}

void rescue_task(void)
{
	if(FunctionMODE == RESCUEMODE)
	{
		TIM4->CCR3 = 750;
		TIM4->CCR4 = 800;
		if(Key_Check_Hold(&Keys.KEY_B))		
				FunctionMODE = STANDBY;
	}
}

void barriar_task(void)
{
	bool is_press = false;
	if(FunctionMODE == BARRIARMODE)
	{
		//�����ӽ�
		//ͼ������Ļ
		if(barriar_stage == 0)			//ƽ
		{
			TIM4->CCR1 = 750;
			TIM4->CCR2 = 750;
			barriar_stage = 1;
		}
		
		if(barriar_stage == 1)			//̧
		{
			if(RC_CtrlData.mouse.press_l == 1)
			{
				is_press = true;
				while(RC_CtrlData.mouse.press_l);
			}
			if(is_press == true)
			{
				TIM4->CCR1 = 1150;
				TIM4->CCR2 = 350;
				is_press = false;
				barriar_stage = 2;
			}
		}
		if(barriar_stage == 2)		//ƽ
		{
			if(RC_CtrlData.mouse.press_l == 1)
			{
				is_press = true;
				while(RC_CtrlData.mouse.press_l);
			}
			if(is_press == true)
			{
				TIM4->CCR1 = 750;
				TIM4->CCR2 = 750;
				is_press = false;
				barriar_stage = 1;
			}			
		}	
		if(Key_Check_Hold(&Keys.KEY_B))		
			FunctionMODE = STANDBY;
	}
}

void card_task(void)
{
	if(FunctionMODE == CARDMODE)
		motor_msg[15].angle_desired = CARD_POS;
	if(Key_Check_Hold(&Keys.KEY_B))
		FunctionMODE = STANDBY;
}

