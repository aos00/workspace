/*
*	GoPro Header File
*	@Author: ANDERSON OLIVEIRA SOUSA.
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#ifndef __GOPRO_HPP_INCLUDED__
#define __GOPRO_HPP_INCLUDED__

#include <photo.hpp>
#include <curl/curl.h>
#include <curl/easy.h>
#include <string>

using namespace std;

class GoPro
{
	private:
		CURL *curl;
		CURLcode res;
		FILE *fp;
		short PHOTO_ID;
//		Photo pht;

		short getID();

		//size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream); Obsoleto: usando funcao default do curl

	public:	

		GoPro(short);
		void takePicture(); //Bater uma foto
		void takePicture(double, double, double); //Bater uma foto
		Photo * getImage(short); //Download da imagem localizada no servidor da camera a partir do id
		Photo * getImage(); //Download da ultima imagem fotografada
};

#endif
