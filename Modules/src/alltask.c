#include "alltask.h"
#include "chassis.h"
#include "can.h"
#include "ore.h"
#include "eng_control.h"
#include "remote_control.h"
#include "Key.h"

#define CARD_POS 10000U

//空接任务，兑换任务
//分开的抬升、夹子位置、伸缩位置等任务

uint8_t card = 0;
uint8_t barriar_stage = 0;

void chassis_task(void)
{
  uint8_t i;
  
  if(mode == STOP || mode == DEBUG)   //停止模式或调试模式，底盘无力
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
  lift_task();			//抬升PID计算
	flip_spin_task(); //夹子、转矿PID计算
	groundore_card_task(); //地面矿、救援卡PID计算
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
		//后视视角
		//图传看屏幕
		if(barriar_stage == 0)			//平
		{
			TIM4->CCR1 = 750;
			TIM4->CCR2 = 750;
			barriar_stage = 1;
		}
		
		if(barriar_stage == 1)			//抬
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
		if(barriar_stage == 2)		//平
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

