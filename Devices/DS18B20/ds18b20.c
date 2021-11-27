# include "ds18b20.h"

//��ʱԼ16΢��
void delay_us(uint8_t n) {
  while(n--);
}

void DS18B20_init() {
  uint8_t i;
  
  DPORT = 1;
  delay_us(1);      //������ʱ
  DPORT = 0;
  delay_us(80);     //��ʱ480��960us
  DPORT = 1;
  
  i = 0;
  while(DPORT) {    //�ȴ�DS18B20��������
    delay_us(100);
    i++;
    if(i > 5) {     //Լ�ȴ�>5MS
      return;       //��ʼ��ʧ��
    }
  }
}

//дһ���ֽ�
void write_byte(uint8_t dat) {
  uint8_t i;
  
  for(i = 0; i < 8; i++) {
    DPORT = 0;            //ÿд��һλ����֮ǰ�Ȱ���������1us
    _nop_();
    DPORT = dat & 0x01;   //ȡ���λд��
    delay_us(10);         //��ʱ68us������ʱ������60us
    DPORT = 1;            //Ȼ���ͷ�����
    dat = dat >> 1;       //�ӵ�λ��ʼд
  }
  delay_us(10);
}

//��һ���ֽ�
uint8_t read_byte() {
  uint8_t i, dat = 0;
  
  for(i = 0; i < 8; i++) {
    DPORT = 0;            //�Ƚ���������1us
    _nop_();
    DPORT = 1;            //Ȼ���ͷ�����
    _nop_(); _nop_();
    _nop_(); _nop_();
    if(DPORT) {
      dat = dat | 0x80;   //ÿ�ζ�һλ
    }
    dat = dat >> 1;       //�����λ��ʼ��
    delay_us(10);         //��ȡ��֮��ȴ�48us�ٽ��Ŷ�ȡ��һ����
  }
  return dat;
}

int16_t read_temper() {    
  uint8_t a, b;         
  int16_t t = 0;
  
  DS18B20_init();     
  delay_us(15);
  write_byte(0xcc);       //����ROM��������
  write_byte(0x44);       //���������¶�ת������
  
  DS18B20_init();       
  delay_us(15);
  write_byte(0xcc);       //����ROM��������
  write_byte(0xbe);       //���Ͷ��¶ȼĴ�������
  
  a = read_byte();        //�ȶ��Ͱ�λ
  b = read_byte();        //�ٶ��߰�λ
  t = b;        
  t <<= 8;                //���ư�λ
  t = t | a;              //tΪ16λ������ʹ�߰�λΪb��ֵ���Ͱ�λΪa��ֵ  
  return t;               //�����¶�ֵ
}

int16_t temper_change() {
  int16_t temper;
  float tp;
  temper = read_temper();
  if(temper < 0) {              //���Ǹ��¶ȵ����
    temper -= 1;
    temper = ~temper;
  }
  tp = temper;
  temper = tp * 0.0625 * 100 + 0.5;
  return temper;
}
