#ifndef COMMON_H
#define COMMON_H

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include "const.h"

using namespace std;

class Vector3{
public:
	double x,y,z;
	Vector3(double x = 0.0f,double y = 0.0f,double z = 0.0f):x(x),y(y),z(z){};
	~Vector3(){}
	Vector3 operator +(Vector3 dt){
		return Vector3(x + dt.x,y + dt.y,z + dt.z);
	}
	Vector3 operator -(Vector3 dt){
		return Vector3(x - dt.x,y + dt.y,z + dt.z);
	}
	Vector3 operator *(double indx){
		return Vector3(x * indx,y * indx,z * indx);
	}
	Vector3 operator /(double div){
		return Vector3(x / div,y / div,z / div);
	}
	Vector3 operator *(Vector3 dt){
		return Vector3(y * dt.z - z * dt.y,z * dt.x - x * dt.z,x * dt.y - y * dt.x);
	}
	double Dot(Vector3 dt){
		return x * dt.x + y * dt.y + z * dt.z;
	}
	double Module2(){
		return x * x + y * y + z * z;
	}
	double Module(){
		return sqrt(x * x + y * y + z * z);
	}
	void operator =(const Vector3 &a){
		x = a.x; y = a.y; z = a.z;
	}
	void Normalize(){
		double len = Module();
		x /= len; y /= len; z /= len;
	}
	void Invert(){
		x = -x; y = -y; z = -z;
	}
	void Read(stringstream &in){
		in >> x >> y >> z;
	}
	void Print(){
		cout << x << y << z;
	}
	bool IsZero(){
		return fabs(x) < Eps && fabs(y) < Eps && fabs(z) < Eps; 
	}
};

#endif
