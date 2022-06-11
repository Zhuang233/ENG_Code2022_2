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
  
    
  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);  //ÉãÏñÍ·
	
  TIM4->CCR1 = 900;							//ÓÒ¶æ»ú³õÊ¼900£¬ÊúÆð Ë®Æ½330 Ì§ÕÏ°­¿é 760
	TIM4->CCR2 = 650;								//×ó¶æ»ú		650					1230         800
  
	TIM4->CCR3 = 1250;								//¾ÈÔ®×ó³õÊ¼1250		810
	TIM4->CCR4 = 300;								  //¾ÈÔ®ÓÒ    300    770
	TIM8->CCR1 = 750;								
  
}

