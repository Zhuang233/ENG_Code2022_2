#ifndef __CAN_H
#define __CAN_H	 

#include "motor.h"
    
 	 
typedef enum
{
	CAN_Motor_ALL_ID = 0x200,
	CAN_Motor1_ID = 0x201,
	CAN_Motor2_ID = 0x202,
	CAN_Motor3_ID = 0x203,
	CAN_Motor4_ID = 0x204,
	CAN_Motor5_ID = 0x205,
	CAN_Motor6_ID = 0x206,
	CAN_Motor7_ID = 0x207,
	CAN_Motor8_ID = 0x208,
}CAN_Message_ID;


void can_filter_init(void);
										 					
void cal_angle(Motor_Msg *motor);
void getMotor_message(Motor_Msg *motor,uint8_t *data);
void CAN2_Set_Current(void);
void CAN1_Set_Current(void);
void CAN1_Set_AheadCur(int16_t C1, int16_t C2, int16_t C3, int16_t C4);
void CAN1_Set_BackCur(int16_t C1, int16_t C2, int16_t C3, int16_t C4);
void CAN2_Set_AheadCur(int16_t C1, int16_t C2, int16_t C3, int16_t C4);
void CAN2_Set_BackCur(int16_t C1, int16_t C2, int16_t C3, int16_t C4);

#endif

