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

#define RECORDING_ON 1
#define RECORDING_OFF 0


using namespace std;



class GoPro
{
	private:
			
		FILE *fp;
		short PHOTO_ID;
		vector<Photo> photos;
		unsigned short mode;
		unsigned short submode;
		bool executeCommand(const char*);

	public:
		int init(const short, const short); //Initializing camera
		void takePicture(const location*); //Bater uma foto
		bool downloadImage(short); //Download da imagem localizada no servidor da camera a partir do id
		bool downloadAllPhotos(void);
        bool writePhotoRecords(void);
        bool setCameraMode(const short);
        bool pressShutter(void);
        bool setNextMode(void);
        vector<Photo>& getPhotos(void);
};

#endif
