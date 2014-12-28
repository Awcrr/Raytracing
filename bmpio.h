#ifndef BMPIO_H
#define BMPIO_H

#include "common.h"

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

class Color{
public:
	double r,g,b;
	Color(double R = 0,double G = 0,double B = 0):r(R),g(G),b(B){}
	~Color(){}

	Color operator + (Color dt){
		return Color(r + dt.r,g + dt.g,b + dt.b);
	}
	Color operator * (Color dt){
		return Color(r * dt.r,g * dt.g,b * dt.b);
	}
	Color operator * (double indx){
		return Color(r * indx,g * indx,b * indx);
	}
	Color operator / (double div){
		return Color(r / div,g / div,b / div);
	}
	bool NotZero(){
		return r > Eps || g > Eps || b > Eps;
	}

	void Read(stringstream &input){
		input >> r >> g >> b;
	}
	void Read(ifstream &input){
		input >> r >> g >> b;
	}
	void Limit(){
		r = min(r,1.0); g = min(g,1.0); b = min(b,1.0);
	}
};

struct rgb{
	int red,green,blue;
	rgb() {}
	rgb(int r,int g,int b):red(r),green(g),blue(b){}
	void print();
	void legal();
};

class Bmp{
public:
	void Initialize(const int &H,const int &W);
	void Input(const char *fl);
	void Output(const char *fl);

	rgb *pic;
	int biHeight,biWidth;
};

#endif
