/*Viết code để nút BT1 có chức năng chọn màu hiển thị, 
BT2 tăng độ sáng của màu hiển thị, BT3
giảm độ sáng của màu hiển thị*/
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <math.h>
#include "LED_library.h"

#define R 0//11
#define G 1//12
#define B 2//13
#define BT1 3//15
#define BT2 4//16
#define BT3 5//18
#define BT4 6//22
#define BT5 21//29

int state = 0;
int state_tang = 0;
int state_giam = 0;
int MIN, MAX;
unsigned int mode = 0;
int r = 0;
int g = 0;
int b = 0;
unsigned int color[11][3] = {{0, 0, 0},{100, 0, 0}, {0, 100, 0}, {0, 0, 100},
							{100,100,0}	,{100,0,100}, {0,100,100}, {100,100,100},
							{54,96,157}, {200,60,40},{50,50,180} };



void ngatBT1()
{	
	
	if (state == 0)
	{	
		mode++;
		r = color[mode][0] ; 
		g = color[mode][1] ; 
		b = color[mode][2] ;
		if (mode == 11) mode = 1;
		state = 1;
		
	}
	

}
void ngatBT2()
{	
	
	if (state_tang == 0)
	{	
			
		r = r + color[mode][0]/20; 
		g = g + color[mode][1]/20; 
		b = b + color[mode][2]/20; 
		if ((r > 255)|(g > 255)|(b > 255)) 
		{
			r = color[mode][0] ; 
			g = color[mode][1] ; 
			b = color[mode][2] ;
		}

		
	}

}
void ngatBT3()
{
	
	if (state_giam == 0)
	{	
		
		r = r - color[mode][0]/20;
		g = g - color[mode][1]/20;
		b = b - color[mode][2]/20;
		if ((r <= 0)&(g <= 0)&(b <= 0)) 
		{
			r = color[mode][0] ; 
			g = color[mode][1] ; 
			b = color[mode][2] ;
		}
		
	}
}
void ngatBT4()
{
	
}
void ngatBT5()
{
	
}

int main(void)
{
	//Set up wiringPi
	wiringPiSetup();
	//Set up GPIO 
	pinMode(R, OUTPUT);
	pinMode(G, OUTPUT);
	pinMode(B, OUTPUT);
	//Set up PWM
	softPwmCreate(R, 0, 100);
	softPwmCreate(G, 0, 100);
	softPwmCreate(B, 0, 100);
	//Set up Interrupt
	wiringPiISR(BT1, INT_EDGE_RISING, &ngatBT1);
	wiringPiISR(BT2, INT_EDGE_RISING, &ngatBT2);
	wiringPiISR(BT3, INT_EDGE_RISING, &ngatBT3);
	wiringPiISR(BT4, INT_EDGE_RISING, &ngatBT4);
	wiringPiISR(BT5, INT_EDGE_RISING, &ngatBT5);
	unsigned int rd, gd, bd;
	while(1)
	{	
		
		rd = r; gd = g; bd = b;
			
		if (r > 255) rd = 255;
		if (g > 255) gd = 255;
		if (b > 255) bd = 255;
		if (r < 0) rd = 0;
		if (g < 0) gd = 0;
		if (b < 0) bd = 0;
		softPwmWrite(R, rd*100/255);
		softPwmWrite(G, gd*100/255);
		softPwmWrite(B, bd*100/255);
		printf(" %d, %d, %d, ", r,g,b);
		fflush(stdout);
		delay(500);
		state = 0;
		
		
		
	}
	
}

