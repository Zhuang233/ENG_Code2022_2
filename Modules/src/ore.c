#include "ore.h"
#include "Key.h"
#include "motor.h"
#include "eng_control.h"
//#include "math.h"
#include "can.h"
#include "stdlib.h"
#include "remote_control.h"
#include "timer.h"
#include "math.h"
#include "cmsis_os.h"

float pkp = 0.0;
float pki = 0.0;
float pkd = 0.0;
float skp = 0.0;
float ski = 0.0;
float skd = 0.0;

//所有与矿石有关的函数，主要修改的文件
//为应付中期写的，大部分需要重写

#define SPINSPD_FAST 5000
#define SPINSPD_SLOW 1000
#define AIR_POS 200000U
#define BIG_LIFT_POS 300000U
#define SMALL_LIFT_POS 100000U
#define EXCHANGE_POS 200000U
#define DOWN_POS 1000U
#define FLIP_POS 74000U
#define FLIP_HOLD_POS 30000U

uint32_t d_pos = 1000;
uint8_t fetch_exchange_stage = 0;
uint8_t fetch_flag = 0;
uint8_t fetch_which = 0;
uint8_t ore_suckin_flag = 0;
float para_cos = 2000;

void fetch_in_air(void)
{
	static bool is_trigger = false;
	static bool is_exit = false;
	if(FunctionMODE == FETCHAIRMODE)
	{
		if(fetch_exchange_stage == 0)
		{
			HAL_GPIO_WritePin(VALVE11_GPIO_Port,VALVE11_Pin,GPIO_PIN_SET); 		//夹子张开
			HAL_GPIO_WritePin(VALVE12_GPIO_Port,VALVE12_Pin,GPIO_PIN_SET);
		
//			delay_in_seconds(&delay_time,0.5);
//			if(delay_flag == 1)
//			{
				osDelay(500);
				HAL_GPIO_WritePin(VALVE11_GPIO_Port,VALVE11_Pin,GPIO_PIN_RESET); //泄压
				fetch_exchange_stage = 1;
//			}
		}
		
		if(fetch_exchange_stage == 1)							//抬升
		{
			lift(AIR_POS);						
			if(desired_angle[4] == AIR_POS && motor_msg[4].angle > (AIR_POS - 10000U))	
				fetch_exchange_stage = 2;
		}
		
		if(fetch_exchange_stage == 2)																										//夹子翻出
		{
			flip(FLIP_POS);
			if(desired_angle[8] == FLIP_POS && motor_msg[8].angle > (FLIP_POS - 10000U))
			{
				HAL_GPIO_WritePin(VALVE2_GPIO_Port,VALVE2_Pin,GPIO_PIN_SET);					//伸出
				fetch_exchange_stage = 3;
			}
		}
		
		if(fetch_exchange_stage == 3)																				//空接
		{
			if(HAL_GPIO_ReadPin(SENSOR_GPIO_Port,SENSOR_Pin) == GPIO_PIN_RESET)		//光电被触发
			{
				HAL_GPIO_WritePin(VALVE11_GPIO_Port,VALVE11_Pin,GPIO_PIN_RESET);			//加紧
				HAL_GPIO_WritePin(VALVE12_GPIO_Port,VALVE12_Pin,GPIO_PIN_RESET);
			  is_trigger = true;
			}
			if(is_trigger == true)
				delay_in_seconds(&delay_time,1);
			if(delay_flag == 1)
			{
				fetch_exchange_stage = 4;
				is_trigger = false;
			}			
		}				
		if(fetch_exchange_stage == 4)															
		{
			HAL_GPIO_WritePin(VALVE2_GPIO_Port,VALVE2_Pin,GPIO_PIN_RESET);				//缩回
			flip(FLIP_HOLD_POS);
			if(desired_angle[8] == FLIP_HOLD_POS && motor_msg[8].angle < (FLIP_HOLD_POS + 5000U))						//抬起矿
				fetch_exchange_stage = 5;
		}
		if(fetch_exchange_stage == 5)															//降下
		{
			lift(1000);
			if(desired_angle[4] == 1000 && motor_msg[4].angle < 10000)
				fetch_exchange_stage = 6;
		}
		if(fetch_exchange_stage == 6)
			FunctionMODE = STANDBY;
		
		if(Key_Check_Hold(&Keys.KEY_B))		
			is_exit = true;
		
		if(is_exit == true)
		{			
			HAL_GPIO_WritePin(VALVE2_GPIO_Port,VALVE2_Pin,GPIO_PIN_RESET);				//缩回
			flip(5000);
			lift(1000);
			if(desired_angle[8] == 5000 && motor_msg[8].angle < 15000 && desired_angle[4] == 1000 && motor_msg[4].angle<10000)
			{
				is_exit = false;
				FunctionMODE = STANDBY;			
			}
		}
	}
}

void fetch_ore_in_hole(void)
{
	static int32_t lift_pos;
	static bool is_press = false;
	static bool is_exit = false;
	if(FunctionMODE == FETCHBIGMODE || FunctionMODE == FETCHSMALLMODE)
	{
		HAL_GPIO_WritePin(VALVE11_GPIO_Port,VALVE11_Pin,GPIO_PIN_SET); 		//夹子张开
		HAL_GPIO_WritePin(VALVE12_GPIO_Port,VALVE12_Pin,GPIO_PIN_SET);
		if(fetch_exchange_stage == 0)							//抬升
		{
			if(FunctionMODE == FETCHBIGMODE)
				lift_pos = BIG_LIFT_POS;
			else if(FunctionMODE == FETCHSMALLMODE)
				lift_pos = SMALL_LIFT_POS;
			lift(lift_pos);						
			if(desired_angle[4] == lift_pos && motor_msg[4].angle > (lift_pos - 10000U))
				fetch_exchange_stage = 1;
		}
		
		if(fetch_exchange_stage == 1)
		{
			flip(FLIP_POS);								//翻转
			if(desired_angle[8] == FLIP_POS && motor_msg[8].angle > (FLIP_POS - 10000U))
			{
				HAL_GPIO_WritePin(VALVE2_GPIO_Port,VALVE2_Pin,GPIO_PIN_SET);					//伸出
				fetch_exchange_stage = 2;			
			}
		}
		
		if(fetch_exchange_stage == 2)
		{
			if(RC_CtrlData.mouse.press_l == 1)    //按下鼠标左键
				is_press = true;
			
			if(is_press == true && !RC_CtrlData.mouse.press_l)
			{
				HAL_GPIO_WritePin(VALVE11_GPIO_Port,VALVE11_Pin,GPIO_PIN_RESET);			//加紧
				HAL_GPIO_WritePin(VALVE12_GPIO_Port,VALVE12_Pin,GPIO_PIN_RESET);		
				
				delay_in_seconds(&delay_time,1);
				if(delay_flag == 1)				
				{
					fetch_exchange_stage = 3;	
					is_press = false;			
				}					
			}
		}
		
		if(fetch_exchange_stage == 3)
		{
			flip(FLIP_HOLD_POS);
			if(desired_angle[8] == FLIP_HOLD_POS && motor_msg[8].angle < (FLIP_HOLD_POS + 5000))						//抬起矿	
			{
				HAL_GPIO_WritePin(VALVE2_GPIO_Port,VALVE2_Pin,GPIO_PIN_RESET);				//缩回
				fetch_exchange_stage = 4;
			}				
		}
		
		if(fetch_exchange_stage == 4)
		{
			delay_in_seconds(&delay_time,1);
			if(delay_flag == 1)
				fetch_exchange_stage = 5;
		}
		
		if(fetch_exchange_stage == 5)
		{
			lift(1000);
			if(desired_angle[4] == 1000 && motor_msg[4].angle < 10000)
				FunctionMODE = STANDBY;					
		}
		
		if(Key_Check_Hold(&Keys.KEY_B))		
			is_exit = true;
		
		if(is_exit == true)
		{			
			HAL_GPIO_WritePin(VALVE2_GPIO_Port,VALVE2_Pin,GPIO_PIN_RESET);				//缩回
			is_press = false;
			flip(5000);
			lift(1000);
			if(desired_angle[8] == 5000 && motor_msg[8].angle < 15000 && desired_angle[4] == 1000 && motor_msg[4].angle<10000)
			{
				is_exit = false;
				FunctionMODE = STANDBY;			
			}
		}
	}
}

void exchange_task(void)
{
	static bool is_press = false;
	if(FunctionMODE == EXCHANGEMODE)
	{
		if(fetch_exchange_stage == 0)
		{
			lift(EXCHANGE_POS);
			if(desired_angle[4] == EXCHANGE_POS && motor_msg[4].angle > (EXCHANGE_POS - 10000U))
				fetch_exchange_stage = 1;
		}
		
		if(fetch_exchange_stage == 1)
		{
			HAL_GPIO_WritePin(VALVE2_GPIO_Port,VALVE2_Pin,GPIO_PIN_SET);					//伸出
			if(RC_CtrlData.mouse.press_l == 1)
				is_press = true;
			if(is_press == true && !RC_CtrlData.mouse.press_l)
			{
				flip(FLIP_POS);
				if(desired_angle[8] == FLIP_POS && motor_msg[8].angle > (FLIP_POS - 10000U))				
				{
					fetch_exchange_stage = 2;
					is_press = false;
				}
			}
		}
		
		if(fetch_exchange_stage == 2)
		{
			if(RC_CtrlData.mouse.press_l == 1)			
				is_press = true;
			
			if(is_press == true && !RC_CtrlData.mouse.press_l)
			{
				HAL_GPIO_WritePin(VALVE11_GPIO_Port,VALVE11_Pin,GPIO_PIN_SET);			//松开
				HAL_GPIO_WritePin(VALVE12_GPIO_Port,VALVE12_Pin,GPIO_PIN_SET);
				is_press = false;
				fetch_exchange_stage = 3;
			}
		}
		
		if(fetch_exchange_stage == 3)
		{
			if(RC_CtrlData.mouse.press_l == 1)
				is_press = true;

			if(is_press == true && !RC_CtrlData.mouse.press_l)
			{
				HAL_GPIO_WritePin(VALVE11_GPIO_Port,VALVE11_Pin,GPIO_PIN_RESET);		//合起来
				HAL_GPIO_WritePin(VALVE12_GPIO_Port,VALVE12_Pin,GPIO_PIN_RESET);
				is_press = false;
				fetch_exchange_stage = 4;				
			}
		}
		
		if(fetch_exchange_stage == 4)
		{
			if(RC_CtrlData.mouse.press_l == 1)
				is_press = true;
			if(is_press == true && !RC_CtrlData.mouse.press_l)
			{
				flip(5000);
				if(desired_angle[8] == 5000 && motor_msg[8].angle < 20000U)
				{
					is_press = false;
					fetch_exchange_stage = 5;
				}
			}
		}
		if(fetch_exchange_stage == 5)
		{
			lift(1000);
			if(desired_angle[4] == 1000 && motor_msg[4].angle < 10000U)
				FunctionMODE = STANDBY;
		}
	}
}



void lift_task(void)
{
	uint8_t i;
	uint32_t avg_lift_angle = 0;
	if(mode == STOP)
		pid_lift_spd[0].outPID = pid_lift_spd[1].outPID = pid_lift_spd[2].outPID = pid_lift_spd[3].outPID = 0;
	else
	{
		avg_lift_angle = (abs(motor_msg[4].angle) + abs(motor_msg[5].angle) + abs(motor_msg[6].angle) + abs(motor_msg[7].angle)) / 4;
		for(i=0;i<4;i++)
		{
			pid_lift_syn[i].pid_calculate(&pid_lift_syn[i],avg_lift_angle,abs(motor_msg[i+4].angle));
			pid_lift_syn[i].outPID *= (motor_msg[i+4].angle < 0 ? -1 : 1);
			
			pid_lift_pos[i].pid_calculate(&pid_lift_pos[i],motor_msg[i+4].angle_desired,motor_msg[i+4].angle);
			pid_lift_spd[i].pid_calculate(&pid_lift_spd[i],pid_lift_pos[i].outPID + pid_lift_syn[i].outPID,motor_msg[i+4].speed_actual);
		}
	}
	CAN1_Set_BackCur(pid_lift_spd[0].outPID,pid_lift_spd[1].outPID,pid_lift_spd[2].outPID,pid_lift_spd[3].outPID);
}

void flip_spin_task(void)
{
	uint8_t i;
	if(mode == STOP)
		pid_flip_spd[0].outPID = pid_flip_spd[1].outPID = pid_spin_spd[0].outPID = pid_spin_spd[1].outPID = 0;
	else
	{
		for(i=0;i<2;i++)
		{ 
//			pid_flip_pos[i].kp = pkp;
//			pid_flip_pos[i].ki = pki;
//			pid_flip_pos[i].kd = pkd;
//			pid_flip_spd[i].kp = skp;
//			pid_flip_spd[i].ki = ski;
//			pid_flip_spd[i].kd = skd;
		
			pid_flip_pos[i].pid_calculate(&pid_flip_pos[i],motor_msg[i+8].angle_desired,motor_msg[i+8].angle);
			pid_flip_spd[i].pid_calculate(&pid_flip_spd[i],pid_flip_pos[i].outPID,motor_msg[i+8].speed_actual);
		}
			
			motor_msg[8].given_current = para_cos*cos(3.1415/80000*motor_msg[8].angle)+pid_flip_spd[0].outPID;
			motor_msg[9].given_current = -para_cos*cos(3.1415/80000*motor_msg[8].angle)+pid_flip_spd[1].outPID;
		
		if(spin_flag == 1)
			pid_spin_spd[0].pid_calculate(&pid_spin_spd[0],SPINSPD_SLOW,motor_msg[10].speed_actual);
		else if(spin_flag == 2)
			pid_spin_spd[0].pid_calculate(&pid_spin_spd[0],-SPINSPD_SLOW,motor_msg[10].speed_actual);
		else
			pid_spin_spd[0].pid_calculate(&pid_spin_spd[0],0,motor_msg[10].speed_actual);
		if(spin_flag == 3)
			pid_spin_spd[1].pid_calculate(&pid_spin_spd[1],SPINSPD_SLOW,motor_msg[11].speed_actual);
		else if(spin_flag == 4)
			pid_spin_spd[1].pid_calculate(&pid_spin_spd[1],-SPINSPD_SLOW,motor_msg[11].speed_actual);
		else
			pid_spin_spd[1].pid_calculate(&pid_spin_spd[1],0,motor_msg[11].speed_actual);
	}

	CAN2_Set_AheadCur(motor_msg[8].given_current, motor_msg[9].given_current, pid_spin_spd[0].outPID, pid_spin_spd[1].outPID);
	//CAN2_Set_AheadCur(pid_flip_spd[0].outPID, pid_flip_spd[1].outPID, pid_spin_spd[0].outPID, pid_spin_spd[1].outPID);
}

void groundore_card_task(void)
{
	if(mode == STOP)
		pid_card_spd.outPID = 0;
	else
	{
		pid_card_pos.pid_calculate(&pid_card_pos,motor_msg[15].angle_desired,motor_msg[15].angle);
		pid_card_spd.pid_calculate(&pid_card_spd,pid_card_pos.outPID,motor_msg[15].speed_actual);
		CAN2_Set_BackCur(0,0,0,pid_card_spd.outPID);	
	}
}
void fetch_exchange_task(void)
{
	fetch_in_air();
	fetch_ore_in_hole();
	exchange_task();
	//空接1 空接2 夹取1 夹取2
	//兑换1 兑换2	
}

//升降
void lift(int32_t pos)
{
	int32_t error = motor_msg[4].angle - pos;
	if(error > d_pos)
	{
		desired_angle[4] -= d_pos;
		if(desired_angle[4] <= pos)
			desired_angle[4] = pos;
	}
	else if(error < -d_pos)
	{
		desired_angle[4] += d_pos;
		if(desired_angle[4] >= pos)
			desired_angle[4] = pos;	
	}
	else
		desired_angle[4] = pos;
	
	desired_angle[5]  = -desired_angle[4];
	desired_angle[6] = desired_angle[4];
	desired_angle[7] = -desired_angle[4];
}

//直接升降
void d_lift(int32_t pos)
{
	desired_angle[4] = pos;
	desired_angle[5] = -desired_angle[4];
  desired_angle[6] = desired_angle[4];
  desired_angle[7] = -desired_angle[4];  
}

//翻转
void flip(int32_t pos)
{
//	int32_t error = motor_msg[8].angle - pos;
//	if(error > d_pos)
//	{
//		desired_angle[8] -= d_pos;
//		if(desired_angle[8] <= pos)
//			desired_angle[8] = pos;
//	}
//	else if(error < -d_pos)
//	{
//		desired_angle[8] += d_pos;
//		if(desired_angle[8] >= pos)
//			desired_angle[8] = pos;	
//	}
//	else
//		desired_angle[8] = pos;
	if(pos > 80000) pos = 80000;
	else if(pos < 0) pos = 0;
	
	desired_angle[8] = pos;
	desired_angle[9] = -desired_angle[8];
}



