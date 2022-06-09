#ifndef __ENG_CONTROL_H
#define __ENG_CONTROL_H
#include "main.h"
#include "pid.h"

//extern uint8_t PITCHRATECTRL;
//extern uint8_t YAWATECTRL;

//extern PidObject pid_pitch;//holder pitch control.
//extern PidObject pid_pitchRate;//holder pitch rate control.
//extern PidObject pid_yaw;//holder yaw control.
//extern PidObject pid_yawRate;//holder yaw rate control.
//extern PidObject pid_friction_1,pid_friction_2;
extern PidObject pid_chassis_spd[4];

extern PidObject pid_lift_spd[4]; 
extern PidObject pid_lift_pos[4]; 
extern PidObject pid_lift_syn[4]; 

extern PidObject pid_flip_spd[2]; 
extern PidObject pid_flip_pos[2];

extern PidObject pid_spin_spd[2]; 

extern PidObject pid_gndrotate_spd; 
extern PidObject pid_gndrotate_pos; 

extern PidObject pid_gndgrip_spd;	
extern PidObject pid_gndgrip_pos;	

extern PidObject pid_gndmove_spd;
extern PidObject pid_gndmove_pos;

extern PidObject pid_card_spd;
extern PidObject pid_card_pos;	

extern PidObject pid_testmoto_spd[2];
extern PidObject pid_testmoto_pos[2];
extern PidObject pid_sync[2];

extern TIME delay_time;

extern uint8_t zero_flag;
//extern PidObject pid_chassis_spd[6];
//extern float yawRateDesired;
//extern float pitchRateDesired;

void SingleMotoTest(int32_t left,int32_t right);
void Testmoto_PID_Init(void);
void eng_pid_init(void);
void zeropoint_init(void);
void SteerengineControl(void);


#endif 
