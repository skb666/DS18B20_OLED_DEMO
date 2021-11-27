#include "usart.h"

volatile bit busy = 0;

void UsartInit() {
#if(PARITYBIT==NONE_PARITY)
  SCON = 0x50;    //8-bit variable UART
#elif(PARITYBIT==ODD_PARITY)||(PARITYBIT==EVEN_PARITY)||(PARITYBIT==MARK_PARITY)
  SCON = 0xda;    //9-bit variable UART, parity bit initial to 1
#elif(PARITYBIT==SPACE_PARITY)
  SCON = 0xd2;    //9-bit variable UART, parity bit initial to 0
#endif
	TMOD = 0X20;	  //���ü�����������ʽ2
  TH1 = TL1 = -(FOSC/12/32/BAUD);
  TR1=1;					//�򿪼�����
  ES=1;						//�򿪽����ж�
	EA=1;						//�����ж�
}

void SendData(uint8_t dat) {
  while(busy);
  ACC = dat;
  if(P) {
  #if(PARITYBIT==ODD_PARITY)
    TB8 = 0;
  #elif(PARITYBIT==EVEN_PARITY)
    TB8 = 1;
  #endif
  } else {
  #if(PARITYBIT==ODD_PARITY)
    TB8 = 1;
  #elif(PARITYBIT==EVEN_PARITY)
    TB8 = 0;
  #endif
  }
  busy = 1;
  SBUF = ACC;
}

void SendString(uint8_t *str, uint16_t _size) {
  uint16_t i;
  for(i = 0; i < _size; ++i) {
    SendData(str[i]);
  }
}

void Usart() interrupt 4 using 2 {
	if(RI) {
    RI = 0;     //��������жϱ��
  }
  if(TI) {
    TI = 0;     //��������жϱ��
    busy = 0;   //�������æ���
  }
}
