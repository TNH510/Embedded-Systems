#include "lib2.c"
//Cac ham setting thong so
void Init_7219(void);
void display_ON(bool select);
void setting_7219(unsigned int numDisplay, unsigned int Intensity);
void decode_mode(bool select);
void sendData(unsigned char address, unsigned char value);

//Cac ham tien ich
void clear_data();
void test_display(bool select);

//Cac hafm bai tap
float cpu_temp();
void move_left(unsigned char string[8], bool is_left, unsigned int solanchay, unsigned int time_milisec);

//Cac ham hien thi					
void display_number(unsigned int position, unsigned int number);
void display_real_number(float sothuc);
void display_uint_number(unsigned int pos, unsigned int songuyen);
unsigned int font_display(char text);

//LED
void light_up(char LED);
