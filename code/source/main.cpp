/*
*	Main Source File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#define __DEBUG__

#include <wiringPi.h>
#include <pthread.h>
#include <stdio.h>
#include "gopro.hpp"
#include "gpsdevice.hpp"
#include "photohandler.hpp"
#include "photo.hpp"
#include "targetarea.hpp"
#include "common.hpp"


#define PIN_SHUTTER 0
#define PIN_MODE 2
#define PIN_START 3
#define PIN_LED_GPS 1
#define PIN_LED_CAMERA 4
#define PIN_LED_TARGET 5
#define PIN_LED_DWL 6

#define GPS_PORT 2947
#define DEBOUNCING_TIME 500
const string filepath = "/home/pi/raspi_local_repo/code/quadra";

using namespace std;

unsigned short STATUS_CAMERA;
unsigned short STATUS_GPS;
unsigned short STATUS_IN_TARGET;

const char * GPS_ADDRESS = "localhost";
unsigned int time_new = 0;
unsigned int time_old = 0;
GoPro camera;
pthread_t tread_checkleds;

int setInterrupts(void);
int setupWiringPi(void);
void* checkLEDs(void*);

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

void driveLEDs_ALL_RED(){
	
	
}


int main(){ //obter ID inicial da foto, latitude e longitude do alvo...

	if(setupWiringPi() < 1 ){
			perror("##INIT ERROR CANNOR SETUP WIRING PI!! EXITTING...\n");
			return -1;
	}
	
	try{
		camera.init((short) 1404, PHOTO_MODE);
	}catch (CURLcode res){
		fprintf(stderr, "##GoPro CAMERA_INIT() ERROR! curl_easy_perform() failed: %s  ERROR NUMBER: %i\n", curl_easy_strerror(res), res);
	}

	GPSDevice gps(GPS_ADDRESS, GPS_PORT);

	PhotoHandler handler;

	TargetArea area(filepath);
	
	
	
	pthread_create(&tread_checkleds, NULL, checkLEDs, NULL);
	//testLEDs();
	
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

int setupWiringPi(){
	
	#ifdef __DEBUG__
	printf("##INIT Stetting up WiringPi...\n");
	#endif
	
	if (wiringPiSetup () < 0) {
	     perror("Unable to setup wiringPi: %s\n");
     	 return -1;
	}
	pinMode(PIN_LED_GPS, OUTPUT);
	pinMode(PIN_LED_CAMERA, OUTPUT);
	pinMode(PIN_LED_TARGET, OUTPUT);
	pinMode(PIN_LED_DWL, OUTPUT);
	
	#ifdef __DEBUG__
	printf("##INIT WiringPi finished!\n");
	#endif	
	
	return 1;
}

int setInterrupts(void){
	
	#ifdef __DEBUG__
	printf("##INIT Stetting interrupts...\n");
	#endif
	

	if( wiringPiISR (PIN_MODE, INT_EDGE_FALLING, &nextMode) < 0) {
		perror("Unable to setup ISR: %s\n");
      		return -1;
  	}
  	
  	if( wiringPiISR (PIN_SHUTTER, INT_EDGE_FALLING, &shutter) < 0) {
		perror("Unable to setup ISR: %s\n");
      		return -1;
  	}
  	
  	if( wiringPiISR (PIN_START, INT_EDGE_FALLING, &startRoutine) < 0) {
		perror("Unable to setup ISR: %s\n");
      		return -1;
  	}
  	
  	#ifdef __DEBUG__
	printf("##INIT  All Interrupts were set!\n");
	#endif
  	return 1;
}

void *checkLEDs(void*){
	
	#ifdef __DEBUG__
	printf("##INIT checinkg LEDs...\n");
	#endif

	for(short i=0; i<3; i++){
		digitalWrite(PIN_LED_GPS, LOW);	delay(250);
		digitalWrite(PIN_LED_GPS, HIGH); delay(250);
		digitalWrite(PIN_LED_CAMERA, LOW);	delay(250);
		digitalWrite(PIN_LED_CAMERA, HIGH);	delay(250);
		digitalWrite(PIN_LED_TARGET, LOW); delay(250);
		digitalWrite(PIN_LED_TARGET, HIGH);	delay(250);
		digitalWrite(PIN_LED_DWL,LOW); delay(250);
		digitalWrite(PIN_LED_DWL, HIGH); delay(250);		
	}
	
	#ifdef __DEBUG__
	printf("##INIT LEDs check finished!\n");
	#endif
}
