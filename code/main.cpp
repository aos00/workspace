/*
*	Main Source File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#include <stdio.h>
#include "GoPro/gopro.h"

int main(){ //obter ID inicial da foto

GoPro camera((short) 1378);
camera.takePicture();
sleep(1);
camera.getImage(1386);

return 0;


}
