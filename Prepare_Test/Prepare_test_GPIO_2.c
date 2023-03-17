#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPiI2C.h>
#include <stdint.h>

#define R 0//11
#define G 1//12
#define B 2//13
#define BT1 3//15
#define BT2 4//16
#define BT3 5//18
#define BT4 6//22

unsigned int color[11][3] = {{0, 0, 0},{127, 0, 0}, {0, 127, 0}, {0, 0, 127},
							{127,127,0}	,{127,0,127}, {0,127,127}, {127,127,127},
							{70,120,70}, {150,70,200},{50,50,180} };

unsigned int r = 200;
unsigned int g = 0;
unsigned int b = 0;
unsigned int start = 0,end = 0;
unsigned int time, K = 1, mode = 2, state = 1;
int c_mode = 0;
unsigned int count = 1;
unsigned int level = 5, D_level = 15;		
		
void ngatBT1()
{	
	level++;
	if (level > 9) level = 1;
	start = millis();

}
void ngatBT2()
{	
	state++;
	if (state > 2) state = 1;
		
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
	wiringPiISR(BT1, INT_EDGE_FALLING, &ngatBT1);
	wiringPiISR(BT2, INT_EDGE_FALLING, &ngatBT2);
	unsigned int rd,gd,bd;
	
	while(1)
	{

		while(state == 1)
		{
			
			rd = (unsigned int)r;
			gd = (unsigned int)g;
			bd = (unsigned int)b;
			softPwmWrite(R, rd*100/255);
			softPwmWrite(G, gd*100/255);
			softPwmWrite(B, bd*100/255);
			delay(166);
			if (digitalRead(BT1) == LOW)
			{
				end = millis();
				time = end - start;
				if (time > 2000)
				{
					state = 3;
					start = 0;
					end = 0;
					time = 0;
				}
			}
			softPwmWrite(R, 0);
			softPwmWrite(G, 0);
			softPwmWrite(B, 0);
			delay(166);
			
		}
		
		while(state == 2)
		{
			
			r = level*D_level*color[mode][0]/100;//5*20*100/100
			g = level*D_level*color[mode][1]/100;
			b = level*D_level*color[mode][2]/100;
			rd = (unsigned int)r;
			gd = (unsigned int)g;
			bd = (unsigned int)b;
			softPwmWrite(R, rd*100/255);
			softPwmWrite(G, gd*100/255);
			softPwmWrite(B, bd*100/255);
			if (digitalRead(BT1) == LOW)
			{
				end = millis();
				time = end - start;
				if (time > 2000)
				{
					state = 3;
					start = 0;
					end = 0;
					time = 0;
				}
			}
			if (mode > 9)
				mode = 1;
			delay(500);
			mode++;
			
		}
		
		while(state == 3)
		{
			softPwmWrite(R, 0);
			softPwmWrite(G, 0);
			softPwmWrite(B, 0);
			
			if (digitalRead(BT1) == LOW)
			{
				end = millis();
				time = end - start;
				if (time > 2000)
				{
					state = 3;
					start = 0;
					end = 0;
					time = 0;
				}
			}
		}
	
	}
	return 0;
}
