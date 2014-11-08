#define __DEBUG__

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

#define MAX_POLYGON_VERT 10;
using namespace std;

const string filepath = "/home/pi/raspi_local_repo/RayCasting/";
vector<double> x_vert;
vector<double> y_vert;
short n_vertices = 0;


int main(void) {

	string coordinate;
	string full_file_path = filepath + string("areapoints");

	#ifdef __DEBUG__
	printf("TargetArea(): Filepath: %s \n full_file_path: %s\n",filepath, full_file_path.c_str());
	#endif

	ifstream areapoints(full_file_path.c_str());

	if(areapoints.is_open()) {
		size_t delmpos;
		while(getline(areapoints,coordinate)) {
			delmpos = coordinate.find(",");
			x_vert.push_back(atof((coordinate.substr(0,delmpos)).c_str()));
			y_vert.push_back(atof((coordinate.substr(delmpos+1, string::npos)).c_str()));

			#ifdef __DEBUG__
			printf("New file Line: %s\n", coordinate.c_str());
			printf("%f\n",x);
			printf("%f\n",y);
			#endif
		}
		areapoints.close();
	}else{
		perror("Cant open this file!");
	}

}


/* Point Inclusion in Polygon Test
 * Copyright (c) 1970-2003, Wm. Randolph Franklin
*/

int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
{
  int i, j, c = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
	 (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}
