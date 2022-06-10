#ifndef __WHT101_H
#define __WHT101_H
#include "stdint.h"

extern uint8_t yaw_data;
extern float chassis_yaw;
extern float last_yaw;
extern int32_t yaw_turns;

void decode_WHT101_data(void);


#endif
