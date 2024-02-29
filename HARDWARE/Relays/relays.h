#ifndef _RELAY_H 
#define _RELAY_H 
#include "sys.h"
#include "stm32f10x.h"
void relay_init(void);
void relay_on(void);
void relay_off(void);
#define relays PBout(1)
#endif
