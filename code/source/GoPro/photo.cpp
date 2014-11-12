/*
*	Photo Class Source File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/

#include "photo.hpp"



Photo::Photo(short ID, const location *locat, bool dl) {
	id = ID;
	downloaded = dl;
	if(locat != NULL){
        loc.coordinate.latitude = locat->coordinate.latitude;
        loc.coordinate.longitude = locat->coordinate.longitude;
        loc.date = locat->date;
	}else{
        loc.coordinate.latitude = 0.0;
        loc.coordinate.longitude = 0.0;
        loc.date = "nodate";
	}
}
