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
#include "common.hpp"


class GPSDevice
{
	private:
		gpsmm * gps_receiver;


	public:
		struct gps_data_t * data;
		struct location current_location;
		char scr[100];

		void init(const char*, const int);
		void setLocation();
		int read_data();


};


#endif
