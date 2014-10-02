#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, const char** argv){
	Mat imagem = imread("./image.jpg");
	namedWindow("Minha imagem");
	
/*
if(!imagem){
		cout << "Erro: Não foi possível carregar a foto!" << endl;
	}
*/
	
	cout << "Tamanho: " << imagem.size() << endl;
	cout << "Tipo" << imagem.type() << endl;
	
	Mat output(imagem.size(), imagem.type());
	vector<Mat> rgb;
	imshow("Minha imagem", imagem);
	
	split(imagem,rgb);
	namedWindow("b");
	namedWindow("g");
	namedWindow("r");
		
	//imshow("b",rgb[0]);
	//imshow("g",rgb[1]);
	
	imshow("r",(rgb[2]-rgb[0]/(rgb[2] + rgb[0])));
	
	waitKey(0);
}
