#ifndef __I2C_H_
#define __I2C_H_
#include <STC12C5A.h>

sbit SCL=P2^1;
sbit SDA=P2^0;


void I2cStart();
void I2cStop();

unsigned char I2cSendByte(unsigned char dat);  //发送数据

void oled_Write_cmd(unsigned char cmd); //写命令
void oled_Write_data(unsigned char dat); //写数据
void oled_initial(); //初始化
void oled_clear( ); //清屏
void delay( unsigned char i);
void Delay10us();
void oled_put_char_16x16(unsigned char x,unsigned char y,unsigned char t);/*设置显示坐标函数,t为0时，字符为8x16 。t为1时，字符为16x16*/
void oleddisplay (unsigned char displaymode);//此为显示                                                                   
 

#endif
