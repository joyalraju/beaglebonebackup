#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>

#define I2C_BUS        "/dev/i2c-2" // I2C bus device on Beaglebone
#define I2C_ADDR       0x27        // I2C slave address for the LCD module
 
int i2cFile;

void i2c_start() {
   if((i2cFile = open(I2C_BUS, O_RDWR)) < 0) {
      printf("Error failed to open I2C bus \n");
      exit(-1);} // check if bus is working or not  
      if (ioctl(i2cFile, I2C_SLAVE, I2C_ADDR) < 0) {
      printf("Error failed to set I2C address.\n");
      exit(-1); //setting slave address for all transfer
   }
}

void i2c_stop() {
    close(i2cFile);// closing connection
    }

void i2c_send_byte(unsigned char data) //function send data to i2c slave
{
   unsigned char byte[1];
   byte[0] = data;
   write(i2cFile, byte, sizeof(byte)); // writing data to the bus
   usleep(1000);
}

void lcd_init()
{
   i2c_send_byte(0b00110100); // D7=0, D6=0, D5=1, D4=1, RS,RW=0 EN=1
   i2c_send_byte(0b00110000); // D7=0, D6=0, D5=1, D4=1, RS,RW=0 EN=0
   usleep(4100);              // wait 4.1msec
   i2c_send_byte(0b00110100); //
   i2c_send_byte(0b00110000); // same
   usleep(100);               // wait 100usec
   i2c_send_byte(0b00110100); //
   i2c_send_byte(0b00110000); // 8-bit mode init complete
   usleep(4100);              // wait 4.1msec
   i2c_send_byte(0b00100100); //
   i2c_send_byte(0b00100000); // switched now to 4-bit mode
   usleep(40);                // wait 40usec
   LCD_cmd(0x28); // 4-bit mode with 2 lines char 5x8
   usleep(40);                // wait 40usec
   LCD_cmd(0x08);            //Display off, cursor off
   usleep(40);     // wait 40usec
   LCD_cmd(0x08); //Display off, cursor off
   LCD_cmd(0x01);
   usleep(40);                // wait 40usec
   LCD_cmd(0x06); //Increment cursor (shift cursor to right)
   usleep(40);                // wait 40usec
   LCD_cmd(0x0E); // Display on, cursor blinking
   LCD_cmd(0x80);   //Force cursor to beginning ( 1st line)
 usleep(40);      // wait 40usec
   }
void
LCD_cmd (unsigned char cmd)// function to give data to command register of LCD  
{
char upper = cmd>>4 ;    // seperate msb of the data
char up1 =(upper<<4)|0x4; // addding enable pin high, rs pin 0, rw pin 0
char up2 =(upper<<4)|0x0; // addding enable pin low, rs pin 0, rw pin 0
char lower= cmd & 0x0f; // seperate lsb of the data
char lw1= (lower<<4) | 0x4; // addding enable pin high, rs pin 0, rw pin 0
char lw2= (lower<<4) | 0x0; // addding enable pin low, rs pin 0, rw pin 0
i2c_send_byte(up1); // sending data part by part
i2c_send_byte(up2);
i2c_send_byte(lw1);
i2c_send_byte(lw2);
  usleep (10);
}
void
LCD_data (unsigned char data) // function to give data to data register of LCD
{
char upper = data>>4 ; // seperate msb of the data
char up1 =(upper<<4)|0xd; // addding enable pin high, rs pin 1, rw pin 0
char up2 =(upper<<4)|0x9; // addding enable pin low, rs pin 1, rw pin 0
char lower= data & 0x0f;  // seperate lsb of the data
char lw1= (lower<<4) | 0xd;  // addding enable pin high, rs pin 1, rw pin 0
char lw2= (lower<<4) | 0x9; // addding enable pin low, rs pin 1, rw pin 0
i2c_send_byte(up1); // sending data part by part
i2c_send_byte(up2);
i2c_send_byte(lw1);
i2c_send_byte(lw2);
  usleep (10);
}
void
LCD_string (const char *buf) // function to send sting of data
{
  while (*buf)
    {
      usleep (10); // 10us delay
      LCD_data (*buf); // send each character to the bus
      buf++;
      usleep (10); // 10us delay
    }
}
void main() {
   i2c_start();      //initalizing i2c connection
   usleep(15000);             // wait 15msec
 lcd_init();       //initializing lcd display
 LCD_string("  ** WELCOME **  ");
 sleep(5);
 LCD_cmd(0x01);    //clear display
 usleep(1000);
 LCD_cmd(0x80);//Force cursor to beginning ( 1st line)
 usleep(100);
 LCD_string ("  SMART PARKING  ");
 sleep(2);
 LCD_cmd(0xC0);//Force cursor to beginning ( 2nd line)
 usleep(100);
 LCD_string("  By GROUP 1  ");
 sleep(2);
int data=10;
 LCD_cmd(0x01);    //clear display
 usleep(1000);
 LCD_cmd(0x80);//Force cursor to beginning ( 1st line)
 usleep(100);
 LCD_string("FREE SPACES =");
 usleep(1000);
char res[5];
sprintf(res,"%d",data);
LCD_string(res);
 i2c_stop(); //disconnecting i2c connection
}

