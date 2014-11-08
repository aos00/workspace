#define __DEBUG__


#include "targetarea.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

TargetArea::TargetArea(const char * filepath) {	
	string line;
	string full_file_path = filepath + string("areapoints");
	
	#ifdef __DEBUG__
	printf("TargetArea(): Filepath: %s \n full_file_path: %s\n",filepath, full_file_path.c_str());
	#endif
	
	ifstream areapoints(full_file_path.c_str());
	
	if(areapoints.is_open()) {
		while(getline(areapoints,line)) {
			printf("%s",line.c_str());
		}
		areapoints.close();
	}else{
		perror("Cant open this file!");	
	}
}
