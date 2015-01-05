#ifndef TRACER_H
#define TRACER_H

#include "common.h"
#include "bmpio.h"

class Ray{
public:
	Ray():Origin(Vector3(0,0,0)),Direction(Vector3(0,0,0)){};
	Ray(const Vector3 &a_Origin,const Vector3 &a_Direction){
		Origin = a_Origin; Direction = a_Direction;		
	}
	void SetOrigin(const Vector3 &a_Origin) {Origin = a_Origin;}
	void SetDirection(const Vector3 &a_Direction) {Direction = a_Direction;}
	Vector3& GetOrigin() {return Origin;}
	Vector3& GetDirection() {return Direction;}

	Vector3 Origin,Direction;
};

class World;
class Primitive;
class Tracer{
public:
	Tracer(){};

	Color Diffuse(Ray X,double dis,Primitive &pri);
	Color Tracing(Ray X,int depth,double nindx);// Changed
};

class Camera{
public:
	double d,start_time,real_time,Dx,Dy;
	int iwidth,iheight,qua;

	Vector3 compass(double i,double j,const int &H,const int &W);
	void TickTick(const char *msg,int done,int all);
	void Shooting();
};

#endif
