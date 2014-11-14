/*
*	Main Source File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#define __DEBUG__

#include <stdio.h>
#include "gopro.hpp"
#include "gpsdevice.hpp"
#include "photohandler.hpp"
#include "photo.hpp"
#include "targetarea.hpp"
#include <wiringPi.h>

#define BUTTON_PIN 0
#define GPS_PORT 2947
#define DEBOUNCING_TIME 1000
const string filepath = "/home/pi/raspi_local_repo/code/quadra";

using namespace std;

const char * GPS_ADDRESS = "localhost";
unsigned int time_new = 0;
unsigned int time_old = 0;
GoPro camera;


void shutter(void){
	time_new = millis();
	if((time_new - time_old) < DEBOUNCING_TIME)
		return;
	camera.takePicture(NULL);
	time_old = time_new;
}



int main(){ //obter ID inicial da foto, latitude e longitude do alvo...

	camera.init((short) 1404);

	GPSDevice gps(GPS_ADDRESS, GPS_PORT);

	PhotoHandler handler;

	TargetArea area(filepath);

	camera.setCameraMode(PHOTO_MODE);

	if (wiringPiSetup () < 0) {
	      perror("Unable to setup wiringPi: %s\n");
     	 return -1;
	}

	if( wiringPiISR (BUTTON_PIN, INT_EDGE_FALLING, &shutter) < 0) {
		perror("Unable to setup ISR: %s\n");
      		return 1;
  	}


	int m=0;
	while(1){
/*
	    if(m>50)
            break;
		if(gps.read_data()){
			if(area.inTarget(gps.current_location.coordinate)){
			    gps.setLocation();
				cout << "take picture" << endl;
				camera.takePicture(&gps.current_location);
				m++;


			}
		}
*/
	}

	camera.writePhotoRecords();

	return 0;
}
