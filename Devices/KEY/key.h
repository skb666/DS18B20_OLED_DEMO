#ifndef __KEY_H_
#define __KEY_H_

#include <STC89C5xRC.H>
#include "type.h"

sbit KEY0 = P2 ^ 1;
sbit KEY1 = P2 ^ 0;

typedef enum
{
    KEY_CHECK = 0,  //�������״̬
    KEY_COMFIRM,    //����ȷ��״̬
    KEY_RELEASE     //�����ͷ�״̬
} KEY_STATE;

typedef struct
{
    KEY_STATE state;
    uint8_t flag;
} KEY;

void KeyScan(KEY *key, uint8_t pin);

#endif
