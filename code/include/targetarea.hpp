#ifndef __TARGETAREA_H_INCLUDED__
#define __TARGETAREA_H_INCLUDED__

#include <vector>

using namespace std;

class TargetArea
{
	private:
		vector<double> x_vert;
		vector<double> y_vert;
		int n_vertices;	
		
	public:
		TargetArea(const string&);
		int inpoly(int, vector<double>&, vector<double>&, double, double);
		void printVector(vector<double> &src);
			
};
#endif
