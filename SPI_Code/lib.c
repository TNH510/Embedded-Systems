#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

unsigned int number_7seg[10] = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b};
typedef enum { false, true } bool;
#define R 0//11
#define G 1//12
#define B 2//13
#define BT1 3//15
#define BT2 4//16
#define BT3 5//18
#define BT4 6//22
#define BT5 21//29
// MOSI --> DIN
//SCLK --> CLK
//CE0 --> CS

//max7219 library
void sendData(unsigned char address, unsigned char value)
{
	unsigned char data[2] ;//16bit
	data[0] = address; //Dia chi hexa de chon mode can thay doi(datasheet)
	data[1] = value;   //Chon cac tuy chon thay doi(datasheet)
	wiringPiSPIDataRW(1, data, 2); //Gui data gom 2 byte qua channel 0
	
}

void Init_7219(void)
{
	
	//set decode mode
	sendData(0x09, 0xff);
	//set intensity
	sendData(0x0A, 0x01);
	//set scan limit
	sendData(0x0B, 0x07);
	//set shutdown
	sendData(0x0C, 0x01);
	//set display test
	sendData(0x0F, 0x00);
	
}

void decode_mode(bool select)
{
	if (select == true) sendData(0x09, 0xff);
	else sendData(0x09, 0x00);
}

//cpu
float cpu_temp()
{
	FILE *tempfile;
	float temp;
	tempfile  = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
	fscanf(tempfile, "%f", &temp);
	temp = temp / 1000;
	fclose(tempfile);
	
	return temp;
}

//Hien thi so thuc (.2f)
void display_real_number(float sothuc)
{
		float sothucx100;
		unsigned int dvi = 0, chuc = 0, tram = 0, ngan = 0;
		sothucx100 = sothuc*100;
		
		printf("\r%f", sothuc);
		fflush(stdout);
		
		ngan = sothucx100 / 1000;
		tram = (sothucx100 - ngan*1000) / 100;
		chuc = (sothucx100 - ngan*1000 - tram*100) / 10;
		dvi =  (int)sothucx100%10;
		
		sendData(0x09, 0x00);
		sendData(1, number_7seg[dvi]);
		sendData(2, number_7seg[chuc]);
		sendData(3, number_7seg[tram]|0x80);
		sendData(4, number_7seg[ngan]);		
}


//Hien thi so nguyen bat ki len man hinh
void display_uint_number(unsigned int songuyen)
{
	unsigned int dvi = 0, chuc = 0, tram = 0, ngan = 0;
		
	ngan = songuyen / 1000;
	tram = (songuyen - ngan*1000) / 100;
	chuc = (songuyen - ngan*1000 - tram*100) / 10;
	dvi =  songuyen%10;
		
	sendData(0x09, 0x00);
	sendData(1, number_7seg[dvi]);
	sendData(2, number_7seg[chuc]);
	sendData(3, number_7seg[tram]);
	sendData(4, number_7seg[ngan]);	
}
//Set up pro (so chu so, do sang, ...)
void setting_7219(unsigned int numDisplay, unsigned int Intensity)
{

	sendData(0x0A, Intensity);
	sendData(0x0B, numDisplay - 1);

}


//Clear display
void clear_data()
{
	decode_mode(true);
	for (int i = 1; i < 9; i ++)
	{
		sendData(i, 0);	
	}
	decode_mode(false);
	
}

//shutdown
void display_ON(bool select)
{
	if (select == 1) sendData(0x0C, 0x01);
	else sendData(0x0C, 0x00);
}

void test_display(bool select)
{
	if (select == 1) sendData(0x0F, 0x01);
	else sendData(0x0F, 0x00);
}
//Hien thi ki tu bat ki tai o bat ki
void display_number(unsigned int position, unsigned int number)
{
	decode_mode(true);
	sendData(position, number);
}
//Move right, move left 8 char in screen
void move_left(char string[8], bool is_left, unsigned int solanchay,
				unsigned int time_milisec)
{
	decode_mode(true);
	unsigned int m[8] = {8,7,6,5,4,3,2,1};
	/**/
	
	if (is_left == false)
	{
		for (int i = 0; i < solanchay*8; i ++)
		{	
			
			sendData(m[0], string[0]);
			sendData(m[1], string[1]);
			sendData(m[2], string[2]);
			sendData(m[3], string[3]);
			sendData(m[4], string[4]);
			sendData(m[5], string[5]);
			sendData(m[6], string[6]);
			sendData(m[7], string[7]);
		
			for (int j = 0; j<=7; j++)
			{
				m[j] = m[j] - 1;
				if (m[j] < 1) m[j] = 8;
			}
			
			delay(time_milisec);
			
		}
	}
	
	else 
	{	for (int i = 0; i < solanchay*8; i ++)
		{	
			
			sendData(m[0], string[0]);
			sendData(m[1], string[1]);
			sendData(m[2], string[2]);
			sendData(m[3], string[3]);
			sendData(m[4], string[4]);
			sendData(m[5], string[5]);
			sendData(m[6], string[6]);
			sendData(m[7], string[7]);
		
			for (int j = 0; j<=7; j++)
			{
				m[j] = m[j] + 1;
				if (m[j] > 8) m[j] = 1;
			}
			
			delay(time_milisec);
			
		}
	}
	
}

//Hien thi ngay thang nam




