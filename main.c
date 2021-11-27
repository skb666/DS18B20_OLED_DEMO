#include <rtx51tny.h>
#include <STC89C5xRC.H>
#include "main.h"
#include <stdio.h>
#include <string.h>

volatile KEY S3 = {KEY_CHECK, 0},
             S4 = {KEY_CHECK, 0};
volatile uint8_t tx_buf[25] = {0};
volatile int16_t temperature;

void main_task(void) _task_ 0 {
  LED0 = 1;
  LED1 = 1;
  UsartInit();
  OLED_Init();
  OLED_Clear();
  
//  sprintf(tx_buf, "Hello World!\r\n");
//  SendString(tx_buf, strlen(tx_buf));
  OLED_ShowChinese(20, 3, 0);
  OLED_ShowChinese(20+16, 3, 1);
  
  os_create_task (1);   //创建任务1、2
  os_create_task (2);    
//  os_delete_task (0);   //挂起本任务0，不再运行本任务
  
  for(;;) {
    if(S3.flag == 1) {
      LED0 = 0;
      S3.flag = 0;
    } else if(S3.flag == 2) {
      LED0 = 1;
      S3.flag = 0;
    }
    
    if(S4.flag == 1) {
      LED1 = !LED1;
      S4.flag = 0;
    } else if(S4.flag == 2) {
      sprintf(tx_buf, "temp: %d\r\n", temperature);
      SendString(tx_buf, strlen(tx_buf));
      S4.flag = 0;
    }
    
    sprintf(tx_buf, ":%6d", temperature);
    OLED_ShowString(20+32, 3, tx_buf, S8x16);
  }
}

void key_task(void) _task_ 1 {
  for(;;) {
    KeyScan(&S3, KEY0);
    KeyScan(&S4, KEY1);
    os_wait2(K_TMO, 10);
  }
}

void ds18b20_task(void) _task_ 2 {
  temperature = temper_change();
  for(;;) {
    os_wait2(K_TMO, 1000);
    temperature = temper_change();
  }
}
