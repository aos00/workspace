//putText(result, "Differencing the two images.", cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

#define RUIDO_INFERIOR 5

using namespace std;
using namespace cv;


//----CALCULO DO NDVI COM WEBCAM-----------------------

void blueIteration(Mat& I);
void redIteration(Mat& I);
void antiLowerNoise(Mat& I);
void ndviCalculation(Mat& src, Mat& dst);

Size frameSize;

int main(int argc, const char** argv){
	short limiteInferior = 3;
	Mat frame;	

	VideoCapture capture(CV_CAP_ANY);

	CV_Assert(capture.isOpened());

	//if (!capture.isOpened()){
	//		cout << "Nenhuma camera detectada" << endl;

	//	}else{
			cout << "In capure ..." << endl;
			
			frameSize = frame.size();

			Mat soma(frameSize, CV_32F);
			Mat subtracao(frameSize, CV_32F);
			Mat ndvi(frameSize, CV_32F);

			for (;;){
			
				capture >> frame;
				//cout << "Is continuous: " << frame.isContinuous() << endl;
				//Criar Matrizes de float
				

				vector<Mat> rgb;		
				split(frame, rgb);		
				
				//Adicionar e subtrair os canais
				add(rgb[2], rgb[0], soma, noArray(), CV_32F);

				antiLowerNoise(soma); // Evitar ruido de baixa intensidade;

				subtract(rgb[2], rgb[0], subtracao, noArray(), CV_32F);

				//Dividir os canais
				divide(subtracao, soma, ndvi, 1, CV_32F);

				add(ndvi, 1, ndvi, noArray(), CV_32F);
				multiply(ndvi, 127, ndvi, 1.0, CV_32F);

				ndvi.convertTo(ndvi, CV_8U);

				Mat ndviImage[3];

				ndviImage[2] = (ndvi - 128) * 2; //Canal do vermelho
				ndviImage[0] = ((255 - ndvi) - 128) * 2; //Canal do azul
				
				redIteration(ndviImage[2]);
				blueIteration(ndviImage[0]);

				ndviImage[1] = 255 - (ndviImage[2] + ndviImage[0]);

				Mat imagem;
				merge(ndviImage, 3, imagem);


				//cout << "Canais: " << ndvi.channels() << endl;
				imshow("NDVI", imagem);

				/*Point3_<uchar> *p = imagem.ptr < Point3_<uchar>(300, 200);

				cout << "ndvi1 " <<  << endl;
				cout << "ndvi2 " << (int)imagem.at<uchar>(50, 50) << endl;
				cout << "ndvi3 " << (int)imagem.at<uchar>(200, 300) << endl;*/

				//system("pause");
				
				if (waitKey(10) >= 0)
					break;
			}
			waitKey(0);

		//}
		//cvReleaseCapture(&capture);
		//cvDestroyWindow("NDVI");


	return 0;
}

void ndviCalculation(Mat& src, Mat& dst){
	
	CV_Assert(src.channels() == 3); //Garantir que o frame possui 3 canais
	CV_Assert(dst.channels() == 1); //Garantir que a matriz de saida tem 1 canal (Grayscale)
	CV_Assert(src.isContinuous()); //Garantir que a matriz é continua

	int nCols = src.cols * 3;

	Mat rgb[3];
	Mat soma(frameSize, CV_32F);
	Mat subtracao(frameSize, CV_32F);
	Mat ndvi(frameSize, CV_32F);

	split(src, rgb);

	//Adicionar e subtrair os canais
	add(rgb[2], rgb[0], soma, noArray(), CV_32F);

	antiLowerNoise(soma); // Evitar ruido de baixa intensidade;

	subtract(rgb[2], rgb[0], subtracao, noArray(), CV_32F); // fazer subtracao

	//Dividir os canais
	divide(subtracao, soma, ndvi, 1, CV_32F);

	add(ndvi, 1, ndvi, noArray(), CV_32F);
	multiply(ndvi, 127, ndvi, 1.0, CV_32F);

	ndvi.convertTo(ndvi, CV_8U);

	



}

void antiLowerNoise(Mat& I){
	int channels = I.channels();

	int nRows = I.rows;
	int nCols = I.cols * channels;

	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* p;
	for (i = 0; i < nRows; ++i)
	{
		p = I.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j)
		{
			if (p[j] < RUIDO_INFERIOR)
				p[j] = RUIDO_INFERIOR;
		}
	}
}

void blueIteration(Mat& I){
	int channels = I.channels();

	int nRows = I.rows;
	int nCols = I.cols * channels;

	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* p;
	for (i = 0; i < nRows; ++i)
	{
		p = I.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j)
		{
			if (p[j] >= 128)
				p[j] = 0;
		}
	}
}

void redIteration(Mat& I)
{
	// accept only char type matrices
	//CV_Assert(I.depth() != sizeof(uchar));

	int channels = I.channels();

	int nRows = I.rows;
	int nCols = I.cols * channels;

	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* p;
	for (i = 0; i < nRows; ++i)
	{
		p = I.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j)
		{
			if (p[j] < 128)
				p[j] = 0;
		}
	}
}



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

//Mat& iterate(Mat& I);
//
//int main(int argc, const char** argv){
//
//	Mat red(10, 10, CV_32F, Scalar(255));
//	Mat blue(red.size(), CV_32F, Scalar(255));
//
//	/*Mat soma(red.size(), CV_32F);
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
//	cout << "tela" << red << endl << endl;*/
//
//	iterate(red);
//	cout << "red" << red << endl << endl; 
//	imshow("matriz", red);
//
//	system("pause");
//
//	return 0;
//}
//
//Mat& iterate(Mat& I)
//{
//	// accept only char type matrices
//	CV_Assert(I.depth() != sizeof(uchar));
//
//	int channels = I.channels();
//
//	int nRows = I.rows;
//	int nCols = I.cols * channels;
//
//	if (I.isContinuous())
//	{
//		nCols *= nRows;
//		nRows = 1;
//	}
//
//	int i, j;
//	float* p;
//	for (i = 0; i < nRows; ++i)
//	{
//		p = I.ptr<float>(i);
//		for (j = 0; j < nCols; ++j)
//		{
//			cout << (float)p[j] << endl;
//			if (p[j] > 100)
//				p[j] = 0;
//			//p[j] > 100 ? 0 : p[j];
//		}
//	}
//	return I;
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


	


//------------------------TESTANDO WEBCAM-----------------------------------------------------------

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