/*
*	GPS Device Header File.
*	@Author: ANDERSON OLIVEIRA SOUSA
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#ifndef __GPSDEVICE_HPP_INCLUDED__
#define __GPSDEVICE_HPP_INCLUDED__

#include <libgpsmm.h>
#include <iostream>

struct coordinates {
	double latitude;
	double longitude;
};

struct photo_info {
	coordinates coord;
	double altitude;
	char * date;
};


class GPSDevice
{
	private:
	
	/*Target points which define the earth surface*/
		struct coordinates target_ptA;
		struct coordinates target_ptB;
		struct coordinates target_ptC;
		struct coordinates target_ptD;
		
		gpsmm * gps_receiver;
		
	public:
		struct gps_data_t * data;
	
		GPSDevice(const char*, const int);
		void setTarget(coordinates,coordinates,coordinates,coordinates);
		void read_data();
		void setStatus(int);
		bool inSurface();
	
};


#endif
