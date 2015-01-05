#ifndef WORLD_H
#define WORLD_H

#include "common.h"
#include "bmpio.h"
#include "tracer.h"

extern Camera camera;

class Material{
public:
	Material();
	Material(const Color &_c,double _r,double _d,double _s,double _refr,double _rindx):col(_c),refl(_r),diff(_d),spec(_s),refr(_refr),rindx(_rindx){};

	Color col;
	double refl,diff,spec,refr,rindx;
};

class Primitive{
public:
	enum{
		SPHERE = 1,
		PLANE
	};
	virtual ~Primitive(){};
	virtual int Intersect(Ray X,double &dis) = 0;
	virtual Vector3 GetNormal(Vector3 &pi) = 0;

	short type;
	int id;
	Material material;
	Primitive *next;
};

class Sphere: public Primitive{
public:
	Sphere(const Vector3 &_o,const double &_r):O(_o),R(_r){
		type = SPHERE; id = 0; next = NULL;
	};
	~Sphere(){};
	int Intersect(Ray X,double &dis);
	Vector3 GetNormal(Vector3 &pi){
		return (pi - O) * R;
	}

	Vector3 O;
	double R;
};

class Plane: public Primitive{
public:
	Plane(const Vector3 &_n,const double &_d):N(_n),D(_d){
		type = PLANE; id = 0; next = NULL;
	};
	~Plane(){};
	int Intersect(Ray X,double &dis);
	Vector3 GetNormal(Vector3 &pi){
		return N;
	}

	Vector3 N;
	double D;
};

class Light{
public:
	enum{
		POINT = 1,
		AREA
	};
	Light();
	int Intersect(Ray X,double &dis);
	
	short type;
	int id;
	Light *next;
	Vector3 O;
	Color col;
};

class World{
public:
	World();
	~World();
	void SetBackground(FILE *in);
	void SetSphere(FILE *in);
	void SetPlane(FILE *in);
	void SetCamera(FILE *in);
	void SetLight(FILE *in);
	void CreateWorld(const char *fl);

	Color background;
	Primitive *headPrimitive;
	Light *headLight;
};

#endif
