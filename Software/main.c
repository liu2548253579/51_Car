#include"i2c.h"
#include"oledchar.h"
#include"intrins.h"
#include"STC12C5A.h"

#define uchar unsigned char
#define uint  unsigned int

uchar PuZh[36] = "�յ�";


//--����ʹ�õ�IO--//
 sbit Trig_P = P1^6;		// ������ģ���Trig�ܽ�
 sbit Echo_P = P1^7;		// ������ģ���Echo�ܽ�
 sbit PWM = P2^5;   //�����pwm����

 sbit A1 = P0^0;      //���Ͻǵ��
 sbit A2 = P0^1;
 sbit B1 = P0^2;      //���Ͻǵ��
 sbit B2 = P0^3;
 sbit C1 = P0^4;      //���½ǵ��
 sbit C2 = P0^5;
 sbit D1 = P0^6;      //���½ǵ��
 sbit D2 = P0^7;  
 sbit lse1  = P1^0;//�ұ���ߺ��⴫���������ݿ�
 sbit lse2  = P1^1;//�ұ���ߺ��⴫���������ݿ� 
 sbit lse3  = P1^2;//�����ߺ��⴫���������ݿ�
 sbit lse4  = P1^3;//�����ߺ��⴫���������ݿ�
 
 
 //--��������ȫ�ֺ���--// 
void bizhang ();
void judge ();
void Delay80ms();
void bizhangrun ();
void bizhangbackrun ();
void bizhangleftrun ();
void bizhangrightrun ();
void duoji_Init();
 //--����oledȫ�ֺ���--// 
 void oleddisplay (unsigned char oleddisplaymode);
 void DelayMs(unsigned int time);
 void detect ();
 
//--����wifiȫ�ֺ���--//   
 void UsartConfiguration();
 void Delay_1ms(uint i);
 void pwmdelay (unsigned int t);

//--����Ѱ��ȫ�ֺ���--//
void pwmdelay(unsigned int t); 
void run(void);
void stop(void);
void leftrun(void);
void rightrun(void);
void xunji();
void Delay350ms();

//����wifiȫ�ֱ���//
unsigned int t;
unsigned int wifispeed=1000;		   //WiFiģʽ�ٶ�      ��Χ��500-2000��
unsigned int wifiturningspeed=800 ;  //WiFiģʽת���ٶ�    ��Χ��800-2000��
unsigned char wifisrunningtime=70;	 //�˴�����wifiģʽֱ������ʱ��
unsigned char wifiturningtime =40;        //�˴�����wifiģʽת������ʱ��
unsigned int wificircle=2000;    //����wifiģʽpwm����
unsigned char receive_data;

//����Ѱ��ȫ�ֱ���//
 unsigned char flag;
 unsigned int	turningspeed = 2000;  //Ѱ��ת���ٶ�     ��Χ��300-2000��
 unsigned int speed=800;		   //Ѱ��ֱ���ٶ�     ��Χ��300-2000��
 unsigned int turningtime=4;	 //Ѱ��ת������ʱ��
 unsigned int srunnningtime=10;   //Ѱ��ֱ������ʱ�� 
 unsigned int circle=2000;			  //��������
 unsigned char runtimes = 0;


//����oledȫ�ֱ���
unsigned char displaymode;
unsigned char percent,operate;
double fenzi,fenmu;


//�������ȫ�ֱ���
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
	unsigned int ss;					// ���ڼ�¼��õľ�����������

	TH0=0;
	TL0=0;

	Trig_P=1;					// ��������ģ��һ����ʼ����
	DelayMs(1);
	Trig_P=0;

	while(!Echo_P);		// �ȴ�������ģ��ķ�������
	TR0=1;						// ������ʱ������ʼ��ʱ
	while(Echo_P);		// �ȴ�������ģ��ķ����������
	TR0=0;						// ֹͣ��ʱ����ֹͣ��ʱ

	ss = ((TH0*256+TL0)*gSpeed)/2;		// ����cm=��ʱ��us * �ٶ�cm/us��/2
    ss1 = ((TH0*256+TL0)*gSpeed*10)/2;
	ss1 = (ss1 - ss*10) * 100;

	if(ss>999)				// �Ѽ��������999������
		ss=999;
	
	return ss;
}





//����Ϊ���벿��----------------------------------------------------------------------------------------------------------


void oleddisplay (unsigned char oleddisplaymode)
{
unsigned char ge =0,shi=0 ,bai=0,i,j,jl1=0,jl2=0,jl3=0,cz1=0,cz2=0;      //(iΪԪ�ظ���jΪ��������)
	 if (oleddisplaymode == 1) //ң����ʾģʽ
      {	
				
	i=0;  while (1)//��һ�й̶���ʾ   ģʽ���ֻ�ң�ء�-
	{
	  oled_put_char_16x16(i,0,1);   //�����ŵ�λ�ú�����(x��,y��,����)(0ΪӢ�Ļ�����1Ϊ����)
		
	  j=0; while (1)
	  {
	    oled_Write_data(zifu[i][j]);  // �����ŵĶ���
			j= j+1;	
    if (j >= 32){break;}
	  }
		
i = i+1;
if (i >= 8){break;}		
	}
	
	for(i=0;i<=4;i++)//�ڶ��й̶���ʾ   ����ʱ�٣�   %  -  ��λ����
	{
	  oled_put_char_16x16(i,1,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+8][j]);  
		}			
	}
	
oled_put_char_16x16(13,1,0);  //�ٷֺ�
		for(j=0;j<16;j++)
	  {
	    oled_Write_data(number[10][j]);  
	  }
		
	for(i=0;i<=4;i++)//�����й̶���ʾ   ң�ز�����      -   ��λ����
	{
	  oled_put_char_16x16(i,2,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+13][j]);  
		}	

	}


	for(i=0;i<=4;i++)//�����й̶���ʾ   �ϰ������    CM-  ��λ����
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

	if (oleddisplaymode == 2)//Ѱ����ʾģʽ
{
	for(i=0;i<=7;i++)//��һ�й̶���ʾ   ģʽ���Զ�Ѱ����-
	{
	  oled_put_char_16x16(i,0,1);   //�����ŵ�λ�ú�����(x��,y��,����)(0ΪӢ�Ļ�����1Ϊ����)
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+23][j]);  // �����ŵĶ���
	  }		
	}
	
	for(i=0;i<=4;i++)//�ڶ��й̶���ʾ   ����ʱ�٣�   %  -  ��λ����
	{
	  oled_put_char_16x16(i,1,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+8][j]);  
		}			
	}
	
oled_put_char_16x16(13,1,0);  //�ٷֺ�
		for(j=0;j<16;j++)
	  {
	    oled_Write_data(number[10][j]);  
	  }
		

	
	for(i=0;i<=4;i++)//�����й̶���ʾ   Ѱ��������      -   ��λ����
	{
	  oled_put_char_16x16(i,2,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+31][j]);  
		}	

	}


	for(i=0;i<=4;i++)//�����й̶���ʾ   �ϰ������    CM-  ��λ����
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


		
 
 if (oleddisplaymode == 3)//������ʾģʽ
 {
 	for(i=0;i<=7;i++)//��һ�й̶���ʾ   ģʽ���Զ����ϡ�-
	{
	  oled_put_char_16x16(i,0,1);   //�����ŵ�λ�ú�����(x��,y��,����)(0ΪӢ�Ļ�����1Ϊ����)
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+36][j]);  // �����ŵĶ���
	  }		
	}
	
	for(i=0;i<=4;i++)//�ڶ��й̶���ʾ   ����ʱ�٣�   %  -  ��λ����
	{
	  oled_put_char_16x16(i,1,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+8][j]);  
		}			
	}
	
oled_put_char_16x16(13,1,0);  //�ٷֺ�
		for(j=0;j<16;j++)
	  {
	    oled_Write_data(number[10][j]);  
	  }
	
	for(i=0;i<=4;i++)//�����й̶���ʾ   ���ϲ�����      -   ��λ����
	{
	  oled_put_char_16x16(i,2,1);
	  for(j=0;j<32;j++)
	  {
	    oled_Write_data(zifu[i+44][j]);  
		}	

	}


	for(i=0;i<=4;i++)//�����й̶���ʾ   �ϰ������    CM-  ��λ����
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
		


 if (oleddisplaymode == 4)  //������ʾ����
 { 

	 
bai = percent/100;
shi = percent/10-bai*10;
ge  = percent%10;
jl1 = distance/100;
jl2 = distance/10-jl1*10;
jl3 = distance%10;
cz1 = operate/10;     //operate ǰ��1 ����23 ��ת45 ��ת67 ��ֹ89
cz2 = operate%10;	 
	
if (percent >= 100)
{	
 		oled_put_char_16x16(10,1,0);  //�ٷֱȰ�λ
	j=0; while (1){
	   oled_Write_data(number[bai][j]);  
	j=j+1;if (j >= 16){break;}}

		oled_put_char_16x16(11,1,0);  //�ٷֱ�ʮλ
	j=0; while (1){
	    oled_Write_data(number[shi][j]);  
	j=j+1;if (j >= 16){break;}}
	
		oled_put_char_16x16(12,1,0);  //�ٷֱȸ�λ
	j=0; while (1){
	    oled_Write_data(number[ge][j]);  
	j=j+1;if (j >= 16){break;}}
}

if (percent < 100 && percent >= 10)
{	
 		oled_put_char_16x16(10,1,0);  //�ٷֱȰ�λ
	j=0; while (1){
	   oled_Write_data(number[13][j]);  
	j=j+1;if (j >= 16){break;}}

		oled_put_char_16x16(11,1,0);  //�ٷֱ�ʮλ
	j=0; while (1){
	    oled_Write_data(number[shi][j]);  
	j=j+1;if (j >= 16){break;}}
	
		oled_put_char_16x16(12,1,0);  //�ٷֱȸ�λ
	j=0; while (1){
	    oled_Write_data(number[ge][j]);  
	j=j+1;if (j >= 16){break;}}
}

if (percent < 10 && percent >= 0)
{	
 		oled_put_char_16x16(10,1,0);  //�ٷֱȰ�λ
	j=0; while (1){
	   oled_Write_data(number[13][j]);  
	j=j+1;if (j >= 16){break;}}

		oled_put_char_16x16(11,1,0);  //�ٷֱ�ʮλ
	j=0; while (1){
	    oled_Write_data(number[13][j]);  
	j=j+1;if (j >= 16){break;}}
	
		oled_put_char_16x16(12,1,0);  //�ٷֱȸ�λ
	j=0; while (1){
	    oled_Write_data(number[ge][j]);  
	j=j+1;if (j >= 16){break;}}
}

	

  oled_put_char_16x16(5,2,1);     //������һλ
	j=0; while (1){
	    oled_Write_data(caozuo[cz1][j]);  
	j=j+1;if (j >= 32){break;}}
	
		oled_put_char_16x16(6,2,1);    //�����ڶ�λ
	j=0; while (1){
	    oled_Write_data(caozuo[cz2][j]);  
	j=j+1;if (j >= 32){break;}}
	

	
	
	
	
	if (distance >= 100)
{	
	  oled_put_char_16x16(11,3,0);  //�����λ
	j=0; while (1){
	    oled_Write_data(number[jl1][j]);  
	j=j+1;if (j >= 16){break;}}
	
	
	  oled_put_char_16x16(12,3,0);  //����ʮλ
	j=0; while (1){
	    oled_Write_data(number[jl2][j]);  
	j=j+1;if (j >= 16){break;}}


	  oled_put_char_16x16(13,3,0);  //�����λ
	j=0; while (1){
	    oled_Write_data(number[jl3][j]);  
	j=j+1;if (j >= 16){break;}}
}	
	
	
	if (distance >= 10 && distance < 100)
{	
	  oled_put_char_16x16(11,3,0);  //�����λ
	j=0; while (1){
	    oled_Write_data(number[13][j]);  
	j=j+1;if (j >= 16){break;}}
	
	
	  oled_put_char_16x16(12,3,0);  //����ʮλ
	j=0; while (1){
	    oled_Write_data(number[jl2][j]);  
	j=j+1;if (j >= 16){break;}}


	  oled_put_char_16x16(13,3,0);  //�����λ
	j=0; while (1){
	    oled_Write_data(number[jl3][j]);  
	j=j+1;if (j >= 16){break;}}
}


if (distance < 10 && distance >0)
{	
	  oled_put_char_16x16(11,3,0);  //�����λ
	j=0; while (1){
	    oled_Write_data(number[13][j]);  
	j=j+1;if (j >= 16){break;}}
	
	
	  oled_put_char_16x16(12,3,0);  //����ʮλ
	j=0; while (1){
	    oled_Write_data(number[13][j]);  
	j=j+1;if (j >= 16){break;}}


	  oled_put_char_16x16(13,3,0);  //�����λ
	j=0; while (1){
	    oled_Write_data(number[jl3][j]);  
	j=j+1;if (j >= 16){break;}}
}

	 }	
 	
}


//����ΪѰ��ģʽ����------------------------------------------------------------------------------------------------------ 
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
	    flag = 0; //��߼�⵽����     
    }
    
    else if((lse1==0)&&(lse2==0)&&(lse3==0)&&(lse4==1))
    {
		flag = 1;//��߼�⵽����
	  }
    
    else if((lse1==0)&&(lse2==0)&&(lse3==1)&&(lse4==1))
    {
		flag = 2;//��߼�⵽����
	  }
    
    else if((lse1==0)&&(lse2==1)&&(lse3==1)&&(lse4==1))
    {
		flag = 3;//��߼�⵽����
	  }
    
    else if((lse1==0)&&(lse2==0)&&(lse3==0)&&(lse4==0))
    {
		flag = 4;//û�м�⵽����
	  }
		
    else if((lse1==0)&&(lse2==1)&&(lse3==1)&&(lse4==0))
    {
		flag = 5;//�м��⵽����
	  }
    
    else if((lse1==0)&&(lse2==1)&&(lse3==0)&&(lse4==0))
    {
		flag = 6;//�ұ߼�⵽����
	  }
	
	else if((lse1==1)&&(lse2==0)&&(lse3==0)&&(lse4==0))
    {
		flag = 7;//�ұ߼�⵽����
	  }
	
	else if((lse1==1)&&(lse2==1)&&(lse3==0)&&(lse4==0))
    {
		flag = 8;//�ұ߼�⵽����
	  }
	
	else if((lse1==1)&&(lse2==1)&&(lse3==1)&&(lse4==0))
    {
		flag = 9;//�ұ߼�⵽����
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

 //����ΪѰ�����ִ���-------------------------------------------------------------------------------------------------------------------------------- 
 
 //����Ϊwifi���ִ���--------------------------------------------------------------------------------------------------------------------------------

void Com_Int(void) interrupt 4
{
	uchar i;
  EA = 0;
  if(RI == 1) //��Ӳ�����յ�һ������ʱ��RI����λ
	{ 		
		RI = 0; 
		receive_data = SBUF;//���յ�������	
		displaymode = 1;  //�л�Ϊң����ʾ
//  	angle = 4.5;count =0;Delay80ms();//����Ƕȵ���Ϊ����
		
	 if(receive_data == '1'&&judgement == 2)                    //ǰ��	 
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


	 if(receive_data == '2'&&judgement == 2)                        //����	 
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
	

		
	 if(receive_data == '3'&&judgement == 2)                      //���
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

		
	 if(receive_data == '4'&&judgement == 2)                      //�ҹ�                                  
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

	 


	 if(receive_data == '0')                        //ͣ���˳�����
		{
			operate = 89;
			displaymode = 1;
			judgement = 2;
	 A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0;
  percent =  0;
		}

	
   if (receive_data == 'u'&&judgement == 2)    //���� 
       {
     wifispeed =  wifispeed+200;
fenzi = wifispeed;fenmu = wificircle;
percent =  (fenzi/fenmu)*100;
if (wifispeed < 600)     //WiFiģʽ�ٶȿ��� ����ֹ�ٶȹ����޷������� 
	   {
	 wifispeed = 600;
	   }
		 
if (wifispeed > wificircle) //WiFiģʽ�ٶȿ��� ����ֹ�ٶȹ��ߴ���
     {
   wifispeed = wificircle;
     }
       }
	
   if (receive_data == 'd'&&judgement == 2)    //���� 
       {
     wifispeed =  wifispeed-200;
fenzi = wifispeed;fenmu = wificircle;
percent =  (fenzi/fenmu)*100;
if (wifispeed < 600)     //WiFiģʽ�ٶȿ��� ����ֹ�ٶȹ����޷������� 
	   {
	 wifispeed = 600;
	   }
		 
if (wifispeed > wificircle) //WiFiģʽ�ٶȿ��� ����ֹ�ٶȹ��ߴ���
     {
   wifispeed = wificircle;
     }
       }
	
 
      if (receive_data == 'u'&&judgement == 1)        //���ϼ��� 
							{
							bizhangspeed = bizhangspeed + 200;								
								
if (bizhangspeed < 800)     //Ѱ��ģʽ�ٶȿ��� ����ֹ�ٶȹ����޷������� 
	   {
	 bizhangspeed = 800;
	   }
		 
if (bizhangspeed > bizhangcircle) //Ѱ��ģʽ�ٶȿ��� ����ֹ�ٶȹ��ߴ���
     {
   bizhangspeed = bizhangcircle;
     }
							}
							
		 if (receive_data == 'd'&&judgement == 1)        //���ϼ���
							{
							bizhangspeed = bizhangspeed - 200;
if (bizhangspeed < 800)     //Ѱ��ģʽ�ٶȿ��� ����ֹ�ٶȹ����޷������� 
	   {
	 bizhangspeed = 800;
	   }
		 
if (bizhangspeed > bizhangcircle) //Ѱ��ģʽ�ٶȿ��� ����ֹ�ٶȹ��ߴ���
     {
   bizhangspeed = bizhangcircle;
     }
							}	
			 
			 
	 if (receive_data == 'e')    //�л�ΪѰ��ģʽ 
       {
				displaymode = 2; 
				 runtimes = 0;
				    while(1)
	          {
  if(runtimes >= 5)
	{srunnningtime=35;}		
	
	if(runtimes <= 5)
	{srunnningtime=(0.000009722*speed*speed)-(0.03806*speed)+46.2;}  //�˴�Ϊƽ�⳵��ת�ٵĳ���0.000009722222222222223*speed*speed-0.03805555555555556*speed+46.22222222222222
	
		   oleddisplay (displaymode);  //��ʾ��
							 detect ();
							xunji() ;        			//Ѱ������
							
				if (distance <= 20)        //��������			
				{
					displaymode = 1;
			A1 = 0,A2 = 1,B1 = 0,B2 = 1,C1 = 0,C2 = 1,D1 = 0,D2 = 1; 
			pwmdelay(speed*20);
			A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
							break;}					
							

							
							
		  if(RI == 1) //��Ӳ�����յ�һ������ʱ��RI����λ
	{ 		
		RI = 0;
		receive_data = SBUF;//���յ�������
		
		if (receive_data == '0')        //�˳�Ѱ��ģʽ 
							{
								   A1 = 0,A2 = 0,B1 = 0,B2 = 0,C1 = 0,C2 = 0,D1 = 0,D2 = 0; 
							break;
							}		
				
	
     if (receive_data == 'u')    //Ѱ������ 
       {
     speed =  speed+200;
fenzi = speed;fenmu = circle;
percent =  (fenzi/fenmu)*100;
 if (speed < 800)     //Ѱ��ģʽ�ٶȿ��� ����ֹ�ٶȹ����޷������� 
	   {
	 speed = 800;
	   }
		 
if (speed > circle) //Ѱ��ģʽ�ٶȿ��� ����ֹ�ٶȹ��ߴ���
     {
   speed = circle;
     }
       }
	
     if (receive_data == 'd')    //Ѱ������ 
       {
     speed = speed-200;
fenzi = speed;fenmu = circle;
percent =  (fenzi/fenmu)*100;

 if (speed < 800)     //Ѱ��ģʽ�ٶȿ��� ����ֹ�ٶȹ����޷������� 
	   {
	 speed = 800;
	   }
		 
if (speed > circle) //Ѱ��ģʽ�ٶȿ��� ����ֹ�ٶȹ��ߴ���
     {
   speed = circle;
     }
       }			
	}         	
													
	          } 
				displaymode = 1;   //����ʾת��ΪĬ�ϵ�ң��ģʽ	
 	
          }
	


					
	 if (receive_data == 'z')//�������ģʽ----------------------------------------------------------------------------
		{	
			displaymode = 3;		

  judgement = 1;
		}						
							
       }
		
		for(i=0; i<36; i++)
		{
			SBUF = PuZh[i];   //��Ҫ���͵����ݷ��뵽���ͼĴ���
			while(!TI);		    //�ȴ������������
			TI=0;			        //���������ɱ�־λ
			Delay_1ms(1);
		}
EA    = 1; 

}	
		

//����Ϊwifiģʽ���ִ���-----------------------------------------------------------------------------------------------------------------------------


//һ��Ϊ���ϲ��ִ���---------------------------------------------------------------------------------------------------------------------------------
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
	TL1 = 0;		//���ö�ʱ��ֵ
	TH1 = 0;		//���ö�ʱ��ֵ
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL1 = 0xD2;		//���ö�ʱ��ֵ
	TH1 = 0xFF;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF0��־
	TR1 = 1;		//��ʱ��0��ʼ��ʱ
  EA=1;
  ET1 = 1;		
	}
	
void duoji() interrupt 3 
{
	TR1 = 0; 
	TL1 = 0xD7;		//���ö�ʱ��ֵ
	TH1 = 0xFD;		//���ö�ʱ��ֵ

	if(count <= angle) //5==0�� 15==90��
	{ 
		PWM = 1; 
	}
	else 
	{ 
		PWM = 0; 
	}
	count = count + 0.5;
	if (count >= 200) //T = 20ms����
	{ 
		count = 0;
	}
	TR1 = 1; //����T0
}

void judge ()
{
angle = 4.5;count =0;Delay80ms();
detect ();//����
distance = distance;
oleddisplay (displaymode);		

angle = 1.5;count =0;Delay80ms();
detect ();//��תͷ�����ݸ����ұ�
rightdistance = distance;
oleddisplay (displaymode);

angle = 4.5;count =0;Delay80ms();
detect ();//����
distance = distance;
oleddisplay (displaymode);		

angle = 7.5;count =0;Delay80ms();
detect ();//��תͷ�����ݸ������
leftdistance = distance;
oleddisplay (displaymode);

angle = 4.5;count =0;Delay80ms();
detect ();//����
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
	case 0: 	bizhangrun(); break;               //��Χ�����ϰ�
	case 1:   bizhangbackrun();break;            //��Χ�ڶ����ϰ�
	case 2:   bizhangrightrun ();break;          //������ϰ�
	case 3:   bizhangleftrun  ();break;          //�ұ����ϰ�
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

void Delay_1ms(uint i)//1ms��ʱ
{
  uchar x,j;
  
  for(j=0;j<i;j++)
    for(x=0;x<=148;x++);
}

void main()
{
	duoji_Init();
	angle = 4.5;count =0;Delay80ms();//����Ƕȵ���Ϊ����
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
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xFB;		//���������ʷ�����ʱ��ΪFosc/12,��12T
	BRT = 0xFD;		//�趨���������ʷ�������װֵ
	AUXR |= 0x01;		//����1ѡ����������ʷ�����Ϊ�����ʷ�����
	AUXR |= 0x10;		//�������������ʷ�����
	ES = 1;       //�������ж�
  EA = 1;       //�����ж�
	
}
