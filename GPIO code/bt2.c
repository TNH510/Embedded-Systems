#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "LED_library.h"

#define R 0//11
#define G 1//12
#define B 2//13
#define BT1 3//15
#define BT2 4//16
#define BT3 5//18
#define BT4 6//22
#define BT5 21//29

int main(void)
{
	//Set up wiringpi
	wiringPiSetup();
	//Set up I/O
	pinMode(R, OUTPUT);
	pinMode(G, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(BT1, INPUT);
	pinMode(BT2, INPUT);
	pinMode(BT3, INPUT);
	pinMode(BT4, INPUT);
	//Set up Pwm
	softPwmCreate(R, 0, 100);
	softPwmCreate(G, 0, 100);
	softPwmCreate(B, 0, 100);

	while(1)
	{
		if (digitalRead(BT1) == 0)
		{
			softPwmWrite(R, 20);
			softPwmWrite(G, 20);
			softPwmWrite(B, 20);
		}		
		if (digitalRead(BT2) == 0)
		{
			softPwmWrite(R, 40);
			softPwmWrite(G, 40);
			softPwmWrite(B, 40);
		}
		if (digitalRead(BT3) == 0)
		{
			softPwmWrite(R, 60);
			softPwmWrite(G, 60);
			softPwmWrite(B, 60);
		}
		if (digitalRead(BT4) == 0)
		{
			softPwmWrite(R, 80);
			softPwmWrite(R, 80);
			softPwmWrite(R, 80);
		}
	}
	return 0;
}
