
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
#include <ctime>
#include "gopro.hpp"
#include "gpsdevice.hpp"
#include "photohandler.hpp"
#include "photo.hpp"
#include "targetarea.hpp"
#include "common.hpp"


#define PIN_SHUTTER 0
#define PIN_MODE 2
#define PIN_START 3
#define PIN_DWL 11
#define PIN_LED_GPS 1
#define PIN_LED_CAMERA 4
#define PIN_LED_TARGET 5
#define PIN_LED_DWL 6
#define PIN_LED_ROUTINE 7

#define STATUS_ERROR 0
#define STATUS_OK 1

#define GPS_PORT 2947
#define DEBOUNCING_TIME 500

#define ESTADO_STANDBY 1
#define ESTADO_SHOOT 2
#define ESTADO_DWL 3
#define ESTADO_ROUTINE 4


const string filepath = "/home/pi/raspi_local_repo/code/quadra";

using namespace std;

bool STATUS_CAMERA = false;
bool STATUS_GPS = false;
bool STATUS_IN_TARGET = false;
bool STATUS_ROUTINE = false;

int volatile ESTADO_ATUAL;

const char * GPS_ADDRESS = "localhost";
unsigned int time_new = 0;
unsigned int time_old = 0;
GoPro camera;
GPSDevice gps;
TargetArea area;
PhotoHandler handler;

int setInterrupts(void);
int setupWiringPi(void);
void checkLEDs(void);

void shutter(void){
	time_new = millis();
	if((time_new - time_old) < DEBOUNCING_TIME)
		return;
	ESTADO_ATUAL = ESTADO_SHOOT;	
	time_old = time_new;
	#ifdef __DEBUG__
		printf("##Interrupt Shutter ...\n");
	#endif
}

void nextMode(void) {
	time_new = millis();
	if((time_new - time_old) < DEBOUNCING_TIME)
		return;
	ESTADO_ATUAL = ESTADO_NEWMODE;
	time_old = time_new;
}

void download(void) {
	time_new = millis();
	if((time_new - time_old) < DEBOUNCING_TIME)
		return;
	ESTADO_ATUAL = ESTADO_DWL;
	time_old = time_new;
}

void driveLEDs_ALL_RED(){
	digitalWrite(PIN_LED_GPS, LOW);
	digitalWrite(PIN_LED_CAMERA, LOW);
	digitalWrite(PIN_LED_TARGET, LOW);
	digitalWrite(PIN_LED_DWL,LOW);
	digitalWrite(PIN_LED_ROUTINE,LOW);	
}

int main(int argc, char *argv[]){
	
	if(argc == 1){
		printf("You must pass the initial photo ID\n Exitting...");
		return -1;
	}	
		
	unsigned short id  = atoi(argv[1]);
	
	#ifdef __DEBUG__
	printf("ID: %d\n",id);
	#endif	
	
	try{
		setupWiringPi();
		
	}catch (const char * msg){
		perror(msg);
		driveLEDs_ALL_RED();
		return -1;
	}
	
	checkLEDs();
	
	try{
		camera.init(id, PHOTO_MODE);
		digitalWrite(PIN_LED_CAMERA, LOW);
		STATUS_CAMERA = STATUS_OK;
	}catch (CURLcode res){
		fprintf(stderr, "##GoPro CAMERA_INIT() ERROR! curl_easy_perform() failed: %s  ERROR NUMBER: %i\n", curl_easy_strerror(res), res);
		digitalWrite(PIN_LED_CAMERA, HIGH);
		STATUS_CAMERA = STATUS_ERROR;
		return -1;
	}catch (const char* msg){
		perror(msg);
		digitalWrite(PIN_LED_CAMERA, HIGH);
		STATUS_CAMERA = STATUS_ERROR;
		return -1;
	}
	
	try{
		gps.init(GPS_ADDRESS, GPS_PORT);
		digitalWrite(PIN_LED_GPS, LOW);
			
			STATUS_GPS = STATUS_OK;
		
	}catch (const char * msg){
		perror(msg);
		digitalWrite(PIN_LED_GPS, HIGH);
		STATUS_GPS = STATUS_ERROR;
	}	
	
	try{
		int n = 15;
		while(n>=0){
			gps.read_data();
			n--;
			cout<< "aquii" << endl;
		}
		digitalWrite(PIN_LED_GPS, LOW);
		STATUS_GPS = STATUS_OK;
	}catch(const char * msg){
		perror(msg);
		digitalWrite(PIN_LED_GPS, HIGH);
		STATUS_GPS = STATUS_ERROR;
	}
	
	digitalWrite(PIN_LED_GPS, HIGH);
		STATUS_GPS = STATUS_ERROR;
	
	try{
		setInterrupts();
	}catch (const char * msg){
		perror(msg);
		driveLEDs_ALL_RED();
		return -1;
	}


	camera.setCameraMode(PHOTO_MODE);
	ESTADO_ATUAL = ESTADO_STANDBY;
	
	while(true){

		switch (ESTADO_ATUAL){
		case ESTADO_STANDBY:
			break;
			
		case ESTADO_ROUTINE:
			printf("Status routine %d\n", STATUS_ROUTINE);
			#ifdef __DEBUG__
			printf("Routine running...\n");
			#endif
		
		
			
			default:
				break;
		}
	}
	return 0;
}

int setupWiringPi(){
	
	printf("##INIT Stetting up WiringPi...\n");	
	
	if (wiringPiSetup () < 0) {
     	 throw("Unable to setup wiringPi: %s\n");
	}
	
	pinMode(PIN_LED_GPS, OUTPUT);
	pinMode(PIN_LED_CAMERA, OUTPUT);
	pinMode(PIN_LED_TARGET, OUTPUT);
	pinMode(PIN_LED_DWL, OUTPUT);
	pinMode(PIN_LED_ROUTINE, OUTPUT);
	
	printf("##INIT WiringPi finished!\n");	
	
	return 1;
}

int setInterrupts(void){
	
	printf("##INIT Stetting interrupts...\n");
	

	if( wiringPiISR (PIN_MODE, INT_EDGE_FALLING, &nextMode) < 0) {
		throw("Unable to setup ISR: nextMode %s\n");
      		return -1;
  	}
  	
  	if( wiringPiISR (PIN_SHUTTER, INT_EDGE_FALLING, &shutter) < 0) {
		throw("Unable to setup ISR: shutter %s\n");
      		return -1;
  	}
  	
	if (wiringPiISR(PIN_DWL, INT_EDGE_FALLING, &download) < 0) {
		throw("Unable to setup ISR: startRoutine %s\n");
      		return -1;
  	}
  	
  	printf("##INIT  All Interrupts were set!\n");
	
  	return 1;
}

void checkLEDs(){	
	#ifdef __DEBUG__
	printf("##INIT checinkg LEDs...\n");
	#endif
	
	unsigned short delay_time = 100;

	for(short i=0; i<2; i++){
		digitalWrite(PIN_LED_GPS, LOW);	delay(delay_time);
		digitalWrite(PIN_LED_GPS, HIGH); delay(delay_time);
		digitalWrite(PIN_LED_CAMERA, LOW);	delay(delay_time);
		digitalWrite(PIN_LED_CAMERA, HIGH);	delay(delay_time);
		digitalWrite(PIN_LED_TARGET, LOW); delay(delay_time);
		digitalWrite(PIN_LED_TARGET, HIGH);	delay(delay_time);
		digitalWrite(PIN_LED_DWL,LOW); delay(delay_time);
		digitalWrite(PIN_LED_DWL, HIGH); delay(delay_time);
		digitalWrite(PIN_LED_ROUTINE,LOW); delay(delay_time);
		digitalWrite(PIN_LED_ROUTINE,HIGH); delay(delay_time);		
	}	
	#ifdef __DEBUG__
	printf("##INIT LEDs check finished!\n");
	#endif
}
