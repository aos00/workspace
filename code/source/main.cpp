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


#define GPS_PORT 2947

using namespace std;

const char * GPS_ADDRESS = "localhost";

int main(){ //obter ID inicial da foto, latitude e longitude do alvo...

	Photo * foto;
	
	GoPro camera((short) 1404);

	GPSDevice gps(GPS_ADDRESS, GPS_PORT);

	PhotoHandler handler;
	
	while(gps.data->status != 1)
		gps.read_data();
	
		if(gps.inSurface()){
			camera.takePicture();
			foto = camera.getImage();
			handler.stampCoordinates(foto,gps.data->fix.latitude, gps.data->fix.longitude);
 		}
		//sleep(3);
	

return 0;


}
