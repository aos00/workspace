/*
*	Main Source File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#include <pthread.h>
#include <stdio.h>
#include "GoPro/gopro.h"
#include "GPSDevice/gpsdevice.h"


#define NUM_SHOT_THREADS 10

using namespace std;

const char * GPS_ADDRESS = "localhost";
pthread_t shot;
int thread_status;


int main(){ //obter ID inicial da foto, latitude e longitude do alvo...


	GoPro camera((short) 1378);

	GPSDevice gps(GPS_ADDRESS);	
	
	for(;;){
		gps.read_data();
		
		if(gps.inSurface()){
			camera.takePicture();
			camera.getImage();
		}
		sleep(3);
	}

return 0;


}
