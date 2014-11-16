#ifndef __TARGETAREA_H_INCLUDED__
#define __TARGETAREA_H_INCLUDED__

#include <vector>
#include <GeographicLib/UTMUPS.hpp>
#include "common.hpp"

using namespace std;

class TargetArea
{
	private:
		vector<double> vertx;
		vector<double> verty;
		int nvert;

		void convertCoordinates(const double&, const double&, double&, double&);

	public:
		void init(const string&);
		//int inpoly(int, vector<double>&, vector<double>&, double, double);
		bool inTarget(const coordinates&);
		void printVector(vector<double> &src);

};
#endif
