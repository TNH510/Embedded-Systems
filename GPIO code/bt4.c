
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
float r = 255;
float g = 255;
float b = 255;
int Random = 0;
int state = 2;
int point = 0;
int mode = 700;

void ngatBT1()
{	
	if (r == 255)
	{
		state = 0;
		point++;
		printf("\n Red Point: %d\n", point);
	}
		
}
void ngatBT2() 
{
	if (g == 255) 
	{
		state = 0;
		point++;
		printf("\nGreen Point: %d\n", point);
	}
}
void ngatBT3() 
{
	if (b == 255) 
	{
		state = 0;
		point++;
		printf("\nBlue Point: %d\n", point);
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
	pinMode(BT5, INPUT);
	//Set up Pwm
	softPwmCreate(R, 0, r);
	softPwmCreate(G, 0, g);
	softPwmCreate(B, 0, b);
	//set up interrupt
	wiringPiISR(BT1, PUD_DOWN, &ngatBT1);
	wiringPiISR(BT2, PUD_DOWN, &ngatBT2);
	wiringPiISR(BT3, PUD_DOWN, &ngatBT3);
	wiringPiISR(BT4, PUD_DOWN, &ngatBT4);
	wiringPiISR(BT5, PUD_DOWN, &ngatBT5);
	//loop
	while(1)
	{		

			
			while (state == 1)
			{	
				r = 0;
				g = 0;
				b = 0;
				softPwmWrite(R, 0);
				softPwmWrite(G, 0);
				softPwmWrite(B, 0);
				delay(2000);
				Random = rand()%3;
				if (Random == 0) {r = 255; g = 0; b = 0;}
				if (Random == 1) {r = 0; g = 255; b = 0;}
				if (Random == 2) {r = 0; g = 0; b = 255;}
				softPwmWrite(R, (int)r);
				softPwmWrite(G, (int)g);
				softPwmWrite(B, (int)b);
				delay(500);
				
			}
			printf("Hay lam\n");
			fflush(stdout);
			state = 1;
			r = 0;
			g = 0;
			b = 0;
			softPwmWrite(R, 255);
			softPwmWrite(G, 255);
			softPwmWrite(B, 255);
			delay(400);
			softPwmWrite(R, 0);
			softPwmWrite(G, 0);
			softPwmWrite(B, 0);
			delay(400);
			softPwmWrite(R, 255);
			softPwmWrite(G, 255);
			softPwmWrite(B, 255);
			delay(400);
			softPwmWrite(R, 0);
			softPwmWrite(G, 0);
			softPwmWrite(B, 0);
			delay(400);
			
	}
	return 0;
}
