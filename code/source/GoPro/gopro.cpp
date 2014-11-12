/*
*	GoPro Class Source File.
*	@Author: ANDERSON OLIVEIRA SOUSA
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#define __DEBUG__

#include <iostream>
#include <fstream>
#include <iostream>
#include <assert.h>
#include <iomanip>
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

void GoPro::takePicture(const location *loc)
{

    #ifdef __HAS_CAMERA__
	curl = curl_easy_init();
	assert(curl != NULL);

	if(curl_easy_setopt(curl, CURLOPT_URL, TAKE_PIC_URL) !=  CURLE_OK) perror("Take picture: curl set url opt error !");
	if(curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L) != CURLE_OK) perror("takePicture: curl set write function opt error !");

	#ifdef __DEBUG__d
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	#endif

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){

	      fprintf(stderr, "ERRO - takePicture(): curl_easy_perform() failed: %s  ERRO NUMERO: %i\n", curl_easy_strerror(res), res);
	}
	//ERRO numero 7 quando o raspi nao esta conectado no GoPro (Network unreachable)

	curl_easy_cleanup(curl);

	#endif

    cout << " aqui!" << endl;
	photos.push_back(Photo(PHOTO_ID, loc, false));

	PHOTO_ID++;
}

Photo * GoPro::downloadImage(){ return downloadImage(PHOTO_ID); }

Photo * GoPro::downloadImage(short ID)
{
	char num[10];
	sprintf(num, "%hu", ID);

	string url_tmp = GET_IMG_URL + num + ".JPG";
	const char * url = url_tmp.c_str();

	string fn_tmp = IMAGES_PATH + num + ".JPG";
	const char * filename = fn_tmp.c_str();

	#ifdef __DEBUG__
	printf("url: %s\n", url);
	printf("filename: %s\n", filename);
	#endif

	curl = curl_easy_init();
	if(curl == NULL) perror("downloadImage: Curl initialization error !");

	fp = fopen(filename, "wb");
	if(fp == NULL) perror("downloadImage: open file error !");

	if(curl_easy_setopt(curl, CURLOPT_URL, url) !=  CURLE_OK) perror("downloadImage: curl set url opt error !");
	if(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL) != CURLE_OK) perror("downloadImage: curl set write function opt error !");
	if(curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp) != CURLE_OK) perror("downloadImage: curl set write data opt error !");
	if(curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L) != CURLE_OK) perror("downloadImage: curl set fail error opt error !");

	#ifdef __DEBUG__
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	#endif

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){

	      fprintf(stderr, "ERRO - downloadImage(): curl_easy_perform() failed: %s  ERRO NUMERO: %i\n", curl_easy_strerror(res), res);
	}
	//Erro numero 22 quando nao foi encontrado o arquivo no servidor web da gopro

	curl_easy_cleanup(curl);
	fclose(fp);

	return NULL;
}

short GoPro::getID() { return PHOTO_ID; }

bool GoPro::writePhotoRecords(){
    ofstream photoInfo("./img/photo");

    if(!photoInfo.is_open())
        return -1;

    #ifdef __DEBUG__
    printf("Writing photo information to file...\n");
    #endif

    for(vector<Photo>::iterator it = photos.begin(); it != photos.end(); it++){
		printf("%i,%f,%f,%f,%s\n",it->id,it->loc.coordinate.latitude,it->loc.coordinate.longitude,it->loc.altitude,it->loc.date.c_str());
        photoInfo << std::setprecision(6) << it->id << "," << it->loc.coordinate.latitude << "," << it->loc.coordinate.longitude << "," << it->loc.altitude << "," << it->loc.date << endl;
        //photoInfo << "\n" << endl;
    }

    photoInfo.close();
    return true;

}
