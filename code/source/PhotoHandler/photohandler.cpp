
#define __DEBUG__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "photohandler.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

void PhotoHandler::stampCoordinates(const Photo &ph)
{
	
	#ifdef __DEBUG__
	printf("##PhotoHandler stampCoordinates: Stamping coordinates. Photo id %i\n", ph.id);
	#endif

	Mat image;
	char lat[10];
	char longt[10];
	char alt[10];
	sprintf(lat, "%f", ph.loc.coordinate.latitude);
	sprintf(longt, "%f", ph.loc.coordinate.longitude);
	sprintf(alt, "%f", ph.loc.altitude);
	string label = string("PHOTO INFO: Latitude: ") + lat + string(", Longitude: ") + longt + string(", Altitude: ") + alt;
	
	printf("filepath %s\n\n\n",ph.filepath.c_str());
	
	image = imread(ph.filepath, CV_LOAD_IMAGE_COLOR);

	if(image.data){
		putText(image, label, Point(50,50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,255), 4, 5, false);
		vector<int> params;
		params.push_back(CV_IMWRITE_JPEG_QUALITY);
		params.push_back(100);
		imwrite(ph.filepath, image, params);
		//imshow("Foto", image);
		printf("feito\n\n\n");
	}else{
		printf("erro\n\n\n");
		throw("Erro ao carregar a imagem ");
	}

	//waitKey(0);

}


void PhotoHandler::stampCoordinates(const vector<Photo> &photos){
	
	#ifdef __DEBUG__
	printf("##PhotoHandler stampCoordinates(vector): Stamping coordinates.\n");
	#endif
	
	for(unsigned int i = 0; i < photos.size(); i++){
		try{
			stampCoordinates(photos[i]);
		}catch (const char * msg){
			throw msg;
		}
    }
}
