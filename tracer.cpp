#include "common.h"
#include "bmpio.h"
#include "world.h"
#include "tracer.h"

using namespace std;

extern World world;
extern Bmp bmp;
extern Camera camera;
extern Tracer tracer;

Color Tracer::Diffuse(Ray X,double dis,Primitive &pri){
	Vector3 pi,p;	
	pi = X.Origin + X.Direction * dis;
	Color ret;
	for(Light *now = world.headLight;now;now = now->next){
		Vector3 L = now->O - pi;
		L.Normalize();
		p = pi;
		p.Normalize();
		if(pri.material.diff > Eps){
			double dot = L.Dot(p);
			if(dot > Eps){
				double diff = dot * pri.material.diff;
				ret = ret + pri.material.col * now->col * diff;
			}
		}
	}
	return ret;
}

Color Tracer::Tracing(Ray X){
	double dis = 1e9;
	int ret,tmp;
	Primitive *pri = NULL;
	for(Primitive *now = world.headPrimitive;now;now = now->next){
		tmp = now->Intersect(X,dis);
		if(tmp){
			pri = now;
			ret = tmp;
		}
	}
	Light *lit = NULL;
	for(Light *now = world.headLight;now;now = now->next){
		tmp = now->Intersect(X,dis);
		if(tmp){
			lit = now;
			ret = tmp;
		}
	}
	Color result;
	if(lit != NULL) return lit->col;
	if(pri != NULL)
		result = Diffuse(X,dis,*pri);
	result.Limit();
	return result;
}

Vector3 Camera::compass(int i,int j,const int &H,const int &W){
	Vector3 term(((double)i * 2 / W - 1) * (lwidth / 2),0,((double)j * 2 / H - 1) * (lheight / 2));
	return term;
}

void Camera::TickTick(char *msg,int done,int all){
	double now = clock();
	if(done == 0 || done == all || (now - real_time) / CLOCKS_PER_SEC > 1){
		real_time = now;
		printf("Time flies: %.2lf Secs\n",(real_time - start_time) / (CLOCKS_PER_SEC));
		printf("%s %d/%d [%.2lf%%]\n",msg,done,all,100.0 * done / all);
	}
}	

void Camera::Shooting(){
	int H = bmp.biHeight;
	int W = bmp.biWidth;
	Vector3 O(0,0,-d),v;
	start_time = clock(); real_time = start_time;
	TickTick("Start shooting...",0,1);

	for(int j = 0;j < H;++ j){
		for(int i = 0;i < W;++ i){
			Color acc(0,0,0);
			v = compass(i + 1,j + 1,H,W) - O;
			v.Normalize();
			/*
			if(j + 1 == (H >> 1)){
			printf("%d %d\n",i + 1,j + 1);
			printf("%.2lf %.2lf %2lf\n",v.x,v.y,v.z);
			}
			*/
			Ray X(O,v);
			acc = tracer.Tracing(X);
			bmp.pic[i * H + W].red = (int)(acc.r * 256);
			bmp.pic[i * H + W].green = (int)(acc.g * 256);
			bmp.pic[i * H + W].blue = (int)(acc.b * 256);
			//bmp.pic[i * H + W].legal();
		}
		TickTick("Shooting...",j + 1,H);
	}
}
