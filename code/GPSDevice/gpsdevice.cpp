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
	for(;;){
		struct gps_data_t * data;
		
		if(!gps_receiver->waiting(500))
			continue;
		
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

/*
GPSDevice::open_conn()
{
	if(gps_open(server,port,&GPS_DATA) != 0)
	{
		(void) fprintf(stderr, "gps_open: gpsd nao esta rodando ou houve um network error! %d, %s\n", errno, gps_errstr(errno));
		exit(EXIT_FAILURE);
	}else{
		wait_powering_on();
	}
	
	(void) gps_stream(&GPS_DATA, WATCH_ENABLE | WATCH_JSON, NULL);	
}

GPSDevice::read_data()
{
	while(1){
		if(gps_waiting(&GPS_DATA, 500)){ //gps_waiting checa se exite novos dados vindos do deamon. O segundo argumento é o tempo máximo de espera em microsegundos
			errno = 0;
			if(gps_read(&GPS_DATA) == -1){
				(void) fprintf(stderr, "gps_read: erro no socket: %d, %s\n", errno, gps_errstr(errno));
			}else{
				printf("\nStatus: %i", GPS_DATA.status);
				printf("\nLatitude: %f , Longitude: %f", fabs(GPS_DATA.fix.longitude), fabs(GPS_DATA.fix.latitude));
				printf("\nData: %s", unix_to_iso8601(GPS_DATA.fix.time, scr, sizeof(scr)));

			}
		}
	}
	
}
*/

//Thread will sleep for 5 seconds and try o open the connection again
/*
GPSDevice::wait_powering_on()
{
	cout << "GPSDevice error." << endl;
	cout << "GPSDevice will sleep for 5 seconds..." << endl;
	sleep(5);
	open_conn();	
}
*/
