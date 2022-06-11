#include "remote_control.h"
#include "pid.h"
#include "Key.h"
#include "eng_control.h"
#include "motor.h"
#include "mpu6500.h"
#include "math.h"
#include "stdlib.h"
#include "usart.h"
#include "arm_math.h"
#include "main.h"
#include "ore.h"
#include "pwm.h"
#include "alltask.h"

uint8_t FunctionMODE = 0;

#define LIFT_MAX 300000U

RC_Ctl_t RC_CtrlData;

//receive data, 18 bytes one frame, but set 36 bytes 
//接收原始数据，为18个字节，给了36个字节长度，防止DMA传输越界

extern UART_HandleTypeDef huart1;

TIME chassis_gyro_time;

int32_t desired_angle[16];

uint8_t mode = STOP;

uint8_t spin_flag = 0;

int16_t test_angle = 750;


/**
  * @brief          remote control protocol resolution
  * @param          buff: raw data point
  * @retval         none
  */
/**
  * @brief          遥控器协议解析
  * @param          buff: 原生数据指针
  * @retval         none
  */
void DBUS_decode(uint8_t *buff)
{
	RC_CtrlData.rc.sw1 = ((buff[5] >> 4) & 0x000C) >> 2;
	RC_CtrlData.rc.sw2 = (buff[5] >> 4) & 0x0003;
	if(!(RC_CtrlData.rc.sw1 && RC_CtrlData.rc.sw2))
	{
		RC_CtrlData.rc.sw1 = RC_CtrlData.rc.sw1_last;
		RC_CtrlData.rc.sw2 = RC_CtrlData.rc.sw2_last;
		return;
	}
	RC_CtrlData.rc.ch1 = (buff[0] | buff[1] << 8) & 0x07FF;
	RC_CtrlData.rc.ch1 -= 1024;
	RC_CtrlData.rc.ch2 = (buff[1] >> 3 | buff[2] << 5) & 0x07FF;
	RC_CtrlData.rc.ch2 -= 1024;
	RC_CtrlData.rc.ch3 = (buff[2] >> 6 | buff[3] << 2 | buff[4] << 10) & 0x07FF;
	RC_CtrlData.rc.ch3 -= 1024;
	RC_CtrlData.rc.ch4 = (buff[4] >> 1 | buff[5] << 7) & 0x07FF;
	RC_CtrlData.rc.ch4 -= 1024;

	RC_CtrlData.mouse.x = ((int16_t)buff[6]) | ((int16_t)buff[7] << 8);
	RC_CtrlData.mouse.y = ((int16_t)buff[8]) | ((int16_t)buff[9] << 8);
	RC_CtrlData.mouse.z = ((int16_t)buff[10]) | ((int16_t)buff[11] << 8);

	RC_CtrlData.mouse.press_l = buff[12];
	RC_CtrlData.mouse.press_r = buff[13];

	RC_CtrlData.key.info = ((int16_t)buff[14]) | ((int16_t)buff[15] << 8);
	
	RC_CtrlData.rc.sw1_last = RC_CtrlData.rc.sw1;
	RC_CtrlData.rc.sw2_last = RC_CtrlData.rc.sw2;

	if (	(abs(RC_CtrlData.rc.ch1) > 660) || \
	        (abs(RC_CtrlData.rc.ch2) > 660) || \
	        (abs(RC_CtrlData.rc.ch3) > 660) || \
	        (abs(RC_CtrlData.rc.ch4) > 660)	)
	{
		memset(&RC_CtrlData.rc, 0, sizeof(rc_info_t));
	}

	//清遥控器误差
	if(fabs((float)RC_CtrlData.rc.ch1)<=6)	RC_CtrlData.rc.ch1=0;
	if(fabs((float)RC_CtrlData.rc.ch2)<=6)	RC_CtrlData.rc.ch2=0;
	if(fabs((float)RC_CtrlData.rc.ch3)<=6)	RC_CtrlData.rc.ch3=0;
	if(fabs((float)RC_CtrlData.rc.ch4)<=6)	RC_CtrlData.rc.ch4=0;

	if(RC_CtrlData.rc.ch1>650)	RC_CtrlData.rc.ch1 =  650;
	if(RC_CtrlData.rc.ch1<-650)	RC_CtrlData.rc.ch1 = -650;
	if(RC_CtrlData.rc.ch2>650)	RC_CtrlData.rc.ch2 =  650;
	if(RC_CtrlData.rc.ch2<-650)	RC_CtrlData.rc.ch2 = -650;
	if(RC_CtrlData.rc.ch3>650)	RC_CtrlData.rc.ch3 =  650;
	if(RC_CtrlData.rc.ch3<-650)	RC_CtrlData.rc.ch3 = -650;
	if(RC_CtrlData.rc.ch4>650)	RC_CtrlData.rc.ch4 =  650;
	if(RC_CtrlData.rc.ch4<-650)	RC_CtrlData.rc.ch4 = -650;

}


void SetInputMode(void)	//设置输入模式
{
	if (RC_CtrlData.rc.sw1 == 1)		//上
		mode = OK;
  if (RC_CtrlData.rc.sw1 == 3)		//中
    mode = DEBUG;
  if(RC_CtrlData.rc.sw1 == 2) 	//下
		mode = STOP;
}

//按B退出任何模式（FunctionMODE == STANDBY）
void SetFuncMode(void)  //设置功能模式
{
	static int32_t pos = 0;
  if(FunctionMODE == STANDBY)
  {
     //shift+Q 进入空接（不用考虑转矿）
    if(Key_Check_Hold(&Keys.KEY_Q) && Key_Check_Hold(&Keys.KEY_SHIFT))
      FunctionMODE = FETCHAIRMODE;

    //shift+E 进入取大资源岛矿（坑内）（考虑转矿）
    if(Key_Check_Hold(&Keys.KEY_E) && Key_Check_Hold(&Keys.KEY_SHIFT))
      FunctionMODE = FETCHBIGMODE;
    
    //shift+R 进入取小资源岛矿（不用考虑转矿）
    if(Key_Check_Hold(&Keys.KEY_R) && Key_Check_Hold(&Keys.KEY_SHIFT))
      FunctionMODE = FETCHSMALLMODE;    
//    
    //shift+F 进入兑换（两种情况）
    if(Key_Check_Hold(&Keys.KEY_F) && Key_Check_Hold(&Keys.KEY_SHIFT))
      FunctionMODE = EXCHANGEMODE;
    
    //shift+G 进入拖拽救援
    if(Key_Check_Hold(&Keys.KEY_G) && Key_Check_Hold(&Keys.KEY_SHIFT))
      FunctionMODE = RESCUEMODE; 
		
		//ctrl+G 进入伸卡救援
		if(Key_Check_Hold(&Keys.KEY_G) && Key_Check_Hold(&Keys.KEY_CTRL))
			FunctionMODE = CARDMODE;

    //shift+Z 进入障碍块搬运
    if(Key_Check_Hold(&Keys.KEY_Z) && Key_Check_Hold(&Keys.KEY_SHIFT))
      FunctionMODE = BARRIARMODE; 
		//ctrl+E 进入夹地上矿
		if(Key_Check_Hold(&Keys.KEY_E) && Key_Check_Hold(&Keys.KEY_CTRL))
      FunctionMODE = GNDOREMODE;
		
    if(Key_Check_Hold(&Keys.KEY_X) && Key_Check_Hold(&Keys.KEY_SHIFT))
		{
			pos = motor_msg[4].angle + 2000;
			if(pos >= LIFT_MAX)
				pos = LIFT_MAX;
			d_lift(pos);
		}
		if(Key_Check_Hold(&Keys.KEY_X) && Key_Check_Hold(&Keys.KEY_CTRL))
		{
			pos = motor_msg[4].angle - 2000;
			if(pos <= 1000)
				pos = 1000;
			d_lift(pos);		
		}
		BARRIAR_L = BARRIAR_DEAUULT_L;   //救援障碍块复原
		BARRIAR_R = BARRIAR_DEAUULT_R;	
		CAMERA = 	DEFAULT_CAMERA_ANGEL;
		
		HAL_GPIO_WritePin(VALVE2_GPIO_Port,VALVE2_Pin,GPIO_PIN_RESET);//缩回
		
		motor_msg[15].angle_desired = 1000;
    fetch_flag = 0;		
    fetch_exchange_stage = 0;
		barriar_stage = 0;
  }
	
	if(Key_Check_Hold(&Keys.KEY_C) && Key_Check_Hold(&Keys.KEY_SHIFT))
		spin_flag = 1;
	else if(Key_Check_Hold(&Keys.KEY_C) && Key_Check_Hold(&Keys.KEY_CTRL))
		spin_flag = 2;
	else if(Key_Check_Hold(&Keys.KEY_V) && Key_Check_Hold(&Keys.KEY_SHIFT))
		spin_flag = 3;
	else if(Key_Check_Hold(&Keys.KEY_V) && Key_Check_Hold(&Keys.KEY_CTRL))
		spin_flag = 4;
	else spin_flag = 0;
}

/*	解算所有电机期望 	motor_msg 	[0]~[3] 	底盘电机	\
                                  [4]~[5]		翻转		\

                                  [6]   		伸缩		\
                                  [8]~[10]	转矿		\
                                  [11]~[14]	抬升	
*/


void remote_control(void)
{  
  uint8_t i;

  SetInputMode();

  if(mode == OK || mode == DEBUG)
  {
    SetFuncMode();					
		for(i=0;i<12;i++)
      motor_msg[i+4].angle_desired = desired_angle[i+4];
  }
  else;

}

