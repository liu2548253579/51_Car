#ifndef __I2C_H_
#define __I2C_H_
#include <STC12C5A.h>

sbit SCL=P2^1;
sbit SDA=P2^0;


void I2cStart();
void I2cStop();

unsigned char I2cSendByte(unsigned char dat);  //��������

void oled_Write_cmd(unsigned char cmd); //д����
void oled_Write_data(unsigned char dat); //д����
void oled_initial(); //��ʼ��
void oled_clear( ); //����
void delay( unsigned char i);
void Delay10us();
void oled_put_char_16x16(unsigned char x,unsigned char y,unsigned char t);/*������ʾ���꺯��,tΪ0ʱ���ַ�Ϊ8x16 ��tΪ1ʱ���ַ�Ϊ16x16*/
void oleddisplay (unsigned char displaymode);//��Ϊ��ʾ                                                                   
 

#endif
