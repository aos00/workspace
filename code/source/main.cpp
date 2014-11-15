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

#define SHUTTER_PIN 0
#define MODE_PIN 3
#define START_STOP_PIN 4

#define GPS_PORT 2947
#define DEBOUNCING_TIME 1000
const string filepath = "/home/pi/raspi_local_repo/code/quadra";

using namespace std;

const char * GPS_ADDRESS = "localhost";
unsigned int time_new = 0;
unsigned int time_old = 0;
GoPro camera;

int setInterrupts(void);

void shutter(void){
	time_new = millis();
	if((time_new - time_old) < DEBOUNCING_TIME)
		return;
	camera.pressShutter();
	time_old = time_new;
}

void nextMode(void) {
	time_new = millis();
	if((time_new - time_old) < DEBOUNCING_TIME)
		return;
	camera.setNextMode();
	time_old = time_new;
}

void startRoutine(void) {
	time_new = millis();
	if((time_new - time_old) < DEBOUNCING_TIME)
		return;
	printf("Starting Routine..\n");
	//camera.setNextMode();
	time_old = time_new;
}


int main(){ //obter ID inicial da foto, latitude e longitude do alvo...

	camera.init((short) 1404, VIDEO_MODE);

	GPSDevice gps(GPS_ADDRESS, GPS_PORT);

	PhotoHandler handler;

	TargetArea area(filepath);
	
	setInterrupts();

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

int setInterrupts(){
	
	#ifdef __DEBUG__
	printf("Stetting interrupts...\n");
	#endif
	
	
	if (wiringPiSetup () < 0) {
	      perror("Unable to setup wiringPi: %s\n");
     	 return -1;
	}

	if( wiringPiISR (MODE_PIN, INT_EDGE_FALLING, &nextMode) < 0) {
		perror("Unable to setup ISR: %s\n");
      		return -1;
  	}
  	
  	if( wiringPiISR (SHUTTER_PIN, INT_EDGE_FALLING, &shutter) < 0) {
		perror("Unable to setup ISR: %s\n");
      		return -1;
  	}
  	
  	if( wiringPiISR (START_STOP_PIN, INT_EDGE_FALLING, &startRoutine) < 0) {
		perror("Unable to setup ISR: %s\n");
      		return -1;
  	}
  	return 1;
}
	
