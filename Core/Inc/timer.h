#ifndef __TIME_H
#define __TIME_H
#include "stm32f4xx_hal.h"

typedef struct 
{
	uint32_t time_now;
	uint32_t time_last;
	float 	 dt;
	float	 time_total;
}TIME;

extern uint8_t delay_flag;

void TIM2_Init(uint32_t prd,uint32_t psc);
void get_dt_in_micros(TIME *time);
void get_dt_in_millis(TIME *time);
void get_dt_in_seconds(TIME *time);
void calculate_dt_in_micros(uint32_t time_now,uint32_t time_last,uint32_t *dt);
void calculate_dt_in_mills(uint32_t time_now,uint32_t time_last,float *dt);
void calculate_dt_in_seconds(uint32_t time_now,uint32_t time_last,float *dt);
void get_totaltime_in_seconds(TIME *time);
void delay_in_seconds(TIME *time,float delay_time);
void delay_ms(uint16_t time);

#endif
