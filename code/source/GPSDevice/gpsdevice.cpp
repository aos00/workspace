/*
 * gpsdevice.cpp
 * 
 * Copyright 2014  <pi@raspberrypi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 *	GPS Device Source File.
 *	@Author: ANDERSON OLIVEIRA SOUSA.
 * 	@UNIVERSIDADE FEDERAL DE GOIAS - EMC - ENGENHARIA DE COMPUTACAO
 *	@Date: October, 2014.
 */

#include <stdio.h>
#include <stdlib.h>
#include "gpsdevice.hpp"

using namespace std;


GPSDevice::GPSDevice(const char *address, const int port)
{
	printf("Starting GPS...\nChecking if processor is available...\n");
	if(system(NULL)){
		printf("Processor available! Executing commmand to gpsd start\n");
		if(system("sudo gpsd /dev/ttyAMA0 -F /var/run/gpsd.sock") == 0){
			printf("GPSD running...");
		}else{
			perror("Error starting GPSD!\n");
		}
	}else{ 
		perror("Processor not available!\n");
	}
	
	gps_receiver = new gpsmm(address, DEFAULT_GPSD_PORT);
	
	printf("Checking if GPSD is running:\n");
	if (gps_receiver->stream(WATCH_ENABLE|WATCH_JSON) == NULL)
        perror("GPSD nao esta executando!\n");
    else
		printf("GPS receiver is running!\n");
}


void GPSDevice::read_data()
{		
		if(gps_receiver->waiting(5000)){ //Return true if theres data ready for the client
		
			if((data = gps_receiver->read()) == NULL){
				printf("Erro ao ler os dados do gps\n");
				//return 1;
			} else{
				#ifdef __DEBUG__
				if(!(isnan(data->fix.latitude) || isnan(data->fix.data))){
					printf("Status: %i\n",data->status);
					printf("Latitude: %f, Longitude: %f \n",data->fix.latitude, data->fix.longitude);
					//cout << "Data: " << unix_to_iso8601(data->fix.time, scr, sizeof(scr)) << endl;
				}else{
					printf("Latitude or Longitude is NaN");
				}
				#endif
			}
	}
}

bool GPSDevice::inSurface(){return true;}

void GPSDevice::convertCoordinates(coordinates coord)
{
	try {    
      
      int zone;
      bool northp;
      double x, y;
      UTMUPS::Forward(coord.latitude, coord.longitude, zone, northp, x, y);
      string zonestr = UTMUPS::EncodeZone(zone, northp);
      
      #ifdef __DEBUG__
      printf("Zone: %s ; Latitude: %f ; Longitude: %f", zonestr, x ,y);
      //cout << fixed << setprecision(2) << zonestr << " " << x << " " << y << "\n";
      #endif
      
    }catch (const exception& e) {
    cerr << "Caught exception: " << e.what() << "\n";
    return 1;
  }
	
	
	
}


