#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "photohandler.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

void PhotoHandler::stampCoordinates(const Photo &ph)
{

	Mat image;
	char lat[10];
	char longt[10];
	sprintf(lat, "%f", ph.loc.coordinate.latitude);
	sprintf(longt, "%f", ph.loc.coordinate.longitude);
	string label = string("PHOTO INFO: Latitude: ") + lat + string(", Longitude: ") + longt;

	image = imread(ph.filepath, CV_LOAD_IMAGE_COLOR);

	if(image.data){
		putText(image, label, Point(50,50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255), 4, 5, false);
		imshow("Foto", image);
	}else{
		throw("Erro ao carregar a imagem ");
	}

	waitKey(0);

}


void PhotoHandler::stampCoordinates(const vector<Photo> &photos){
	for(unsigned int i = 0; i < photos.size(); i++){
		stampCoordinates(photos[i]);
    }
}
