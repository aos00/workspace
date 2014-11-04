/*
*	Main Source File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#include <thread>
#include <stdio.h>
#include "GoPro/gopro.h"
#include "GPSDevice/gpsdevice.h"


#define NUM_SHOT_THREADS 10

using namespace std;

const char * GPS_ADDRESS = "localhost";

int main(){ //obter ID inicial da foto, latitude e longitude do alvo...


	GoPro camera((short) 1378);
	
	
	//camera.takePicture();
	//sleep(1);
	//camera.getImage(1386);

	GPSDevice gps(GPS_ADDRESS);
	//gps.connection_open();
	
	
	for(;;){
		gps.read_data();
		
		if(gps.inSurface()){
			std::thread shot(camera.takePicture());
			std::thread getImage(camera.getImage(++camera.PHOTO_ID));
		}
		
	}

return 0;


}
