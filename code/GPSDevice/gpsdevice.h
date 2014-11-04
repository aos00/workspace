/*
*	GPS Device Header File.
*	@Author: ANDERSON OLIVEIRA SOUSA
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#include <libgpsmm.h>
#include <iostream>

struct target {
	double latitude;
	double longitude;
};


class GPSDevice
{
	private:
	
	/*Target points which define the earth surface*/
		target ptA;
		target ptB;
		target ptC;
		target ptD;
		
		gpsmm * gps_receiver;
		
	public:
		GPSDevice(const char*);
		void setTarget(target,target,target,target);
		void read_data();
		void setStatus(int);
		bool inSurface();
	
};
