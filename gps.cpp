#include<iostream>
#include<cstdio>
#include<stdlib.h>
#include<gps.h>   // gps header file

int main(){

printf("gps is intialized: \n");
while(1){
   gps_init();                   // initialize the device
  loc_t gps;                    // a location structure
   gps_location(&gps);           // determine the location data
   printf("location: %lf,%lf\n", gps.latitude, gps.longitude);
        //printing the location
//sleep(5); // delay
}
}
