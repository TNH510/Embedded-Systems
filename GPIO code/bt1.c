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
	//Set up Pwm
	softPwmCreate(R, 0, 100);
	softPwmCreate(G, 0, 100);
	softPwmCreate(B, 0, 100);
	//loop
	while(1)
	{	
		light_up(R);
		delay(1);
		light_up(G);
		delay(1);
		light_up(B);
		delay(1);
	}
}
