#include "key.h"

void KeyScan(KEY *key, uint8_t pin)
{
    switch(key->state)
    {
        case KEY_CHECK:
        {
            if(pin == 0)
            {
                key->state = KEY_COMFIRM;
            }
        }
        break;

        case KEY_COMFIRM:
        {
            if(pin == 0)
            {
                key->state = KEY_RELEASE;
                key->flag = 1;
            }
            else
            {
                key->state = KEY_CHECK;
            }
        }
        break;

        case KEY_RELEASE:
        {
            if(pin == 1)
            {
                key->state = KEY_CHECK;
                key->flag = 2;
            }
        }
        break;
    }
}
