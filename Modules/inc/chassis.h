#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "timer.h"
#include "user_lib.h"


extern ramp_struct chassis_ramp[2];
void chassis_ramp_init(float slope,float dt);
void chassis_control(void);
#endif