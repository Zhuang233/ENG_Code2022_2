#ifndef __BASECALCULATE_H
#define __BASECALCULATE_H

#include "main.h"
#include "timer.h"

typedef struct 
{
	int16_t FIFO_SIZE;
	int16_t Buf_Pointer;
	int16_t data_count;
	float buf_sum;
	float *FIFO_BUF;
}slide_mean_struct;

typedef struct
{
  float output;
  float slope;
  float dt;
}ramp_struct;

float sildemeanfilter(slide_mean_struct *obj,float newValue);
void ramp_calculate(ramp_struct *obj,float target);
void ramp_init(ramp_struct *ramp,float slope,float dt);
void int16_constraint(int16_t *data,int16_t max,int16_t min);
void float_constraint(float *data,float max,float min);
void int32_constraint(int32_t *data,int32_t max,int32_t min);

#endif
