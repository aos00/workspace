#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, const char** argv){
	CvCapture* capture = 0;
	Mat frame, frameCopy;
	
	capture = cvCaptureFromCAM(CV_CAP_ANY);
	
	if(!capture)
		cout << "Nenhuma camera detectada"<< endl;
	
	if(capture){
		cout << "In capure ..." << endl;
		IplImage* iplImg;
		for(;;){
			iplImg = cvQueryFrame(capture);
			frame = iplImg;
			//cout << "Canais:" << iplImg->nChannels << endl;
			
			if(frame.empty()) break;
			
			if(iplImg->origin == IPL_ORIGIN_TL)
				frame.copyTo(frameCopy);
			else
				flip(frame, frameCopy, 0);
				
			//cvShowImage("NDVI",iplImg);
			Mat output(frame.size(), frame.type());
			vector<Mat> rgb;	
			split(frame,rgb);
			imshow("NDVI",(rgb[2]-rgb[0]/(rgb[2] + rgb[0])));
			
			if(waitKey(10) >= 0)
				break;
		}
		waitKey(0);
		
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("NDVI");
	return 0;
}
