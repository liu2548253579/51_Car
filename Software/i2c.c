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
	Delay10us();//建立时间是SDA保持时间>4.7us
	SDA=0;
	Delay10us();//保持时间是>4us
	SCL=0;			
	Delay10us();		
}
void I2cStop()
{
	SDA=0;
	Delay10us();
	SCL=1;
	Delay10us();//建立时间大于4.7us
	SDA=1;
	Delay10us();		
}
unsigned char I2cSendByte(unsigned char dat)
{
    unsigned char a=0,b=0;//最大255，一个机器周期为1us，最大延时255us。		
a=0;  while (1){//要发送8位，从最高位开始
		SDA=dat>>7;	 //起始信号之后SCL=0，所以可以直接改变SDA信号
		dat=dat<<1;
		Delay10us();
		SCL=1;
		Delay10us();//建立时间>4.7us
		SCL=0;
		Delay10us();//时间大于4us		
a = a+1;if (a >= 8){break;}	}
	SDA=1;
	Delay10us();
	SCL=1;
	while(SDA)//等待应答，也就是等待从设备把SDA拉低
	{
		b++;
		if(b>200)	 //如果超过2000us没有应答发送失败，或者为非应答，表示接收结束
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
	I2cSendByte(0x78);//发送写器件地址
	I2cSendByte(0x00);//发送要写入内存地址
	I2cSendByte(cmd);	//写入命令
	I2cStop();
}

void oled_Write_data(unsigned char dat)
{
	I2cStart();
	I2cSendByte(0x78);//发送写器件地址
	I2cSendByte(0x40);//发送要写入内存地址
	I2cSendByte(dat);	//写入数据
	I2cStop();
}

void oled_clear()  //页寻址下的oled清屏函数
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

void oled_initial()//oled初始化函数
{
			 
	oled_Write_cmd(0xae);//--turn off oled panel 关闭显示
    oled_Write_cmd(0x00);//---set low column address设置起始列的低四位 0x0x
    oled_Write_cmd(0x10);//---set high column address设置起始列的高四位0x1x
    oled_Write_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    oled_Write_cmd(0x81);//--set contrast control register设置对比度
    oled_Write_cmd(0xff); // Set SEG Output Current Brightness对比度为oxff
    oled_Write_cmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    oled_Write_cmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
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
    oled_Write_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)设置地址模式
						//水平寻址，垂直寻址，页寻址
    oled_Write_cmd(0x02);//	地址模式为页寻址
    oled_Write_cmd(0x8d);//--set Charge Pump enable/disable
    oled_Write_cmd(0x14);//--set(0x10) disable
    oled_Write_cmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    oled_Write_cmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
    oled_Write_cmd(0xaf);//--turn on oled panel开启显示

	oled_clear();//清屏
}

void oled_put_char_16x16(unsigned char x,unsigned char y,unsigned char t)/*设置显示坐标函数,t为0时，字符为8x16
                                                       t为1时，字符为16x16*/                 
{	 
     oled_Write_cmd(0x20);
	 oled_Write_cmd(0x00);//设置地址模式为水平选址
     //set page
     oled_Write_cmd(0x22);
	 oled_Write_cmd(y*2);
	 oled_Write_cmd(0x01+y*2);

	 //set colum
     oled_Write_cmd(0x21);
	 oled_Write_cmd((0x08+0x08*t)*x);
	 oled_Write_cmd((0x08+0x08*t)*x+(0x07+0x08*t));
}



 


