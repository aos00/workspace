#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, const char** argv){
	CvCapture* capture = 0;
	Mat frame;
	


	VideoCapture frame2;
	

	frame2.open(CV_CAP_ANY);
	frame2.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	frame2.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

	//capture = cvCaptureFromCAM(CV_CAP_ANY);
	
	//capture = cvCaptureFromCAM(1);

	if (!frame2.isOpened()){
		cout << "Nenhuma camera detectada" << endl;

	}else{
		cout << "In capure ..." << endl;
		
		//for (;;){
		
			frame2 >> frame;
					
			Mat output(frame.size(), frame.type());

			vector<Mat> rgb;
			
			
			split(frame, rgb);

			
			//Criar Matrizes de float
			Mat soma(frame.size(), CV_32F);
			Mat subtracao(frame.size(), CV_32F);
			Mat red(frame.size(), CV_32F);
			Mat blue(frame.size(), CV_32F);
			Mat nvdi(frame.size(), CV_32F);
			
			//Converter valores RGB para float
			rgb[2].convertTo(red, CV_32F);
			rgb[0].convertTo(blue, CV_32F);

			//Adicionar e subtrair os canais
			add(red, blue, soma);
			subtract(red, blue, subtracao);

			//Dividir os canais
			divide(subtracao, soma);
			

			//imshow("subt", sub);
			//cout << "sub= " << endl << red/blue << endl << endl;

			imshow("soma", frame);

			while(1);

			

			//imshow("NDVI", ((rgb[2] - rgb[0]) / (rgb[2] + rgb[0])));

			

			//if (waitKey(10) >= 0)
				//break;
		//}
		waitKey(0);

	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("NDVI");
	return 0;
}
