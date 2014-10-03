#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, const char** argv){
	//CvCapture* capture = 0;
	//Mat frame;
	//int linha, coluna;

	
	cout << "In capure ..." << endl;
	waitKey(0);

	//VideoCapture frame2;
	//

	////frame2.open(CV_CAP_ANY);
	//frame2.set(CV_CAP_PROP_FRAME_WIDTH, 80);
	//frame2.set(CV_CAP_PROP_FRAME_HEIGHT, 60);

	////capture = cvCaptureFromCAM(CV_CAP_ANY);

	//if (!frame2.isOpened()){
	//	cout << "Nenhuma camera detectada" << endl;

	//}else{
	//	cout << "In capure ..." << endl;
	//	
	//	for (;;){
	//	
	//		frame2 >> frame;

	//		//Criar Matrizes de float
	//		Mat soma(frame.size(), CV_32FC1);
	//		Mat subtracao(frame.size(), CV_32FC1);
	//		Mat red(frame.size(), CV_32FC1);
	//		Mat blue(frame.size(), CV_32FC1);
	//		Mat ndvi(frame.size(), CV_32FC1);
	//		Mat ndvi2(frame.size(), CV_32FC1);
	//		Mat ndvi3(frame.size(), CV_32FC1);
	//				
	//		//Mat output(frame.size(), frame.type());

	//		vector<Mat> rgb;		
	//		
	//		split(frame, rgb);		
	//		
	//		//Converter valores RGB para float
	//		rgb[2].convertTo(red, CV_32F);
	//		rgb[0].convertTo(blue, CV_32F);

	//		//Adicionar e subtrair os canais
	//		add(rgb[2], rgb[0], soma);
	//		subtract(rgb[2], rgb[0], subtracao);

	//		//Dividir os canais
	//		divide(subtracao, soma, ndvi);

	//		multiply(ndvi, 127, ndvi2);

	//		add(ndvi2, 1, ndvi3);
	//		
	//		//imshow("subt", sub);
	//		//cout << "ndvi" << endl << ndvi << endl << endl;

	//		//imshow("soma", ndvi3);
	//		cout << "Blue = " << "soma="<< endl << soma << endl;
	//		/*cout << "Red = " <<  (int) rgb[2].at<uchar>(100,100) << "; ";
	//		cout << "Blue = " << (int) rgb[0].at<uchar>(100, 100) << "; " << endl;*/
	//		//cout << "Soma = " << soma.at<float>(100,100) << "; ";
	//		/*cout << "Subtracao = " << (int)subtracao.at<double>(100, 100) << "; " << endl;
	//		cout << "Divisao = " << (int)ndvi.at<double>(100, 100) << "; ";
	//		cout << "offset = " << (int)ndvi3.at<double>(100, 100) << "; " << endl;*/


	//		//while(1);

	//		

	//		//imshow("NDVI", ((rgb[2] - rgb[0]) / (rgb[2] + rgb[0])));

	//		

	//		if (waitKey(10) >= 0)
	//			break;
	//	}
	//	waitKey(0);

	//}
	//cvReleaseCapture(&capture);
	//cvDestroyWindow("NDVI");
	return 0;
}
