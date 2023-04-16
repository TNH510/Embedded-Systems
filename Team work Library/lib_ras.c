#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPiI2C.h>
#include <stdint.h>

#define R 0//11
#define G 1//12
#define B 2//13
#define BT1 3//15
#define BT2 4//16
#define BT3 5//18
#define BT4 6//22
#define INT_pin 7//7

#define SPEAKER 25//37

#define MPU_ADDRESS 0x68
#define WHO_AM_I 0x75
#define TEMP_OUT 0x41
#define DLPF_CFG 0x1A
#define SR 0X19
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define INT_ENABLE 0x38
#define PWR_MGMT_1 0x6B

#define ACCEL_XOUT 0x3B
#define ACCEL_YOUT 0x3D
#define ACCEL_ZOUT 0x3F

//SPI: MOSI --> DIN, SCLK --> CLK, CE0 --> CS
//I2C: Theo thu tu + D C NC GND tuong tung 1 3 5 7 9 

//Khai bao cac bien can dung

//Enable True/False
typedef enum { false, true } bool;

int mpu;

unsigned int number_7seg[10] = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b};

unsigned char font[][2] = {
  {'A',0b1110111},{'B',0b1111111},{'C',0b1001110},{'D',0b1111110},{'E',0b1001111},{'F',0b1000111},       
  {'G',0b1011110},{'H',0b0110111},{'I',0b0110000},{'J',0b0111100},{'L',0b0001110},{'N',0b1110110},       
  {'O',0b1111110},{'P',0b1100111},{'R',0b0000101},{'S',0b1011011},{'T',0b0001111},{'U',0b0111110},       
  {'Y',0b0100111},{'[',0b1001110},{']',0b1111000},{'_',0b0001000},{'a',0b1110111},{'b',0b0011111},       
  {'c',0b0001101},{'d',0b0111101},{'e',0b1001111},{'f',0b1000111},{'g',0b1011110},{'h',0b0010111},       
  {'i',0b0010000},{'j',0b0111100},{'l',0b0001110},{'n',0b0010101},{'o',0b1100011},{'p',0b1100111},       
  {'r',0b0000101},{'s',0b1011011},{'t',0b0001111},{'u',0b0011100},{'y',0b0100111},{'-',0b0000001},
  {' ',0b0000000},{'0',0b1111110},{'1',0b0110000},{'2',0b1101101},{'3',0b1111001},{'4',0b0110011},
  {'5',0b1011011},{'6',0b1011111},{'7',0b1110000},{'8',0b1111111},{'9',0b1111011},{'V',0b0111110},
  {'.',0b10000000},{'?',0b1100101},{'\0',0b0000000}};
  
//Cac ham xu li co ban

	//cpu
	float cpu_temp()
	{
		FILE *tempfile;
		float temp;
		tempfile  = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
		fscanf(tempfile, "%f", &temp);
		temp = temp / 1000;
		fclose(tempfile);
		return temp;
	} 

//Cac ham lien quan den RGB
	
	//Thay doi do sang, mau sac RGB
	void led(unsigned int do_sang, unsigned int r, unsigned int g, unsigned int b)
	{
		unsigned int rd=0, gd=0, bd=0;
		
		//Tinh toan gi tri r,g,b phu hop voi do sang 0-->30
		rd = do_sang*8*r/100;
		gd = do_sang*8*g/100;
		bd = do_sang*8*b/100;
		
		//Hien thi mau
		softPwmWrite(R, rd*100/255);
		softPwmWrite(G, gd*100/255);
		softPwmWrite(B, bd*100/255);
	}
	
	//Tang giam do sang LED tuan tu
	void light_up(char LED)
	{
		for (int i = 0; i< 100; i++)
		{
			softPwmWrite(LED, i);
			delay(10);
		}
		for (int i = 0; i< 100; i++)
		{
			softPwmWrite(LED, 100 - i);
			delay(10);
		}
	}

//Cac ham lien quan SPI

	//Truyen data
	void sendData(unsigned char address, unsigned char value)
	{
		unsigned char data[2] ;//16bit
		data[0] = address; //Dia chi hexa de chon mode can thay doi(datasheet)
		data[1] = value;   //Chon cac tuy chon thay doi(datasheet)
		wiringPiSPIDataRW(0, data, 2); //Gui data gom 2 byte qua channel 0	
	} 
	
	unsigned int font_display(char text)
	{
		for(int i = 0; font[i][0] != '\0'; i++)
		{
			if(font[i][0] == text) return font[i][1];
		}
	}
	
	//Khoi tao man LCD
	void Init_7219(void)
	{
		//set decode mode
		sendData(0x09, 0xff);
		//set intensity
		sendData(0x0A, 0x01);
		//set scan limit
		sendData(0x0B, 0x07);
		//set shutdown
		sendData(0x0C, 0x01);
		//set display test
		sendData(0x0F, 0x00);
	}


	void decode_mode(bool select)
	{
		if (select == true) sendData(0x09, 0xff);
		else sendData(0x09, 0x00);
	}

	//Clear display
	void clear_data()
	{
		decode_mode(true);
		for (int i = 1; i < 9; i ++)
		{
			sendData(i, 0);	
		}
		decode_mode(false);
	}

	//Hien thi so thuc (.2f)
	void display_real_number(float sothuc)
	{
		float sothucx100;
		int16_t dvi = 0, chuc = 0, tram = 0, ngan = 0;
		sothucx100 = sothuc*100;
			
		printf("\r%f", sothuc);
		fflush(stdout);
			
		ngan = sothucx100 / 1000;
		tram = (sothucx100 - ngan*1000) / 100;
		chuc = (sothucx100 - ngan*1000 - tram*100) / 10;
		dvi =  (int)sothucx100%10;
			
		sendData(0x09, 0x00);
		sendData(1, font_display('C'));
		sendData(2, font_display('o'));
		sendData(3, number_7seg[dvi]);
		sendData(4, number_7seg[chuc]);
		sendData(5, number_7seg[tram]|0x80);
		sendData(6, number_7seg[ngan]);		
	}	
	
	//Hien thi so nguyen 2 chu so bat ki len man hinh
	void display_uint_number(unsigned int pos, unsigned int songuyen)
	{
		unsigned int dvi = 0, chuc = 0, tram = 0, ngan = 0;
			
		ngan = songuyen / 1000;
		tram = (songuyen - ngan*1000) / 100;
		chuc = (songuyen - ngan*1000 - tram*100) / 10;
		dvi =  songuyen%10;
			
		sendData(0x09, 0x00);
		sendData(pos, number_7seg[dvi]);
		sendData(pos + 1, number_7seg[chuc]);
	}
	
	//Set up pro (so chu so, do sang, ...)
	void setting_7219(unsigned int numDisplay, unsigned int Intensity)
	{
		sendData(0x0A, Intensity);
		sendData(0x0B, numDisplay - 1);
	}
	
	//shutdown
	void display_ON(bool select)
	{
		if (select == 1) sendData(0x0C, 0x01);
		else sendData(0x0C, 0x00);
	}
	
	//Sang het man hinh
	void test_display(bool select)
	{
		if (select == 1) sendData(0x0F, 0x01);
		else sendData(0x0F, 0x00);
	}
	
	//Hien thi ki tu bat ki tai vi tri bat ki
	void Display_char_no_decode(unsigned char position, char text)
	{
		unsigned char text_bina;
		for(int i = 0; font[i][0] != '\0'; i++){
			if(font[i][0] == text){
				text_bina = font[i][1];
				sendData(position, text_bina);
			}
		}
	}
	
	//Nhap chuoi hien thi len man hinh
	void Display_text_position(char* str, unsigned char position)
	{
		for(int i = 0; str[i] != '\0'; i++){
			Display_char_no_decode(position-i, str[i]);
		}
		for(int j = 1; j < 8 - strlen(str); j++){
			Display_char_no_decode(j, ' ');
		}
	}
	
	//Move in screen
	void move_left(unsigned char string[100], bool is_left, unsigned int solanchay,
					unsigned int time_milisec)
	{
		decode_mode(false);
		unsigned int length;
		unsigned int m[20] = {20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
		length = strlen(string);

		if (is_left == false)
		{
			for (int i = 0; i < solanchay*length; i ++)
			{	
				for (int x = 0; x < length; x++)
				{
					if ((m[x] < 9)&(m[x] > 0))
					sendData(m[x], font_display(string[x]));
				}
			
				for (int j = 0; j < length; j++)
				{
					m[j] = m[j] - 1;
					if (m[j] < 1) m[j] = length;
				}
				delay(time_milisec);
			}
		}
	}
	
	//Run chuoi 8 ki tu
	void Text_run(char* str,int speed,unsigned char mode)
	{
		int a,b,c,g;
		char temp_str[5];
		char rev_str[300];
		char temp_string[8];
		char slave_strings[100][9];
		int num_slave_strings, i;
		int count = 0;
		if(mode == 's'){		
			for(int i = 0; str[i] != '\0'; i++){
				Display_char_no_decode(8-i, str[i]);
			}
		}
		if(mode == 'l'){
			for(int n = 8; n >= 1; n--){ //n is position
				if(n < strlen(str)){
					a = strlen(str) - n;
					//on the right
					for(int i = 0; i < n; i++){
						Display_char_no_decode(n - i, str[i]);
					}
					for(int i = 0; i < strlen(str) - n; i++){
						Display_char_no_decode(8 - i, str[strlen(str) - a]);
						a--;
					}
					delay(speed);
					Display_char_no_decode(n, ' ');
				}
				else{
					for(int i = 0; str[i] != '\0'; i++){
						Display_char_no_decode(n - i, str[i]);
						
					}
					delay(speed);
					Display_char_no_decode(n, ' ');					
				}
			}
		}
		if(mode == 'r'){
			for(int n = 1; n <= 8; n++){ //n is position		
				if(n > 8 - strlen(str) + 1){
					b = n-1;
					for(int i = 0; i < 8 - n + 1; i++){
						Display_char_no_decode(n + i, str[(strlen(str) - 1) - i]);
					}
					for(int i = 0; i < n - (8 - strlen(str)) - 1; i++){
						Display_char_no_decode((i + 1), str[b - (8 - strlen(str))-1]);
						b--;
					}
					delay(speed);
					Display_char_no_decode(n, ' '); //Direction		
					
				}
				else{
					for(int i = 0; str[i] != '\0'; i++){
						Display_char_no_decode(n + i, str[(strlen(str) - 1) - i]);
					}
					delay(speed);
					Display_char_no_decode(n, ' '); //Direction				
				}

			}
		}
		
	}
	
	//
	void create_slave_strings(char* string, char slave_strings[][9], int *num_slave_strings) {
			int i, j, k = 0;

			// Iterate through the original string and create substrings of 8 characters
			for (i = 0; i < strlen(string); i++) {
				if (i + 8 <= strlen(string)) {
					for (j = 0; j < 8; j++) {
						slave_strings[k][j] = string[i+j];
					}
					slave_strings[k][8] = '\0'; // Add null terminator to make it a valid string
					k++;
				}
			}
			*num_slave_strings = k;
		}
		
		//
		void reverse_string(char *str, char *rev_str) {
		char *ptr, *last;	
		int len;
		int count;
		//Take the length of the string
		len = strlen(str);
		ptr = str;
		last = str + len - 1;

		// Reverse the entire string
		while (last > ptr) {
			char temp = *last;
			*last = *ptr;
			*ptr = temp;
			last--;
			ptr++;
		}
		
		// Reverse each word in the string
		ptr = str;
		while (*ptr) {
			char *start = ptr;
			while (*ptr != ' ' && *ptr != '\0') {
				ptr++;
			}
			last = ptr - 1;
			while (last > start) {
				char temp = *last;
				*last = *start;
				*start = temp;
				last--;
				start++;
			}
			if (*ptr == ' ') {
				ptr++;
			}
		}
		
		//When it convert to the reverse string, in the while loop, it's can't convert repeatedly
		//so we need to assign it into a subvariable, and declare a count so as to assign only 1 times
		//to avoid being the same mistake spoken above.
		for(int i = 0; str[i] != '\0'; i++){
			if(count == 0){rev_str[i] = str[i];}
		}
		count = 1;
	}
	
	//Run text on display
	void Display_text(char* str, int speed, unsigned char mode){
		int a,b,c,g;
		char temp_str[5];
		char rev_str[300];
		char temp_string[8];
		char slave_strings[100][9];
		int num_slave_strings, i;
		int count = 0;
	if(mode == 'r')
	{
		if(strlen(str) < 8){
			Text_run(str,speed,'r');
		}
		else{
			create_slave_strings(str, slave_strings,&num_slave_strings);
			for (int i = 0; i < num_slave_strings; i++) {
				for(int j = 0; slave_strings[i][j] != '\0'; j++){
					Display_char_no_decode(8 - j, slave_strings[i][j]);
				}
				delay(speed);
			}
			for(int n = 1; n <= 7; n++){
				b = 7;
				//Display the last string from right to left
				for(int i = 0; i < 8 - n; i++){
					Display_char_no_decode(n + i + 1, slave_strings[num_slave_strings - 1][b]);
					b--;
				}
				//Display the first string from right to left
				for(int i = 0; i < n; i++){
					Display_char_no_decode(n - i, slave_strings[0][i]);
				}
				delay(speed);
			}
		}
	}
		if(mode == 'l'){
			if(strlen(str) < 8){
				Text_run(str,speed,'l');
			}
			else{
				reverse_string(str,rev_str);
				create_slave_strings(rev_str, slave_strings,&num_slave_strings);
				for(int i = num_slave_strings - 1; i >= 0; i--) {
					for(int j = 0; slave_strings[i][j] != '\0'; j++){
						Display_char_no_decode(8 - j, slave_strings[i][j]);
					}
					delay(speed);
				}
				for(int n = 7; n >= 1; n--){
					b = 7;
					for(int i = 0; i < n; i++){
						Display_char_no_decode(n - i, slave_strings[0][i]);
					}
					for(int i = 0; i < 8 - n; i++){ // i is the times iterating to put the char in the led
						Display_char_no_decode(n + i + 1, slave_strings[num_slave_strings - 1][b]);
						b--;
					}
					delay(speed);
				}	
			}		
		}
}

//Cac ham lien quan I2C

	//Khoi tao MPU6050
	void initMPU()
	{
		wiringPiI2CWriteReg8(mpu, SR, 9);
		wiringPiI2CWriteReg8(mpu, DLPF_CFG, 0x02);
		wiringPiI2CWriteReg8(mpu, GYRO_CONFIG, 0x08);
		wiringPiI2CWriteReg8(mpu, ACCEL_CONFIG, 0x10);
		wiringPiI2CWriteReg8(mpu, INT_ENABLE, 1);
		wiringPiI2CWriteReg8(mpu, PWR_MGMT_1, 1);
	}

	int16_t readSensor(uint8_t address)
	{
		int16_t value;
		value = wiringPiI2CReadReg8(mpu, address) << 8 | wiringPiI2CReadReg8(mpu, address+1);
		return value;
	}