#include <iostream>
#include <stdio.h>

using namespace std;

struct inner{
	double z;
};

struct point {
	double x;
	double y;
	inner in;
};

void fn(const point * src, point &dst){
	//point pt;
	//dst->x = src->x;
	//dst->y = src->y;
	dst = *src;
}

int main(){
	point p1;
	p1.x = 100;
	p1.y = 200;

	point p2;

	point * ptr1 = &p1;

	fn(&p1, p2);

	cout << p1.x << "," << p1.y << endl;
	cout << p2.x << "," << p2.y << endl;
	p1.x++;
	p1.y++;

	cout << p1.x << "," << p1.y << endl;
	cout << p2.x << "," << p2.y << endl;


/*
	int i = 10;
	int * ptr = &i;
	int a = *ptr;

	cout << i << "," << a << endl;

	i++;

	cout << i << "," << a << endl;

*/
}
