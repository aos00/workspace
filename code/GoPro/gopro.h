/*
*	GoPro Header File
*	@Author: ANDERSON OLIVEIRA SOUSA.
*	@Date: October, 2014.
*/

#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <stdio.h>

class GoPro
{
	private:
		CURL *curl;
		CURLcode res;
		FILE *fp;
		short PHOTO_ID;
		
		size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
		
	public:
		
	
		GoPro(short);
		void takePicture(); //Bater uma foto
		void getImage(short); //Download da imagem localizada no servidor da camera
};
