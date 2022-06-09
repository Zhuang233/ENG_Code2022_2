#ifndef __ORE_H
#define __ORE_H
#include "main.h"
#include <stdbool.h>

#define SMALL 1
#define BIG 2

extern uint8_t fetch_exchange_stage;
extern uint8_t fetch_flag;

void lift_task(void);

void flip_spin_task(void);

void fetch_in_air(void);

void ONE_Key_Exchange(void);
void Ore_Spin(void);
uint8_t Ore_Suckin(void);
void flip(int32_t pos);
void lift(int32_t pos);
void d_lift(int32_t pos);
void fetch_exchange_task(void);
void fetch_ore_in_hole(void);
void groundore_card_task(void);
void card_task(void);
#endif
