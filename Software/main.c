#include"i2c.h"
#include"oledchar.h"
#include"intrins.h"
#include"STC12C5A.h"

#define uchar unsigned char
#define uint  unsigned int

uchar PuZh[36] = "收到";


//--定义使用的IO--//
 sbit Trig_P = P1^6;		// 超声波模块的Trig管脚
 sbit Echo_P = P1^7;		// 超声波模块的Echo管脚
 sbit PWM = P2^5;   //舵机的pwm引脚

 sbit A1 = P0^0;      //左上角电机
 sbit A2 = P0^1;
 sbit B1 = P0^2;      //右上角电机
 sbit B2 = P0^3;
 sbit C1 = P0^4;      //左下角电机
 sbit C2 = P0^5;
 sbit D1 = P0^6;      //右下角电机
 sbit D2 = P0^7;  
 sbit lse1  = P1^0;//右边外边红外传感器的数据口
 sbit lse2  = P1^1;//右边里边红外传感器的数据口 
 sbit lse3  = P1^2;//左边里边红外传感器的数据口
 sbit lse4  = P1^3;//左边外边红外传感器的数据口
 
 
 //--声明避障全局函数--// 
void bizhang ();
void judge ();
void Delay80ms();
void bizhangrun ();
void bizhangbackrun ();
void bizhangleftrun ();
void bizhangrightrun ();
void duoji_Init();
 //--声明oled全局函数--// 
 void oleddisplay (unsigned char oleddisplaymode);
 void DelayMs(unsigned int time);
 void detect ();
 
//--声明wifi全局函数--//   
 void UsartConfiguration();
 void Delay_1ms(uint i);
 void pwmdelay (unsigned int t);

//--声明寻迹全局函数--//
void pwmdelay(unsigned int t); 
void run(void);
void stop(void);
void leftrun(void);
void rightrun(void);
void xunji();
void Delay350ms();

//定义wifi全局变量//
unsigned int t;
unsigned int wifispeed=1000;		   //WiFi模式速度      范围（500-2000）
unsigned int wifiturningspeed=800 ;  //WiFi模式转弯速度    范围（800-2000）
unsigned char wifisrunningtime=70;	 //此处决定wifi模式直线运行时长
unsigned char wifiturningtime =40;        //此处决定wifi模式转弯运行时长
unsigned int wificircle=2000;    //决定wifi模式pwm周期
unsigned char receive_data;

//定义寻迹全局变量//
 unsigned char flag;
 unsigned int	turningspeed = 2000;  //寻迹转弯速度     范围（300-2000）
 unsigned int speed=800;		   //寻迹直线速度     范围（300-2000）
 unsigned int turningtime=4;	 //寻迹转弯运行时长
 unsigned int srunnningtime=10;   //寻迹直线运行时长 
 unsigned int circle=2000;			  //决定周期
 unsigned char runtimes = 0;


//定义oled全局变量
unsigned char displaymode;
unsigned char percent,operate;
double fenzi,fenmu;


//定义避障全局变量
unsigned int bizhangspeed = 1300;
unsigned char bizhangrunnningtime = 15;
unsigned char bizhangturningtime = 10;
unsigned int bizhangturningspeed = 2000;
unsigned int bizhangcircle = 2000;
unsigned char	distance,leftdistance,rightdistance,location,judgement;
double count = 0,angle;
extern unsigned int GetDistance(void);
unsigned int GetDistance(void)
{
	float gSpeed=0.03460000;						
  unsigned int ss1;			
	unsigned int ss;					// 用于记录测得的距离整数部分

	TH0=0;
	TL0=0;

	Trig_P=1;					// 给超声波模块一个开始脉冲
	DelayMs(1);
	Trig_P=0;

	while(!Echo_P);		// 等待超声波模块的返回脉冲
	TR0=1;						// 启动定时器，开始计时
	while(Echo_P);		// 等待超声波模块的返回脉冲结束
	TR0=0;						// 停止定时器，停止计时

	ss = ((TH0*256+TL0)*gSpeed)/2;		// 距离cm=（时间us * 速度cm/us）/2
    ss1 = ((TH0*256+TL0)*gSpeed*10)/2;
	ss1 = (ss1 - ss*10) * 100;

	if(ss>999)				// 把检测结果限制999厘米内
		ss=999;
	
	return ss;
}





//以下为代码部分----------------------------------------------------------------------------------------------------------


void oleddisplay (unsigned char oleddisplaymode)
{
unsigned char ge =0,shi=0 ,bai=0,i,j,jl1=0,jl2=0,jl3=0,cz1=0,cz2=0;      //(i为元素个数j为数据类型)
	 if (oleddisplaymode == 1) //遥控显示模式
      {	
				
	i=0;  while (1)//第一行固定显示   模式：手机遥控。-
	{
	  oled_put_char_16x16(i,0,1);   //决定放的位置和属性(x轴,y轴,属性)(0为英文或数字1为中文)
		
	  j=0; while (1)
	  {
	    oled_Write_data(zifu[i][j]);  // 决定放的东西
			j= j+1;	
    if (j >= 32){break;}
	  }
		
i = i+1;
if (i >= 8){break;}		
	}
	
	for(i=0;i<=4;i++)//第二行固定显示   运行时速：   %  -  三位空余
	{
	  oled_put_char_16x16(i,1,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+8][j]);  
		}			
	}
	
oled_put_char_16x16(13,1,0);  //百分号
		for(j=0;j<16;j++)
	  {
	    oled_Write_data(number[10][j]);  
	  }
		
	for(i=0;i<=4;i++)//第三行固定显示   遥控操作：      -   六位空余
	{
	  oled_put_char_16x16(i,2,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+13][j]);  
		}	

	}


	for(i=0;i<=4;i++)//第四行固定显示   障碍物距离    CM-  四位空余
	{
	  oled_put_char_16x16(i,3,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+18][j]);  
		}	
	}
	
	oled_put_char_16x16(14,3,0);    //C
	  for(j=0;j<16;j++)
	  {
	    oled_Write_data(number[11][j]);  
		}	
	oled_put_char_16x16(15,3,0);   //M
	  for(j=0;j<16;j++)
	  {
	    oled_Write_data(number[12][j]);  
		}	
		
	fenzi = wifispeed;fenmu = wificircle;
percent =  (fenzi/fenmu)*100;
		
	displaymode = 4;
    }	

	if (oleddisplaymode == 2)//寻迹显示模式
{
	for(i=0;i<=7;i++)//第一行固定显示   模式：自动寻迹。-
	{
	  oled_put_char_16x16(i,0,1);   //决定放的位置和属性(x轴,y轴,属性)(0为英文或数字1为中文)
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+23][j]);  // 决定放的东西
	  }		
	}
	
	for(i=0;i<=4;i++)//第二行固定显示   运行时速：   %  -  三位空余
	{
	  oled_put_char_16x16(i,1,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+8][j]);  
		}			
	}
	
oled_put_char_16x16(13,1,0);  //百分号
		for(j=0;j<16;j++)
	  {
	    oled_Write_data(number[10][j]);  
	  }
		

	
	for(i=0;i<=4;i++)//第三行固定显示   寻迹操作：      -   六位空余
	{
	  oled_put_char_16x16(i,2,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+31][j]);  
		}	

	}


	for(i=0;i<=4;i++)//第四行固定显示   障碍物距离    CM-  四位空余
	{
	  oled_put_char_16x16(i,3,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+18][j]);  
		}	
	}
	
	oled_put_char_16x16(14,3,0);    //C
	  for(j=0;j<16;j++)
	  {
	    oled_Write_data(number[11][j]);  
		}	
	oled_put_char_16x16(15,3,0);   //M
	  for(j=0;j<16;j++)
	  {
	    oled_Write_data(number[12][j]);  
		}	

fenzi = speed;fenmu = circle;
percent =  (fenzi/fenmu)*100;		
		
	displaymode = 4;
    }	


		
 
 if (oleddisplaymode == 3)//避障显示模式
 {
 	for(i=0;i<=7;i++)//第一行固定显示   模式：自动避障。-
	{
	  oled_put_char_16x16(i,0,1);   //决定放的位置和属性(x轴,y轴,属性)(0为英文或数字1为中文)
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+36][j]);  // 决定放的东西
	  }		
	}
	
	for(i=0;i<=4;i++)//第二行固定显示   运行时速：   %  -  三位空余
	{
	  oled_put_char_16x16(i,1,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+8][j]);  
		}			
	}
	
oled_put_char_16x16(13,1,0);  //百分号
		for(j=0;j<16;j++)
	  {
	    oled_Write_data(number[10][j]);  
	  }
	
	for(i=0;i<=4;i++)//第三行固定显示   避障操作：      -   六位空余
	{
	  oled_put_char_16x16(i,2,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+44][j]);  
		}	

	}


	for(i=0;i<=4;i++)//第四行固定显示   障碍物距离    CM-  四位空余
	{
	  oled_put_char_16x16(i,3,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+18][j]);  
		}	
	}
	
	oled_put_char_16x16(14,3,0);    //C
	  for(j=0;j<16;j++)
	  {
	    oled_Write_data(number[11][j]);  
		}	
	oled_put_char_16x16(15,3,0);   //M
	  for(j=0;j<16;j++)
	  {
	    oled_Write_data(number[12][j]);  
		}	
	displaymode = 4;
  }
		


 if (oleddisplaymode == 4)  //变量显示部分
 { 

	 
bai = percent/100;
shi = percent/10-bai*10;
ge  = percent%10;
jl1 = distance/100;
jl2 = distance/10-jl1*10;
jl3 = distance%10;
cz1 = operate/10;     //operate 前进1 后退23 左转45 右转67 静止89
cz2 = operate%10;	 
	
if (percent >= 100)
{	
 		oled_put_char_16x16(10,1,0);  //百分比百位
	j=0; while (1){
	   oled_Write_data(number[bai][j]);  
	j=j+1;if (j >= 16){break;}}

		oled_put_char_16x16(11,1,0);  //百分比十位
	j=0; while (1){
	    oled_Write_data(number[shi][j]);  
	j=j+1;if (j >= 16){break;}}
	
		oled_put_char_16x16(12,1,0);  //百分比个位
	j=0; while (1){
	    oled_Write_data(number[ge][j]);  
	j=j+1;if (j >= 16){break;}}
}

if (percent < 100 && percent >= 10)
{	
 		oled_put_char_16x16(10,1,0);  //百分比百位
	j=0; while (1){
	   oled_Write_data(number[13][j]);  
	j=j+1;if (j >= 16){break;}}

		oled_put_char_16x16(11,1,0);  //百分比十位
	j=0; while (1){
	    oled_Write_data(number[shi][j]);  
	j=j+1;if (j >= 16){break;}}
	
		oled_put_char_16x16(12,1,0);  //百分比个位
	j=0; while (1){
	    oled_Write_data(number[ge][j]);  
	j=j+1;if (j >= 16){break;}}
}

if (percent < 10 && percent >= 0)
{	
 		oled_put_char_16x16(10,1,0);  //百分比百位
	j=0; while (1){
	   oled_Write_data(number[13][j]);  
	j=j+1;if (j >= 16){break;}}

		oled_put_char_16x16(11,1,0);  //百分比十位
	j=0; while (1){
	    oled_Write_data(number[13][j]);  
	j=j+1;if (j >= 16){break;}}
	
		oled_put_char_16x16(12,1,0);  //百分比个位
	j=0; while (1){
	    oled_Write_data(number[ge][j]);  
	j=j+1;if (j >= 16){break;}}
}

	

  oled_put_char_16x16(5,2,1);     //操作第一位
	j=0; while (1){
	    oled_Write_data(caozuo[cz1][j]);  
	j=j+1;if (j >= 32){break;}}
	
		oled_put_char_16x16(6,2,1);    //操作第二位
	j=0; while (1){
	    oled_Write_data(caozuo[cz2][j]);  
	j=j+1;if (j >= 32){break;}}
	

	
	
	
	
	if (distance >= 100)
{	
	  oled_put_char_16x16(11,3,0);  //距离百位
	j=0; while (1){
	    oled_Write_data(number[jl1][j]);  
	j=j+1;if (j >= 16){break;}}
	
	
	  oled_put_char_16x16(12,3,0);  //距离十位
	j=0; while (1){
	    oled_Write_data(number[jl2][j]);  
	j=j+1;if (j >= 16){break;}}


	  oled_put_char_16x16(13,3,0);  //距离个位
	j=0; while (1){
	    oled_Write_data(number[jl3][j]);  
	j=j+1;if (j >= 16){break;}}
}	
	
	
	if (distance >= 10 && distance < 100)
{	
	  oled_put_char_16x16(11,3,0);  //距离百位
	j=0; while (1){
	    oled_Write_data(number[13][j]);  
	j=j+1;if (j >= 16){break;}}
	
	
	  oled_put_char_16x16(12,3,0);  //距离十位
	j=0; while (1){
	    oled_Write_data(number[jl2][j]);  
	j=j+1;if (j >= 16){break;}}


	  oled_put_char_16x16(13,3,0);  //距离个位
	j=0; while (1){
	    oled_Write_data(number[jl3][j]);  
	j=j+1;if (j >= 16){break;}}
}


if (distance < 10 && distance >0)
{	
	  oled_put_char_16x16(11,3,0);  //距离百位
	j=0; while (1){
	    oled_Write_data(number[13][j]);  
	j=j+1;if (j >= 16){break;}}
	
	
	  oled_put_char_16x16(12,3,0);  //距离十位
	j=0; while (1){
	    oled_Write_data(number[13][j]);  
	j=j+1;if (j >= 16){break;}}


	  oled_put_char_16x16(13,3,0);  //距离个位
	j=0; while (1){
	    oled_Write_data(number[jl3][j]);  
	j=j+1;if (j >= 16){break;}}
}

	 }	
 	
}


//以下为寻迹模式代码------------------------------------------------------------------------------------------------------ 
void pwmdelay(unsigned int t)
{
while (t--);
}



void run(void)
{ 
	operate = 1;
	detect ();
	fenzi = speed;fenmu = circle;
percent =  (fenzi/fenmu)*100;
for (t=0;t<srunnningtime;t++) 
{
	
	if((lse1==1)||(lse2==1)||(lse3==1)||(lse4==1)||(distance <= 15))
	{ A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0;break;}
    A1 = 1,A2 = 0,B1 = 1,B2 = 0,C1 = 1,C2 = 0,D1 = 1,D2 = 0;
    pwmdelay(speed);
    A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
    pwmdelay(circle-speed);
	if((lse1==1)||(lse2==1)||(lse3==1)||(lse4==1)||(distance <= 15))
	{ A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0;break;}
	
	

}
}



void leftrun(void)
{ 
	operate = 67;
	fenzi = speed;fenmu = circle;
percent =  (fenzi/fenmu)*100;
for (t=0;t<turningtime;t++) 
{
    A1 = 1,A2 = 0,B1 = 0,B2 = 1,C1 = 1,C2 = 0,D1 = 0,D2 = 1;	
    pwmdelay(turningspeed);
    A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
    pwmdelay(circle-turningspeed);
}

}

void rightrun(void)
{ 
	operate = 45;
	fenzi = speed;fenmu = circle;
percent =  (fenzi/fenmu)*100;
for (t=0;t<turningtime;t++) 
{
    A1 = 0,A2 = 1,B1 = 1,B2 = 0,C1 = 0,C2 = 1,D1 = 1,D2 = 0;	
    pwmdelay(turningspeed);
    A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
    pwmdelay(circle-turningspeed);
}
}


void xunji()
{  
    if((lse1==0)&&(lse2==0)&&(lse3==1)&&(lse4==0))
    {
	    flag = 0; //左边检测到黑线     
    }
    
    else if((lse1==0)&&(lse2==0)&&(lse3==0)&&(lse4==1))
    {
		flag = 1;//左边检测到黑线
	  }
    
    else if((lse1==0)&&(lse2==0)&&(lse3==1)&&(lse4==1))
    {
		flag = 2;//左边检测到黑线
	  }
    
    else if((lse1==0)&&(lse2==1)&&(lse3==1)&&(lse4==1))
    {
		flag = 3;//左边检测到黑线
	  }
    
    else if((lse1==0)&&(lse2==0)&&(lse3==0)&&(lse4==0))
    {
		flag = 4;//没有检测到黑线
	  }
		
    else if((lse1==0)&&(lse2==1)&&(lse3==1)&&(lse4==0))
    {
		flag = 5;//中间检测到黑线
	  }
    
    else if((lse1==0)&&(lse2==1)&&(lse3==0)&&(lse4==0))
    {
		flag = 6;//右边检测到黑线
	  }
	
	else if((lse1==1)&&(lse2==0)&&(lse3==0)&&(lse4==0))
    {
		flag = 7;//右边检测到黑线
	  }
	
	else if((lse1==1)&&(lse2==1)&&(lse3==0)&&(lse4==0))
    {
		flag = 8;//右边检测到黑线
	  }
	
	else if((lse1==1)&&(lse2==1)&&(lse3==1)&&(lse4==0))
    {
		flag = 9;//右边检测到黑线
	  }
	
		else
	{
		  flag = 10;
	}

	
		switch(flag)
	{
		case 0:  rightrun();rightrun();rightrun();runtimes = 0;break;
		case 1:  rightrun();rightrun();rightrun();runtimes = 0;break;
		case 2:  rightrun();rightrun();runtimes = 0;break;
		case 3:  rightrun();rightrun();rightrun();runtimes = 0;break;
		case 4:  run();runtimes = runtimes + 1;break;
		case 5:  run();runtimes = runtimes + 1;break;
		case 6:  leftrun();leftrun();runtimes = 0;break;
		case 7:  leftrun();leftrun();leftrun();runtimes = 0;break;
		case 8:  leftrun();leftrun();leftrun();runtimes = 0;break;
		case 9:  leftrun();leftrun();leftrun();runtimes = 0;break; 
		case 10: rightrun();rightrun();rightrun();leftrun();leftrun();leftrun();runtimes = 0;break;
		default: rightrun();rightrun();rightrun();leftrun();leftrun();leftrun();runtimes = 0;break;
	}
}

 //以上为寻迹部分代码-------------------------------------------------------------------------------------------------------------------------------- 
 
 //以下为wifi部分代码--------------------------------------------------------------------------------------------------------------------------------

void Com_Int(void) interrupt 4
{
	uchar i;
  EA = 0;
  if(RI == 1) //当硬件接收到一个数据时，RI会置位
	{ 		
		RI = 0; 
		receive_data = SBUF;//接收到的数据	
		displaymode = 1;  //切换为遥控显示
//  	angle = 4.5;count =0;Delay80ms();//舵机角度调整为正向
		
	 if(receive_data == '1'&&judgement == 2)                    //前进	 
		{
			operate = 1;
		displaymode = 1	;	
for (t=0;t<wifisrunningtime;t++) 
{
    A1 = 1,A2 = 0,B1 = 1,B2 = 0,C1 = 1,C2 = 0,D1 = 1,D2 = 0;	
    pwmdelay(wifispeed);
    A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
    pwmdelay(wificircle-wifispeed);
}
A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
fenzi = wifispeed;fenmu = wificircle;
percent =  (fenzi/fenmu)*100;
		}


	 if(receive_data == '2'&&judgement == 2)                        //后退	 
		{
			operate = 23;
			displaymode = 1;
for (t=0;t<wifisrunningtime;t++) 
{
    A1 = 0,A2 = 1,B1 = 0,B2 = 1,C1 = 0,C2 = 1,D1 = 0,D2 = 1;	
    pwmdelay(wifispeed);
    A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
    pwmdelay(wificircle-wifispeed);
}
A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
fenzi = wifispeed;fenmu = wificircle;
percent =  (fenzi/fenmu)*100;	 
		}
	

		
	 if(receive_data == '3'&&judgement == 2)                      //左拐
		{
			operate = 45;
			displaymode = 1;
for (t=0;t<wifiturningtime;t++) 
{
    A1 = 0,A2 = 1,B1 = 1,B2 = 0,C1 = 0,C2 = 1,D1 = 1,D2 = 0;	
    pwmdelay(wifiturningspeed);
    A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
    pwmdelay(wificircle-wifiturningspeed);
}
A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
fenzi = wifispeed;fenmu = wificircle;
percent =  (fenzi/fenmu)*100;	 
		}

		
	 if(receive_data == '4'&&judgement == 2)                      //右拐                                  
		{
			operate = 67;
			displaymode = 1;
for (t=0;t<wifiturningtime;t++) 
{
    A1 = 1,A2 = 0,B1 = 0,B2 = 1,C1 = 1,C2 = 0,D1 = 0,D2 = 1;	
    pwmdelay(wifiturningspeed);
    A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
    pwmdelay(wificircle-wifiturningspeed);
}
A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
fenzi = wifispeed;fenmu = wificircle;
percent =  (fenzi/fenmu)*100;	 
		}

	 


	 if(receive_data == '0')                        //停车退出避障
		{
			operate = 89;
			displaymode = 1;
			judgement = 2;
	 A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0;
  percent =  0;
		}

	
   if (receive_data == 'u'&&judgement == 2)    //加速 
       {
     wifispeed =  wifispeed+200;
fenzi = wifispeed;fenmu = wificircle;
percent =  (fenzi/fenmu)*100;
if (wifispeed < 600)     //WiFi模式速度控制 （防止速度过低无法启动） 
	   {
	 wifispeed = 600;
	   }
		 
if (wifispeed > wificircle) //WiFi模式速度控制 （防止速度过高错误）
     {
   wifispeed = wificircle;
     }
       }
	
   if (receive_data == 'd'&&judgement == 2)    //减速 
       {
     wifispeed =  wifispeed-200;
fenzi = wifispeed;fenmu = wificircle;
percent =  (fenzi/fenmu)*100;
if (wifispeed < 600)     //WiFi模式速度控制 （防止速度过低无法启动） 
	   {
	 wifispeed = 600;
	   }
		 
if (wifispeed > wificircle) //WiFi模式速度控制 （防止速度过高错误）
     {
   wifispeed = wificircle;
     }
       }
	
 
      if (receive_data == 'u'&&judgement == 1)        //避障加速 
							{
							bizhangspeed = bizhangspeed + 200;								
								
if (bizhangspeed < 800)     //寻迹模式速度控制 （防止速度过低无法启动） 
	   {
	 bizhangspeed = 800;
	   }
		 
if (bizhangspeed > bizhangcircle) //寻迹模式速度控制 （防止速度过高错误）
     {
   bizhangspeed = bizhangcircle;
     }
							}
							
		 if (receive_data == 'd'&&judgement == 1)        //避障减速
							{
							bizhangspeed = bizhangspeed - 200;
if (bizhangspeed < 800)     //寻迹模式速度控制 （防止速度过低无法启动） 
	   {
	 bizhangspeed = 800;
	   }
		 
if (bizhangspeed > bizhangcircle) //寻迹模式速度控制 （防止速度过高错误）
     {
   bizhangspeed = bizhangcircle;
     }
							}	
			 
			 
	 if (receive_data == 'e')    //切换为寻迹模式 
       {
				displaymode = 2; 
				 runtimes = 0;
				    while(1)
	          {
  if(runtimes >= 5)
	{srunnningtime=35;}		
	
	if(runtimes <= 5)
	{srunnningtime=(0.000009722*speed*speed)-(0.03806*speed)+46.2;}  //此处为平衡车子转速的程序0.000009722222222222223*speed*speed-0.03805555555555556*speed+46.22222222222222
	
		   oleddisplay (displaymode);  //显示屏
							 detect ();
							xunji() ;        			//寻迹本体
							
				if (distance <= 20)        //保护程序			
				{
					displaymode = 1;
			A1 = 0,A2 = 1,B1 = 0,B2 = 1,C1 = 0,C2 = 1,D1 = 0,D2 = 1; 
			pwmdelay(speed*20);
			A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
							break;}					
							

							
							
		  if(RI == 1) //当硬件接收到一个数据时，RI会置位
	{ 		
		RI = 0;
		receive_data = SBUF;//接收到的数据
		
		if (receive_data == '0')        //退出寻迹模式 
							{
								   A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
							break;
							}		
				
	
     if (receive_data == 'u')    //寻迹加速 
       {
     speed =  speed+200;
fenzi = speed;fenmu = circle;
percent =  (fenzi/fenmu)*100;
 if (speed < 800)     //寻迹模式速度控制 （防止速度过低无法启动） 
	   {
	 speed = 800;
	   }
		 
if (speed > circle) //寻迹模式速度控制 （防止速度过高错误）
     {
   speed = circle;
     }
       }
	
     if (receive_data == 'd')    //寻迹减速 
       {
     speed = speed-200;
fenzi = speed;fenmu = circle;
percent =  (fenzi/fenmu)*100;

 if (speed < 800)     //寻迹模式速度控制 （防止速度过低无法启动） 
	   {
	 speed = 800;
	   }
		 
if (speed > circle) //寻迹模式速度控制 （防止速度过高错误）
     {
   speed = circle;
     }
       }			
	}         	
													
	          } 
				displaymode = 1;   //将显示转变为默认的遥控模式	
 	
          }
	


					
	 if (receive_data == 'z')//进入避障模式----------------------------------------------------------------------------
		{	
			displaymode = 3;		

  judgement = 1;
		}						
							
       }
		
		for(i=0; i<36; i++)
		{
			SBUF = PuZh[i];   //将要发送的数据放入到发送寄存器
			while(!TI);		    //等待发送数据完成
			TI=0;			        //清除发送完成标志位
			Delay_1ms(1);
		}
EA    = 1; 

}	
		

//以上为wifi模式部分代码-----------------------------------------------------------------------------------------------------------------------------


//一下为避障部分代码---------------------------------------------------------------------------------------------------------------------------------
void Delay80ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 9;
	j = 104;
	k = 139;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void detect ()
{
	   Trig_P=0;	
	TMOD |= 0x01;	
	distance=GetDistance();
}
 
void duoji_Init()           
  {
	TL1 = 0;		//设置定时初值
	TH1 = 0;		//设置定时初值
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL1 = 0xD2;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
	TF1 = 0;		//清除TF0标志
	TR1 = 1;		//定时器0开始计时
  EA=1;
  ET1 = 1;		
	}
	
void duoji() interrupt 3 
{
	TR1 = 0; 
	TL1 = 0xD7;		//设置定时初值
	TH1 = 0xFD;		//设置定时初值

	if(count <= angle) //5==0° 15==90°
	{ 
		PWM = 1; 
	}
	else 
	{ 
		PWM = 0; 
	}
	count = count + 0.5;
	if (count >= 200) //T = 20ms清零
	{ 
		count = 0;
	}
	TR1 = 1; //开启T0
}

void judge ()
{
angle = 4.5;count =0;Delay80ms();
detect ();//回正
distance = distance;
oleddisplay (displaymode);		

angle = 1.5;count =0;Delay80ms();
detect ();//右转头把数据赋给右边
rightdistance = distance;
oleddisplay (displaymode);

angle = 4.5;count =0;Delay80ms();
detect ();//回正
distance = distance;
oleddisplay (displaymode);		

angle = 7.5;count =0;Delay80ms();
detect ();//左转头把数据赋给左边
leftdistance = distance;
oleddisplay (displaymode);

angle = 4.5;count =0;Delay80ms();
detect ();//回正
distance = distance;
oleddisplay (displaymode);	


}

void bizhang ()
{	
	
if (distance>18)
{
	if(leftdistance>14 && rightdistance>14)
	{location = 0;}
	
  else if (leftdistance<15 && rightdistance<15)
	{location = 0;}	
	
	else if (leftdistance<15 && rightdistance>15)
	{location = 2;}
	
	else if (leftdistance>15 && rightdistance<15)
	{location = 3;}
}

if (distance<18)	
{

if (leftdistance<20 && rightdistance<20)	
{location = 1;}

else if (leftdistance>20 && rightdistance<20)
{location = 3;}	

else if (leftdistance<20 && rightdistance>20)
{location = 2;}

if (leftdistance>=20 && rightdistance>=20)
{
if (leftdistance > rightdistance)
{location = 3;}
	
if (leftdistance < rightdistance)
{location = 2;}	
}

}

switch (location)
{
	case 0: 	bizhangrun(); break;               //范围内无障碍
	case 1:   bizhangbackrun();break;            //范围内都是障碍
	case 2:   bizhangrightrun ();break;          //左边有障碍
	case 3:   bizhangleftrun  ();break;          //右边有障碍
}

}

void bizhangrun ()
{
			operate = 1;
	fenzi = bizhangspeed;fenmu = bizhangcircle;
percent =  (fenzi/fenmu)*100;
for (t=0;t<bizhangrunnningtime;t++) 
{
    A1 = 1,A2 = 0,B1 = 1,B2 = 0,C1 = 1,C2 = 0,D1 = 1,D2 = 0;	
    pwmdelay(bizhangspeed);
    A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
    pwmdelay(bizhangcircle-bizhangspeed);
}


}

void bizhangbackrun ()
{
			operate = 23;
	fenzi = bizhangspeed;fenmu = bizhangcircle;
percent =  (fenzi/fenmu)*100;
for (t=0;t<bizhangrunnningtime;t++) 
{
    A1 = 0,A2 = 1,B1 = 0,B2 = 1,C1 = 0,C2 = 1,D1 = 0,D2 = 1;	
    pwmdelay(bizhangspeed);
    A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
    pwmdelay(bizhangcircle-bizhangspeed);
}

}

void bizhangrightrun ()
{
	operate = 67;
	fenzi = bizhangspeed;fenmu = bizhangcircle;
percent =  (fenzi/fenmu)*100;
for (t=0;t<bizhangturningtime;t++) 
{
    A1 = 1,A2 = 0,B1 = 0,B2 = 1,C1 = 1,C2 = 0,D1 = 0,D2 = 1;	
    pwmdelay(bizhangturningspeed);
    A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
    pwmdelay(bizhangcircle-bizhangturningspeed);
}

}
void bizhangleftrun ()
{
	operate = 45;
	fenzi = bizhangspeed;fenmu = bizhangcircle;
percent =  (fenzi/fenmu)*100;
for (t=0;t<bizhangturningtime;t++) 
{
    A1 = 0,A2 = 1,B1 = 1,B2 = 0,C1 = 0,C2 = 1,D1 = 1,D2 = 0;	
    pwmdelay(bizhangturningspeed);
    A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
    pwmdelay(bizhangcircle-bizhangturningspeed);
}

}

void DelayMs(unsigned int time)
{
	unsigned int i,j;
	for(i=0;i<time;i++)
		for(j=0;j<112;j++);
}

void Delay_1ms(uint i)//1ms延时
{
  uchar x,j;
  
  for(j=0;j<i;j++)
    for(x=0;x<=148;x++);
}

void main()
{
	duoji_Init();
	angle = 4.5;count =0;Delay80ms();//舵机角度调整为正向
oled_initial();
displaymode = 1;
operate = 89;
	judgement = 2;
while (1)
{
		detect ();
		oleddisplay (displaymode);
   UsartConfiguration();
	
	if (judgement == 1)
	{
		while (1)
		{		
  judge ();	
bizhang ();
	if (judgement == 2)
			{	angle = 4.5;count =0;Delay80ms();break;}
		}
	}
	
	
 }
}


 
 void UsartConfiguration()
{	
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xFB;		//独立波特率发生器时钟为Fosc/12,即12T
	BRT = 0xFD;		//设定独立波特率发生器重装值
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
	ES = 1;       //开串口中断
  EA = 1;       //开总中断
	
}
