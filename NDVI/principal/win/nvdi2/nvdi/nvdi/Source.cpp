//#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


//-----------INICIANDO CAPTURA CAMERA -------------
//int main(int argc, const char** argv){
//
//	Mat frame;
//
//	VideoCapture capture(0);
//	//capture.set()
//
//	if (!capture.isOpened()){
//		cout << "Nenhuma camera detectada" << endl;
//		return -1;
//	}
//
//	for (;;){
//		capture >> frame;
//		imshow("Camera", frame);
//		if (waitKey(10) >= 0)
//			break;
//	}
//
//	capture.release();
//	return 0;
//}

//---------------MANIPULANDO MATRIZES DE CONSTANTES ----------
//int main(int argc, const char** argv){
//
//	Mat red(10, 10, CV_32F, Scalar(255));
//	Mat blue(red.size(), CV_32F, Scalar(255));
//
//	Mat soma(red.size(), CV_32F);
//	Mat divisao(red.size(), CV_32F);
//	Mat subtracao(red.size(), CV_32F);
//
//	add(red, blue, soma, noArray(), CV_32F);
//	cout << "Soma" << soma << endl << endl;
//	subtract(red, blue, red, noArray(), CV_32F);
//	cout << "Subtracao" << red << endl << endl;
//	divide(red, soma, red, 1, CV_32F);
//	cout << "Divisao" << red << endl << endl;
//
//	add(red, 1, red, noArray(), CV_32F);
//	cout << "adicao" << red << endl << endl;
//	multiply(red, 127, red, 1, CV_32F);
//	cout << "tela" << red << endl << endl;
//	imshow("matriz", divisao);
//
//	system("pause");
//
//	return 0;
//}

	//------------------------------------------------------------------------------
/**			LER IMAGEM DO DISCO

	int main(int argc, const char** argv){

	Mat imagem = Mat::zeros(48, 48, CV_32FC3);
	imagem = imread("imagem.jpg", 1);

	vector<Mat> rgb;
	split(imagem, rgb);

	Mat red(rgb[2].size(), CV_32F);
	Mat blue(rgb[0].size(), CV_32F);
	Mat soma(red.size(), CV_32F);
	Mat subtracao(red.size(), CV_32F);
	Mat ndvi(red.size(), CV_32F);

	add(rgb[2], rgb[0], soma, noArray(), CV_32F);
	subtract(rgb[2], rgb[0], subtracao, noArray(), CV_32F);

	divide(subtracao, soma, ndvi, 1, CV_32F);

	cout << "Imagem" << ndvi << endl << endl;
	system("pause");
	return 0;
	}
	*/


	//----CALCULO DO NDVI COM WEBCAM-----------------------

	int main(int argc, const char** argv){
		
		Mat frame;
	
		VideoCapture capture(CV_CAP_ANY);
	
		if (!capture.isOpened()){
				cout << "Nenhuma camera detectada" << endl;
	
			}else{
				cout << "In capure ..." << endl;
				
				for (;;){
				
					capture >> frame;
	
					//Criar Matrizes de float
					Mat soma(frame.size(), CV_32F);
					Mat subtracao(frame.size(), CV_32F);
					Mat ndvi(frame.size(), CV_32F);
	
					vector<Mat> rgb;		
					split(frame, rgb);		
					
					//Adicionar e subtrair os canais
					add(rgb[2], rgb[0], soma, noArray(), CV_32F);
					subtract(rgb[2], rgb[0], subtracao, noArray(), CV_32F);
	
					//Dividir os canais
					divide(subtracao, soma, ndvi, 1, CV_32F);
					add(ndvi, 1, ndvi, noArray(), CV_32F);
					multiply(ndvi, 127, ndvi, 1.0, CV_32F);

					cvtColor(ndvi, ndvi, CV_GRAY2RGB, 3);

					imshow("NDVI", ndvi);
	
					cout << "ndvi " << ndvi.at<float>(300,200) << endl;
					cout << "ndvi " << ndvi.at<float>(50, 50) << endl;
					cout << "ndvi " << ndvi.at<float>(200, 300) << endl;
	
					//system("pause");

					if (waitKey(10) >= 0)
						break;
				}
				waitKey(0);
	
			}
			//cvReleaseCapture(&capture);
			//cvDestroyWindow("NDVI");
	
	
		return 0;
	}


//-----------------------------------------------------------------------------------

//int main(int argc, const char** argv){
//	VideoCapture capture;
//	Mat frame;
//
//	capture.open(CV_CAP_ANY);
//
//	if (!capture.isOpened()){
//		cout << "Nenhuma camera detectada" << endl;
//
//	}
//	else{
//		cout << "In capure ..." << endl;
//
//		for (;;){
//
//			capture >> frame;
//			imshow("NDVI", frame);
//
//			if (waitKey(10) >= 0)
//				break;
//		}
//			waitKey(0);
//		
//	}
//	return 0;
//}