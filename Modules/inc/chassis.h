#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "timer.h"
#include "user_lib.h"
#include <stdbool.h>
#include "arm_math.h"
#include "math.h"
#include <stdlib.h>
#include "Key.h"
#include "timer.h"
#include "main.h"
#include "user_lib.h"
#include "remote_control.h"
#include "motor.h"

extern int16_t vx,vy,vr;		//其他任务底盘控制接口
extern bool chassis_auto_control;

extern ramp_struct chassis_ramp[2];
void chassis_ramp_init(float slope,float dt);
void chassis_control(void);
#endif

