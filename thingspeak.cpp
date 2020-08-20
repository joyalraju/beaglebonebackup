//#include <stdio.h>
#include <stdlib.h>
#include <iobb.h>
#include <iostream>
#include<cstdio>
#include <unistd.h>
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
        int counter =3;
       trigger(7);
       iolib_delay_ms (3);
       echo1= echo(8);
       if(echo1==1)  //check the voltage from echo pin of ultrasonic 1
       printf("space 1:vaccant \n");
       else
        {
            printf("space 1: occupied \n");
            counter--;  // reduce the number of available slots if occupied
        }
          ostringstream head, data;
          SocketClient sc("api.thingspeak.com",80);
        data << "field1=" << echo1 << endl;
         sc.connectToServer();
    head << "POST /update HTTP/1.1\n"
        << "Host: api.thingspeak.com\n"
        << "Connection: close\n"
        << "X-THINGSPEAKAPIKEY:7OE9JRDYIRM34MHQ \n"
        << "Content-Type: application/x-www-form-urlencoded\n"
        << "Content-Length:" << string(data.str()).length() << "\n\n";
cout<< string(data.str()).length()<<endl;  
 sc.send(string(head.str()));
   sc.send(string(data.str()));
    string rec = sc.receive(1024);
//    cout<< rec << endl;
sleep(1);
        trigger(9);
        iolib_delay_ms (3);
        echo2= echo(10);
       if(echo2==1)  //check the voltage from echo pin of ultrasonic 2
        printf("space 2:vaccant \n");
       else
        {
        printf("space 2: occupied \n");
        counter--; // reduce the number of available slots if occupied
        }
        ostringstream head1, data1;
         SocketClient sc1("api.thingspeak.com",80);
        data1 << "field2=" << echo2 << endl;
         sc1.connectToServer();
    head1 << "POST /update HTTP/1.1\n"
        << "Host: api.thingspeak.com\n"
        << "Connection: close\n"
        << "X-THINGSPEAKAPIKEY:7OE9JRDYIRM34MHQ \n"
        << "Content-Type: application/x-www-form-urlencoded\n"
        << "Content-Length:" << string(data1.str()).length() << "\n\n";
cout<< string(data1.str()).length()+string(data.str()).length()<<endl;  
   sc1.send(string(head1.str()));
   sc1.send(string(data1.str()));
    string rec1 = sc1.receive(1024);
  //  cout<< rec1 << endl;
        sleep(1); 
	trigger(11);
         iolib_delay_ms (3);
        echo3= echo(12);
        if(echo3==1)  //check the voltage from echo pin of ultrasonic 3
        printf("space 3:vaccant \n");
        else
        {
            printf("space 3: occupied \n");
        counter--;//reduce the number of available slots if occupied
        }
        ostringstream head2, data2;
     SocketClient sc2("api.thingspeak.com",80);
        data2 << "field3=" << echo3 << endl;
         sc2.connectToServer();
    head2 << "POST /update HTTP/1.1\n"
        << "Host: api.thingspeak.com\n"
        << "Connection: close\n"
        << "X-THINGSPEAKAPIKEY:7OE9JRDYIRM34MHQ \n"
        << "Content-Type: application/x-www-form-urlencoded\n"
        << "Content-Length:" << string(data.str()).length() << "\n\n";
   cout<< string(data2.str()).length()<<endl;  
sc2.send(string(head2.str()));
   sc2.send(string(data2.str()));
    string rec2 = sc2.receive(1024);
   // cout<< rec2 << endl;
sleep(1);       
 if(counter==0)
        printf("Parking full \n");
        else
        printf("no. of parking available= %d \n",counter); // print available slots
    
    ostringstream head3, data3;
     SocketClient sc3("api.thingspeak.com",80);
        data3 << "field4=" << counter << endl;
         sc3.connectToServer();
    head3 << "POST /update HTTP/1.1\n"
        << "Host: api.thingspeak.com\n"
        << "Connection: close\n"
        << "X-THINGSPEAKAPIKEY:7OE9JRDYIRM34MHQ \n"
        << "Content-Type: application/x-www-form-urlencoded\n"
        << "Content-Length:" << string(data3.str()).length() << "\n\n";
   sc3.send(string(head3.str()));
   sc3.send(string(data3.str()));
    string rec3 = sc3.receive(1024);
   // cout<< rec3 << endl;
       sleep(15);	
}
iolib_free (); // closing iobb library
}
