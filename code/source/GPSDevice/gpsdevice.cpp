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

//#define __DEBUG__

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "gpsdevice.hpp"

using namespace std;


GPSDevice::GPSDevice(const char *address, const int port)
{
	printf("Starting GPS...\nChecking if processor is available...\n");
	if(system(NULL)){
		printf("Processor available! Executing commmand to gpsd start\n");
		if(system("sudo gpsd /dev/ttyAMA0 -F /var/run/gpsd.sock") == 0){
			printf("GPSD Daemon started...");
		}else{
			perror("Error starting GPSD!\n");
		}
	}else{ 
		perror("Processor not available!\n");
	}
	
	gps_receiver = new gpsmm(address, DEFAULT_GPSD_PORT);
	
	printf("Checking if data is available:\n");
	if (gps_receiver->stream(WATCH_ENABLE|WATCH_JSON) == NULL)
        perror("GPSD daemon is not running!\n");
    else
		printf("GPSD daemon is running!\n");		
}


int GPSDevice::read_data()
{	
	if(!gps_receiver->waiting(500000)){
		//gps_clear_fix();
		#ifdef __DEBUG__
		printf(" No data available to the client, is the gps module connected to UART?\n" );
		#endif
		return 0;
	}
	
	if((data = gps_receiver->read()) == NULL){
		#ifdef __DEBUG__
		printf("Erro ao ler os dados do gps\n");
		printf("3Status: %i\n",data->status);
		printf("3Mode: %i\n",data->fix.mode);
		#endif
		return 0;
	}
	

	
	if(!(std::isnan(data->fix.latitude) || std::isnan(data->fix.longitude))){
		current_coord.latitude = data->fix.latitude;
		current_coord.longitude = data->fix.longitude;
		
		#ifdef __DEBUG__
		printf("4Status: %i\n",data->status);
		printf("4Mode: %i\n",data->fix.mode);
		printf("Latitude: %f, Longitude: %f \n",data->fix.latitude, data->fix.longitude);
		cout << "Data: " << unix_to_iso8601(data->fix.time, scr, sizeof(scr)) << endl;
		#endif
		return 1;
	}else{		
		printf("Latitude or Longitude is NaN");		
		return 0;
	}
}
