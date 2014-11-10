#define __FUNCAO_PRINCIPAL__
//#define __TESTE_NDVI_IMG_DISCO__
//#define __TESTE_DAS_FUNCOES_NDVI__
//#define __TESTE_WEBCAM__
//#define __LER_IMG_DISCO__
//#define __MANIPULANDO_MAT_CONSTANTES__
//#define __TESTE_DAS_FUNCOES_NDVI__
//#define __USING_NDVI_VGYRM_LUT_COLORMAP__ 

#include <iostream>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RUIDO_INFERIOR 5

using namespace std;
using namespace cv;

void blueIteration(Mat& scr, Mat& ndvi);
void redIteration(Mat& scr, Mat& ndvi);
void antiLowerNoise(Mat& I);
void ndviCalculation(Mat& src, Mat& dst);
void convertToGrayScale(Mat& src, Mat& dst);
void colormapFromGray(Mat& src, Mat& dst);
void colormapFromLUT(Mat& src, Mat& dst);
void printMat(Mat& src);

Size frameSize;

#ifndef __TESTE_DAS_FUNCOES_NDVI__


/* The following functions implement the NDVI and Pseudocolor calculations
	NDVI: (nir-vis)/(nir+vis)
	PseudoColdors:
		Blue channel: ((255-ndvi)-128)*2
		Red Channel: (ndvi-128)*2
		Green Channel: (25-(bluechannel+redchannel)
*/
void ndviCalculation(Mat& src, Mat& dst){

	CV_Assert(src.type() == CV_8UC3); //Garantir que o frame possui 3 canais
	CV_Assert(dst.type() == CV_32FC1); //Garantir que a matriz de saida tem 1 canal (Grayscale)
	CV_Assert(src.isContinuous()); //Garantir que a matriz é continua

	Mat rgb[3];
	Mat soma(frameSize, CV_32F);
	Mat subtracao(frameSize, CV_32F);
	Mat divisao1(frameSize, CV_32F);

	/*Matrizes de três canais são armazenadas na memoria usando a seguinte ordem:
	M[0][0] = [azul,verde,vermelho]
	ref: http://docs.opencv.org/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html
	*/


	split(src, rgb);
	
	////Adicionar e subtrair os canais
	add(rgb[2], rgb[0], soma, noArray(), CV_32F);
	
	antiLowerNoise(soma); // Evitar ruido de baixa intensidade;
	
	subtract(rgb[2], rgb[0], subtracao, noArray(), CV_32F); // fazer subtracao
	
	////Dividir os canais
	divide(subtracao, soma, dst, 1, CV_32F);
	
	#ifdef _DEBUG_NDVI_CALC_
	cout << "NDVI Calculation: Canal Vermelho: " << (int)rgb[2].at<uchar>(30, 30) << endl;
	cout << "NDVI Calculation: Canal Azul: " << (int)rgb[0].at<uchar>(30, 30) << endl;

	cout << "tipo" << dst.type() << endl;
	//dst = ((rgb[2] - rgb[0]) / (rgb[2] + rgb[0]));
	cout << "tipo" << dst.type() << endl;
	#endif

}

void convertToGrayScale(Mat& src, Mat& dst){

	CV_Assert(src.type() == CV_32FC1);
	CV_Assert(dst.type() == CV_8UC1);

	add(src, 1, src, noArray(), CV_32F);
	multiply(src, 127, src, 1.0, CV_32F);
	src.convertTo(dst, CV_8U);
}

void antiLowerNoise(Mat& I){

	CV_Assert(I.type() == CV_32F);

	int channels = I.channels();

	int nRows = I.rows;
	int nCols = I.cols * channels;

	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	float* p;
	for (i = 0; i < nRows; ++i)
	{
		p = I.ptr<float>(i);
		for (j = 0; j < nCols; ++j)
		{
			if (p[j] < RUIDO_INFERIOR)
				p[j] = RUIDO_INFERIOR;
		}
	}
}

void blueIteration(Mat& src, Mat& ndvi)
{
	CV_Assert(src.type() == ndvi.type());
	CV_Assert(src.channels() == src.channels());

	int channels = src.channels();

	int nRows = src.rows;
	int nCols = src.cols * channels;

	if (src.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* pNdvi;
	uchar* pBlueChannel;
	for (i = 0; i < nRows; ++i)
	{
		pBlueChannel = src.ptr<uchar>(i);
		pNdvi = ndvi.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j)
		{
			if (pNdvi[j] >= 128)
				pBlueChannel[j] = 0;
		}
	}
}

void redIteration(Mat& src, Mat& ndvi)
{
	CV_Assert(src.type() == ndvi.type());
	CV_Assert(src.channels() == src.channels());

	int channels = src.channels();

	int nRows = src.rows;
	int nCols = src.cols * channels;

	if (src.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* pNdvi;
	uchar* pRedChannel;
	for (i = 0; i < nRows; ++i)
	{
		pRedChannel = src.ptr<uchar>(i);
		pNdvi = ndvi.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j)
		{
			if (pNdvi[j] < 128)
				pRedChannel[j] = 0;
		}
	}
}

void colormapFromGray(Mat& src, Mat& dst){
	CV_Assert(src.channels() == 1);
	CV_Assert(src.type() == CV_8U);
	CV_Assert(dst.type() == CV_8UC3);

	Mat ndviPseudoColored[3];

	ndviPseudoColored[2] = (src - 128) * 2; //Canal do vermelho
	

	ndviPseudoColored[0] = ((255 - src) - 128) * 2; //Canal do azul

	ndviPseudoColored[1] = 255 - (ndviPseudoColored[2] + ndviPseudoColored[0]);

	merge(ndviPseudoColored, 3, dst);
	

	#ifdef _DEBUG_pseudocolor_
	cout << "ndviPseudoColored[2]" << ndviPseudoColored[2] << endl;
	cout << "ndviPseudoColored[0]" << ndviPseudoColored[0] << endl;
	cout << "Pseudo colored NDVI" << dst << endl;
	#endif
}


/*static LUT tables for NDVI_VGYRM colormap
 ref http://publiclab.org/notes/cfastie/08-26-2014/new-ndvi-colormap
*/

	static const unsigned short red_LUT[256] =
	{
		255, 250, 246, 242, 238, 233, 229, 225, 221, 216,
		212, 208, 204, 200, 195, 191, 187, 183, 178, 174,
		170, 166, 161, 157, 153, 149, 145, 140, 136, 132,
		128, 123, 119, 115, 111, 106, 102, 98, 94, 90,
		85, 81, 77, 73, 68, 64, 60, 56, 52, 56,
		60, 64, 68, 73, 77, 81, 85, 90, 94, 98,
		102, 106, 111, 115, 119, 123, 128, 132, 136, 140,
		145, 149, 153, 157, 161, 166, 170, 174, 178, 183,
		187, 191, 195, 200, 204, 208, 212, 216, 221, 225,
		229, 233, 238, 242, 246, 250, 255, 250, 245, 240,
		235, 230, 225, 220, 215, 210, 205, 200, 195, 190,
		185, 180, 175, 170, 165, 160, 155, 151, 146, 141,
		136, 131, 126, 121, 116, 111, 106, 101, 96, 91,
		86, 81, 76, 71, 66, 61, 56, 66, 77, 87,
		98, 108, 119, 129, 140, 131, 122, 113, 105, 96,
		87, 78, 70, 61, 52, 43, 35, 26, 17, 8,
		0, 7, 15, 23, 31, 39, 47, 55, 63, 71,
		79, 87, 95, 103, 111, 119, 127, 135, 143, 151,
		159, 167, 175, 183, 191, 199, 207, 215, 223, 231,
		239, 247, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255
	};


	static const unsigned short green_LUT[256] =
	{
		255, 250, 246, 242, 238, 233, 229, 225, 221, 216,
		212, 208, 204, 200, 195, 191, 187, 183, 178, 174,
		170, 166, 161, 157, 153, 149, 145, 140, 136, 132,
		128, 123, 119, 115, 111, 106, 102, 98, 94, 90, 85,
		81, 77, 73, 68, 64, 60, 56, 52, 56, 60, 64, 68, 73,
		77, 81, 85, 90, 94, 98, 102, 106, 111, 115, 119, 123,
		128, 132, 136, 140, 145, 149, 153, 157, 161, 166, 170,
		174, 178, 183, 187, 191, 195, 200, 204, 208, 212, 216,
		221, 225, 229, 233, 238, 242, 246, 250, 255, 250, 245,
		240, 235, 230, 225, 220, 215, 210, 205, 200, 195, 190,
		185, 180, 175, 170, 165, 160, 155, 151, 146, 141, 136,
		131, 126, 121, 116, 111, 106, 101, 96, 91, 86, 81, 76,
		71, 66, 61, 56, 66, 77, 87, 98, 108, 119, 129, 140, 147,
		154, 161, 168, 175, 183, 190, 197, 204, 211, 219, 226,
		233, 240, 247, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 249, 244, 239, 233, 228, 223, 217, 212,
		207, 201, 196, 191, 185, 180, 175, 170, 164, 159, 154,
		148, 143, 138, 132, 127, 122, 116, 111, 106, 100, 95,
		90, 85, 79, 74, 69, 63, 58, 53, 47, 42, 37, 31, 26, 21,
		15, 10, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	static const unsigned short blue_LUT[256] =
	{
		255, 250, 246, 242, 238, 233, 229, 225, 221, 216,
		212, 208, 204, 200, 195, 191, 187, 183, 178, 174,
		170, 166, 161, 157, 153, 149, 145, 140, 136, 132,
		128, 123, 119, 115, 111, 106, 102, 98, 94, 90, 85,
		81, 77, 73, 68, 64, 60, 56, 52, 56, 60, 64, 68, 73,
		77, 81, 85, 90, 94, 98, 102, 106, 111, 115, 119, 123,
		128, 132, 136, 140, 145, 149, 153, 157, 161, 166, 170,
		174, 178, 183, 187, 191, 195, 200, 204, 208, 212, 216,
		221, 225, 229, 233, 238, 242, 246, 250, 255, 250, 245,
		240, 235, 230, 225, 220, 215, 210, 205, 200, 195, 190,
		185, 180, 175, 170, 165, 160, 155, 151, 146, 141, 136,
		131, 126, 121, 116, 111, 106, 101, 96, 91, 86, 81, 76,
		71, 66, 61, 56, 80, 105, 130, 155, 180, 205, 230, 255,
		239, 223, 207, 191, 175, 159, 143, 127, 111, 95, 79, 63,
		47, 31, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 31, 47, 63, 79, 95, 111,
		127, 143, 159, 175, 191, 207, 223, 239
	};


	/* Em Desenvolvimento*/
	/*
void colormapFromLUT(Mat& src, Mat& dst) {
	CV_Assert(src.type() == CV_8UC1);
	CV_Assert(dst.type() == CV_8UC3);

	//Might not be the most efficient way of accessing each channel
	//but this is what we have for tdy
	Mat rgb[3];
	split(src, rgb);

	for (short i = 0; i < 256; i++){
		rgb[0].at(Point(i)) = blue_LUT[rgb[0][i]];
		rgb[1][i] = green_LUT[rgb[1][i]];
		rgb[2][i] = red_LUT[rgb[2][i]];
	}
}
*/

#endif

#ifdef __FUNCAO_PRINCIPAL__

//----CALCULO DO NDVI COM WEBCAM-----------------------

int main(int argc, const char** argv){
	short limiteInferior = 5;
	Mat frame;	

	VideoCapture capture(CV_CAP_ANY);

	CV_Assert(capture.isOpened());

		cout << "In capure ..." << endl;
			
		frameSize = frame.size();

		Mat gray(frameSize, CV_8UC1);
		Mat ndvi(frameSize, CV_32FC1);

		for (;;){
			capture >> frame;
			

			//Mat rgb[3];
			//split(frame, rgb);

			//imshow("Vermelho", rgb[2]);

			
			imshow("Frame", frame);

			//cout << frame << endl;
			//
			ndviCalculation(frame, ndvi); //Calculate NDVI

			//imshow("NDVI", ndvi);

			convertToGrayScale(ndvi, gray); //Convert ndvi matrix to 8 bit gray scale
		
			imshow("gray", gray);

			colormapFromGray(gray, frame);			

			imshow("False color", frame);
				
			if (waitKey(10) >= 0)
				break;
		}
		waitKey(0);

		//}
		//cvReleaseCapture(&capture);
		//cvDestroyWindow("NDVI");
		destroyAllWindows();


	return 0;
}

#endif

#ifdef __MANIPULANDO_MAT_CONSTANTES__

//---------------MANIPULANDO MATRIZES DE CONSTANTES ----------

Mat& iterate(Mat& I);

int main(int argc, const char** argv){

	Mat red(10, 10, CV_32F, Scalar(255));
	Mat blue(red.size(), CV_32F, Scalar(255));

	/*Mat soma(red.size(), CV_32F);
	Mat divisao(red.size(), CV_32F);
	Mat subtracao(red.size(), CV_32F);

	add(red, blue, soma, noArray(), CV_32F);
	cout << "Soma" << soma << endl << endl;
	subtract(red, blue, red, noArray(), CV_32F);
	cout << "Subtracao" << red << endl << endl;
	divide(red, soma, red, 1, CV_32F);
	cout << "Divisao" << red << endl << endl;

	add(red, 1, red, noArray(), CV_32F);
	cout << "adicao" << red << endl << endl;
	multiply(red, 127, red, 1, CV_32F);
	cout << "tela" << red << endl << endl;*/

	iterate(red);
	cout << "red" << red << endl << endl; 
	imshow("matriz", red);

	system("pause");

	return 0;
}

Mat& iterate(Mat& I)
{
	// accept only char type matrices
	CV_Assert(I.depth() != sizeof(uchar));

	int channels = I.channels();

	int nRows = I.rows;
	int nCols = I.cols * channels;

	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	float* p;
	for (i = 0; i < nRows; ++i)
	{
		p = I.ptr<float>(i);
		for (j = 0; j < nCols; ++j)
		{
			cout << (float)p[j] << endl;
			if (p[j] > 100)
				p[j] = 0;
			//p[j] > 100 ? 0 : p[j];
		}
	}
	return I;
}

#endif

#ifdef __LER_IMG_DISCO__

//----------------------------LER IMAGEM DO DISCO--------------------------------------------------
			

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

#endif

#ifdef __TESTE_WEBCAM__

//------------------------TESTANDO WEBCAM-----------------------------------------------------------


int main(int argc, const char** argv){
	VideoCapture capture;
	Mat frame;

	capture.open(CV_CAP_ANY);
	//VideoCapture capture(0);

	if (!capture.isOpened()){
		cout << "Nenhuma camera detectada" << endl;

	}
	else{
		cout << "In capure ..." << endl;

		for (;;){

			capture >> frame;
			imshow("NDVI", frame);

			if (waitKey(10) >= 0)
				break;
		}
			waitKey(0);
		
	}
	return 0;
}

#endif


#ifdef __TESTE_DAS_FUNCOES_NDVI__

//----------------------TESTE DAS FUNÇÕES DE CALCULO DO NDVI --------------------------------

int main(int argc, const char** argv){
	
	/*
	*	Generating static frames
	*/
	Mat red(2,2, CV_8U, Scalar(200));
	Mat blue(red.size(), CV_8U, Scalar(100));
	Mat green(red.size(), CV_8U, Scalar(50));

	/*Matrizes de três canais são armazenadas na memoria usando a seguinte ordem:
	M[0][0] = [azul,verde,vermelho]
	ref: http://docs.opencv.org/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html
	*/

	Mat frame[3];
	frame[0] = blue;
	frame[1] = green;
	frame[2] = red;
	
	Mat mergedFrame;
	merge(frame, 3, mergedFrame);

	
	cout << mergedFrame << endl;

	imshow("Frame generated", mergedFrame);

	//PrintMat(mergedFrame);

	/*
	*	End of frame generation
	*/
		
	Mat ndvi(red.size(), CV_32FC1);

	ndviCalculation(mergedFrame, ndvi); //Calculate NDVI

	convertToGrayScale(ndvi, ndvi); //Convert ndvi matrix to 8 bit gray scale
	cout << "GRAY SCALE" << ndvi << endl;

	//colormapFromGray(ndvi, ndvi);
		
	waitKey(0);
	//system("pause");
	


	return 0;
}

void printMat(Mat& src){
	cout << endl << "------------- PRINT MATRIX ---------" << endl;
	
	CV_Assert(src.isContinuous());

	short nChannels = src.channels();

	cout << "Number of channels: " << nChannels << endl;

	int nRows = src.rows;
	int nCols = src.cols;

	short i, j;
	uchar* p;

	short channel =0;
	p = src.ptr<uchar>(0);	

	while (channel < nChannels){
		cout << endl << "Imprimindo canal " << channel << endl;
		j = 0;
		short n = 0;
		for (i = 0 ; i < nRows*nCols; i++){
			j = i * 3 + channel;
			if (n >= nCols){
				n = 0;
				cout << endl;
			}
				cout << " " << (int)p[j];
				n++;
		}
		channel++;
	}
	cout << endl;
	cout << "------------- END OF PRINT MATRIX ---------" << endl;
}

void printMat2(Mat& src){
	cout << endl << "------------- PRINT MATRIX ---------" << endl;

	CV_Assert(src.isContinuous());

	short nChannels = src.channels();

	cout << "Number of channels: " << nChannels << endl;

	int nRows = src.rows;
	int nCols = src.cols;

	short i, n, j;
	uchar* p;

	short channel = 0;
	p = src.ptr<uchar>(0);
	j = 0;
	n = 0;
	for (i = 0; i < nRows*nCols; i++){
		if (n >= nCols){
			n = 0;
			cout << endl;
		}
		cout << " " << (int)p[i];
		n++;
	}
	cout << endl;
	cout << "------------- END OF PRINT MATRIX ---------" << endl;
}


void ndviCalculation(Mat& src, Mat& dst){

	CV_Assert(src.type() == CV_8UC3); //Garantir que o frame possui 3 canais
	CV_Assert(dst.type() == CV_32FC1); //Garantir que a matriz de saida tem 1 canal (Grayscale)
	CV_Assert(src.isContinuous()); //Garantir que a matriz é continua

	Mat rgb[3];
	Mat soma(frameSize, CV_32F);
	Mat subtracao(frameSize, CV_32F);

	split(src, rgb);

	cout << "rgb[0]" << rgb[0] << endl;
	cout << "rgb[1]" << rgb[1] << endl;
	cout << "rgb[2]" << rgb[2] << endl;

	//Adicionar e subtrair os canais
	add(rgb[2], rgb[0], soma, noArray(), CV_32F);
	cout << "NVDI SOMA" << soma << endl;

	antiLowerNoise(soma); // Evitar ruido de baixa intensidade;
	 
	subtract(rgb[2], rgb[0], subtracao, noArray(), CV_32F); // fazer subtracao
	cout << "NVDI SUBTRACAO" << subtracao << endl;

	//Dividir os canais
	divide(subtracao, soma, dst, 1, CV_32F);
	cout << "NVDI DIVISAO" << dst << endl;	

}

void convertToGrayScale(Mat& src, Mat& dst){
	
	add(src, 1, src, noArray(), CV_32F);
	cout << "convertToGrayScale - add fn:" << src << endl;
	multiply(src, 127, src, 1.0, CV_32F);
	cout << "convertToGrayScale - multiply fn:" << src << endl;
	src.convertTo(dst, CV_8U);
	cout << "convertToGrayScale - after conversion to float:" << src << endl;
	

	
}

void antiLowerNoise(Mat& I){

	CV_Assert(I.type() == CV_32F);

	int channels = I.channels();

	int nRows = I.rows;
	int nCols = I.cols * channels;

	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	float* p;
	for (i = 0; i < nRows; ++i)
	{
		p = I.ptr<float>(i);
		for (j = 0; j < nCols; ++j)
		{
			if (p[j] < RUIDO_INFERIOR)
				p[j] = RUIDO_INFERIOR;
		}
	}
}

void blueIteration(Mat& src, Mat& ndvi)
{
	// accept only char type matrices
	//CV_Assert(I.depth() != sizeof(uchar));
	CV_Assert(src.type() == ndvi.type());
	CV_Assert(src.channels() == src.channels());

	int channels = src.channels();

	int nRows = src.rows;
	int nCols = src.cols * channels;

	if (src.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* pNdvi;
	uchar* pBlueChannel;
	for (i = 0; i < nRows; ++i)
	{
		pBlueChannel = src.ptr<uchar>(i);
		pNdvi = ndvi.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j)
		{
			if (pNdvi[j] >= 128)
				pBlueChannel[j] = 0;
		}
	}
}

void redIteration(Mat& src, Mat& ndvi)
{
	// accept only char type matrices
	//CV_Assert(I.depth() != sizeof(uchar));
	CV_Assert(src.type() == ndvi.type());
	CV_Assert(src.channels() == src.channels());

	int channels = src.channels();

	int nRows = src.rows;
	int nCols = src.cols * channels;

	if (src.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* pNdvi;
	uchar* pRedChannel;
	for (i = 0; i < nRows; ++i)
	{
		pRedChannel = src.ptr<uchar>(i);
		pNdvi = ndvi.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j)
		{
			if (pNdvi[j] < 128)
				pRedChannel[j] = 0;
		}
	}
}

void colormapFromGray(Mat& src, Mat& dst){
	CV_Assert(src.channels() == 1);
	CV_Assert(src.type() == CV_8U);
	//CV_Assert(dst.channels() == 3);
	CV_Assert(dst.type() == CV_8U);

	Mat ndviPseudoColored[3];

	ndviPseudoColored[2] = (src - 128) * 2; //Canal do vermelho
	cout << "ndviPseudoColored[2]" << ndviPseudoColored[2] << endl;

	ndviPseudoColored[0] = ((255 - src) - 128) * 2; //Canal do azul
	cout << "ndviPseudoColored[0]" << ndviPseudoColored[0] << endl;

	ndviPseudoColored[1] = 255 - (ndviPseudoColored[2] + ndviPseudoColored[0]);

	merge(ndviPseudoColored, 3, dst);
	cout << "Pseudo colored NDVI" << dst << endl;
	


}

#endif


#ifdef __TESTE_NDVI_IMG_DISCO__

//------------------ Teste do NDVI com imagem do disco -------------------------

int main(int argc, const char** argv){

	Mat localImage = imread("foto-ajustada.png", 1);
	Mat infragramImage = imread("foto-ajustada-infragram-basic.png", 1);
	Mat grayndvi(frameSize, CV_8UC1);	

	frameSize = localImage.size();

	Mat ndvi(frameSize, CV_32FC1);

	if (!localImage.data)
	{
		cout << "Could not open local image" << std::endl;
		system("pause");
		return -1;
	}
	if (!infragramImage.data)
	{
		cout << "Could not open infragram image" << std::endl;
		system("pause");
		return -1;
	}

	
	ndviCalculation(localImage, ndvi);
	//cout << ndvi << endl;
	convertToGrayScale(ndvi, grayndvi);
	//cout << grayndvi << endl;
	////cout << "Image type:" << infragramImage
	//cout << "Frame size:" << frameSize << endl;
	cout << "Local ndvi " << (int) grayndvi.at<uchar>(0,0) << endl;
	cout << "Infragram ndvi " << (int)infragramImage.at<uchar>(0,0) << endl;

	imshow("Gray ndvi", grayndvi);
	//waitKey(1);
	imshow("Infragram", infragramImage);
	//imshow("Original", localImage);
	waitKey(0);
	//system("pause");
	
	destroyAllWindows();
	return 0;
}

#endif

#ifdef __USING_NDVI_VGYRM_LUT_COLORMAP__ 

int main(){

	system("pause");
	return 0;
}

#endif