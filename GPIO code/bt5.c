#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdlib.h>
#include "LED_library.h"
#include <unistd.h>

#define R 0//11
#define G 1//12
#define B 2//13
#define BT1 3//15
#define BT2 4//16
#define BT3 5//18
#define BT4 6//22
#define BT5 21//29
//Chinh do sang den R,G,B
float r = 0;
float g = 0;
float b = 0;

//Tinh thoi gian trung binh
int point_data[10];
int point = 0;
unsigned int start,end;
unsigned int count = 0;

//Dung de tao dieu kien rang buoc 
int state = 0;
unsigned int enable = 0;

void ngatBT1()
{	
	if (enable == 1) //Viec nhan nut chi co tac dung khi den LED chua tat (< 1000ms)
	{
		softPwmWrite(R, 0); //Tat LED ngay khi nhan 
		end = millis(); //Ket thuc dem
		printf("Time: %d\n ", end - start);
		
		//State = 1 de ham main biet viec nhan nut da co tac dung (Khong hien thi ki tu "...")
		state = 1; 
		fflush(stdout);
		//Count de dem so lan nhan thanh cong
		count++;
		//Tinh thoi gian tu khi LED sang den luc nhan nut thanh cong va gan vao mang
		point_data[count - 1] = end - start;
		
		//Gan enable = 0 de loai bo truong hop nhieu lan nhan nut lien tiep ma duoi 1000ms
		//enable = 1 chi khi LED bat dau sang
		enable = 0;
	}

		
}
void ngatBT2() 
{
	
}
void ngatBT3() 
{

}

void ngatBT4() 
{	


}

void ngatBT5() 
{	

}


int main(void)
{
	//Set up wiringpi
	wiringPiSetup();
	//Set up I/O
	pinMode(R, OUTPUT);
	pinMode(G, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(BT1, INPUT);
	pinMode(BT2, INPUT);
	pinMode(BT3, INPUT);
	pinMode(BT4, INPUT);
	pinMode(BT5, INPUT);
	//Set up Pwm
	softPwmCreate(R, 0, 100);
	softPwmCreate(G, 0, 100);
	softPwmCreate(B, 0, 100);
	//set up interrupt
	wiringPiISR(BT1, PUD_DOWN, &ngatBT1);
	wiringPiISR(BT2, PUD_DOWN, &ngatBT2);
	wiringPiISR(BT3, PUD_DOWN, &ngatBT3);
	wiringPiISR(BT4, PUD_DOWN, &ngatBT4);
	wiringPiISR(BT5, PUD_DOWN, &ngatBT5);
	//loop
	while(1)
	{	
		softPwmWrite(R, 100);
		start = millis();
		enable = 1; //Viec nhan nut bat dau co tac dung
		delay(1000);
		enable = 0; //Khong the nhan nut sau khi den tat (> 1000ms)
		softPwmWrite(R, 0);
		//Neu viec nhan nut duoc chap nhan thi state = 1
		//Neu khong nhan hoac nhan lau hon 1000 ms, hien thi "..."
		if (state != 1) {printf("...\n"); fflush(stdout);}
		
		//Tao thoi gian delay ngau nhien
		delay(rand()%2000);
		//Reset trang thai state
		state = 0;
		
		//Neu nhan du 10 lan duoi 1000ms, tinh trung binh va tong ket diem
		if (count == 10)
		{
			for (int i = 0; i<10; i++)
			{
				point = point + point_data[i];
			}
			
			//Tinh trung binh
			point = (int)(point/10);
			printf("Time trung binh: %d ms\n ----------------- \n ", point); 
			fflush(stdout);
			if (point < 300) {printf("Bạn rất tỉnh đấy :>"); fflush(stdout);}
			if (point >= 300 ) {printf("Mày say rồi con:)"); fflush(stdout);}
			
			//Hien thi den xanh de nguoi dung biet
			light_up(G);
			//Reset bien 
			state = 1;
			point = 0;
			count = 0;
			//Tat LED
			softPwmWrite(G,0);
		}
		
		
	}
	return 0;
}
