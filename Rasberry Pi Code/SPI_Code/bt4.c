#include "lib.h"

int main(void)
{
	wiringPiSPISetup(1, 10000000); //channel 1, 10MHz
	Init_7219();
	clear_data();
	setting_7219(8, 2);
	int i = 0;
	while(1)
	{
		move_left("24681357", true, 5, 1000);
		
	}
	return 0;
}
