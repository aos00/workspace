/*
*	GoPro Class Source File.
*	@Author: ANDERSON OLIVEIRA SOUSA
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#include <iostream>
#include <assert.h>
#include "gopro.hpp"


const char * TAKE_PIC_URL = "10.5.5.9/bacpac/SH?t=goprohero&p=%01";
const string GET_IMG_URL = "10.5.5.9:8080/DCIM/100GOPRO/GOPR";
const string IMAGES_PATH = "/home/pi/raspi_local_repo/code/img/GOPR";
//const char * GET_IMG_URL = "http://upload.wikimedia.org/wikipedia/commons/3/36/Hopetoun_falls.jpg";

using namespace std;

GoPro::GoPro(short id)
{
	PHOTO_ID = id;
}

void GoPro::takePicture()
{
	curl = curl_easy_init();
	assert(curl != NULL);
   	curl_easy_setopt(curl, CURLOPT_URL, TAKE_PIC_URL);
	curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	res = curl_easy_perform(curl);

	if(res != CURLE_OK)
	      fprintf(stderr, "ERRO - takePicture(): curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

	curl_easy_cleanup(curl);
	PHOTO_ID++;
}

/*Atualmente usa-se a funcao default
size_t GoPro::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	cout << "Size: " << size << endl;
	cout << "nmemb: " << nmemb << endl;
	cout << "size*nmemb: " << size*nmemb << endl;
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	cout << "Written: " << written << endl;
	return written;
}
* */

Photo * GoPro::getImage(){ return getImage(PHOTO_ID); }

Photo * GoPro::getImage(short ID)
{
	char num[10];
	sprintf(num, "%hu", ID);

	string url_tmp = GET_IMG_URL + num + ".JPG";
	const char * url = url_tmp.c_str();
	cout << "url: " << url << endl;

	string fn_tmp = IMAGES_PATH + num + ".JPG";
	const char * filename = fn_tmp.c_str();
	cout << "filename: " << filename << endl;

	curl = curl_easy_init();
	assert(curl != NULL);

	fp = fopen(filename, "wb");
	assert(fp != NULL);
	curl_easy_setopt(curl, CURLOPT_URL, url);
	assert(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL) == CURLE_OK);
	//assert(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &GoPro::write_data	) == CURLE_OK);
	assert(curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp) == CURLE_OK);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	res = curl_easy_perform(curl);
	/* always cleanup */
	curl_easy_cleanup(curl);
	fclose(fp);
	
	Photo pht(filename,ID);
	
	return &pht;

}

short GoPro::getID() { return PHOTO_ID; }

	


