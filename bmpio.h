#ifndef BMPIO_H
#define BMPIO_H

#include "common.h"

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

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

struct BITMAPFILEHEADER{
	word bfType;
	dword bfSize;
	word bfReserved1;
	word bfReserved2;
	dword bfOffBits;
};

struct BITMAPINFOHEADER{
	dword biSize;
	long biWidth;
	long biHeight;
	word biPlanes;
	word biBitCount;
	dword biCompression;
	dword biSizeImage;
	long biXPelsPerMeter;
	long biYPelsPerMeter;
	dword biClrUsed;
	dword biClrImportant;
};

struct RGBQUAD{
	byte rgbBlue;
	byte rgbGreen;
	byte rgbRed;
	byte rgbReserved;
};

struct IDATA{
	byte red;
	byte green;
	byte blue;
	Color GetColor(){
		return Color((double)red / 256,(double)green / 256,(double)blue / 256);
	}
};

class Bmp{
public:
	BITMAPFILEHEADER bmpHead;
	BITMAPINFOHEADER bmpInfo;
	IDATA pic[Max_pixel][Max_pixel];

	Bmp(){}
	~Bmp(){}

	void Initialize(int H = 420,int W = 420);
	void Input(char* file);
	void Output(char* file);
};

#endif
