#include "full_lib.h"

//Class 2
// Struct for the snake
typedef struct {
    int x;
    int y;
} Snake;

Snake ran[8*8];
int ran_length = 2;
int direction = 1; //1 right, 2 down, 3 left, 4 up
// Variables for the food
int food_row;
int food_col;
int state = 0;
// Initialize the snake's starting position

void ran_move()
{
	// Move the snake's body
    for(int z = ran_length; z > 0; z--) {
    ran[z].x = ran[z-1].x;
    ran[z].y = ran[z-1].y;
	
	}
}

void check_wall()
{
	// Check if the snake has hit the wall
    /*if(ran[0].x < 0 || ran[0].x >= 8 || ran[0].y < 0 || ran[0].y >= 8) {
    printf("Game over!\n");
    state = 1;
    }*/
	
	//Check snake eat itself
	/*for (int i = 1; i < ran_length; i++)
	{
		if ((ran[0].x == ran[i].x) && (ran[0].y == ran[i].y))
		state = 1;
	}*/
	
}

void check_button()
{
	
	switch (direction)
	{
		case 1:
			ran[0].y++;
			break;
		case 2:
			ran[0].x++;
			break;
		case 3:
			ran[0].y--;
			break;
		case 4:
			ran[0].x--;
			break;
	}
	check_wall();
	ran_move();
	
	if (ran[0].x > 7) ran[0].x = 0;
	if (ran[0].x < 0) ran[0].x = 7;
	if (ran[0].y > 7) ran[0].y = 0;
	if (ran[0].y < 0) ran[0].y = 7;
	
}


void ngatBT2() 
{
	direction = 4;
}
void ngatBT3() 
{
	direction = 2;
}

void ngatBT4() 
{	

	direction = 3;
}

void ngatBT1() 
{	
	direction = 1;
}

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


void update_matrix()
{
	for (int t = 1; t < ran_length; t++)
	{
		matrix[ran[t].x][ran[t].y] = 1;
	}
}

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


void generate_food() {
	
    food_row = rand() % 8;
    food_col = rand() % 8;   
}

void check_food()
{
	// Check if the snake has eaten the food
    if(ran[0].x == food_row && ran[0].y == food_col) {
        ran_length++;
		digitalWrite(SPEAKER, HIGH);
		delay(250);
		digitalWrite(SPEAKER, LOW);
        generate_food();
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


int main(void)
{
	ran[0].x = 0;
	ran[0].y = 0;
	//Set up wiringPi
	wiringPiSetup();
	
	//Set up SPI
	wiringPiSPISetup(0, 10000000); //channel 0, 10MHz
	Init_7219();
	setting_7219(8,2);
	
	//Clear data
	clear_data();
	
	//Set up I2C interrupt
	mpu = wiringPiI2CSetup(MPU_ADDRESS);
	
	//Test I2C connection
	if (wiringPiI2CReadReg8(mpu, WHO_AM_I) != MPU_ADDRESS)
	{
		printf("Connection fail.\n");
		exit(1);
	}
	
	//Set up I/O
	pinMode(R, OUTPUT);
	pinMode(G, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(BT1, INPUT);
	pinMode(BT2, INPUT);
	pinMode(BT3, INPUT);
	pinMode(BT4, INPUT);
	pinMode(SPEAKER, OUTPUT);
	//Set up Pwm
	softPwmCreate(R, 0, 100);
	softPwmCreate(G, 0, 100);
	softPwmCreate(B, 0, 100);
	//set up interrupt
	wiringPiISR(BT1, PUD_DOWN, &ngatBT1);
	wiringPiISR(BT2, PUD_DOWN, &ngatBT2);
	wiringPiISR(BT3, PUD_DOWN, &ngatBT3);
	wiringPiISR(BT4, PUD_DOWN, &ngatBT4);
	
	srand(time(NULL));
	
	// Generate the food
    generate_food();
	
	//
	int count = 0;
	int count1 = 0;
	int count2 = 0;
	//
	int Angle1_old = 0 ;
	int Angle2_old = 0 ;
	//loop
	while(state == 0)
	{	
		reset_matrix();
		
		if (count == 0) check_button();
		count++;
		if (count > 10)
		count = 0;
	
		if (count1 == 0) 
		matrix[food_row][food_col] = 0;
		count1++;
		if (count1 > 4)
		{matrix[food_row][food_col] = 1;
		count1 = 0;}
		check_food();
		
		update_matrix();
		display_8x8(matrix);
		
		Ax = readSensor(ACCEL_XOUT)/4096.0;
		Ay = readSensor(ACCEL_YOUT)/4096.0;
		Az = readSensor(ACCEL_ZOUT)/4096.0;
		
		Angle2 = atan2(Ay, sqrt(Ax*Ax + Az*Az))*180.0/3.14;
		Angle1 = atan2(Ax, sqrt(Ay*Ay + Az*Az))*180.0/3.14;
		printf("Pitch %.5f Roll %.5f\n", Angle1, Angle2);
		
		if (Angle1 > Angle1_old + 5)
		{
			printf("Right %.5f\n", Angle1);
			direction = 1;
		}
	
		if (Angle1 < Angle1_old - 5)
		{
			printf("Left %.5f\n", Angle1);
			direction = 3;
		}
		
		if (Angle2 > Angle2_old + 10)
		{
			printf("Up %.5f\n", Angle2);
			direction = 2;
		}
	
		if (Angle2 < Angle2_old - 10)
		{
			printf("Down %.5f\n", Angle2);
			direction = 4;
		}
		fflush(stdout);
		Angle1_old  = Angle1;
		Angle2_old  = Angle2;
		delay(20);	
	}
	
	digitalWrite(SPEAKER, HIGH);
	delay(250);
	digitalWrite(SPEAKER, LOW);
	delay(250);
	digitalWrite(SPEAKER, HIGH);
	delay(250);
	digitalWrite(SPEAKER, LOW);
	delay(250);
	digitalWrite(SPEAKER, HIGH);
	delay(250);
	digitalWrite(SPEAKER, LOW);

	while(state == 1)
	{	
		
		count = 0;
		food_row = rand() % 8;
		food_col = rand() % 8; 
		matrix[food_row][food_col] = 1;
		display_8x8(matrix);
		delay(10);
	}
}