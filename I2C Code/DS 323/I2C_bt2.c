#include "lib2.h"

int ds232;
unsigned int HH = 0, MM = 0, SS = 0;
unsigned char thoi_gian[5];

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
	//Set up wiringPi
	wiringPiSetup();
	
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
	wiringPiSPISetup(0, 10000000); //channel 0, 10MHz
	Init_7219();
	
	//Set up PWM
	softPwmCreate(R, 0, 100);
	softPwmCreate(G, 0, 100);
	softPwmCreate(B, 0, 100);
	
	//Nguoi dung nhap gio hen
	printf("Moi ban nhap gio hen:");
	scanf("%s", thoi_gian);
	
	//Doc gio tu chuoi ma nguoi dung nhap (HH:MM)
	HH = ((unsigned int)thoi_gian[0] - 48)*10 + (unsigned int)thoi_gian[1] - 48;
	MM = ((unsigned int)thoi_gian[3] - 48)*10 + (unsigned int)thoi_gian[4] - 48;
	SS = ((unsigned int)thoi_gian[6] - 48)*10 + (unsigned int)thoi_gian[7] - 48;
	
	//In gio nhap len man hinh
	printf("Gio ban da nhap la: %d:%d:%d", HH, MM, SS);
	
	clear_data();
	setting_7219(8,2);
	int count = 2;
	unsigned int state = 0;
	
	while (1)
	{
		
		//Doc gio tu DS3231
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
		
		
		//Set dem nguoc
		if ((hexTodec(mm) == MM)&(hexTodec(hh) == HH)&(hexTodec(ss) == SS)) state = 1;
			
		//while de thoat ra
		while(state == 1)
		{
			move_left(" -------- ", false, 3, 100);
			delay(100);
			light_up(R);
		}
		delay(1000);
		
		
		
		
	}
		
	
}
