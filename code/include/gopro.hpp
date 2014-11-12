/*
*	GoPro Header File
*	@Author: ANDERSON OLIVEIRA SOUSA.
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#ifndef __GOPRO_HPP_INCLUDED__
#define __GOPRO_HPP_INCLUDED__

#include <string>
#include <vector>
#include <photo.hpp>
#include <curl/curl.h>
#include <curl/easy.h>
#include "common.hpp"

using namespace std;

class GoPro
{
	private:
		CURL *curl;
		CURLcode res;
		FILE *fp;
		short PHOTO_ID;
		vector<Photo> photos;
		short getID();

	public:

		GoPro(short);
		void takePicture(); //Bater uma foto
		void takePicture(const location*); //Bater uma foto
		Photo * downloadImage(short); //Download da imagem localizada no servidor da camera a partir do id
		Photo * downloadImage(); //Download da ultima imagem fotografada
};

#endif
