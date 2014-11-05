#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "photohandler.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

void PhotoHandler::stampCoordinates(Photo *ph, double latitude, double longitude)
{
	
	Mat image;
	char lat[10];
	char longt[10];
	sprintf(lat, "%f", latitude);
	sprintf(longt, "%f", longitude);
	string label = string("IMAGE INFO: Latitude:") + lat + string(", Longitude: ") + longt;
	
	image = imread(ph->filepath, CV_LOAD_IMAGE_COLOR);
	
	if(image.data){
		putText(image, label, Point(30,30), FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,255)); 
		imshow("Foto", image);
	}else{
		cout << "Erro ao carregar a imagem " << endl;
	}
	
	waitKey(0);	
	
}
