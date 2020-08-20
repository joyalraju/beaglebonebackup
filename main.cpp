/*----------------------------------------------------------------------
-----------IOT BASED SMART PARKING USING BEAGLEBONE BLACK WIRELESS------
----Author: Joyal Raju--------------------------------------------------
----Team members: Merlin Mariam Thankachan, Sruthy Krishnan-------------
----Mentor:Prof. Dr. Mike Aleshams--------------------------------------
----Lambton college Toronto--------------------------------------------*/

#include <stdlib.h>
#include <iobb.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <gps.h>
#include <sstream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "SocketClient.h"
using namespace std;
using namespace exploringRPi;

int echo1,echo2,echo3,counter,i2cFile;
#define degree90 132    //step count for stepper motor
#define I2C_BUS        "/dev/i2c-2" // I2C bus device on Beaglebone
#define I2C_ADDR       0x27        // I2C slave address for the LCD module

/*----------function to trigger ultrasonic---------*/
void
trigger(int port,int tig) 
{
pin_high (port, tig);
iolib_delay_ms (1);
pin_low (port, tig);
}

/*----------function to read echo pin of ultrasonic---------*/
int echo (int port,int ech){ 
int value;
if(is_high(port,ech))
value=1;
else
value=0;
return value;
}

/*-------------lcd functions-------------*/
void i2c_start() {
   if((i2cFile = open(I2C_BUS, O_RDWR)) < 0) {
      printf("Error: Failed to open I2C bus \n");
      exit(-1);} // check if bus is working or not  
      if (ioctl(i2cFile, I2C_SLAVE, I2C_ADDR) < 0) {
      printf("Error: Failed to set I2C address.\n");
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
LCD_string (const char *buf) // function to send string of data
{
  while (*buf)
    {
      usleep (10); // 10us delay
      LCD_data (*buf); // send each character to the bus
      buf++;
      usleep (10); // 10us delay
    }
}


/*------------slot checking function-----------*/
void slotcheck()
{
     counter =3; // counter for parking slots
/*gps_init();                   // initialize the device
loc_t gps;                    // a location structure
gps_location(&gps);*/           // determine the location data 
char lat[50],log[50];
long double lati= 43.7441040;
long double longi= -79.2462439;
sprintf(lat,"%Lf",lati);
sprintf(log,"%Lf",longi);
LCD_cmd(0x01);// clear display
usleep(1000);
LCD_cmd(0x80);//Force cursor to beginning ( 1st line)
        usleep(100);
LCD_string("LAT: ");
LCD_string(lat);
usleep(1000);
LCD_cmd(0xC0);//Force cursor to beginning ( 2nd line)
usleep(100);
LCD_string("LOGI: ");
LCD_string(log);
sleep(2);
LCD_cmd(0x01);// clear display

 /*--------Ultrasonic 1 is triggered-------------*/
        trigger(8,7);
       iolib_delay_ms (5);
       echo1= echo(8,8);
       LCD_cmd(0xC0);//Force cursor to beginning ( 2nd line)
       usleep(100);
       if(echo1==1)  //check the voltage from echo pin of ultrasonic 1
       LCD_string("LOT 1: Vaccant");
	      
else
        {
        LCD_string("LOT 1: Occupied");  
        counter--;  // reduce the number of available slots if occupied
        }
        sleep(2);
    /*--------Ultrasonic 2 is triggered-------------*/
        trigger(8,9);
        iolib_delay_ms (5);
        echo2= echo(8,10);
       LCD_cmd(0xC0);//Force cursor to beginning ( 2nd line)
       usleep(100);
       if(echo2==1)  //check the voltage from echo pin of ultrasonic 2
        
        LCD_string("LOT 2: Vaccant"); 
	else
        {
        LCD_string("LOT 2: Occupied");
        counter--; // reduce the number of available slots if occupied
        }
        sleep(2);
    /*--------Ultrasonic 3 is triggered-------------*/
         trigger(8,11);
         iolib_delay_ms (5);
        echo3= echo(8,12);
        LCD_cmd(0xC0);//Force cursor to beginning ( 2nd line)
       usleep(100);
        if(echo3==1)  //check the voltage from echo pin of ultrasonic 3
        LCD_string("LOT 3: Vaccant");
	else
        {
        LCD_string("LOT 3: Occupied");
        counter--;//reduce the number of available slots if occupied
        }
	sleep(2);
        LCD_cmd(0x01);// clear display
        usleep(1000);
        LCD_cmd(0x80);//Force cursor to beginning ( 1st line)
        usleep(100);
        if(counter==0)
        LCD_string("PARKING FULL!");
        else
	{char num[2];
	sprintf(num,"%d",counter);
        LCD_string("SLOTS AVAIL= "); 
        LCD_string(num);}// print available number of slots
	sleep(1);
}

/*----------Thingspeak server update------------------------*/

void thingspeak(int field,int dat) // function to update the thingspeak cloud
{
    ostringstream head,data;
          SocketClient sc("api.thingspeak.com",80);
       switch(field)//selecting field
{ case 1:
        data << "field1=" << dat << endl;
break;
case 2:
        data << "field2=" << dat << endl;
break;
case 3:
        data << "field3=" << dat << endl;
break;
case 4:
        data << "field4=" << dat << endl;
break;
}
 sc.connectToServer();
 /*---head data for server---*/
    head << "POST /update HTTP/1.1\n"
        << "Host: api.thingspeak.com\n"
        << "Connection: close\n"
        << "X-THINGSPEAKAPIKEY:7OE9JRDYIRM34MHQ \n"
        << "Content-Type: application/x-www-form-urlencoded\n"
        << "Content-Length:" << string(data.str()).length() << "\n\n";
   sc.send(string(head.str()));
   sc.send(string(data.str()));
    string rec = sc.receive(1024);//uploading data and head
}

/*-------------function to de-energize all coils of motor------------*/
void
PINLOW () 
{
pin_low (8, 12);
pin_low (8, 14);
pin_low (8, 16);
pin_low (8, 18);
}

/*--------------------stepper motor control function-------------*/
void
motor()
{
int i = 0;              //i = 524 for 360 degree, i= 262 for 180 degree
      while (i < degree90)     // closewise rotation
        {
PINLOW ();
pin_high (8, 15);
iolib_delay_ms (2);     //Coil A is energized for 2 millisecond
          PINLOW ();
pin_high (8, 14);
iolib_delay_ms (2);     //Coil B is energized for 2 millisecond
          PINLOW ();
pin_high (8, 16);
iolib_delay_ms (2);     //Coil C is energized for 2 millisecond
          PINLOW ();
pin_high (8, 18);
iolib_delay_ms (2);     //Coil D is energized for 2 millisecond
          i++;
        }
        
sleep(2);    // delay to hold barrier   
while (i > 0)           // anti-closewise rotation
        {
PINLOW ();
pin_high (8, 18);
iolib_delay_ms (2);     //Coil D is energized for 2 millisecond
          PINLOW ();
pin_high (8, 16);
iolib_delay_ms (2);     //Coil C is energized for 2 millisecond
          PINLOW ();
pin_high (8, 14);
iolib_delay_ms (2);     //Coil B is energized for 2 millisecond
          PINLOW ();
pin_high (8, 15);
iolib_delay_ms (2);     //Coil A is energized for 2 millisecond
          i--;
        }  
}

int
main ()
{
int enter;
iolib_init (); // initializing iobb library
iolib_setdir (8, 7, BBBIO_DIR_OUT); // trigger pin ultrasonic 1
iolib_setdir (8, 8, BBBIO_DIR_IN); // echo pin of Ultrasonic 1
iolib_setdir (8, 9, BBBIO_DIR_OUT);//trigger pin ultrasonic 2
iolib_setdir (8, 10, BBBIO_DIR_IN);// echo pin of Ultrasonic 2
iolib_setdir (8, 11, BBBIO_DIR_OUT);//trigger pin ultrasonic 3
iolib_setdir (8, 12, BBBIO_DIR_IN);// echo pin of Ultrasonic 3
iolib_setdir (8, 17, BBBIO_DIR_OUT);//trigger pin ultrasonic enterance
iolib_setdir (8, 26, BBBIO_DIR_IN);// echo pin of Ultrasonic enterance
iolib_setdir (8, 15, BBBIO_DIR_OUT);//motor contol int1
iolib_setdir (8, 14, BBBIO_DIR_OUT);//motor contol int2
iolib_setdir (8, 16, BBBIO_DIR_OUT);//motor contol int3
iolib_setdir (8, 18, BBBIO_DIR_OUT); //motor contol int4
i2c_start();      //initalizing i2c connection
 usleep(15000);             // wait 15msec
 lcd_init();       //initializing lcd display
LCD_string("  ** WELCOME ** ");
 sleep(3);
 LCD_cmd(0x01);// clear display
 usleep(1000);
 LCD_cmd(0x80);//Force cursor to beginning ( 1st line)
 usleep(100);
 LCD_string ("  SMART PARKING ");
 sleep(3);
 LCD_cmd(0xC0);//Force cursor to beginning ( 2nd line)
 usleep(100);
 LCD_string ("   BY GROUP 1");
 sleep(2);
 LCD_cmd(0x01);// clear display
 usleep(1000);
 slotcheck();
while (1)
    {
       trigger(8,17);
       iolib_delay_ms (2);
       enter=echo(8,26);
       if (enter==0 && counter!=0)
       {motor();
	cout<<"car"<<endl;
       sleep(5);
       slotcheck();
/*--------------updating available parking slots to thingspeak--------------*/
     thingspeak(4,counter);
     sleep(15);
     thingspeak(1,echo1);
     sleep(15);
     thingspeak(2,echo2);
     sleep(15);
     thingspeak(3,echo3);
     sleep(15);
       }
       else if(enter==0&&counter==0)
       {
         LCD_cmd(0x01);// clear display
        usleep(1000);
        LCD_cmd(0x80);//Force cursor to beginning ( 1st line)
        usleep(100);
         LCD_string("PARKING FULL!");
       }

slotcheck();
}
iolib_free (); // closing iobb library
}
