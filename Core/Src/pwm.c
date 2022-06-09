#include "pwm.h"

// Õº¥´*1 º–◊”*1 …„œÒÕ∑*1 æ»‘Æ*2 ∏¥ªÓø®*2 ’œ∞≠øÈ*2

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;

void PWM_Start(void)
{
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);  //’œ∞≠øÈ”“∂Êª˙
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);  //’œ∞≠øÈ◊Û
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);  //æ»‘Æ◊Û
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);  //æ»‘Æ”“
  
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);  //∏¥ªÓø®œÚœ¬
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);  //∏¥ªÓø®…Ï≥ˆ
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);  //’œ∞≠øÈ◊Û
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);  //’œ∞≠øÈ”“
    
  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);  //…„œÒÕ∑
	
  TIM4->CCR1 = 1250;							//”“∂Êª˙≥ı º1250£¨ ˙∆ ÀÆ∆Ω750 Ãß’œ∞≠øÈ1150
	TIM4->CCR2 = 250;								//◊Û∂Êª˙		250						 750         350
  
	TIM4->CCR3 = 1250;								//æ»‘Æ◊Û≥ı º1250		750
	TIM4->CCR4 = 300;								  //æ»‘Æ”“    300     800
  
}