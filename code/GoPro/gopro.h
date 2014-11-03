/*
*	GoPro Header File
*	@Author: ANDERSON OLIVEIRA SOUSA.
*	@Date: October, 2014.
*/

#include <stdio.h>
#include <curl/curl.h>

class GoPro
{
	private:
		CURL *curl;
		CURLcode res;
		
	public:
		void takePicture(); //Bater uma foto
		void getImage(short); //Download da imagem localizada no servidor da camera
};
