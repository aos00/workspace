/*
*	GoPro Header File
*	@Author: ANDERSON OLIVEIRA SOUSA.
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

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

		short getID();

		Photo * setPhoto(char*);

		//size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream); Obsoleto: usando funcao default do curl

	public:	

		GoPro(short);
		void takePicture(); //Bater uma foto
		const char* getImage(short); //Download da imagem localizada no servidor da camera a partir do id
		const char* getImage(); //Download da ultima imagem fotografada
};
