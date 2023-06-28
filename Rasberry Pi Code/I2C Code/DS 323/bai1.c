#include "lib2.h"

int ds232;

unsigned int decTohex(unsigned int value)
{
	unsigned int h,ch,dv;
	ch = value/10;
	dv = value%10;
	
	//Hang chuc dich trai + hang don vi
	h = (ch<<4) | dv;
	
	return h; 
}

unsigned int hexTodec(unsigned int value)
{
	unsigned int h;
	h = (value >> 4)*10 +(value & 0b00001111);
	return h;
}

int main(void)
{
	//set up I2C interrface
	ds232 = wiringPiI2CSetup(0x68);
	

	
	//Read time
	unsigned int hh,mm,ss;
	//hh = wiringPiI2CSetup(ds232, 0x02);
	
	time_t rawtime;
    struct tm *ct;

    time (&rawtime);
    ct = localtime(&rawtime);
	
	//Set time for module
	wiringPiI2CWriteReg8(ds232, 0x02,decTohex(ct->tm_hour));
	wiringPiI2CWriteReg8(ds232, 0x01,decTohex(ct->tm_min));
	wiringPiI2CWriteReg8(ds232, 0x00,decTohex(ct->tm_sec));
	
	//Set up SPI
	wiringPiSPISetup(1, 10000000); //channel 1, 10MHz
	Init_7219();
	
	clear_data();
	setting_7219(8,2);
	int count = 2;
	
	while (1)
	{
		
		
		ss = wiringPiI2CReadReg8(ds232, 0x00);
		mm = wiringPiI2CReadReg8(ds232, 0x01);
		hh = wiringPiI2CReadReg8(ds232, 0x02);
		display_uint_number(1, hexTodec(ss));
		display_uint_number(4, hexTodec(mm));
		display_uint_number(7, hexTodec(hh));
		
		sendData(3, 0b0000001);
		sendData(6, 0b0000001);
		
		printf ("\r %X %X %X", hh, mm, ss);
		fflush(stdout);
		
		if (count < 9)
		count ++;
		else count = 2;
		
		setting_7219(8,count);
		
		delay(1000);
		
		
		
		
	}
		
	
}
