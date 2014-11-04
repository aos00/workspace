/*
*	Main Source File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#include <stdio.h>
#include "GoPro/gopro.h"
#include "GPSDevice/gpsdevice.h"

const char * GPS_ADDRESS = "localhost";

int main(){ //obter ID inicial da foto

	//GoPro camera((short) 1378);
	//camera.takePicture();
	//sleep(1);
	//camera.getImage(1386);

	GPSDevice gps(GPS_ADDRESS);
	//gps.connection_open();
	gps.read_data();

return 0;


}
