#include "lib_ras.c"

//Cac ham xu li co ban

	//cpu
	float cpu_temp();
	
//Cac ham lien quan den RGB
	
	//Thay doi do sang, mau sac RGB
	void led(unsigned int do_sang, unsigned int r, unsigned int g, unsigned int b);
	
	//Tang giam do sang LED tuan tu
	void light_up(char LED);
	
//Cac ham lien quan SPI

	//Khoi tao man LCD
	void Init_7219(void);
	
	//Truyen data
	void sendData(unsigned char address, unsigned char value);
	
	//Hien thi ki tu tu tao
	unsigned int font_display(char text);
	
	//Hien thi ki tu bat ki tai vi tri bat ki
	void Display_char_no_decode(unsigned char position, char text);
	
	//Nhap chuoi hien thi len man hinh
	void Display_text_position(char* str, unsigned char position);
	
	//Run chuoi 8 ki tu
	void Text_run(char* str,int speed,unsigned char mode);
	
	//
	void create_slave_strings(char* string, char slave_strings[][9], int *num_slave_strings);
	
	//
	void reverse_string(char *str, char *rev_str);
	
	//Run text on display
	void Display_text(char* str, int speed, unsigned char mode);
	
	//Chuyen sang dung ki tu tu tao
	void decode_mode(bool select);
	
	//Clear display
	void clear_data();
	
	//Hien thi so thuc (.2f)
	void display_real_number(float sothuc);
	
	//Hien thi so nguyen 2 chu so bat ki len man hinh
	void display_uint_number(unsigned int pos, unsigned int songuyen);
	
	//Set up pro (so chu so, do sang, ...)
	void setting_7219(unsigned int numDisplay, unsigned int Intensity);
	
	//shutdown
	void display_ON(bool select);
	
	//Test man hinh
	void test_display(bool select);
	
	//Move in screen
	void move_left(unsigned char string[100], bool is_left, unsigned int solanchay,
					unsigned int time_milisec);
	
//Cac ham lien quan I2C

	//Khoi tao MPU6050
	void initMPU();
	
	//Doc gia tri cam bien tu thanh ghi (2 thanh ghi 8 bit ke tiep nhau)
	int16_t readSensor(uint8_t address);
	
	

	
	