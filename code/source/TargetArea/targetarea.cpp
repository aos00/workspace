#define __DEBUG__

#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "targetarea.hpp"

using namespace std;

const string filepath = "/home/pi/raspi_local_repo/RayCasting/";

#ifdef __DEBUG__
void printVector(vector<double>&);
#endif

/* Read file with the area points and load them to memory */
TargetArea::TargetArea(const char*) {
	n_vertices = 0;
	string coordinate;
	string full_file_path = filepath + string("areapoints");

	#ifdef __DEBUG__
	printf("TargetArea(): Filepath: %s \n full_file_path: %s\n",filepath.c_str(), full_file_path.c_str());
	#endif

	ifstream areapoints(full_file_path.c_str());

	if(areapoints.is_open()) {
		size_t delmpos;
		while(getline(areapoints,coordinate)) {
			delmpos = coordinate.find(",");
			x_vert.push_back(atof((coordinate.substr(0,delmpos)).c_str()));
			y_vert.push_back(atof((coordinate.substr(delmpos+1, string::npos)).c_str()));
			n_vertices++;
		}
		
		areapoints.close();

		#ifdef __DEBUG__
		printf("Number of vertices: %i\n", n_vertices);
		printf("X vertices:\n");
		printVector(x_vert);
		printf("\nY vertices: \n");
		printVector(y_vert);
		double x = 10.00001;
		double y = 10.00001;
		printf("Point(%f,%f) is in area? %i\n", x, y, inpoly(n_vertices, x_vert, y_vert, x, y));
		#endif		

	}else{
		perror("Cant open this file!");
	}
}


void TargetArea::printVector(vector<double> &src) {

	for(vector<double>::iterator it = src.begin(); it != src.end(); it++)
		printf("\n%f",*it);
}


/* 
 * Point Inclusion in Polygon Algorithm
 * Copyright (c) 1970-2003, Wm. Randolph Franklin
*/

int TargetArea::inpoly(int nvert, vector<double> &vertx, vector<double> &verty, double testx, double testy)
{
  int i, j, c = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
	 (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}
