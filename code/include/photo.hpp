/*
*	Photo header File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#ifndef __PHOTO_H_INCLUDED__
#define __PHOTO_H_INCLUDED__

#include <stdio.h>

class Photo
{
	private:
		char *filename;
		short id;
		int  latitude;
		int longitude;
		short altitude;
		FILE *data;
		
	public:
		Photo(short,int,int); //Constructor parameters: ID, latitude and longitude.
		//Photo(short,int,int, FILE*); //Constructor parameters: ID, latitude and longitude, file data.
		//setData(FILE*);
};

#endif

