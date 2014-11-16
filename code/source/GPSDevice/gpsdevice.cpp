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

#define __DEBUG__

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "gpsdevice.hpp"

using namespace std;


void GPSDevice::init(const char *address, const int port)
{
	printf("##INIT GPSDevice: Starting GPS...\n ##GPSDevice: Checking if processor is available...\n");
	if(system(NULL)){
		printf("##GPSDevice: Processor available! Executing commmand to start gpsd\n");
		if(system("sudo gpsd /dev/ttyAMA0 -F /var/run/gpsd.sock") == 0){
			printf("##GPSDevice: GPSD Daemon started...\n");
		}else{
			throw("##GPSDevice: Error starting GPSD!\n");
		}
	}else{
		throw("##GPSDevice: Processor not available!\n");
	}

	gps_receiver = new gpsmm(address, DEFAULT_GPSD_PORT);

	printf("##GPSDevice: Checking if data is available:\n");
	if (gps_receiver->stream(WATCH_ENABLE|WATCH_JSON) == NULL)
        throw("##GPSDevice: GPSD daemon is not running!\n");
    else
		printf("##GPSDevice: GPSD daemon is running!\n");
		
	printf("##INIT GPSDevice: GPS started successfully...\n");
}


int GPSDevice::read_data()
{
	if(!gps_receiver->waiting(5000000)){
		throw("##GPSDevice readd_data(): No data available to the client, is the gps module connected to UART?\n" );
		return 0;
	}

	if((data = gps_receiver->read()) == NULL){
		throw("##GPSDevice readd_data(): Erro ao ler os dados do gps\n");
		return 0;
	}



	if(!(std::isnan(data->fix.latitude) || std::isnan(data->fix.longitude))){
		current_location.coordinate.latitude = data->fix.latitude;
        current_location.coordinate.longitude = data->fix.longitude;

		#ifdef __DEBUG__
		printf("##GPSDevice readd_data(): Status: %i\n",data->status);
		printf("##GPSDevice readd_data(): Mode: %i\n",data->fix.mode);
		printf("##GPSDevice readd_data(): Latitude: %f, Longitude: %f, Altitude: %f \n",data->fix.latitude, data->fix.longitude, data->fix.altitude);
		cout << "##GPSDevice readd_data() Data: " << unix_to_iso8601(data->fix.time, scr, sizeof(scr)) << endl;
		#endif

		return 1;
	}else{
		throw("##GPSDevice readd_data(): Latitude or Longitude is NaN\n");
		return 0;
	}
}

void GPSDevice::setLocation(){
    current_location.coordinate.latitude = data->fix.latitude;
    current_location.coordinate.longitude = data->fix.longitude;
    current_location.altitude = data->fix.altitude;
    current_location.date = string(unix_to_iso8601(data->fix.time, scr, sizeof(scr)));
}
