#include "full_lib.h"

	//Variable
	int16_t temp = 0;
	float Ax = 0;
	float Ay = 0; 
	float Az = 0;
	float Angle1 = 0;
	float Angle2 = 0;
	float Angle3 = 0;

void dataReady() {}

void ngatmpu() 
{
	//Angle = atan2(-Ax, sqrt((int)Ay*(int)Ay + (int)Az*(int)Az))*180.0/3.14;
	//Angle = atan2(Ay, Az) * 180.0 / 3.14;

	
}

int main(void)
{
	//Set up wiringPi
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
	softPwmCreate(R, 0, 100);
	softPwmCreate(G, 0, 100);
	softPwmCreate(B, 0, 100);
	softPwmCreate(SPEAKER, 0, 100);
	
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
	
	printf("Oke");
	fflush(stdout);
	while(1)
	{
		//temp = readSensor(TEMP_OUT);
		Ax = readSensor(ACCEL_XOUT)/4096.0;
		Ay = readSensor(ACCEL_YOUT)/4096.0;
		Az = readSensor(ACCEL_ZOUT)/4096.0;
		
		//printf("%.2f %.2f %.2f \n ", Ax, Ay, Az);
		//fflush(stdout);
		
		Angle1 = atan2(Ax, sqrt(Ay*Ay + Az*Az))*180.0/3.14;
		Angle2 = atan2(Ay, sqrt(Ax*Ax + Az*Az))*180.0/3.14;
		Angle3 = atan2(Az, sqrt(Ay*Ay + Ax*Ax))*180.0/3.14;

		printf("%.5f\n", Angle1);
		fflush(stdout);
		
		//Neu goc am thi cho gia tri bang 0
		if (Angle1 < 0) Angle1 = 0;
		if (Angle2 < 0) Angle2 = 0;
		if (Angle3 < 0) Angle3 = 0;
		
		//Rescale tu 0,90 ve 0,255
		led(5, Angle1*255/90, Angle2*255/90, Angle3*255/90);
		
		delay(500);
		
		//display_real_number(temp/340.0 + 36.53);
	}
	
	return 0;
}

