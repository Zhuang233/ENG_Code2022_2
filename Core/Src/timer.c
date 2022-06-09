#include "timer.h"

uint8_t delay_flag = 0;
void get_dt_in_micros(TIME *time)
{
	time->time_now = TIM2->CNT;
	if(time->time_now <= time->time_last-0xFFFF)
	{
		time->dt = (float)(time->time_now + (0xffffffff - time->time_last));
		time->time_last=time->time_now;
	}
	else if(time->time_now > time->time_last)
	{
		time->dt = (float)(time->time_now - time->time_last)/1000.0f;
		time->time_last=time->time_now;
	}
}

void get_dt_in_millis(TIME *time)
{
	time->time_now = TIM2->CNT;
	if(time->time_now <= time->time_last-0xFFFF)
	{
		time->dt = (float)(time->time_now + (0xffffffff - time->time_last))/1000.0f;
		time->time_last=time->time_now;
	}
	else if(time->time_now > time->time_last)
	{
		time->dt = (float)(time->time_now - time->time_last)/1000.0f;
		time->time_last=time->time_now;
	}
}

void get_dt_in_seconds(TIME *time)
{
	time->time_now=TIM2->CNT;
	if(time->time_now <= time->time_last) 
	{
		time->dt = (float)(time->time_now + (0xffffffff - time->time_last))/1000000.0f;
	}
	else 
	{
		time->dt = (float)(time->time_now - time->time_last)/1000000.0f;
	}
	time->time_last=time->time_now;
}

void calculate_dt_in_micros(uint32_t time_now,uint32_t time_last,uint32_t *dt)
{

	if(time_now <= time_last)
		*dt = time_now + (0xffffffff- time_last);
	else
		*dt = time_now - time_last;
}

void calculate_dt_in_mills(uint32_t time_now,uint32_t time_last,float *dt)
{
	if(time_now <= time_last)
		*dt = (float)(time_now + (0xffffffff- time_last))/1000.0f;
	else
		*dt = (float)(time_now - time_last)/1000.0f;
}

void calculate_dt_in_seconds(uint32_t time_now,uint32_t time_last,float *dt)
{
	if(time_now <= time_last)
		*dt = (float)(time_now + (0xffffffff- time_last))/1000000.0f;
	else
		*dt = (float)(time_now - time_last)/1000000.0f;
}

void get_totaltime_in_seconds(TIME *time)
{
	time->time_now = TIM2->CNT;
	if(time->time_now <= time->time_last-0xFFFF)
	{
		time->dt =(float)(time->time_now + 0xffffffff - time->time_last)/1000000.0f;
		time->time_last=time->time_now;
		time->time_total += time->dt;
	}
	else if(time->time_now > time->time_last)
	{
		time->dt =(float)(time->time_now - time->time_last)/1000000.0f;
		time->time_last=time->time_now;
		time->time_total += time->dt;
	}
}

void delay_in_seconds(TIME *time,float delay_time)
{
	if(time->time_total < delay_time)
	{
		delay_flag = 0;
		if(time->time_now==0 && time->time_last==0)
		{
			time->time_now=time->time_last=TIM2->CNT;
		}
		else get_totaltime_in_seconds(time);		
	}
	else 
	{
		time->time_last = time->time_now = time->time_total = 0;
		delay_flag = 1;
	}
	//		time->time_now=time->time_last=time->time_total=0;
}

void delay_ms(uint16_t time)  //粗延时
{    
   uint16_t i=0;  
   while(time--)
   {
      i=12000;  //自己定义
      while(i--) ;    
   }
 }
