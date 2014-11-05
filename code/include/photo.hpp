/*
*	Photo header File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#ifndef __PHOTO_HPP_INCLUDED__
#define __PHOTO_HPP_INCLUDED__

#include <stdio.h>

class Photo
{		
	public:
		const char *filepath;
		short id;
		int  latitude;
		int longitude;
		short altitude;
		
		Photo(const char*,short);
		Photo(short,int,int); //Constructor parameters: ID, latitude and longitude.
		//Photo(short,int,int, FILE*); //Constructor parameters: ID, latitude and longitude, file data.
		//setData(FILE*);
};

#endif

