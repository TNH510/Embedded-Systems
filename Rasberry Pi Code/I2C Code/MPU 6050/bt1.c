#include "full_lib.h"

void dataReady()
{}

void ngatmpu()
{
	
}

int main(void)
{
	//Set up wiringPi
	wiringPiSetup();
	
	//Set up I2C interrupt
	mpu = wiringPiI2CSetup(MPU_ADDRESS);
	
	//Test I2C connection
	if (wiringPiI2CReadReg8(mpu, WHO_AM_I) != MPU_ADDRESS)
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
		temp = readSensor(TEMP_OUT);
		display_real_number(temp/340.0 + 36.53);
		delay(500);	
	}
	
	return 0;
}
