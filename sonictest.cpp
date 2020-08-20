#include <iobb.h>
#include <unistd.h>
#include <iostream>
void trigger(int tig) // function to trigger ultrasonic 
{ pin_high (8, tig); 
iolib_delay_ms(0.05);
 pin_low (8, tig);
}
int echo (int ech){ // function to read echo pin of ultrasonic
 int value; 
if(is_high(8,ech))
 value=1; 
else value=0;
 return value;
}
int main () { 
int echo1; 
iolib_init (); // initializing iobb library 
iolib_setdir (8, 17, DigitalOut); // trigger pin ultrasonic 1 
iolib_setdir (8, 18, DigitalIn); // echo pin of Ultrasonic 1 
while (1)
    {
       trigger(17);
    iolib_delay_ms (5);
       echo1= echo(18);
       if(echo1==1) //check the voltage from echo pin of ultrasonic 1
       printf("space 1:vaccant \n");
       else
        {
            printf("space 1: occupied \n");
        }
    }
iolib_free (); // closing iobb library
}
