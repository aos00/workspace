/*
*	GPS Device Header File.
*	@Author: ANDERSON OLIVEIRA SOUSA
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

//#include <gps.h>
#include <libgpsmm.h>
#include <iostream>
//#include <stdlib>



class GPSDevice
{
	private:
		int latitude;
		int longitude;
		short altitude;
		static char scr[128];
		
		gpsmm * gps_receiver;
		
	public:
		GPSDevice(const char*);
		void read_data();
		//void wait_powering_on();
		//void update();
		//int getLatitude();
		//int getLongitude();
		//short getAltitude();
	
};
