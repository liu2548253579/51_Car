#include"i2c.h"
#include"intrins.h"


void Delay10us()
{
	unsigned char i;

	_nop_();
	i = 25;
	while (--i);
}
void I2cStart()
{
	SDA=1;
	Delay10us();
	SCL=1;
	Delay10us();//����ʱ����SDA����ʱ��>4.7us
	SDA=0;
	Delay10us();//����ʱ����>4us
	SCL=0;			
	Delay10us();		
}
void I2cStop()
{
	SDA=0;
	Delay10us();
	SCL=1;
	Delay10us();//����ʱ�����4.7us
	SDA=1;
	Delay10us();		
}
unsigned char I2cSendByte(unsigned char dat)
{
    unsigned char a=0,b=0;//���255��һ����������Ϊ1us�������ʱ255us��		
a=0;  while (1){//Ҫ����8λ�������λ��ʼ
		SDA=dat>>7;	 //��ʼ�ź�֮��SCL=0�����Կ���ֱ�Ӹı�SDA�ź�
		dat=dat<<1;
		Delay10us();
		SCL=1;
		Delay10us();//����ʱ��>4.7us
		SCL=0;
		Delay10us();//ʱ�����4us		
a = a+1;if (a >= 8){break;}	}
	SDA=1;
	Delay10us();
	SCL=1;
	while(SDA)//�ȴ�Ӧ��Ҳ���ǵȴ����豸��SDA����
	{
		b++;
		if(b>200)	 //�������2000usû��Ӧ����ʧ�ܣ�����Ϊ��Ӧ�𣬱�ʾ���ս���
		{
			SCL=0;
			Delay10us();
			return 0;
		}
	}
	SCL=0;
	Delay10us();
 	return 1;		
}

void oled_Write_cmd(unsigned char cmd)
{
	I2cStart();
	I2cSendByte(0x78);//����д������ַ
	I2cSendByte(0x00);//����Ҫд���ڴ��ַ
	I2cSendByte(cmd);	//д������
	I2cStop();
}

void oled_Write_data(unsigned char dat)
{
	I2cStart();
	I2cSendByte(0x78);//����д������ַ
	I2cSendByte(0x40);//����Ҫд���ڴ��ַ
	I2cSendByte(dat);	//д������
	I2cStop();
}

void oled_clear()  //ҳѰַ�µ�oled��������
{	
	unsigned char i,j; 
	oled_Write_cmd(0x20);
	oled_Write_cmd(0x02);
i=0;  while (1){ 
	   	oled_Write_cmd(0xb0+i);
		oled_Write_cmd(0x00);
		oled_Write_cmd(0x10);
j=0; while (1){
		   oled_Write_data(0x00); 
j= j+1;if (j >= 128){break;}}

i = i+1;if (i >= 8){break;}	}
}

void oled_initial()//oled��ʼ������
{
			 
	oled_Write_cmd(0xae);//--turn off oled panel �ر���ʾ
    oled_Write_cmd(0x00);//---set low column address������ʼ�еĵ���λ 0x0x
    oled_Write_cmd(0x10);//---set high column address������ʼ�еĸ���λ0x1x
    oled_Write_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    oled_Write_cmd(0x81);//--set contrast control register���öԱȶ�
    oled_Write_cmd(0xff); // Set SEG Output Current Brightness�Աȶ�Ϊoxff
    oled_Write_cmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    oled_Write_cmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    oled_Write_cmd(0xa6);//--set normal display
    oled_Write_cmd(0xa8);//--set multiplex ratio(1 to 64)
    oled_Write_cmd(0x3f);//--1/64 duty
    oled_Write_cmd(0xd3);//-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    oled_Write_cmd(0x00);//-not offset
    oled_Write_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
    oled_Write_cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    oled_Write_cmd(0xd9);//--set pre-charge period
    oled_Write_cmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    oled_Write_cmd(0xda);//--set com pins hardware configuration
    oled_Write_cmd(0x12);
    oled_Write_cmd(0xdb);//--set vcomh
    oled_Write_cmd(0x40);//Set VCOM Deselect Level
    oled_Write_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)���õ�ַģʽ
						//ˮƽѰַ����ֱѰַ��ҳѰַ
    oled_Write_cmd(0x02);//	��ַģʽΪҳѰַ
    oled_Write_cmd(0x8d);//--set Charge Pump enable/disable
    oled_Write_cmd(0x14);//--set(0x10) disable
    oled_Write_cmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    oled_Write_cmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
    oled_Write_cmd(0xaf);//--turn on oled panel������ʾ

	oled_clear();//����
}

void oled_put_char_16x16(unsigned char x,unsigned char y,unsigned char t)/*������ʾ���꺯��,tΪ0ʱ���ַ�Ϊ8x16
                                                       tΪ1ʱ���ַ�Ϊ16x16*/                 
{	 
     oled_Write_cmd(0x20);
	 oled_Write_cmd(0x00);//���õ�ַģʽΪˮƽѡַ
     //set page
     oled_Write_cmd(0x22);
	 oled_Write_cmd(y*2);
	 oled_Write_cmd(0x01+y*2);

	 //set colum
     oled_Write_cmd(0x21);
	 oled_Write_cmd((0x08+0x08*t)*x);
	 oled_Write_cmd((0x08+0x08*t)*x+(0x07+0x08*t));
}



 


