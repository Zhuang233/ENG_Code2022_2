#include "user_lib.h"

void float_constraint(float *data,float max,float min)
{
	if(*data>max)*data=max;
	else if(*data<min)*data=min;
}
void int16_constraint(int16_t *data,int16_t max,int16_t min)
{
	if(*data>max)*data=max;
	else if(*data<min)*data=min;
}

void int32_constraint(int32_t *data,int32_t max,int32_t min)
{
	if(*data>max)*data=max;
	else if(*data<min)*data=min;
}

float sildemeanfilter(slide_mean_struct *obj,float newValue)
{
	int16_t pointer=obj->Buf_Pointer;
	float outValue;
	
	obj->buf_sum +=newValue;
	obj->data_count++;
	if(obj->data_count>obj->FIFO_SIZE)
	{
		obj->data_count=obj->FIFO_SIZE;
		obj->buf_sum-=obj->FIFO_BUF[pointer];
	}
	
	obj->FIFO_BUF[pointer]=newValue;
	obj->Buf_Pointer++;	
	if(obj->Buf_Pointer==obj->FIFO_SIZE)
		obj->Buf_Pointer=0;
	
	outValue=obj->buf_sum/obj->data_count;
	
	return outValue;
}

void ramp_init(ramp_struct *ramp,float slope,float dt)
{
  ramp->output = 0;
  ramp->slope = slope;  
  ramp->dt = dt;
}

void ramp_calculate(ramp_struct *ramp,float target)
{
  if(target > 0)
  {
    if(ramp->output < target)   
      ramp->output += ramp->slope*ramp->dt;   
    if(ramp->output > target) 
      ramp->output = target;
  }
  else if(target < 0)
  {
    if(ramp->output > target)
      ramp->output -= ramp->slope*ramp->dt;
    if(ramp->output < target)
      ramp->output = target;
  }
  else 
  {
    if(ramp->output > 5.0f)
      ramp->output -= 10.0f;
    else if(ramp->output < -5.0f)
      ramp->output += 10.0f;
    else ramp->output = 0;
  }
}