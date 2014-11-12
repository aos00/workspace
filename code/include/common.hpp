#ifndef __COMMON_HPP_INCLUDED__
#define __COMMON_HPP_INCLUDED__

#include <string>

#define VIDEO_MODE 0
#define PHOTO_MODE 1
#define BURST_MODE 2

using namespace std;

struct coordinates {
	double latitude;
	double longitude;
	double x;
	double y;
};

struct location {
    coordinates coordinate;
    double altitude;
    string date;
};

#endif
