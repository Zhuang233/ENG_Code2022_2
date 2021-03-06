#include "alltask.h"
#include "chassis.h"
#include "can.h"
#include "ore.h"
#include "eng_control.h"
#include "remote_control.h"
#include "Key.h"
#include "pwm.h"
#include "stdbool.h"
#include "cmsis_os.h"

#define CARD_POS 950000U

//空接任务，兑换任务
//分开的抬升、夹子位置、伸缩位置等任务

uint8_t card = 0;
uint8_t barriar_stage = 0;

bool rescuing = false;
bool card_out = false;
bool rescue_down = false;

float para_wheel_front = 1.0;

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
	
	pid_chassis_spd[0].outPID *= para_wheel_front;
	pid_chassis_spd[1].outPID *= para_wheel_front;
	if(pid_chassis_spd[0].outPID > 16000) pid_chassis_spd[0].outPID = 16000;
	if(pid_chassis_spd[0].outPID < -16000) pid_chassis_spd[0].outPID = -16000;
	if(pid_chassis_spd[1].outPID > 16000) pid_chassis_spd[1].outPID = 16000;
	if(pid_chassis_spd[1].outPID < -16000) pid_chassis_spd[1].outPID = -16000;
	
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
		CAMERA = RESCUE_CAMERA_ANGEL;//调整图传视角
		para_wheel_front = 1.5;
		if(!rescuing)//还没拖着车
		{
			if(RC_CtrlData.mouse.press_l)//单击左键救援
			{
				RESCUE_L = RESCUE_DOWN_L;
				RESCUE_R = RESCUE_DOWN_R;
				rescuing = true;
				osDelay(300);
			}
		}
		else
		{
			if(RC_CtrlData.mouse.press_l)//单击左键释放(推着防止卡住)
			{
				chassis_auto_control = true;
				vy = 1000;
				vx = 0;
				vr = 0;
				osDelay(500);
				RESCUE_L = RESCUE_UP_L;
				RESCUE_R = RESCUE_UP_R;
				chassis_auto_control = false;
				
				rescuing = false;
			}
		}

		if(Key_Check_Hold(&Keys.KEY_B))//中途退出		
		{
			para_wheel_front = 1.0;
			FunctionMODE = STANDBY;
		}
	}
}

void barriar_task(void)
{
	static bool is_press = false;
	if(FunctionMODE == BARRIARMODE)
	{
		//后视视角
		//图传看屏幕
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
		CAMERA = SCREEN_CAMERA_ANGEL;
		move(-1000000);
		para_wheel_front = 1.5;
		
//		if(!rescue_down && RC_CtrlData.mouse.press_l )
//		{
//			BARRIAR_L = BARRIAR_DOWN_L;
//			BARRIAR_R = BARRIAR_DOWN_R;
//			
//			osDelay(300);
//			rescue_down = true;
//		}
//		else if(rescue_down && RC_CtrlData.mouse.press_l)
//		{
//			BARRIAR_L = BARRIAR_UP_L;
//			BARRIAR_R = BARRIAR_UP_R;
//			osDelay(300);
//			rescue_down = false;
//		}

		if(barriar_stage == 0)			//平
		{
			BARRIAR_L = BARRIAR_DOWN_L;
			BARRIAR_R = BARRIAR_DOWN_R;
			barriar_stage = 1;
		}
		
		if(barriar_stage == 1)			//抬
		{
			if(RC_CtrlData.mouse.press_l == 1)
			{
				is_press = true;
			}
			if(is_press == true && !RC_CtrlData.mouse.press_l)
			{
				BARRIAR_L = BARRIAR_UP_L;
				BARRIAR_R = BARRIAR_UP_R;
				is_press = false;
				barriar_stage = 2;
			}
		}
		if(barriar_stage == 2)		//平
		{
			if(RC_CtrlData.mouse.press_l == 1)
			{
				is_press = true;
			}
			if(is_press == true && !RC_CtrlData.mouse.press_l)
			{
				BARRIAR_L = BARRIAR_DOWN_L;
				BARRIAR_R = BARRIAR_DOWN_R;
				is_press = false;
				barriar_stage = 1;
			}			
		}	
		
		if(Key_Check_Hold(&Keys.KEY_B))
		{
			is_press = false;
			move(0);
			FunctionMODE = STANDBY;
		}
	}
}

void card_task(void)
{
	if(FunctionMODE == CARDMODE)
	{
		CAMERA = CARD_CAMERA_ANGEL;
		para_wheel_front = 1.5;
		if(!card_out && RC_CtrlData.mouse.press_l )
		{
			desired_angle[15] = CARD_POS;
			osDelay(300);
			card_out = true;
		}
		else if(card_out && RC_CtrlData.mouse.press_l)
		{
			desired_angle[15] = 0;
			osDelay(300);
			card_out = false;
		}
	}
	
	if(Key_Check_Hold(&Keys.KEY_B))
	{
		desired_angle[15] = 0;
		para_wheel_front = 1.0;
		card_out = false;
		FunctionMODE = STANDBY;
	}
}

