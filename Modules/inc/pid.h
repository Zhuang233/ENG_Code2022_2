#ifndef __PID_H
#define __PID_H

#include <stdbool.h>
#include "main.h"
#include "timer.h"
//#include "params.h"

#define DEFAULT_PID_INTEGRATION_LIMIT 1000;

typedef struct Pid_Object
{
	float desired;      //< set point
  
	float error;        //< error
	float prevError;    //< previous error
  
	float integ;        //< integral
	float deriv;        //< derivative
  
	float kp;           //< proportional gain
	float ki;           //< integral gain
	float kd;           //< derivative gain
  
	float outP;         //< proportional output (debugging)
	float outI;         //< integral output (debugging)
	float outD;         //< derivative output (debugging)
  float outPID;       //< pid total output
  
	float iLimit;       //< integral limit
	float iLimitLow;    //< integral limit
	bool  wheInteg;     //< whether intergral
  
  float outLimit;      //< output limit
  float outLimitLow;   //< output limit
  
  TIME time;          //calculate dt
  bool first_cal;     //if first calculate pid
  
  void (*pid_calculate)(struct Pid_Object *pid,float desired,float measured); //< pid calculate
  
}PidObject;



void pid_calculate(PidObject* pid,float desired,float measured);
void pidInit(PidObject* pid, const float iLimit, const float outLimit, const float kp,
             const float ki, const float kd);
void pidSetIntegralLimit(PidObject* pid, const float limit);
void pidSetIntegralLimitLow(PidObject* pid, const float limitLow);
void pidSetIntegLimit(PidObject* pid, const float limit, const float limitLow);
float pidUpdate(PidObject* pid, const float measured);
void pidSetDesired(PidObject* pid, const float desired);
void pidSetError(PidObject* pid, const float error);
float pidGetDesired(PidObject* pid);
void pidClearIntegral(PidObject *pid);
void pidSetWhetherIntegral(PidObject *pid,bool wheInteg);
void pidSetDt(PidObject* pid, const float dt);
void pidSetKp(PidObject* pid, const float kp);
void pidSetKi(PidObject* pid, const float ki);
void pidSetKd(PidObject* pid, const float kd);
void pidParameterSet(PidObject* pid,const float kp,const float ki, const float kd);
void pidSetOutputLimit(PidObject* pid, const float limit, const float limitLow);

#endif
