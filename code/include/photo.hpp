/*
*	Photo header File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#ifndef __PHOTO_HPP_INCLUDED__
#define __PHOTO_HPP_INCLUDED__

#include <stdio.h>
#include <string>
#include "common.hpp"

using namespace std;

class Photo
{
	public:
		const char *filepath;
		short id;
		struct location loc;
		bool downloaded;

		Photo(const short,const location*,bool);
};

#endif

