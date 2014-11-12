#ifndef __COMMON_HPP_INCLUDED__
#define __COMMON_HPP_INCLUDED__

#include <string>

using namespace std;

struct coordinates {
	double latitude;
	double longitude;
	double x;
	double y;
};

struct location {
    coordinates coordinate;
    float altitude;
    string date;
};

#endif
