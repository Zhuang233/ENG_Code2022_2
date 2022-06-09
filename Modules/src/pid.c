#include "pid.h"
#include "user_lib.h"

void pidInit(PidObject* pid, const float iLimit, const float outLimit, const float kp,
             const float ki, const float kd)
{
	pid->error     = 0;
	pid->prevError = 0;
	pid->integ     = 0;
	pid->deriv     = 0;
	pid->outPID	   = 0;
	pid->desired = 0;
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->iLimit    = iLimit;
	pid->iLimitLow = -iLimit;
	pid->outLimit = outLimit;
	pid->outLimitLow = -outLimit;
  pid->time.dt = pid->time.time_last = pid->time.time_now = pid->time.time_total = 0;
	pid->wheInteg  = true;
  pid->pid_calculate = pid_calculate;
}

void pidParameterSet(PidObject* pid,float kp,float ki,float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
}

void pid_calculate(PidObject* pid,float desired,float measured)
{
  get_dt_in_seconds(&pid->time);
  if(pid->first_cal == true)
  {
    pid->first_cal = false;
    pid->time.dt = 0.01;
  }
  pid->desired = desired;
  pid->error = pid->desired - measured;
  
	if (pid->wheInteg)
	{
		pid->integ += pid->error * pid->time.dt;
	}

	pid->deriv = (pid->error - pid->prevError) / pid->time.dt;

	pid->outP = pid->kp * pid->error;
	pid->outI = pid->ki * pid->integ;
	pid->outD = pid->kd * pid->deriv;
	float_constraint(&pid->outI,pid->iLimit,pid->iLimitLow);
	pid->outPID = pid->outP + pid->outI + pid->outD;
	float_constraint(&pid->outPID,pid->outLimit,pid->outLimitLow);
  
	pid->prevError = pid->error;
}

void pidSetOutputLimit(PidObject* pid, const float limit, const float limitLow)
{
	pid->outLimit = limit;
	pid->outLimitLow = limitLow;
}

void pidSetError(PidObject* pid, const float error)
{
	pid->error = error;
}

void pidSetDesired(PidObject* pid, const float desired)
{
	pid->desired = desired;
}

float pidGetDesired(PidObject* pid)
{
	return pid->desired;
}

void pidSetIntegralLimit(PidObject* pid, const float limit) {
	pid->iLimit = limit;
}

void pidSetIntegralLimitLow(PidObject* pid, const float limitLow) {
	pid->iLimitLow = limitLow;
}

void pidSetIntegLimit(PidObject* pid, const float limit, const float limitLow)
{
	pid->iLimit = limit;
	pid->iLimitLow = limitLow;
}

void pidSetWhetherIntegral(PidObject *pid,bool wheInteg)
{
	pid->wheInteg = wheInteg;
}
void pidClearIntegral(PidObject *pid)
{
	pid->integ=0;
	pid->outI=0;
}
void pidSetKp(PidObject* pid, const float kp)
{
	pid->kp = kp;
}

void pidSetKi(PidObject* pid, const float ki)
{
	pid->ki = ki;
}

void pidSetKd(PidObject* pid, const float kd)
{
	pid->kd = kd;
}
void pidSetDt(PidObject* pid, const float dt)
{
	pid->time.dt = dt;
}

