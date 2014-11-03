/*
*	Main Source File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
*	@Date: October, 2014.
*/

#include <stdio.h>
#include "GoPro/gopro.h"

int main(){ //obter ID inicial da foto

GoPro camera((short) 1);
camera.takePicture();

return 0;


}
