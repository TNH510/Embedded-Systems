#include "full_lib.h"

typedef struct{
	int x;
	int y;
} Plane;

Plane plane;

unsigned int matrix[8][8] = {{0,0,0,0,0,0,1,1},
							 {0,0,0,0,0,0,0,1},
							 {0,0,0,0,0,0,0,0},
							 {0,0,0,0,0,0,0,0},
							 {0,0,0,0,0,0,0,0},
							 {0,0,0,0,0,0,0,0},
							 {0,0,0,0,0,0,0,0},
							 {0,0,0,0,0,0,0,0}};
							 
void display_8x8(unsigned int matrix[8][8])
{
	for (int m = 0; m < 8; m++)
	{ 
		char data = 0b00000000;
		for (int n = 7; n >=0; n--)
		{ 
			data = data | (matrix[m][7 - n] << n);
		}
		sendData(m + 1, data);
	}
}

//Variable
int16_t temp = 0;
float Ax = 0;
float Ay = 0; 
float Az = 0;
float Angle1 = 0;
float Angle2 = 0;
float Angle3 = 0;

void reset_matrix()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			matrix[i][j] = 0;
		}
	}
}

int main(void)
{
	//Set up wiringPi
	wiringPiSetup();
	
	//Set up SPI
	wiringPiSPISetup(0, 10000000); //channel 0, 10MHz
	Init_7219();
	setting_7219(8,2);
	
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
	
	//Clear data
	clear_data();
	
	//Set up I/O
	pinMode(R, OUTPUT);
	pinMode(G, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(SPEAKER, OUTPUT);
	//Set up Pwm
	softPwmCreate(R, 0, 100);
	softPwmCreate(G, 0, 100);
	softPwmCreate(B, 0, 100);
	
	srand(time(NULL));
	
	//Initilize plane
	plane.x = 4;
	plane.y = 4;
	
	//
	int Angle1_old = 0 ;
	int Angle2_old = 0 ;
	
	//loop
	while(1)
	{
		reset_matrix();
		//temp = readSensor(TEMP_OUT);
		Ax = readSensor(ACCEL_XOUT)/4096.0;
		Ay = readSensor(ACCEL_YOUT)/4096.0;
		Az = readSensor(ACCEL_ZOUT)/4096.0;
		
		Angle2 = atan2(Ay, sqrt(Ax*Ax + Az*Az))*180.0/3.14;
		Angle1 = atan2(Ax, sqrt(Ay*Ay + Az*Az))*180.0/3.14;
		printf("Pitch %.5f Roll %.5f\n", Angle1, Angle2);
		
		if (Angle1 > Angle1_old + 5)
		{
			printf("Right %.5f\n", Angle1);
			plane.y++;
		}
	
		if (Angle1 < Angle1_old - 5)
		{
			printf("Left %.5f\n", Angle1);
			plane.y--;
		}
		
		if (Angle2 > Angle2_old + 5)
		{
			printf("Up %.5f\n", Angle2);
			plane.x++;
		}
	
		if (Angle2 < Angle2_old - 5)
		{
			printf("Down %.5f\n", Angle2);
			plane.x--;
		}
		fflush(stdout);
		
		if (plane.x > 7) plane.x = 7; 
		if (plane.y > 7) plane.y = 7; 
		
		if (plane.x < 0) plane.x = 0; 
		if (plane.y < 0) plane.y = 0; 
		
		matrix[plane.x][plane.y] = 1;
		display_8x8(matrix);
		
		Angle1_old = Angle1;
		Angle2_old = Angle2;
		delay(100);

		
	}
}							 