#ifndef TRACER_H
#define TRACER_H

#include "common.h"

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
}

class World;
class Primitive;
class Tracer{
public:
	Tracer(){};

	Color Tracing();
}

class Camera{
	Color col[Max_pixel][Max_pixel];

	void Shooting();
}

#endif
