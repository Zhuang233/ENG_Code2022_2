#ifndef __REMOTE_CONTROL_H
#define __REMOTE_CONTROL_H
#include "stdint.h"
#include "timer.h"
#include "main.h"


#define RC_FRAME_LENGTH 18

#define STANDBY		0           
#define FETCHAIRMODE 		1
#define FETCHBIGMODE    2
#define FETCHSMALLMODE 		3
#define EXCHANGEMODE 4
#define SWIPEMODE	5
#define RESCUEMODE 6
#define CARDMODE 7
#define BARRIARMODE 8
#define GNDOREMODE 9

//输入模式:遥控器/键盘鼠标/停止运行
#define OK 1
#define DEBUG 3
#define STOP 2

//mouse control parameters
#define MOUSE_TO_PITCH_ANGLE_INC_FACT 		15.0f
#define MOUSE_TO_YAW_ANGLE_INC_FACT 		6.0f

//不能写1，要给自转留点
#define LOW_SPEED 			0.6
#define HIGH_SPEED 			0.8


#define F_POS 50000000.0f
#define VAL_LIMIT(val, min, max) \
if(val<=min)\
{\
	val = min;\
}\
else if(val>=max)\
{\
	val = max;\
}\

extern uint8_t input_mode;
extern TIME ramp_time;
extern int32_t desired_angle[16];
extern uint8_t FunctionMODE;
extern uint8_t mode;
extern uint8_t spin_flag;

typedef __packed struct
{
	/* rocker channel information */
	int16_t ch1;
	int16_t ch2;
	int16_t ch3;
	int16_t ch4;

	/* left and right lever information */
	uint8_t sw1;
	uint8_t sw2;
	uint8_t sw1_last;
	uint8_t sw2_last;
} rc_info_t;

typedef __packed struct
{
	int16_t x;
	int16_t y;
	int16_t z;
	uint8_t last_press_l;
	uint8_t last_press_r;
	uint8_t press_l;
	uint8_t press_r;
} rc_info_mouse;

typedef	__packed struct
{
	uint16_t info;
}rc_info_key;

typedef __packed struct
{
	rc_info_t rc;
	rc_info_mouse mouse;
	rc_info_key key;
} RC_Ctl_t;

extern RC_Ctl_t RC_CtrlData;
extern int32_t pos1,pos2,pos3,pos4,pos5;

void DBUS_decode(uint8_t *buff);
void remote_control(void);
void SetInputMode(void);
void SetFuncMode(void);

#endif
