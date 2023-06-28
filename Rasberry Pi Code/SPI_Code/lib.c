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
unsigned char font[][2] = {
  {'A',0b1110111},{'B',0b1111111},{'C',0b1001110},{'D',0b1111110},{'E',0b1001111},{'F',0b1000111},       
  {'G',0b1011110},{'H',0b0110111},{'I',0b0110000},{'J',0b0111100},{'L',0b0001110},{'N',0b1110110},       
  {'O',0b1111110},{'P',0b1100111},{'R',0b0000101},{'S',0b1011011},{'T',0b0001111},{'U',0b0111110},       
  {'Y',0b0100111},{'[',0b1001110},{']',0b1111000},{'_',0b0001000},{'a',0b1110111},{'b',0b0011111},       
  {'c',0b0001101},{'d',0b0111101},{'e',0b1001111},{'f',0b1000111},{'g',0b1011110},{'h',0b0010111},       
  {'i',0b0010000},{'j',0b0111100},{'l',0b0001110},{'n',0b0010101},{'o',0b0011101},{'p',0b1100111},       
  {'r',0b0000101},{'s',0b1011011},{'t',0b0001111},{'u',0b0011100},{'y',0b0100111},{'-',0b0000001},
  {' ',0b0000000},{'0',0b1111110},{'1',0b0110000},{'2',0b1101101},{'3',0b1111001},{'4',0b0110011},
  {'5',0b1011011},{'6',0b1011111},{'7',0b1110000},{'8',0b1111111},{'9',0b1111011},{'V',0b0111110},
  {'.',0b10000000},{'?',0b1100101},{'\0',0b0000000},};

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
	sendData(position, number);
}


//Hien thi ngay thang nam
unsigned int font_display(char text){
	for(int i = 0; font[i][0] != '\0'; i++)
	{
		if(font[i][0] == text)
		{
			return font[i][1];
		}
	}
}

//Move right, move left 8 char in screen
void move_left(unsigned char string[100], bool is_left, unsigned int solanchay,
				unsigned int time_milisec)
{
	decode_mode(false);
	unsigned int length;
	unsigned int m[20] = {20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
	length = strlen(string);
	/**/
	
	if (is_left == false)
	{
		for (int i = 0; i < solanchay*length; i ++)
		{	
			
			for (int x = 0; x < length; x++)
			{
				if (m[x] < 9)
				sendData(m[x], font_display(string[x]));
			}
		
			for (int j = 0; j < length; j++)
			{
				m[j] = m[j] - 1;
				if (m[j] < 1) m[j] = length;
			}
			
			delay(time_milisec);
			
		}
	}
	
	else 
	{	for (int i = 0; i < solanchay*8; i ++)
		{	
			
			for (int x = 0; x < 8; x++)
			sendData(m[x], font_display(string[x]));
		
			for (int j = 0; j<=7; j++)
			{
				m[j] = m[j] + 1;
				if (m[j] > 8) m[j] = 1;
			}
			
			delay(time_milisec);
			
		}
	}
	
}

void light_up(char LED)
{
	for (int i = 0; i< 100; i++)
	{
		softPwmWrite(LED, i);
		delay(10);
	}
	for (int i = 0; i< 100; i++)
	{
		softPwmWrite(LED, 100 - i);
		delay(10);
	}
}



