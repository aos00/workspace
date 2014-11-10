#define __DEBUG__

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

const string filepath = "/home/pi/raspi_local_repo/RayCasting/";
vector<float> x_vert;
vector<float> y_vert;
short n_vertices = 0;

#ifdef __DEBUG__
void printVector(vector<float>&);
#endif

int inpoly(int, vector<float>&, vector<float>&, float, float);

/* Read file with the area points and load them to memory */
int main(void) {

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

		#ifdef __DEBUG__
		printf("Number of vertices: %i\n", n_vertices);
		printf("X vertices:\n");
		printVector(x_vert);
		printf("\nY vertices: \n");
		printVector(y_vert);

		printf("Point(%f,%f) is in area? %i",0,0,inpoly(n_vertices,x_vert, y_vert, 0.0, 0.0));
		#endif

		areapoints.close();



	}else{
		perror("Cant open this file!");
	}
}

#ifdef __DEBUG__

void printVector(vector<float> &src) {

	for(vector<float>::iterator it = src.begin(); it != src.end(); it++)
		printf("\n%f",*it);
}

#endif __DEBUG__


/* Point Inclusion in Polygon Algorithm
 * Copyright (c) 1970-2003, Wm. Randolph Franklin
*/

int inpoly(int nvert, vector<float> &vertx, vector<float> &verty, float testx, float testy)
{
  int i, j, c = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
	 (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}
