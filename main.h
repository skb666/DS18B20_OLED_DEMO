#ifndef __DEMO_MAIN_H_
#define __DEMO_MAIN_H_

#include <STC89C5xRC.H>
#include "type.h"

sbit LED0 = P3 ^ 7;
sbit LED1 = P4 ^ 0;

#include "usart.h"

#include "key.h"
#include "ds18b20.h"
#include "oled.h"

#endif
