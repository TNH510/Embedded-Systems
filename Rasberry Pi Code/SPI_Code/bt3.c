#include "lib.h"

int main(void)
{	
	wiringPiSPISetup(1, 10000000); //channel 1, 10MHz
	Init_7219();
	sendData(0x0b, 0x05);
	decode_mode(false)
	while(1)
	{	
		display_real_number(cpu_temp());
		delay(500);
	}
	return 0;
}
