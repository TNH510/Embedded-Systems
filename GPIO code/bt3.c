
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdlib.h>
#include "LED_library.h"

#define R 0//11
#define G 1//12
#define B 2//13
#define BT1 3//15
#define BT2 4//16
#define BT3 5//18
#define BT4 6//22
#define BT5 21//29
float r = 100;
float g = 100;
float b = 100;
unsigned int count = 0;

void ngatBT1()
{	

	/*r = rand()%256;
	g = rand()%256;
	b = rand()%256;*/
	count++;
	if (count == 1) {r = 255; g = 0; b = 0;}
	if (count == 2) {r = 0; g = 255; b = 0;}
	if (count == 3) {r = 0; g = 0; b = 255;}
	if (count == 4) count = 0;
		
	

}
void ngatBT2() 
{
		if (count == 1) {r = r - 10;}
		if (count == 2) {g = g - 10;}
		if (count == 3) {b = b - 10;}
}
void ngatBT3() 
{
		if (count == 1) {r = r + 10;}
		if (count == 2) {g = g + 10;}
		if (count == 3) {b = b + 10;}
}


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
	softPwmCreate(R, 0, r);
	softPwmCreate(G, 0, g);
	softPwmCreate(B, 0, b);
	//set up interrupt
	wiringPiISR(BT1, PUD_DOWN, &ngatBT1);
	wiringPiISR(BT2, PUD_DOWN, &ngatBT2);
	wiringPiISR(BT3, PUD_DOWN, &ngatBT3);
	//loop
	while(1)
	{

			softPwmWrite(R, (int)r);
			softPwmWrite(G, (int)g);
			softPwmWrite(B, (int)b);
			delay(1);

	}
	return 0;
}
