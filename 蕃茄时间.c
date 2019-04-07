/*************************************************
*实验名：定时器实验
*实验效果：第一个led灯以500ms一次的精确频率闪烁
*************************************************/
#include<reg52.h>

sbit led = P1^0;
int i = 0;

int now = 1800;

int count0 = 0;
int count1 = 0;
int count2 = 0;
int count3 = 0;

int minutes = 0;
int seconds = 0;

#define duan P0
#define uchar unsigned char 
sbit wei1 = P2^4;//定义第一位数码管
sbit wei2 = P2^5;//定义第二位数码管
sbit wei3 = P2^6;//定义第三位数码管
sbit wei4 = P2^7;//定义第四位数码管

sbit fm =   P2^3;
int key_down = 0;

#define on 0
#define off 1


sbit led3 = P1^3;
sbit led4 = P1^4;
sbit led6 = P1^6;
sbit led7 = P1^7;
sbit k1 = P3^4;
sbit k2 = P3^5;

int s_state = 0; // 0: 30 1:5
int s_doFlag = 1;

#define WORK_TIME (30*60)
#define REST_TIME (5*60)

uchar code sz[17]={0x3f , 0x06 , 0x5b ,0x4f , 0x66 , 0x6d ,0x7d ,
0x07 , 0x7f , 0x6f ,0x77 , 0x7c , 0x39 , 0x5e , 0x79 , 0x71 , 0x00};		

void delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)		      //i=xms即延时约xms毫秒
		for(j=112;j>0;j--);
}

void timer1_init()
{
		TMOD = 0x10; //定时器0选择工作方式1
    TH1 = 0x4C;	 //设置初始值，定时50ms
    TL1 = 0x00; 
    EA = 1;			 //打开总中断
    ET1 = 1;		 //打开定时器0中断
    TR1 = 1;		 //启动定时器0
	
		EX0 = 1;//INT0中断允许
		IT0 = 0;//触发方式为低电平触发
}

void main()
{
	led = 1;
	led3 = 1;
	timer1_init();//定时器1的初始化
	while(1)
	{
		if(i>=20)
		{
		  led = ~led;
		  i = 0; //注意i需要零
			if(s_doFlag) {
					now--;
			}			
		}
		

		
		minutes = now / 60;
		count0 = minutes / 10;
		count1 = minutes % 10;
		seconds = now - minutes * 60;
		count2 = seconds / 10;
		count3 = seconds % 10;
		
		duan = sz[count0];
		wei1 = 1;
		wei2 = 0;
		wei3 = 0;
		wei4 = 0;
		delay(1);
		duan = sz[count1];
		wei1 = 0;
		wei2 = 1;
		wei3 = 0;
		wei4 = 0;
		delay(1);
		duan = sz[count2];
		wei1 = 0;
		wei2 = 0;
		wei3 = 1;
		wei4 = 0;
		delay(1);
		duan = sz[count3];
		wei1 = 0;
		wei2 = 0;
		wei3 = 0;
		wei4 = 1;
		delay(1);
		if (now <= 0)
		{
			if (s_state == 0) {
				now = REST_TIME;
				s_state = 1;
				fm = on;
				delay(15 * 50);
				fm = off;		
				delay(100);
				fm = on;
				delay(15 * 50);
				fm = off;	
				delay(100);
				fm = on;
				delay(15 * 50);
				fm = off;					
				
			} else {
				s_state = 0;
				now = WORK_TIME;
				fm = on;
				delay(15 * 50);
				fm = off;		
				delay(100);
				fm = on;
				delay(15 * 50);
				fm = off;
				delay(100);
				fm = on;	
				delay(15 * 50);
				fm = off;
				delay(100);
				fm = on;	
				delay(15 * 50);
				fm = off;					
			}
		
			s_doFlag = 0;
		}		
		if (key_down)
		{
				now = WORK_TIME;
				fm = on;
				delay(50);
				fm = off;
				EX0 = 1;
				key_down = 0;
				s_state = 0;
		}
		
		if(k1 == 0)//判断是否有按下按键的信号
		{
			delay(10);//延时10ms 消抖
			if(k1 == 0)//再次判断按键是否被按下
			{
				while(k1 == 0);//直到按键判断松开
				led3 = ~led3;//松开后执行程序 翻转led灯
				
				fm = on;
				delay(50);
				fm = off;		
				led6 = ~led6;
				now = REST_TIME;
				s_state = 1;
			}
		}
		if(k2 == 0)//判断是否有按下按键的信号
		{
			delay(10);//延时10ms 消抖
			if(k2 == 0)//再次判断按键是否被按下
			{
				while(k2 == 0);//直到按键判断松开
				led3 = ~led3;//松开后执行程序 翻转led灯
				
				fm = on;
				delay(50);
				fm = off;		
				s_doFlag = !s_doFlag;
			}
		}		
		
	}
}

void jump_low()	 interrupt 0
{
		key_down = 1;
		delay(50);
}

void timer1() interrupt 3
{
    TH1 = 0x4C;	 //设置初始值，定时50ms
    TL1 = 0x00;
		i++;
}