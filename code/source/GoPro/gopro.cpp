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

using namespace std;

const char * CHECK_AVAILABILITY_URL = "10.5.5.9:8080";
const char * TAKE_PIC_URL = "10.5.5.9/bacpac/SH?t=goprohero&p=%01";
const char * SHUTTER_URL = "10.5.5.9/bacpac/SH?t=goprohero&p=%01";
const char * STOP_URL = "10.5.5.9/bacpac/SH?t=goprohero&p=%00";
const char * VIDEO_MODE_URL = "10.5.5.9/camera/CM?t=goprohero&p=%00";
const char * PHOTO_MODE_URL = "10.5.5.9/camera/CM?t=goprohero&p=%01";
const char * BURST_MODE_URL = "10.5.5.9/camera/CM?t=goprohero&p=%02";
const string GET_IMG_URL = "http://10.5.5.9:8080/DCIM/100GOPRO/GOPR";
const string IMAGES_PATH = "/home/pi/raspi_local_repo/code/img/GOPR";


int GoPro::init(const short id, const short cammode)
{
	#ifdef __DEBUG__
	printf("##GoPro Initializing camera ... \n");
	#endif

	CURL *curl;
	CURLcode res;
	
	curl = curl_easy_init();
	
	
	if(curl == NULL){
		throw "#GoPro: INIT cannot initialize curl! \n";
	}
	
	if(curl_easy_setopt(curl, CURLOPT_URL, CHECK_AVAILABILITY_URL) !=  CURLE_OK) throw ("Take picture: curl set url opt error !");
	if(curl_easy_setopt(curl, CURLOPT_NOBODY, true) !=  CURLE_OK) throw("Take picture: curl set url opt error !");
	if(curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true) !=  CURLE_OK) throw("Take picture: curl set url opt error !");
	if(curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10) != CURLE_OK) throw("takePicture: curl set timeout opt error !");
    	if(curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10) != CURLE_OK) throw("takePicture: curl set timeout opt error !");

	#ifdef __DEBUG__
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	#endif

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		throw (res);
	}
	//ERRO numero 7 quando o raspi nao esta conectado no GoPro (Network unreachable)
	curl_easy_cleanup(curl);
	PHOTO_ID = id;

	#ifdef __DEBUG__
	printf("##GoPro init(): camera initialized successfully!\n");
	#endif

	//if(setCameraMode(cammode) == STATUS_ERROR){
	//	STATUS_CAMERA = STATUS_ERROR;
//		perror("##GoPro: init(): Cannot change camera mode\n");
	//	return STATUS_CAMERA;
	//}
	mode = cammode;
	return 1;
}


void GoPro::takePicture(const location *loc)
{
	#ifdef __DEBUG__
	printf("##GoPro: takePicture() \n");
	#endif
	
	CURL *curl;
	CURLcode res;
	
	curl = curl_easy_init();
	if(curl == NULL){
		throw "#GoPro: takePicture cannot initialize curl! \n";
	}

	if(curl_easy_setopt(curl, CURLOPT_URL, TAKE_PIC_URL) !=  CURLE_OK) throw("##GoPro: takePicture(): curl set url opt error !");
	if(curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L) != CURLE_OK) throw("takePicture: curl set write function opt error !");
	if(curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10) != CURLE_OK) throw("takePicture: curl set timeout opt error !");

	#ifdef __DEBUG__
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	#endif

	res = curl_easy_perform(curl);
	
	if(res != CURLE_OK){
		throw (res);
	}
	//ERRO numero 7 quando o raspi nao esta conectado no GoPro (Network unreachable)

	curl_easy_cleanup(curl);

	char num[10];
	sprintf(num, "%hu", PHOTO_ID);
    	string filepath = IMAGES_PATH + num + string(".JPG");

	if( loc != NULL)
		photos.push_back(Photo(PHOTO_ID, loc, filepath, false));
	else
		photos.push_back(Photo(PHOTO_ID, NULL, filepath, false));

	PHOTO_ID++;
}

bool GoPro::downloadImage(short ID)
{
	#ifdef __DEBUG__
	printf("##GoPro: download image() id %i \n", ID);
	#endif
	char num[10];
	if(ID != 0)
		sprintf(num, "%hu", ID);
	else
		sprintf(num, "%hu", PHOTO_ID);

	string url_tmp = GET_IMG_URL + num + string(".JPG");
	const char * url = url_tmp.c_str();

	string fn_tmp = IMAGES_PATH + num + string(".JPG");
	const char * filename = fn_tmp.c_str();

	#ifdef __DEBUG__
	printf("url: %s\n", url);
	printf("filename: %s\n", filename);
	#endif
	
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(curl == NULL) throw("downloadImage: Curl initialization error !");

	fp = fopen(filename, "wb");
	if(fp == NULL) throw("downloadImage: open file error !");

	if(curl_easy_setopt(curl, CURLOPT_URL, url) !=  CURLE_OK) throw("downloadImage: curl set url opt error !");
	if(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL) != CURLE_OK) throw("downloadImage: curl set write function opt error !");
	if(curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp) != CURLE_OK) throw("downloadImage: curl set write data opt error !");
	if(curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L) != CURLE_OK) throw("downloadImage: curl set fail error opt error !");
	if(curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10) != CURLE_OK) throw("##GoPro: download image: curl set timeout opt error !");

	#ifdef __DEBUG__
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	#endif

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){

	      throw(res);
	}
	//Erro numero 22 quando nao foi encontrado o arquivo no servidor web da gopro

	curl_easy_cleanup(curl);
	fclose(fp);

	return true;
}

bool GoPro::writePhotoRecords(){
	printf("Writing records");
    ofstream photoInfo("./img/photo");

    if(!photoInfo.is_open())
    {
		throw("##GoPro: writePhotoRecords(): cannot open the file photo");
	}

    #ifdef __DEBUG__
    printf("Writing photo information to file...\n");
    #endif

    for(vector<Photo>::iterator it = photos.begin(); it != photos.end(); it++){
		printf("%i,%f,%f,%f,%s\n",it->id,it->loc.coordinate.latitude,it->loc.coordinate.longitude,it->loc.altitude,it->loc.date.c_str());
        photoInfo << std::setprecision(8) << it->id << "," << it->loc.coordinate.latitude << "," << it->loc.coordinate.longitude << "," << it->loc.altitude << "," << it->loc.date << endl;
        //photoInfo << "\n" << endl;
    }

    photoInfo.close();
    return true;

}

bool GoPro::setNextMode(){
	if(mode == PHOTO_MODE)
		return setCameraMode(VIDEO_MODE);
	else if(mode == VIDEO_MODE)
		return setCameraMode(BURST_MODE);
	else if(mode == BURST_MODE)
		return setCameraMode(PHOTO_MODE);
	else
		return false;
}

bool GoPro::setCameraMode(const short new_mode){

    #ifdef __DEBUG__
    printf("##GoPro: Switching camera mode: %i\n", mode);
    #endif

    if(new_mode == VIDEO_MODE){
        if(executeCommand(VIDEO_MODE_URL)){
			mode = new_mode;
			submode = RECORDING_OFF;
		}else{
			return false;
		}
    }else if(new_mode == PHOTO_MODE){
         if(executeCommand(PHOTO_MODE_URL))
			mode = new_mode;
		else
			return false;
    }else if(new_mode == BURST_MODE){
        if(executeCommand(BURST_MODE_URL))
			mode = new_mode;
		else
			return false;
    }else{
        throw("set camera mode: mode unrecognized!\n");
        return false;
    }
    return true;
}


bool GoPro::executeCommand(const char* URL)
{
	#ifdef __DEBUG__
    printf("##GoPro: Executing command %s\n", URL);
    #endif
    
    CURL *curl;
	CURLcode res;
	int status = false;
	
    curl = curl_easy_init();
    
    if(curl == NULL){
		throw("##GoPro: Executing command: error initializing curl!\n");
		return false;
	}

	status = curl_easy_setopt(curl, CURLOPT_URL, URL);
	status = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    status = curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    
    #ifdef __DEBUG__
	status = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	#endif
    
    if(status != CURLE_OK){
        throw("##GoPro: Executing command: curl set url opt error\n");
        return false;
    }	

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){

	     fprintf(stderr, "##GoPro: Executing command: curl_easy_perform() failed: %s  Error number: %i\n", curl_easy_strerror(res), res);
		return false;
	}
	//ERRO numero 7 quando o raspi nao esta conectado no GoPro (Network unreachable)

	curl_easy_cleanup(curl);
	return true;
}

bool GoPro::pressShutter(){
	#ifdef __DEBUG__
    printf("##GoPro: press shutter\n");
    #endif
    
    if(mode == VIDEO_MODE && submode == RECORDING_OFF){  
		if(executeCommand(SHUTTER_URL))
			submode = RECORDING_ON;
		else
			return false;
	}else if (mode == VIDEO_MODE && submode == RECORDING_ON){  
		if(executeCommand(STOP_URL))
			submode = RECORDING_OFF;
		else
			return false;
	}else if(mode == PHOTO_MODE){
		takePicture(NULL);	
	}else{
		return executeCommand(SHUTTER_URL);
	}
	return true;
}

bool GoPro::downloadAllPhotos(void){
	
	#ifdef __DEBUG__
	printf("##GoPro: downloadAllPhotos(): downloading photos...\n");
	#endif
	
	if(photos.size() == 0){
	#ifdef __DEBUG__
	printf("##GoPro: downloadAllPhotos(): no picture was taken!\n");
	#endif
	return false;
	}
	
	
	for(vector<Photo>::iterator it = photos.begin(); it != photos.end(); it++){
		try{
			if(downloadImage(it->id))
				it->downloaded = true;
			}catch(const char * msg){
				throw msg;
			}catch(CURLcode res){
				throw res;
			}
    }
    
    return true;
	
}


vector<Photo>& GoPro::getPhotos(void){
	return photos;
}
