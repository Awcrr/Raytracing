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

int Light::Intersect(Ray X,double &dis){
	double tmp;
	int ret = MISS;
	if(type == POINT){
		Vector3 a = O - X.Origin;
		if(true){//To be changed
			tmp = X.Origin.Dis(O);
			if(dis - tmp > Eps){
				dis = tmp;
				ret = HIT;
			}
		}
	}
	return ret;
}

int Sphere::Intersect(Ray X,double &dis){
	Vector3 a = X.Origin - O;
	double b = -X.Direction.Dot(a);
	double dt = b * b - a.Dot(a) + R * R;
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
	headPrimitive = NULL;
	headLight = NULL;
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

void World::SetBackground(FILE *in){
	double r,g,b; char c;
	while((c = fgetc(in)) != '=');
	fscanf(in,"%lf%lf%lf",&r,&g,&b);
	background = Color(r,g,b);
}

void World::SetSphere(FILE *in){
	double x,y,z,r,refl,diff,R,G,B; char c;
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%lf%lf%lf",&x,&y,&z);
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%lf",&r);
	Sphere *now = new Sphere(Vector3(x,y,z),r);
	now->next = headPrimitive;
	while((c = fgetc(in)) != '=');
	fscanf(in,"%lf",&refl);
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%lf",&diff);
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%lf%lf%lf",&R,&G,&B);
	now->material = Material(Color(R,G,B),refl,diff);
	headPrimitive = now;
}

void World::SetPlane(FILE *in){
	double x,y,z,d,refl,diff,R,G,B; char c;
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%lf%lf%lf",&x,&y,&z);
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%lf",&d);
	Plane *now = new Plane(Vector3(x,y,z),d);
	now->next = headPrimitive;
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%lf",&refl);
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%lf",&diff);
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%lf%lf%lf",&R,&G,&B);
	now->material = Material(Color(R,G,B),refl,diff);
	headPrimitive = now;
}

void World::SetCamera(FILE *in){
	char c;
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%lf",&camera.lwidth);
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%lf",&camera.lheight); 
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%d",&camera.iwidth);
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%d",&camera.iheight);
	while((c = fgetc(in)) != '='); 
	fscanf(in,"%lf",&camera.d);
}

void World::SetLight(FILE *in){
	char tp[Cmd_length],c;
	Light *now = new Light;
	while((c = fgetc(in)) != '=');
	fscanf(in,"%s",tp);
	if(tp[0] == 'p') now->type = Light::POINT;
	else now->type = Light::AREA;
	double x,y,z,R,G,B;
	while((c = fgetc(in)) != '=');
	fscanf(in,"%lf%lf%lf",&x,&y,&z);
	now->O = Vector3(x,y,z);
	while((c = fgetc(in)) != '=');
	fscanf(in,"%lf%lf%lf",&R,&G,&B);
	now->col = Color(R,G,B);
	now->next = headLight;
	headLight = now;
}

void World::CreateWorld(char *fl){
	FILE *parameter = fopen(fl,"r");
	char obj[Cmd_length];
	while(fscanf(parameter,"%s",obj) != EOF){
		if(obj[0] == 'b') 
			SetBackground(parameter);
		else if(obj[0] == 's')
			SetSphere(parameter);
		else if(obj[0] == 'p')
			SetPlane(parameter);
		else if(obj[0] == 'c')
			SetCamera(parameter);
		else if(obj[0] == 'l')
			SetLight(parameter);
		else{
			fprintf(stderr,"Illegal object type.\n");
			exit(0);
		}
	}
	//
	puts("The initial world has been constructed.");
	//
	fclose(parameter);
}
