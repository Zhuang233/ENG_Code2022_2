#include "pwm.h"


// Í¼´«*1 ¼Ð×Ó*1 ÉãÏñÍ·*1 ¾ÈÔ®*2 ¸´»î¿¨*2 ÕÏ°­¿é*2

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;

void PWM_Start(void)
{
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);  //ÕÏ°­¿éÓÒ¶æ»ú
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);  //ÕÏ°­¿é×ó
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);  //¾ÈÔ®×ó
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);  //¾ÈÔ®ÓÒ
  
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);  //¸´»î¿¨ÏòÏÂ
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);  //¸´»î¿¨Éì³ö
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);  //ÕÏ°­¿é×ó
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);  //ÕÏ°­¿éÓÒ
    
  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);  //ÉãÏñÍ·
	
  TIM4->CCR1 = 1250;							//ÓÒ¶æ»ú³õÊ¼1250£¬ÊúÆð Ë®Æ½750 Ì§ÕÏ°­¿é1150
	TIM4->CCR2 = 250;								//×ó¶æ»ú		250						 750         350
  
	TIM4->CCR3 = 1250;								//¾ÈÔ®×ó³õÊ¼1250		810
	TIM4->CCR4 = 300;								  //¾ÈÔ®ÓÒ    300    770
	TIM8->CCR1 = 750;								
  
}

