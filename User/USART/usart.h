#ifndef __USART_H_
#define __USART_H_

#include <STC89C5xRC.H>
#include "type.h"

#define FOSC 11059200L
#define BAUD 9600

#define NONE_PARITY   0
#define ODD_PARITY    1
#define EVEN_PARITY   2
#define MARK_PARITY   3
#define SPACE_PARITY  4

#define PARITYBIT NONE_PARITY

void UsartInit();
void SendData(uint8_t dat);
void SendString(uint8_t *str, uint16_t _size);

#endif