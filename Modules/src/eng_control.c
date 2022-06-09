#include "main.h"
#include "sys.h"
#include "stdlib.h"
#include "remote_control.h"
#include "math.h"
#include "motor.h"
#include "pid.h"
#include "can.h"
#include "Key.h"
#include "ANO_Link.h"
#include "eng_control.h"
#include "user_lib.h"
#include "timer.h"


//底盘电机
#define PID_CHASSIS_KP	2.5f//2.5f
#define PID_CHASSIS_KI	2.5f//2.5f
#define PID_CHASSIS_KD	0.0f//0.0f

//抬升
#define PID_LIFT_SPD_KP 4.0f	//8.0f
#define PID_LIFT_SPD_KI 1.0f
#define PID_LIFT_SPD_KD 0.0f
#define PID_LIFT_POS_KP 0.3f
#define PID_LIFT_POS_KI 0.0f
#define PID_LIFT_POS_KD 0.0f
//抬升同步
#define PID_LIFT_SYN_KP 0.1f
#define PID_LIFT_SYN_KI 0.0f
#define PID_LIFT_SYN_KD 0.0f

//翻转
#define PID_FLIP_SPD_KP 4.0f  //4.0
#define PID_FLIP_SPD_KI 1.0f  //1.0
#define PID_FLIP_SPD_KD 0.0f
#define PID_FLIP_POS_KP 0.3f  //0.3
#define PID_FLIP_POS_KI 0.0f
#define PID_FLIP_POS_KD 0.0f //0.01

//地上矿夹子旋转
#define PID_GNDROTATE_SPD_KP 4.0f
#define PID_GNDROTATE_SPD_KI 1.0f
#define PID_GNDROTATE_SPD_KD 0.0f
#define PID_GNDROTATE_POS_KP 0.3f
#define PID_GNDROTATE_POS_KI 0.0f 
#define PID_GNDROTATE_POS_KD 0.01f

//地上夹子加紧
#define PID_GND2006_SPD_KP 3.0f
#define PID_GND2006_SPD_KI 2.0f
#define PID_GND2006_SPD_KD 0.0f
#define PID_GND2006_POS_KP 0.3f
#define PID_GND2006_POS_KI 0.0f
#define PID_GND2006_POS_KD 0.0f

//地上夹子移动
#define PID_GNDMOVE_SPD_KP 4.0f
#define PID_GNDMOVE_SPD_KI 1.0f
#define PID_GNDMOVE_SPD_KD 0.0f
#define PID_GNDMOVE_POS_KP 0.3f
#define PID_GNDMOVE_POS_KI 0.0f
#define PID_GNDMOVE_POS_KD 0.01f

//救援卡
#define PID_CARD_SPD_KP 3.0f
#define PID_CARD_SPD_KI 2.0f
#define PID_CARD_SPD_KD 0.0f
#define PID_CARD_POS_KP 0.3f
#define PID_CARD_POS_KI 0.0f
#define PID_CARD_POS_KD 0.0f

//转矿
#define PID_SPIN_SPD_KP 3.0f
#define PID_SPIN_SPD_KI 2.0f
#define PID_SPIN_SPD_KD 0.0f

//test parameter
#define PID_TEST_SPD_KP 4.0f
#define PID_TEST_SPD_KI 1.0f
#define PID_TEST_SPD_KD 0.0f

#define PID_TEST_POS_KP 0.3f
#define PID_TEST_POS_KI 0.0f
#define PID_TEST_POS_KD 0.01f

#define PID_TEST_SYN_KP 0.0f
#define PID_TEST_SYN_KI 0.0f
#define PID_TEST_SYN_KD 0.0f

PidObject pid_testmoto_spd[2];
PidObject pid_testmoto_pos[2];
PidObject pid_sync[2];
//

PidObject pid_chassis_spd[4];//chassis motor speed pid 3508

PidObject pid_lift_spd[4];    //lift motor speed pid 3508
PidObject pid_lift_pos[4];    //lift motor position pid
PidObject pid_lift_syn[4];    //four lift motors sync pid 

PidObject pid_flip_spd[2];    //flip motor speed pid 3508
PidObject pid_flip_pos[2];    //flip motor position pid 

PidObject pid_spin_spd[2];    //spin ore motor spd pid 2006

PidObject pid_gndrotate_spd;  //ground ore rotate motor spd pid 3508
PidObject pid_gndrotate_pos;  //ground ore rotate motor pos pid

PidObject pid_gndgrip_spd;	 //ground ore grip motor spd pid 2006
PidObject pid_gndgrip_pos;	 //ground ore grip motor pos pid

PidObject pid_gndmove_spd; 	//ground ore move motor spd pid 3508
PidObject pid_gndmove_pos;  //ground ore move motor pos pid

PidObject pid_card_spd;			//ground card motor spd pid 2006
PidObject pid_card_pos;			//ground card motor pos pid


uint8_t zero_flag = 1;



TIME delay_time={0,0,0,0};

//uint32_t fetcher_time_now=0,fetcher_time_last=0;
//float ctrl_time.dt=0.02;

extern RC_Ctl_t RC_CtrlData;
extern uint8_t input_mode;
extern bool CAN1_send_current_flag;
extern bool CAN2_send_current_flag;
extern KEYS Keys;

extern int32_t pos1,pos2,pos3,pos4,pos5;


void eng_pid_init(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		pidInit(&pid_chassis_spd[i],50000,16000,PID_CHASSIS_KP,PID_CHASSIS_KI,PID_CHASSIS_KD);    //chassis speed
		
		pidInit(&pid_lift_spd[i],2000,16000,PID_LIFT_SPD_KP,PID_LIFT_SPD_KI,PID_LIFT_SPD_KD);		//lift speed
		pidInit(&pid_lift_pos[i],0,2000,PID_LIFT_POS_KP,PID_LIFT_POS_KI,PID_LIFT_POS_KD);		  //lift pos	
		pidInit(&pid_lift_syn[i],0,100,PID_LIFT_SYN_KP,PID_LIFT_SYN_KI,PID_LIFT_SYN_KD);			//lift syn
	}

	for(i=0;i<2;i++)
	{
		pidInit(&pid_flip_spd[i],10000,16000,PID_FLIP_SPD_KP,PID_FLIP_SPD_KI,PID_FLIP_SPD_KD);
		pidInit(&pid_flip_pos[i],0,2000,PID_FLIP_POS_KP,PID_FLIP_POS_KI,PID_FLIP_POS_KD);
		
		pidInit(&pid_spin_spd[i],10000,10000,PID_SPIN_SPD_KP,PID_SPIN_SPD_KI,PID_SPIN_SPD_KD);
	}
	
	pidInit(&pid_gndrotate_spd,10000,16000,PID_GNDROTATE_SPD_KP,PID_GNDROTATE_SPD_KI,PID_GNDROTATE_SPD_KD);
	pidInit(&pid_gndrotate_pos,0,1000,PID_GNDROTATE_POS_KP,PID_GNDROTATE_POS_KI,PID_GNDROTATE_POS_KD);
	
	pidInit(&pid_gndgrip_spd,10000,10000,PID_GND2006_SPD_KP,PID_GND2006_SPD_KI,PID_GND2006_SPD_KD);
	pidInit(&pid_gndgrip_pos,0,1000,PID_GND2006_POS_KP,PID_GND2006_POS_KI,PID_GND2006_POS_KD);
	
	pidInit(&pid_gndmove_spd,10000,16000,PID_GNDMOVE_SPD_KP,PID_GNDMOVE_SPD_KI,PID_GNDMOVE_SPD_KD);
	pidInit(&pid_gndmove_pos,0,1000,PID_GNDMOVE_POS_KP,PID_GNDMOVE_POS_KI,PID_GNDMOVE_POS_KD);
	
	pidInit(&pid_card_spd,10000,10000,PID_CARD_SPD_KP,PID_CARD_SPD_KI,PID_CARD_SPD_KD);
	pidInit(&pid_card_pos,0,1000,PID_CARD_POS_KP,PID_CARD_POS_KI,PID_CARD_POS_KD);

}

void Testmoto_PID_Init(void)
{
  pidInit(&pid_testmoto_spd[0],0,PID_TEST_SPD_KP,PID_TEST_SPD_KI,PID_TEST_SPD_KD,0.002);
  pidInit(&pid_testmoto_spd[1],0,PID_TEST_SPD_KP,PID_TEST_SPD_KI,PID_TEST_SPD_KD,0.002);
  pidInit(&pid_testmoto_pos[0],0,PID_TEST_POS_KP,PID_TEST_POS_KI,PID_TEST_POS_KD,0.002);
  pidInit(&pid_testmoto_pos[1],0,PID_TEST_POS_KP,PID_TEST_POS_KI,PID_TEST_POS_KD,0.002);
  pidInit(&pid_sync[0],0,PID_TEST_SYN_KP,PID_TEST_SYN_KI,PID_TEST_SYN_KD,0.002);
  pidInit(&pid_sync[1],0,PID_TEST_SYN_KP,PID_TEST_SYN_KI,PID_TEST_SYN_KD,0.002);
}

void zeropoint_init() 
{
  uint8_t i;
  int16_t temp=0;
  
	HAL_GPIO_WritePin(VALVE11_GPIO_Port,VALVE11_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(VALVE12_GPIO_Port,VALVE12_Pin,GPIO_PIN_SET); //夹子张开
	
  HAL_GPIO_WritePin(VALVE2_GPIO_Port,VALVE2_Pin,GPIO_PIN_RESET); //伸缩收回
	
  //翻转校准
//	if(HAL_GPIO_ReadPin(SWITCH_FLIP_GPIO_Port,SWITCH_FLIP_Pin) != GPIO_PIN_SET)
//	{
//    motor_msg[8].speed_desired = -500;
//		motor_msg[9].speed_desired = 500;		
//		
//		while(HAL_GPIO_ReadPin(SWITCH_FLIP_GPIO_Port,SWITCH_FLIP_Pin) != GPIO_PIN_SET)
//		{
//			for(i=0;i<2;i++)
//				pid_flip_spd[0].pid_calculate(&pid_flip_spd[0],motor_msg[i+8].speed_desired,motor_msg[i+8].speed_actual);
//			CAN2_Set_AheadCur(pid_flip_spd[0].outPID,pid_flip_spd[1].outPID,0,0);
//		}
//		CAN2_Set_AheadCur(0,0,0,0);
//	}
//	while(motor_msg[8].speed_actual != 0 || motor_msg[9].speed_actual != 0){};
//  Set_MotorZeropoint(&motor_msg[8]);
//  Set_MotorZeropoint(&motor_msg[9]);
		
//	//地面矿加紧校准
//	while(motor_msg[13].first_run == true);    			//等待电调启动
//	while(motor_msg[13].speed_actual <250)					//带起速度	
//	{
//		pid_gndgrip_spd.pid_calculate(&pid_gndgrip_spd,1000,motor_msg[13].speed_actual);
//		delay_in_seconds(&delay_time,0.01);
//		if(delay_flag)
//			CAN2_Set_BackCur(0,pid_gndgrip_spd.outPID,0,0);
//	}
//	
//	while(motor_msg[13].speed_actual >= 200)			//等待卡到机械限位
//	{
//		pid_gndgrip_spd.pid_calculate(&pid_gndgrip_spd,500,motor_msg[13].speed_actual);
//		delay_in_seconds(&delay_time,0.01);
//		if(delay_flag)
//			CAN2_Set_BackCur(0,pid_gndgrip_spd.outPID,0,0);
//	}
//	
//	motor_msg[13].given_current = 0;
//	zero_flag = 0;
//	CAN2_Set_BackCur(0,0,0,0);								
//	Set_MotorZeropoint(&motor_msg[13]);
	
	//救援卡复位
//	while(motor_msg[15].first_run == true);
//	motor_msg[15].given_current = -1000;
//	CAN2_Set_BackCur(0,0,0,motor_msg[15].given_current);
//	delay_ms(2000);
//	while(motor_msg[15].speed_actual <= -200);
//	motor_msg[15].given_current = 0;
//	CAN2_Set_BackCur(0,0,0,0);
//	Set_MotorZeropoint(&motor_msg[15]);	

}


/**		从左至右  	红	翻转	PF0		LED4(PG4)
					橙	夹取	PE4		LED3(PG3)
					黄	抬升	PE12	LED1(PG1)
					绿	伸缩	PB0		LED2(PG2)
					蓝	补弹	PB1		LED5(PG5)
  */
