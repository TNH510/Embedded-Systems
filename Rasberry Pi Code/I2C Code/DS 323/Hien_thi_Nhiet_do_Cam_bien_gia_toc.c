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
#define BT5 21//29


#define INT_pin 7//7
// MOSI --> DIN
//SCLK --> CLK
//CE0 --> CS
//Theo thu tu + D C NC GND tuong tung 1 3 5 7 9 

int mpu;
unsigned int number_7seg[10] = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b};
unsigned char font[][2] = {
  {'A',0b1110111},{'B',0b1111111},{'C',0b1001110},{'D',0b1111110},{'E',0b1001111},{'F',0b1000111},       
  {'G',0b1011110},{'H',0b0110111},{'I',0b0110000},{'J',0b0111100},{'L',0b0001110},{'N',0b1110110},       
  {'O',0b1111110},{'P',0b1100111},{'R',0b0000101},{'S',0b1011011},{'T',0b0001111},{'U',0b0111110},       
  {'Y',0b0100111},{'[',0b1001110},{']',0b1111000},{'_',0b0001000},{'a',0b1110111},{'b',0b0011111},       
  {'c',0b0001101},{'d',0b0111101},{'e',0b1001111},{'f',0b1000111},{'g',0b1011110},{'h',0b0010111},       
  {'i',0b0010000},{'j',0b0111100},{'l',0b0001110},{'n',0b0010101},{'o',0b1100011},{'p',0b1100111},       
  {'r',0b0000101},{'s',0b1011011},{'t',0b0001111},{'u',0b0011100},{'y',0b0100111},{'-',0b0000001},
  {' ',0b0000000},{'0',0b1111110},{'1',0b0110000},{'2',0b1101101},{'3',0b1111001},{'4',0b0110011},
  {'5',0b1011011},{'6',0b1011111},{'7',0b1110000},{'8',0b1111111},{'9',0b1111011},{'V',0b0111110},
  {'.',0b10000000},{'?',0b1100101},{'\0',0b0000000},};
typedef enum { false, true } bool;

void sendData(unsigned char address, unsigned char value)
{
	unsigned char data[2] ;//16bit
	data[0] = address; //Dia chi hexa de chon mode can thay doi(datasheet)
	data[1] = value;   //Chon cac tuy chon thay doi(datasheet)
	wiringPiSPIDataRW(0, data, 2); //Gui data gom 2 byte qua channel 0
	
}

void initMPU()
{
	wiringPiI2CWriteReg8(mpu, 0x19, 9);
	wiringPiI2CWriteReg8(mpu, 0x1A, 0x02);
	wiringPiI2CWriteReg8(mpu, 0x1B, 0x08);
	wiringPiI2CWriteReg8(mpu, 0x1C, 0x10);
	wiringPiI2CWriteReg8(mpu, 0x38, 1);
	wiringPiI2CWriteReg8(mpu, 0x6B, 1);
	
}
void dataReady(void)
{}

void ngatmpu()
{
	
}

int16_t readSensor(uint8_t address)
{
	int16_t value;
	value = wiringPiI2CReadReg8(mpu, address) << 8 | wiringPiI2CReadReg8(mpu, address+1);
	return value;
}
	
unsigned int font_display(char text){
	for(int i = 0; font[i][0] != '\0'; i++)
	{
		if(font[i][0] == text)
		{
			return font[i][1];
		}
	}
}	
	
//Hien thi so thuc (.2f)
void display_real_number(float sothuc)
{
		float sothucx100;
		int16_t dvi = 0, chuc = 0, tram = 0, ngan = 0;
		sothucx100 = sothuc*100;
		
		printf("\r%f", sothuc);
		fflush(stdout);
		
		ngan = sothucx100 / 1000;
		tram = (sothucx100 - ngan*1000) / 100;
		chuc = (sothucx100 - ngan*1000 - tram*100) / 10;
		dvi =  (int)sothucx100%10;
		
		sendData(0x09, 0x00);
		sendData(1, font_display('C'));
		sendData(2, font_display('o'));
		sendData(3, number_7seg[dvi]);
		sendData(4, number_7seg[chuc]);
		sendData(5, number_7seg[tram]|0x80);
		sendData(6, number_7seg[ngan]);		
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

void clear_data()
{
	decode_mode(true);
	for (int i = 1; i < 9; i ++)
	{
		sendData(i, 0);	
	}
	decode_mode(false);
	
}



int main(void)
{
	//Set up wiringPi
	wiringPiSetup();
	
	//Set up I2C interrupt
	mpu = wiringPiI2CSetup(0x68);
	
	//Test I2C connection
	if (wiringPiI2CReadReg8(mpu, 0x75) != 0x68)
	{
		printf("Connection fail.\n");
		exit(1);
	}
	
	//set up operation mode for mpu-6050
	initMPU();
	//Set up interrupt
	wiringPiISR(INT_pin, PUD_UP, &ngatmpu);
	
	//Set up SPI
	wiringPiSPISetup(0,10000000); //10Mhz
	Init_7219();
	clear_data();
	
	//Variable
	int16_t temp = 0;
	
	printf("Oke");
	fflush(stdout);
	while(1)
	{
		
		temp = readSensor(0x41);
		//printf("%d\n ", temp);
		//printf("%.2f\n ", temp/340.0 + 36.53);
		//fflush(stdout);
		
		display_real_number(temp/340.0 + 36.53);
		delay(500);
		
		
		
	}
	
	return 0;
}
