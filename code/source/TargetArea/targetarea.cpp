#define __DEBUG__

#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "targetarea.hpp"


using namespace std;
using namespace GeographicLib;

void printVector(vector<double>&);

/* Read file with the area points and load them to memory */
TargetArea::TargetArea(const string &file_path) {
	nvert = 0;
	string coordinate;
	//string full_file_path = filepath + string("areapoints");

	#ifdef __DEBUG__
	printf("TargetArea(): Filepath: %s\n",file_path.c_str());
	#endif

	ifstream areapoints(file_path.c_str());

	if(areapoints.is_open()) {
		size_t delmpos;
		double x, y;
		while(getline(areapoints,coordinate)) {
			delmpos = coordinate.find(",");
			
			double log = atof((coordinate.substr(0,delmpos)).c_str());
			double lat = atof((coordinate.substr(delmpos+1, string::npos)).c_str());
			
			convertCoordinates(lat, log, x, y);					
			
			vertx.push_back(x);
			verty.push_back(y);
			nvert++;
		}
		
		areapoints.close();

		#ifdef __DEBUG__
		printf("Number of vertices: %i\n", nvert);
		printf("X vertices:\n");
		printVector(vertx);
		printf("\nY vertices: \n");
		printVector(verty);
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

#ifdef __OLD_INPOLY__
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

#else

bool TargetArea::inTarget(const coordinates &coord)
{
	double latitude = coord.latitude;
	double longitude = coord.longitude;
	double testx, testy;
	convertCoordinates(latitude, longitude, testx, testy);
	bool c = false;
  int i, j;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
	 (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}

#endif

void TargetArea::convertCoordinates(const double &latitude, const double &longitude, double &x, double &y)
{
	try {  
      
      int zone;
      bool northp;
      UTMUPS::Forward(latitude, longitude, zone, northp, x, y);
      string zonestr = UTMUPS::EncodeZone(zone, northp);
      
      #ifdef __DEBUG__
      printf("Converted Latitude: %f ; Longitude: %f, x: %f; y: %f\n", latitude, longitude, x, y);
      cout << "Zone: " << zonestr << endl;
      #endif
      
    }catch (const exception& e) {
    
		//cerr << "Caught exception: " << e.what() << "\n";
	}	
}


