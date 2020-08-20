#include <stdlib.h>
#include <iobb.h>
#include<unistd.h>
#include <iostream>
#include<cstdio>
#include <sstream>
#include "SocketClient.h"
using namespace std;
using namespace exploringRPi;

void
trigger(int tig) // function to trigger ultrasonic
{
pin_high (8, tig);
iolib_delay_ms (1);
pin_low (8, tig);
}
int echo (int ech){ // function to read echo pin of ultrasonic
int value;
if(is_high(8,ech))
value=1;
else
value=0;
return value;
}
void thingspeak(int field,int dat)
{
    ostringstream head,data; 
          SocketClient sc("api.thingspeak.com",80);
        switch(field)
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
    head << "POST /update HTTP/1.1\n"
        << "Host: api.thingspeak.com\n"
        << "Connection: close\n"
        << "X-THINGSPEAKAPIKEY:7OE9JRDYIRM34MHQ \n"
        << "Content-Type: application/x-www-form-urlencoded\n"
        << "Content-Length:" << string(data.str()).length() << "\n\n";
   sc.send(string(head.str()));
   sc.send(string(data.str()));
    string rec = sc.receive(1024);
}
int
main ()
{
int echo1,echo2,echo3;
iolib_init (); // initializing iobb library
iolib_setdir (8, 7, BBBIO_DIR_OUT); // trigger pin ultrasonic 1
iolib_setdir (8, 8, BBBIO_DIR_IN); // echo pin of Ultrasonic 1
iolib_setdir (8, 9, BBBIO_DIR_OUT);//trigger pin ultrasonic 2
iolib_setdir (8, 10, BBBIO_DIR_IN);// echo pin of Ultrasonic 2
iolib_setdir (8, 11, BBBIO_DIR_OUT);//trigger pin ultrasonic 3
iolib_setdir (8, 12, BBBIO_DIR_IN);// echo pin of Ultrasonic 3
while (1)
    {
        int counter =3; // counter for parking slots
 /*--------Ultrasonic 1 is triggered-------------*/  
       trigger(7);
       iolib_delay_ms (5);
       echo1= echo(8);
       if(echo1==1)  //check the voltage from echo pin of ultrasonic 1
       printf("space 1:vaccant \n");
       else
        {
            printf("space 1: occupied \n");
            counter--;  // reduce the number of available slots if occupied
        }
    /*--------Ultrasonic 2 is triggered-------------*/
        trigger(9);
        iolib_delay_ms (5);
        echo2= echo(10);
       if(echo2==1)  //check the voltage from echo pin of ultrasonic 2
        printf("space 2:vaccant \n");
       else
        {
        printf("space 2: occupied \n");
        counter--; // reduce the number of available slots if occupied
        }
    /*--------Ultrasonic 3 is triggered-------------*/
         trigger(11);
         iolib_delay_ms (5);
        echo3= echo(12);
        if(echo3==1)  //check the voltage from echo pin of ultrasonic 3
        printf("space 3:vaccant \n");
        else
        {
            printf("space 3: occupied \n");
        counter--;//reduce the number of available slots if occupied
        }
        if(counter==0)
        printf("Parking full \n");
        else
        printf("no. of parking available= %d \n",counter); // print available slots
  
   /*--------------updating available parking slots to thingspeak----------------*/
     thingspeak(4,counter);
     sleep(15);
     thingspeak(1,echo1);
     sleep(15);
     thingspeak(2,echo2);
     sleep(15);
     thingspeak(3,echo3);
     sleep(15);    
}
iolib_free (); // closing iobb library
}
