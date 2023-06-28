#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "lib_ras.h"
char hello[5] = "hello";

void Go_back()
{
    sendData(0x09, 0x00);
    sendData(0x07, 0b00011100);
}

void Go_ahead()
{
    sendData(0x09, 0x00);
    sendData(0x07, 0b01100010);
}

void Turn_right()
{
    sendData(0x09, 0x00);
    sendData(0x07, 0b01111000);
}

void Turn_left()
{
    sendData(0x09, 0x00);
    sendData(0x07, 0b01001110);
}

int main()
{   
    //Set up wiringpi
	wiringPiSetup();

    //Set up SPI
	wiringPiSPISetup(0,10000000); //10Mhz
	Init_7219();
	clear_data();

	//Make color Start:))
	welcome();

    clear_data();

    while(1)
    {
        char *fifo = "mypipe";
        int fd = open(fifo, O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        char buffer[256];
        int len = read(fd, buffer, sizeof(buffer));
        buffer[len] = '\0';
        
        display_real_number(atof(buffer));
        if (digitalRead(BT5) == 0) Go_ahead();
        if (digitalRead(BT6) == 0) Go_back();
        if (digitalRead(BT4) == 0) Turn_left();
        if (digitalRead(BT1) == 0) Turn_right();

        
        
    }
    return 0;
}
