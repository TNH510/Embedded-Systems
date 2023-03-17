/*
Nhap gia tri do sang, kenh mau R,G,B --> LED sang theo yeu cau
Sau do LED chuyen qua lai giua 2 che do NHAP NHAY va TANG GIAM DO SANG (khoang thoi gian 
chuyen giua 2 che do la ngau nhien tu 2 den 3 giay)
Yeu cau chuyen doi qua lai giua 2 che do thi van giu nguyen do sang/mau sac

Set thoi gian hen gio bat tat
Bat neu nhap "on hh:mm" hh la gio, mm la phut
Tat neu nhap "off hh:mm"
*/

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

//Khai bao bien
unsigned int do_sang = 0, r=0, g=0, b=0, state = 0, mode = 0;
unsigned int start = 0, stop = 0, Time = 0, K=0, time_set = 0;
unsigned int mm = 0, hh = 0;
unsigned char string1[5];
unsigned char string2[5];
unsigned int start_nguoc = 0, stop_nguoc = 0;;

void led(unsigned int do_sang, unsigned int r, unsigned int g, unsigned int b)
{
	unsigned int rd=0, gd=0, bd=0;
	
	//Tinh toan gi tri r,g,b phu hop voi do sang 0-->30
	rd = do_sang*8*r/100;
	gd = do_sang*8*g/100;
	bd = do_sang*8*b/100;
	
	//Hien thi mau
	softPwmWrite(R, rd*100/255);
	softPwmWrite(G, gd*100/255);
	softPwmWrite(B, bd*100/255);
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

	while(1)
	{
		//Nhap cac gia tri
		printf("Moi ban nhap cac gia tri:");
		fflush(stdout);
		//Gan cac gia tri da nhap vao cac bien
		scanf("%d %d %d %d", &do_sang, &r, &g , &b);
		
		//Moi ban nhap gia tri hen gio
		printf("Nhap gia tri hen gio: ");
		fflush(stdout);
		
		//Doc va in ra de kiem tra
		scanf("%s %s", &string1, &string2);
		printf("%c%c:%c%c\n", string2[0],string2[1],string2[3],string2[4]);
		
		//Convert char to u_int
		hh = ((int)string2[0] - 48)*10 + ((int)string2[1] - 48);
		mm = ((int)string2[3] - 48)*10 + ((int)string2[4] - 48);
		
		//ON or OFF
		if ((string1[0] == 'o')&(string1[1] == 'f')&(string1[2] == 'f')) state = 0;
		if ((string1[0] == 'o')&(string1[1] == 'n')) state = 2;
		printf("Hen gio thanh cong\n");
		fflush(stdout);
	
		//Lay thoi gian hien tai
		time_t rawtime;
        	struct tm *ct;
       		time (&rawtime);
        	ct = localtime (&rawtime);
		printf("Gio hien tai la:\n");
		printf ("%02d:%02d:%02d\n",ct->tm_hour,ct->tm_min,ct->tm_sec);
		fflush(stdout);
		
		//Tinh time dem nguoc theo giay
		if (mm > (ct->tm_min)) 
		{
			time_set = (hh - (ct->tm_hour))*3600 + (mm - (ct->tm_min))*60 - ct->tm_sec;
			printf("\nThoi gian dem nguoc %d giay\n", time_set);
			fflush(stdout);
		}
		else
		{
			time_set = (hh - (ct->tm_hour) - 1)*3600 + (mm + 60 - (ct->tm_min))*60 - ct->tm_sec;
			printf("Thoi gian dem nguoc %d giay\n", time_set);
			fflush(stdout);
		}
		
		//Bat dau tinh thoi gian dem nguoc
		start_nguoc = millis();
			
		//Hien thi len LED
		led(do_sang, r, g, b);
		delay(2000);
		
		while(state == 2) //Mode TAT LED
		{
			led(0,0,0,0);
			//Dem nguoc de bat len
			stop_nguoc = millis();
			printf("\r Con %d giay",time_set - (stop_nguoc - start_nguoc)/1000);
			fflush(stdout);
			if ((stop_nguoc - start_nguoc) >= time_set*1000)
			{
				state = 0;
				time_set = 10;
				start_nguoc = 0;
				stop_nguoc = 0;
				printf("\n");
			}
			delay(500);
		}
		
		//Reset time neu chuyen tu TAT LED sang BAT LED
		start_nguoc = millis();
		
		while(state < 2) //Mode BAT LED
		{
		
			//Che do 1
			if(state == 0)
			{
				//Tang bien do sang va reset
				do_sang++;
				if (do_sang > 50) do_sang = 1;
				
				//Hien thi led
				led(do_sang, r, g, b);			
				
				//Bat dau dem thoi gian de ngau nhien chuyen che do
				if (K == 0)
				{
					start = millis();
					K = 1; //Chi lay gia tri millis 1 lan dau tien
				}
			}
			
			//Che do 2
			if(state == 1)
			{
				//Thay doi mau
				mode++;
				r = color[mode][0];
				g = color[mode][1];
				b = color[mode][2];
				if (mode > 9) mode = 0;
				//Hien thi mau
				led(do_sang, r, g, b);
				
				//Bat dau dem thoi gian de ngau nhien chuyen che do
				if (K == 0)
				{
					start = millis();
					K = 1; //Chi lay gia tri millis 1 lan dau tien
				}
				delay(400);
			
			}
			//Tinh time da trai qua
			stop = millis();
			Time = stop - start;
			
			//Xet dieu kien
			if (Time > (rand()%1000 + 2000)) 
			{
				state++;
				if (state == 2) state = 0;
				K = 0;//Reset de start co the gan lai gia tri millis;
			}
			delay(30);
			
			//Dem nguoc tat
			stop_nguoc = millis();
			printf("\r Con %d giay",time_set - (stop_nguoc - start_nguoc)/1000);
			fflush(stdout);
			if ((stop_nguoc - start_nguoc) >= time_set*1000)
			{
				state = 2;
				printf("\n");
			}
		}
	}
	return 0;
}
