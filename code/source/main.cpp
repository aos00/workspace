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


#define GPS_PORT 2947
const string filepath = "/home/pi/raspi_local_repo/code/quadra";

using namespace std;

const char * GPS_ADDRESS = "localhost";

int main(){ //obter ID inicial da foto, latitude e longitude do alvo...

	GoPro camera((short) 1404);

	GPSDevice gps(GPS_ADDRESS, GPS_PORT);

	PhotoHandler handler;

	TargetArea area(filepath);

	camera.setCameraMode(PHOTO_MODE);

    int m=0;
	while(1){
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
	}
	camera.writePhotoRecords();

	return 0;
}
