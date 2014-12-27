#include "world.h"

using namespace std;

Material::Material(){
	col = Color();
	refl = 0.0;
	diff = 0.0;
}

Light::Light(){
	type = 1;
	id = 0;
	next = NULL;
}

int Sphere::Intersect(Ray X,double &dis){
	Vector3 a = X.Origin - O;
	double b = -X.Direction.Dot(a);
	double dt = b * b - a * a + R * R;
	int ret = MISS;
	if(dt > Eps){
		double t1,t2;
		t1 = b - dt; t2 = b + dt;
		if(t2 > Eps){
			if(t1 > Eps){
				if(dis - t1 > Eps){
					dis = t1;
					ret = HIT;
				}
			}else{
				if(dis - t2 > Eps){
					dis = t2;
					ret = INPRIM; 
				}
			}
		}
	}
	return ret;
}

int Plane::Intersect(Ray X,double &dis){
	double d = X.Direction.Dot(N);
	int ret = MISS;
	if(fabs(d) > Eps){
		double t = (N * D - X.Origin).Dot(N) / d;
		if(dis > Eps && dis - t > Eps){
			dis = t;
			ret = HIT;
		}
	}
	return ret;
}

World::World(){
	background = Color();
	headPrimitive = headLight = NULL;
}

World::~World(){
	Primitive *toBeDel0;
	while(headPrimitive != NULL){
		toBeDel0 = headPrimitive;
		headPrimitive = headPrimitive->next;
		delete toBeDel0;
	}
	Light *toBeDel1;
	while(headLight != NULL){
		toBeDel1 = headLight;
		headLight = headLight->next;
		delete toBeDel1;
	}
}

void World::SetBackground(ifstream &in){
	double r,g,b; char c;
	while((c = fgetc(in)) != '=');
	in >> r >> g >> b;
	background = Color(r,g,b);
}

void World::SetSphere(ifstream &in){
	double x,y,z,r,refl,diff,R,G,B; char c;
	while((c = fgetc(in)) != '='); in >> x >> y >> z;
	while((c = fgetc(in)) != '='); in >> r;
	Sphere *now = new Sphere(Vector3(x,y,z),r);
	now->next = headPrimitive;
	while((c = fgetc(in)) != '='); in >> refl;
	while((c = fgetc(in)) != '='); in >> diff;
	while((c = fgetc(in)) != '='); in >> R >> G >> B;
	now->material = Material(Color(R,G,B),refl,diff);
	headPrimitive = now;
}

void World::SetPlane(ifstream &in){
	double x,y,z,d,refl,diff,R,G,B; char c;
	while((c = fgetc(in)) != '='); in >> x >> y >> z;
	while((c = fgetc(in)) != '='); in >> d;
	Plane *now = new Plane(Vector3(x,y,z),r);
	now->next = headPrimitive;
	while((c = fgetc(in)) != '='); in >> refl;
	while((c = fgetc(in)) != '='); in >> diff;
	while((c = fgetc(in)) != '='); in >> R >> G >> B;
	now->meterial = Material(Color(R,G,B),refl,diff);
	headPrimitive = nowk;
}

void World::SetCamera(ifstream &in){
	char c;
	while((c = getchar(in)) != '='); in >> camera.lwidth;
	while((c = getchar(in)) != '='); in >> camera.rwidth;
	while((c = getchar(in)) != '='); in >> camera.iwidth;
	while((c = getchar(in)) != '='); in >> camera.iheight;
	while((c = getchar(in)) != '='); in >> camera.d;
}

void World::SetLight(ifstream &in){
	string tp;
	while((c = getchar(in)) != '='); in >> tp;
	if(tp == "point") type = Light::POINT;
	else type = Light::AREA;
	Light *now = new Light;
	double x,y,z,R,G,B;
	while((c = getchar(in)) != '='); in >> x >> y >> z;
	now->O = Vector3(x,y,z);
	while((c = getchar(in)) != '='); in >> R >> G >> B;
	now->col = Vector3(R,G,B);
	now->next = headLight;
	headLight = now;
}

void World::CreateWorld(char *fl){
	FILE parameter = open(fl,"r");
	string obj;
	while(parameter >> obj){
		if(obj == "background") 
			SetBackground(parameter);
		else if(obj == "Sphere")
			SetSphere(parameter);
		else if(obj == "Plane")
			SetPlane(parameter);
		else if(obj == "Camera")
			SetCamera(parameter);
		else if(obj == "Light")
			SetLight(parameter);
		else
			fprintf(stderr,"Illegal object type.");
	}
}
