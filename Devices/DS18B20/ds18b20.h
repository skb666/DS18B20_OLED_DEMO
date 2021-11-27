#ifndef __DS18B20_H_
#define __DS18B20_H_

#include <STC89C5xRC.H>
#include "intrins.h"
#include "type.h"

sbit DPORT = P4^2;

void DS18B20_init();
int16_t read_temper();
int16_t temper_change();

#endif
