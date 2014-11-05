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

#include "gpsdevice.h"

using namespace std;


GPSDevice::GPSDevice(const char *address)
{
	gps_receiver = new gpsmm(address, DEFAULT_GPSD_PORT);
	
	if (gps_receiver->stream(WATCH_ENABLE|WATCH_JSON) == NULL)
        cerr << "GPSD nao esta executando!" << endl;	
}


void GPSDevice::read_data()
{
		struct gps_data_t * data;
		
		if(gps_receiver->waiting(5000)){ //Return true if theres data ready for the client
		
			if((data = gps_receiver->read()) == NULL){
				cerr << "Erro ao ler os dados do gps"  << endl;
				//return 1;
			} else {
				cout << "Status: " << data->status << endl;
				cout << "Latitude: " << data->fix.latitude << ", Longitude: " << data->fix.longitude << endl;
				//cout << "Data: " << unix_to_iso8601(data->fix.time, scr, sizeof(scr)) << endl;
			}
	}
}

bool GPSDevice::inSurface(){return true;}


