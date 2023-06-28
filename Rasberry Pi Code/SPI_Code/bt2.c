#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <softPwm.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define R 0//11
#define G 1//12
#define B 2//13
#define BT1 3//15
#define BT2 4//16
#define BT3 5//18
#define BT4 6//22
#define BT5 21//29
// MOSI --> DIN
//SCLK --> CLK
//CE0 --> CS
unsigned char data[2] ;//16bit
unsigned char number[10] = {0,1,2,3,4,5,6,7,8,9};
unsigned int count = 0;
unsigned int dvi = 0, chuc = 0, tram = 0, ngan = 0;
char string[4];
const char string_x[4] = "0000";
int N, k1, k2, k3, k4;

void sendData(unsigned char address, unsigned char value)
{
	data[0] = address; //Dia chi hexa de chon mode can thay doi(datasheet)
	data[1] = value;   //Chon cac tuy chon thay doi(datasheet)
	wiringPiSPIDataRW(1, data, 2); //Gui data gom 2 byte qua channel 0
	
}

void Init(void)
{
	
	//set decode mode
	sendData(0x09, 0xff);
	//set intensity
	sendData(0x0A, 0x01);
	//set scan limit
	sendData(0x0B, 0x00);
	//set shutdown
	sendData(0x0C, 0x01);
	//set display test
	sendData(0x0F, 0x00);
	
}



int main(void)
{
	wiringPiSPISetup(1, 10000000); //channel 0, 10MHz
	
	//Khoi tao SPI
	Init();
	sendData(1, 0);//reset second
	sendData(2, 0);
	sendData(3, 0);
	sendData(4, 0);
	while (1)
	{		
		//Cach 1
		ngan = count / 1000;
		tram = (count - ngan*1000) / 100 ;
		chuc = (count - ngan*1000 - tram*100) / 10;
		dvi = count%10;
		
		sendData(1, number[dvi]);//Gui data roi moi cho hien tren LED de khong bi giat
		if (count == 1) sendData(0x0B, 0x00);
		sendData(2, number[chuc]);
		if (count == 10) sendData(0x0B, 0x01);
		sendData(3, number[tram]);
		if (count == 100) sendData(0x0B, 0x02);
		sendData(4, number[ngan]);//ngan
		if (count == 1000) sendData(0x0B, 0x03);
		
		count++;
		delay(500);
		
		//Cach 2
		/*if (count <= 10) {k1 = 0;k2 = 0;k3 = 0;k4 = 1;delay(1);}
		else if (count <= 100) {sendData(0x0B, 0x01);k1 = 0;k2 = 0;k3 = 1;k4 = 2;}
		else if (count <=1000) {sendData(0x0B, 0x02);k1 = 0;k2 = 1;k3 = 2;k4 = 3;}
		else if (count <= 10000) {sendData(0x0B, 0x03);k1 = 1;k2 = 2;k3 = 3;k4 = 4;}
		
		sendData(k1, string[3]);//ngan
		sendData(k2, string[2]);
		sendData(k3, string[1]);
		sendData(k4, string[0]);//don vi
				
		sprintf(string, "%d", count%10000);
			
		count++;
		delay(500);*/
			
			
		//Cach 3	
		/*for (unsigned int m= 0; m< 10; m++)
		{		
			sendData(1, number[0]);//reset second
			sendData(2, number[0]);
			sendData(3, number[0]);
			sendData(4, number[m]);
			if (m == 1) sendData(0x0B, 0x03);					
			for (unsigned int l= 0; l< 10; l++)
			{		
				sendData(1, number[0]);//reset second
				sendData(2, number[0]);
				sendData(3, number[l]);
				if ((l == 1)&(m == 0)) sendData(0x0B, 0x02);				
				for (unsigned int j= 0; j< 10; j++)
				{		
					sendData(1, number[0]);//reset second
					sendData(2, number[j]);
					if ((j == 1)&(l == 0)&(m == 0)) sendData(0x0B, 0x01);
					for (unsigned int i= 0; i< 10; i++)
					{
						sendData(1, number[i]);
						delay(500);
					}
				}	
			}	
		}*/
	}
	
	return 0;
	
}
