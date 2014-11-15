#ifndef __COMMON_HPP_INCLUDED__
#define __COMMON_HPP_INCLUDED__

#include <string>

#define VIDEO_MODE 0
#define PHOTO_MODE 1
#define BURST_MODE 2

#define STATUS_ERROR 0;
#define STATUS_OK 1;

unsigned short STATUS_CAMERA = 0;
unsigned short STATUS_GPS = 0;
unsigned short STATUS_IN_TARGET = 0;

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
