//putText(result, "Differencing the two images.", cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

#define RUIDO_INFERIOR 5

using namespace std;
using namespace cv;


//----CALCULO DO NDVI COM WEBCAM-----------------------

void blueIteration(Mat& scr, Mat& ndvi);
void redIteration(Mat& scr, Mat& ndvi);
void antiLowerNoise(Mat& I);
void ndviCalculation(Mat& src, Mat& dst);
void convertToGrayScale(Mat& src, Mat& dst);
void pseudoColor(Mat& src, Mat& dst);
void printMat(Mat& src);
Size frameSize;

//int main(int argc, const char** argv){
//	short limiteInferior = 5;
//	Mat frame;	
//
//	VideoCapture capture(CV_CAP_ANY);
//
//	CV_Assert(capture.isOpened());
//
//		cout << "In capure ..." << endl;
//			
//		frameSize = frame.size();
//
//		Mat gray(frameSize, CV_8UC1);
//		Mat ndvi(frameSize, CV_32FC1);
//
//		for (;;){
//			capture >> frame;
//			
//
//			Mat rgb[3];
//			split(frame, rgb);
//
//			imshow("Vermelho", rgb[2]);
//			//imshow("Frame", frame);
//
//			//cout << frame << endl;
//			
//			frameSize = frame.size();
//			
//			//ndviCalculation(frame, ndvi); //Calculate NDVI
//
//			//imshow("NDVI", ndvi);
//
//			//convertToGrayScale(ndvi, gray); //Convert ndvi matrix to 8 bit gray scale
//		
//			//imshow("gray", gray);
//
//			//pseudoColor(gray, frame);
//		
//			//system("pause");				
//
//			//imshow("False color", frame);
//				
//			if (waitKey(10) >= 0)
//				break;
//		}
//		waitKey(0);
//
//		//}
//		//cvReleaseCapture(&capture);
//		//cvDestroyWindow("NDVI");
//		destroyAllWindows();
//
//
//	return 0;
//}

void ndviCalculation(Mat& src, Mat& dst){

	CV_Assert(src.type() == CV_8UC3); //Garantir que o frame possui 3 canais
	CV_Assert(dst.type() == CV_32FC1); //Garantir que a matriz de saida tem 1 canal (Grayscale)
	CV_Assert(src.isContinuous()); //Garantir que a matriz é continua

	Mat rgb[3];
	Mat soma(frameSize, CV_32F);
	Mat subtracao(frameSize, CV_32F);

	split(src, rgb);

	//cout << "Vermelho: " << (int)rgb[2].at<uchar>(30, 30) << endl;
	//cout << "Azul: " << (int)rgb[0].at<uchar>(30, 30) << endl;

	//cout << "tipo" << dst.type() << endl;
	//dst = ((rgb[2] - rgb[0]) / (rgb[2] + rgb[0]));
	//cout << "tipo" << dst.type() << endl;
	////Adicionar e subtrair os canais
	add(rgb[2], rgb[0], soma, noArray(), CV_32F);
	////cout << "NVDI SOMA" << soma << endl;

	antiLowerNoise(soma); // Evitar ruido de baixa intensidade;
	// 
	subtract(rgb[2], rgb[0], subtracao, noArray(), CV_32F); // fazer subtracao
	////cout << "NVDI SUBTRACAO" << subtracao << endl;

	////Dividir os canais
	divide(subtracao, soma, dst, 1, CV_32F);
	//cout << "NVDI DIVISAO" << dst << endl;	

}

void convertToGrayScale(Mat& src, Mat& dst){
	//cout << src.type() << endl;
	//cout << CV_32FC1 << endl;
	CV_Assert(src.type() == CV_32FC1);
	CV_Assert(dst.type() == CV_8UC1);
	
	//cout << "NVDI DIVISAO" << src << endl;

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

void pseudoColor(Mat& src, Mat& dst){
	CV_Assert(src.channels() == 1);
	CV_Assert(src.type() == CV_8U);
	//CV_Assert(dst.channels() == 3);
	CV_Assert(dst.type() == CV_8UC3);

	Mat ndviPseudoColored[3];

	ndviPseudoColored[2] = (src - 128) * 2; //Canal do vermelho
	//cout << "ndviPseudoColored[2]" << ndviPseudoColored[2] << endl;

	ndviPseudoColored[0] = ((255 - src) - 128) * 2; //Canal do azul
	//cout << "ndviPseudoColored[0]" << ndviPseudoColored[0] << endl;

	//redIteration(ndviPseudoColored[2], src); //Remover
	//blueIteration(ndviPseudoColored[0], src); //Remover

	ndviPseudoColored[1] = 128 - (ndviPseudoColored[2] + ndviPseudoColored[0]);

	merge(ndviPseudoColored, 3, dst);
	//cout << "Pseudo colored NDVI" << dst << endl;

}



//-----------INICIANDO E TESTE DE CAPTURA CAMERA -------------


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

//----------------------------LER IMAGEM DO DISCO--------------------------------------------------
/**			

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


//----------------------TESTE DAS FUNÇÕES DE CALCULO DO NDVI --------------------------------

//void blueIteration(Mat& scr, Mat& ndvi);
//void redIteration(Mat& scr, Mat& ndvi);
//void antiLowerNoise(Mat& I);
//void ndviCalculation(Mat& src, Mat& dst);
//void convertToGrayScale(Mat& src, Mat& dst);
//void pseudoColor(Mat& src, Mat& dst);
//void printMat(Mat& src);
//Size frameSize;
//
//int main(int argc, const char** argv){
//	
//	/*
//	*	Generating a frame
//	*/
//		Mat red(2, 2, CV_8U, Scalar(20));
//		Mat blue(red.size(), CV_8U, Scalar(255));
//		Mat green(red.size(), CV_8U, Scalar(0));
//
//		Mat frame[3];
//		frame[0] = blue;
//		frame[1] = green;
//		frame[2] = red;
//
//		Mat mergedFrame;
//		merge(frame, 3, mergedFrame);
//		printMat(mergedFrame);
//
//		/*
//		*	End of frame generation
//		*/
//		
//		Mat ndvi;
//
//		ndviCalculation(mergedFrame, ndvi); //Calculate NDVI
//
//		convertToGrayScale(ndvi, ndvi); //Convert ndvi matrix to 8 bit gray scale
//		cout << "GRAY SCALE" << ndvi << endl;
//
//		pseudoColor(ndvi, ndvi);
//		
//		system("pause");
//	
//
//
//	return 0;
//}
//
//void printMat(Mat& src){
//	cout << endl << "------------- PRINT MATRIX ---------" << endl;
//	
//	CV_Assert(src.isContinuous());
//
//	short nChannels = src.channels();
//
//	cout << "Number of channels: " << nChannels << endl;
//
//	int nRows = src.rows;
//	int nCols = src.cols;
//
//	short i, j;
//	uchar* p;
//
//	short channel =0;
//	p = src.ptr<uchar>(0);	
//
//	while (channel < nChannels){
//		cout << endl << "Imprimindo canal " << channel << endl;
//		j = 0;
//		short n = 0;
//		for (i = 0 ; i < nRows*nCols; i++){
//			j = i * 3 + channel;
//			if (n >= nCols){
//				n = 0;
//				cout << endl;
//			}
//				cout << " " << (int)p[j];
//				n++;
//		}
//		channel++;
//	}
//	cout << endl;
//	cout << "------------- END OF PRINT MATRIX ---------" << endl;
//}
//
//void printMat2(Mat& src){
//	cout << endl << "------------- PRINT MATRIX ---------" << endl;
//
//	CV_Assert(src.isContinuous());
//
//	short nChannels = src.channels();
//
//	cout << "Number of channels: " << nChannels << endl;
//
//	int nRows = src.rows;
//	int nCols = src.cols;
//
//	short i, n, j;
//	uchar* p;
//
//	short channel = 0;
//	p = src.ptr<uchar>(0);
//	j = 0;
//	n = 0;
//	for (i = 0; i < nRows*nCols; i++){
//		if (n >= nCols){
//			n = 0;
//			cout << endl;
//		}
//		cout << " " << (int)p[i];
//		n++;
//	}
//	cout << endl;
//	cout << "------------- END OF PRINT MATRIX ---------" << endl;
//}
//
//
//void ndviCalculation(Mat& src, Mat& dst){
//
//	CV_Assert(src.channels() == 3); //Garantir que o frame possui 3 canais
//	CV_Assert(dst.channels() == 1); //Garantir que a matriz de saida tem 1 canal (Grayscale)
//	CV_Assert(src.isContinuous()); //Garantir que a matriz é continua
//
//	Mat rgb[3];
//	Mat soma(frameSize, CV_32F);
//	Mat subtracao(frameSize, CV_32F);
//
//	split(src, rgb);
//
//	//Adicionar e subtrair os canais
//	add(rgb[2], rgb[0], soma, noArray(), CV_32F);
//	cout << "NVDI SOMA" << soma << endl;
//
//	antiLowerNoise(soma); // Evitar ruido de baixa intensidade;
//	 
//	subtract(rgb[2], rgb[0], subtracao, noArray(), CV_32F); // fazer subtracao
//	cout << "NVDI SUBTRACAO" << subtracao << endl;
//
//	//Dividir os canais
//	divide(subtracao, soma, dst, 1, CV_32F);
//	cout << "NVDI DIVISAO" << dst << endl;	
//
//}
//
//void convertToGrayScale(Mat& src, Mat& dst){
//	cout << "NVDI DIVISAO" << src << endl;
//
//	add(src, 1, src, noArray(), CV_32F);
//	multiply(src, 127, src, 1.0, CV_32F);
//	src.convertTo(dst, CV_8U);
//	
//
//	
//}
//
//void antiLowerNoise(Mat& I){
//
//	CV_Assert(I.type() == CV_32F);
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
//			if (p[j] < RUIDO_INFERIOR)
//				p[j] = RUIDO_INFERIOR;
//		}
//	}
//}
//
//void blueIteration(Mat& src, Mat& ndvi)
//{
//	// accept only char type matrices
//	//CV_Assert(I.depth() != sizeof(uchar));
//	CV_Assert(src.type() == ndvi.type());
//	CV_Assert(src.channels() == src.channels());
//
//	int channels = src.channels();
//
//	int nRows = src.rows;
//	int nCols = src.cols * channels;
//
//	if (src.isContinuous())
//	{
//		nCols *= nRows;
//		nRows = 1;
//	}
//
//	int i, j;
//	uchar* pNdvi;
//	uchar* pBlueChannel;
//	for (i = 0; i < nRows; ++i)
//	{
//		pBlueChannel = src.ptr<uchar>(i);
//		pNdvi = ndvi.ptr<uchar>(i);
//		for (j = 0; j < nCols; ++j)
//		{
//			if (pNdvi[j] >= 128)
//				pBlueChannel[j] = 0;
//		}
//	}
//}
//
//void redIteration(Mat& src, Mat& ndvi)
//{
//	// accept only char type matrices
//	//CV_Assert(I.depth() != sizeof(uchar));
//	CV_Assert(src.type() == ndvi.type());
//	CV_Assert(src.channels() == src.channels());
//
//	int channels = src.channels();
//
//	int nRows = src.rows;
//	int nCols = src.cols * channels;
//
//	if (src.isContinuous())
//	{
//		nCols *= nRows;
//		nRows = 1;
//	}
//
//	int i, j;
//	uchar* pNdvi;
//	uchar* pRedChannel;
//	for (i = 0; i < nRows; ++i)
//	{
//		pRedChannel = src.ptr<uchar>(i);
//		pNdvi = ndvi.ptr<uchar>(i);
//		for (j = 0; j < nCols; ++j)
//		{
//			if (pNdvi[j] < 128)
//				pRedChannel[j] = 0;
//		}
//	}
//}
//
//void pseudoColor(Mat& src, Mat& dst){
//	CV_Assert(src.channels() == 1);
//	CV_Assert(src.type() == CV_8U);
//	//CV_Assert(dst.channels() == 3);
//	CV_Assert(dst.type() == CV_8U);
//
//	Mat ndviPseudoColored[3];
//
//	ndviPseudoColored[2] = (src - 128) * 2; //Canal do vermelho
//	cout << "ndviPseudoColored[2]" << ndviPseudoColored[2] << endl;
//
//	ndviPseudoColored[0] = ((255 - src) - 128) * 2; //Canal do azul
//	cout << "ndviPseudoColored[0]" << ndviPseudoColored[0] << endl;
//
//	redIteration(ndviPseudoColored[2], src); //Remover
//	blueIteration(ndviPseudoColored[0], src); //Remover
//
//	ndviPseudoColored[1] = 255 - (ndviPseudoColored[2] + ndviPseudoColored[0]);
//
//	merge(ndviPseudoColored, 3, dst);
//	cout << "Pseudo colored NDVI" << dst << endl;
//	
//
//
//}


//------------------ Teste do NDVI com imagem do disco -------------------------
int main(int argc, const char** argv){

	Mat ndvi(frameSize, CV_32FC1);
	Mat gray(frameSize, CV_8U);
	Mat ndvi2(frameSize, CV_8U);
	Mat falsecolor(frameSize, CV_8UC3);
	Mat imagem, infragram, diferenca;
	imagem = imread("teste1.png", 1);
	infragram = imread("teste2.png", 1);

	if (!imagem.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	//CV_Assert(imagem != 0);

	frameSize = imagem.size();

	ndvi.convertTo(ndvi2, CV_8U);

	ndviCalculation(imagem, ndvi);

	cout << "ndvi " << ndvi.at<float>(30, 30) << endl;

	convertToGrayScale(ndvi, gray);

	cout << "Gray " << (int)gray.at<uchar>(30, 30) << endl;

	pseudoColor(gray, falsecolor);

	diferenca = falsecolor - infragram;

	//cout << "Diferenca " << diferenca << endl;

	cout << "Size Imagem " << imagem.size() << endl;
	cout << "Size infragram  " << infragram.size() << endl;

	/*cout << "Pixel imagem " << (int)imagem.at<uchar>(30, 30) << endl;
	cout << "Pixel infragram " << (int)infragram.at<uchar>(30, 30) << endl;*/

	Mat rgb[3], rgb2[3];
	split(falsecolor, rgb);
	split(infragram, rgb2);
	cout << "imagem " << (int) rgb[2].at<uchar>(30, 30) << endl;
	cout << "Infragram " << (int)rgb2[2].at<uchar>(30, 30) << endl;
	//cout << "Verde False color " << (int)rgb[1].at<uchar>(30, 30) << endl;
	//cout << "Azul False color " << (int)rgb[0].at<uchar>(30, 30) << endl;

	//cout << imagem << endl;
	//imshow("ndvi", ndvi);
	//imshow("NDVI", ndvi2);
	imshow("gray", gray);
	imshow("False color", falsecolor);
	imshow("imagem", imagem);

	waitKey(0);
	return 0;
}