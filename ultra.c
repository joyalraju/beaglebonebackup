#include <stdio.h> 
#include <stdlib.h> 
#include <iobb.h>
void trigger(char tig) // function to trigger ultrasonic
 {
 pin_high (8,tig);
 iolib_delay_ms (1); 
 pin_low (8,tig);
}
int echo (char ech){ // function to read echo pin of ultrasonic 
int value; 
if(is_high(8,ech)) 
value=1; 
else 
value=0;
return value;
} 
void main () 
{
 int echo1,echo2,echo3;
 iolib_init (); // initializing iobb library 
iolib_setdir (8,7, BBBIO_DIR_OUT); // trigger pin ultrasonic 1 
iolib_setdir (8,8, BBBIO_DIR_IN); // echo pin of Ultrasonic 1 
iolib_setdir (8, 9, BBBIO_DIR_OUT);//trigger pin ultrasonic 2 
iolib_setdir (8, 10, BBBIO_DIR_IN);// echo pin of Ultrasonic 2
iolib_setdir (8, 11, BBBIO_DIR_OUT);//trigger pin ultrasonic 3
iolib_setdir (8, 12, BBBIO_DIR_IN);// echo pin of Ultrasonic 3 
while(1)
{
int counter =3;
      trigger(7);
	 iolib_delay_ms (5); 
       echo1= echo(8);
       if(echo1== 1) //check the voltage from echo pin of ultrasonic 1
       printf("space 1: vaccant \n");
       else
        {
            printf("space 1: occupied \n");
            counter--; // reduce the number of available slots if occupied
	}
	trigger(9);
	 iolib_delay_ms (5); 
        echo2= echo(10);
       if(echo2==1) //check the voltage from echo pin of ultrasonic 2
        printf("space 2: vaccant \n");
       else 
        {
        printf("space 2: occupied \n");
        counter--; // reduce the number of available slots if occupied
        }
         trigger(11);
         iolib_delay_ms (5); 
	echo3= echo(12);
        if(echo3==1) //check the voltage from echo pin of ultrasonic 3
        printf("space 3: vaccant \n");
        else
        {
            printf("space 3: occupied \n");
        counter--;//reduce the number of available slots if occupied
        }
        if(counter==0)
        printf("Parking full \n");
        else
        printf("no. of parking available= %d \n",counter); // print available slots
	 iolib_delay_ms (100); 	
	}
iolib_free (); // closing iobb library
}
