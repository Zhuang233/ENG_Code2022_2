#ifndef __ALLTASK_H
#define __ALLTASK_H

#include "main.h"

extern uint8_t card;
void chassis_task(void);
void ore_task(void);
void remote_task(void);
void other_task(void);
void rescue_task(void);
void barriar_task(void);
void card_task(void);
#endif