#ifndef WORLD_H
#define WORLD_H

#include "common.h"
#include "bmp.h"
#include "tracer.h"

extern camera;

class Material{
public:
	Material(){};

	Color col;
	double refl,diff;
}

class Primitive{
public:
	enum{
		SPHERE = 1,
		PLANE
	};
	short type;
	int id;
	Material material;
	Primitive *next;
}

class Sphere: public Primitive{
public:
	Sphere(const Vector3 &_o,const double &_r):O(_o),R(_r){
		type = SPHERE; isLight = false;
	};
	Vector3 O;
	double R;
}

class Plane: public Primitive{
public:
	Plane(const Vector3 &_n,const double &_d):N(_n),D(_d){
		type = PLANE;
	};
	Vector3 N;
	double D;
}

class Light{
public:
	enum{
		POINT = 1,
		AREA
	};
	Light(){};
	
	short type;
	int id;
	Light *next;
	Vector3 O;
}

class World{
public:
	World(){};
	~World(){};
	void SetBackground(ifstream &in);
	void SetPrimitive(ifstream &in);
	void SetLight(ifstream &in);
	void CreateWorld(char *fl);

	Color background;
	Primitive *headPrimitive;
	Light *headLight;
}

#endif
