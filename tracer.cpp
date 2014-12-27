#include "common.h"

using namespace std;

extern world;
extern bmp;
extern camera;
extern tracer;

Color Tracer::Diffuse(Ray X,double dis,Primitive pri){
	Vector3 pi;
	pi = X.Origin + X.Direction * dis;

}

Color Tracer::Tracing(Ray X){
	double dis = 1e9;
	int ret,tmp;
	Primitive *pri = NULL;
	for(Primitive *now = headPrimitive;now;now = now->next){
		tmp = now->Intersect(X,dis);
		if(tmp){
			pri = now;
			ret = tmp;
		}
	}
	Light *lit = NULL;
	for(Light *now = headLight;now;now = now->next){
		tmp = now->Intersect(X,dis);
		if(tmp){
			lit = now;
			ret = tmp;
		}
	}
	Color result;
	if(lit != NULL) return lit->col;
	if(pri != NULL)
		result = Diffuse(pri,);
	result.limit();
	return result;
}
/*
void Camera::TickTick(char *msg,int done,int all){
			
}	
*/
void Camera::Shooting(){
	int H = bmp.bmpInfo.biHeight;
	int W = bmp.bmpInfo.biWidth;
	Vector3 O(0,-d,0);

	for(int i = 0;i < H;++ i)
		for(int j = 0;j < W;++ j){
			Color acc(0,0,0);
			v = compass(i,j);
			v.Normalize();
			Ray X(O,v);
			acc = Tracing(X);
			bmp.pic[i][j].r = acc.r * 255;
			bmp.pic[i][j].g = acc.g * 255;
			bmp.pic[i][j].b = acc.b * 255;
		}
}
