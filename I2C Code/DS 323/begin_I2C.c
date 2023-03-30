#include "lib2.h"

uint8_t dectTohex(uint8_t, value)
{
	uint8_t h,ch,dv;
	ch = value/10;
	dv = value%dv;
	
	//Hang chuc dich trai + hang don vi
	h = (ch<<4) | dv;
	
	return h; 
}

int main(void)
{
	//set up I2C interrface
	ds232 = wiringPiI2CSetup(0x68);
	//max31100 = wiringPiI2CSetup(0x57);
	
	//Set time for module
	wiringPiI2CWriteReg8(ds232, 0x00,0x35);
	wiringPiI2CWriteReg8(ds232, 0x01,0x14);
	wiringPiI2CWriteReg8(ds232, 0x02,0x48);
	
	//Read time
	uint8_t hh,mm,ss;
	hh = wiringPiI2CSetup(ds232, 0x02);
	
	return 0;
}
