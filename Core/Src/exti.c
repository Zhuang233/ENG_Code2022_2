#include "exti.h"

void Switch_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	//使能 SYSCFG 时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);	//	开启GPIO的时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//PB0 PB1
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_12;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	//PE4 PE12
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	//PF0
}

void CamInit(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_0);
}
