#include "pwm.h"


// ͼ��*1 ����*1 ����ͷ*1 ��Ԯ*2 ���*2 �ϰ���*2

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;

void PWM_Start(void)
{
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);  //�ϰ����Ҷ��
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);  //�ϰ�����
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);  //��Ԯ��
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);  //��Ԯ��
  
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);  //�������
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);  //������
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);  //�ϰ�����
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);  //�ϰ�����
    
  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);  //����ͷ
	
  TIM4->CCR1 = 1250;							//�Ҷ����ʼ1250������ ˮƽ750 ̧�ϰ���1150
	TIM4->CCR2 = 250;								//����		250						 750         350
  
	TIM4->CCR3 = 1250;								//��Ԯ���ʼ1250		810
	TIM4->CCR4 = 300;								  //��Ԯ��    300    770
	TIM8->CCR1 = 750;								
  
}

