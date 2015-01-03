#ifndef COMMON_H
#define COMMON_H

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "const.h"

using namespace std;

class Vector3{
public:
	double x,y,z;
	Vector3(double x = 0.0,double y = 0.0,double z = 0.0):x(x),y(y),z(z){};
	~Vector3(){}
	Vector3 operator +(Vector3 dt){
		return Vector3(x + dt.x,y + dt.y,z + dt.z);
	}
	Vector3 operator -(Vector3 dt){
		return Vector3(x - dt.x,y - dt.y,z - dt.z);//Debug
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
	double Dis2(Vector3 dt){
		return (x - dt.x) * (x - dt.x) + (y - dt.y) * (y - dt.y) + (z - dt.z) * (z - dt.z);
	}
	double Dis(Vector3 dt){
		return sqrt(Dis2(dt));
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
		printf("%.3lf %.3lf %.3lf\n",x,y,z);
	}
	bool IsZero(){
		return fabs(x) < Eps && fabs(y) < Eps && fabs(z) < Eps; 
	}
};

#endif
