#include"iic.h"
#include<reg52.h>
uchar code tabel1[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef,0x00};
uchar code tabel2[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

sbit jidianqi=P2^5;
sbit la=P2^7;
sbit key1=P3^0;
sbit key2=P3^1;
sbit l1=P2^6;
uchar state;
uchar d[8];
uint v,s,q,guang;



void delay_50us(uchar a)
{
	uchar j;
	for(;a>0;a--)
	for(j=19;j>0;j--);
}
void display()
{
	uchar i;
	if(state==1)
	{
		d[0]=20;
		d[1]=10;
		d[2]=5;
		d[3]=0;
		d[4]=v/1000;
		d[5]=v%1000/100+10;
		d[6]=v%100/10;
		d[7]=v%10;	  
	} 
	if(state==0)
	{
		d[0]=20;
		d[1]=10;
		d[2]=5;
		d[3]=0;
		d[4]=q/1000;
		d[5]=q%1000/100+10;
		d[6]=q%100/10;
		d[7]=q%10;
	}
	for(i=0;i<8;i++)
	{
		P0=tabel1[d[i]];
		P1=tabel2[i];
		delay_50us(20);
	}
}
void key()
{
	if(key1==0)
	{
		delay_50us(200);
		if(key1==0)
		{
			state=1;
			jidianqi=0;
			la=0;
			s=0;		
			TR0=1;
			while(!key1)
			{
			display();
			}			 
	    }
	}  
				v=5*s;
				v=v/10;
				q=v*0.5;
				display();
				if(v>9999)
				{
					state=0;
					jidianqi=1;
				}		

	   if(key2==0)
			{
				delay_50us(200);
				if(key2==0)
				{
					state=0;
					jidianqi=1;
					la=1;				
					TR0=0;
					v=5*s;
					v=v/10;
					q=v*0.5;
				
					while(!key2) display();

				}
		
			}
}

void main()
{
	init();
	TMOD=0x01;
	EA=1;
	ET0=1;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	state=0;
	v=0;
	jidianqi=1;
	la=1;
	channel(0x42);
	while(1)
	{
		guang=read_adc();
		guang=guang*1.9608;
		if(guang<125)
		{
			l1=0;
		}
		else 
		{
			l1=1;
		}
		key();
	
		display();
	}

}
void shi () interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	s++;
}