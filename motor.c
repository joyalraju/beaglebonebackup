#include <stdio.h>
#include <stdlib.h>
#include <iobb.h>

#define degree90 132
#define degree180 263
#define degree360 525
  
void
PINLOW () // function to de-energize all coils
{				
pin_low (8, 12);
  
pin_low (8, 14);
  
pin_low (8, 16);
  
pin_low (8, 18);

} 
 
void 

main () 
{
  
iolib_init (); // initializing iobb library
  
iolib_setdir (8, 12, BBBIO_DIR_OUT); 
  
iolib_setdir (8, 14, BBBIO_DIR_OUT);
  
iolib_setdir (8, 16, BBBIO_DIR_OUT);
  
iolib_setdir (8, 18, BBBIO_DIR_OUT); // Setting Gpio pins as output pins
  
while (1)
    {
      
int i = 0;		//i = 524 for 360 degree, i= 262 for 180 degree
      while (i < degree180)	// closewise rotation
	{
	  
PINLOW ();
	  
pin_high (8, 12);
	  
iolib_delay_ms (2);	//Coil A is energized for 2 millisecond
	  PINLOW ();
pin_high (8, 14);

iolib_delay_ms (2);     //Coil B is energized for 2 millisecond
          PINLOW ();

pin_high (8, 16);

iolib_delay_ms (2);     //Coil C is energized for 2 millisecond
          PINLOW ();

	  
pin_high (8, 14);
	  
iolib_delay_ms (2);	//Coil B is energized for 2 millisecond
	  PINLOW ();
	  
pin_high (8, 16);
	  
iolib_delay_ms (2);	//Coil C is energized for 2 millisecond 
	  PINLOW ();
	  
pin_high (8, 18);
	  
iolib_delay_ms (2);	//Coil D is energized for 2 millisecond
	  i++;
	
}
      
while (i > 0)		// anti-closewise rotation
	{
	  
PINLOW ();
	  
pin_high (8, 18);
	  
iolib_delay_ms (2);	//Coil D is energized for 2 millisecond
	  PINLOW ();
	  
pin_high (8, 16);
	  
iolib_delay_ms (2);	//Coil C is energized for 2 millisecond 
	  PINLOW ();
	  
pin_high (8, 14);
	  
iolib_delay_ms (2);	//Coil B is energized for 2 millisecond 
	  PINLOW ();
	  
pin_high (8, 12);
	  
iolib_delay_ms (2);	//Coil A is energized for 2 millisecond
	  i--;
	
}
    
 
}
  
iolib_free ();

}

