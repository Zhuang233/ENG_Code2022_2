#include "remote_control.h"
#include "chassis.h"
#include <stdbool.h>
#include "arm_math.h"
#include "math.h"
#include <stdlib.h>
#include "Key.h"
#include "timer.h"
#include "main.h"
#include "user_lib.h"

#include "motor.h"

#define MAX_MOTOR_RMP	9200                //������ת��
#define MAX_MOVE_RMP	7700                //ƽ������ٶ�
#define MAX_ROTATE_RMP  4000   				//��ת����ٶ�
#define KEY_MOVE_RMP 4000             //���̿����ƶ�ת��
#define MOUSE_ROTATE_RMP 3000         //��������ת�ٶ�
#define MOUSE_ROTATE_PARA 10                	//������������תϵ��

ramp_struct chassis_ramp[2];


void chassis_ramp_init(float slope,float dt)
{
  uint8_t i;
  for(i=0;i<2;i++)
    ramp_init(&chassis_ramp[i],slope,dt);
}

void chassis_control(void)
{
	float norm = 0;
	float c1, c3, c4;
  bool key_flag = false;
	int16_t vx = 0,vy = 0,vr = 0;
  int16_t chassis_x = 0,chassis_y = 0,chassis_r = 0;
  
  if(RC_CtrlData.rc.ch1!=0 || RC_CtrlData.rc.ch3!=0 || RC_CtrlData.rc.ch4!=0)     //ң��������
  {
    norm = abs(RC_CtrlData.rc.ch3) + abs(RC_CtrlData.rc.ch4);
    c1 = (float)RC_CtrlData.rc.ch1/650.0;
    c3 = (float)RC_CtrlData.rc.ch3 / norm;
    c4 = (float)RC_CtrlData.rc.ch4 / norm;
    arm_sqrt_f32(RC_CtrlData.rc.ch3 * RC_CtrlData.rc.ch3 + RC_CtrlData.rc.ch4 * RC_CtrlData.rc.ch4, &norm);
    if(norm > 650)norm = 650;
    norm /= 650;
    norm *= MAX_MOVE_RMP;
    vx = c3 * norm;	// ƽ�Ʒ���
    vy = c4 * norm;	// ǰ������
    vr = c1 * MAX_ROTATE_RMP;
  }
  
  else if(Key_Check_Hold(& Keys.KEY_W) || Key_Check_Hold(& Keys.KEY_S) || Key_Check_Hold(& Keys.KEY_A)\
          || Key_Check_Hold(& Keys.KEY_D) || RC_CtrlData.mouse.x!=0)	     				//�������
  {
    key_flag = true;
    if(Key_Check_Hold(& Keys.KEY_W))
      chassis_y =  KEY_MOVE_RMP;
    if(Key_Check_Hold(& Keys.KEY_S))
      chassis_y = -KEY_MOVE_RMP;
    if(Key_Check_Hold(& Keys.KEY_A))
      chassis_x = -KEY_MOVE_RMP;
    if(Key_Check_Hold(& Keys.KEY_D))
      chassis_x =  KEY_MOVE_RMP;   
    if(RC_CtrlData.mouse.x!=0)   
    {
      chassis_r = RC_CtrlData.mouse.x*MOUSE_ROTATE_PARA;
      if(abs(chassis_r) > MOUSE_ROTATE_RMP)
        chassis_r = MOUSE_ROTATE_RMP * (chassis_r < 0 ? -1:1);
    }
    
  }
  
  ramp_calculate(&chassis_ramp[0],chassis_y);       //���̿����ƶ�ʹ��б�º���
  ramp_calculate(&chassis_ramp[1],chassis_x);
  
  if(key_flag == true)
  {
    vy = chassis_ramp[0].output;
    vx = chassis_ramp[1].output;
    vr = chassis_r;
  }
  
  motor_msg[0].speed_desired = -vy + vx + vr;		// 1  right front
  motor_msg[1].speed_desired =  vy + vx + vr;		// 2  left front
  motor_msg[2].speed_desired =  vy - vx + vr;		// 3	left back
  motor_msg[3].speed_desired = -vy - vx + vr;		// 4	right back  
}
