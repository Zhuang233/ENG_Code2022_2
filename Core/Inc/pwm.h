#ifndef __PWM_H
#define __PWM_H
#include "main.h"


#define RESCUE_L TIM4->CCR3
#define RESCUE_R TIM4->CCR4
#define CAMERA TIM8->CCR1
#define RESCUE_DOWN_L 810
#define RESCUE_DOWN_R 770
#define RESCUE_UP_L 1250
#define RESCUE_UP_R 300
#define RESCUE_CAMERA_ANGEL 600

#define DEFAULT_CAMERA_ANGEL 810;

#define CARD_CAMERA_ANGEL 600;

void PWM_Start(void);

#endif
