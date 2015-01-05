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
	Vector3 pi,N;	
	pi = X.Origin + X.Direction * dis;
	Color ret;
	double shade,rdis;
	for(Light *now = world.headLight;now;now = now->next){
		Vector3 L = now->O - pi;
		//Add Shadow
		rdis = L.Module();
		//
		L.Normalize();
		N = pri.GetNormal(pi);
		N.Normalize();
		//Add Shadow
		shade = 1.0;
		if(now->type == Light::POINT){
			Ray reflectToLight = Ray(pi + L * EPSILON,L);
			for(Primitive *cur = world.headPrimitive;cur;cur = cur->next)
			if(cur->Intersect(reflectToLight,rdis)){
				shade = 0; break;
			}
		}
		//
		if(pri.material.diff > Eps){
			double dot = L.Dot(N);
			if(dot > Eps){
				double diff = dot * pri.material.diff;
				ret = ret + pri.material.col * now->col * diff * shade;
				/*
				printf("Color : %.2lf %.2lf %.2lf\n",pri.material.col.r,pri.material.col.g,pri.material.col.b);
				*/
			}
		}
		if(pri.material.spec > Eps){
			Vector3 fakeReflection = L - (N * (2.0 * L.Dot(N))); 
			double dot = X.Direction.Dot(fakeReflection);
			if(dot > Eps){
				double halo = pow(dot,28) * pri.material.spec;
				ret = ret + pri.material.col * halo * shade;
			}
		}
	}
	return ret;
}

Color Tracer::Tracing(Ray X,int depth,double nindx){//Changed
	double dis = 1e9;
	int ret,tmp;
	Primitive *pri = NULL;
	for(Primitive *now = world.headPrimitive;now;now = now->next){
		tmp = now->Intersect(X,dis);
		if(tmp){
			pri = now;
			ret = tmp;
			/*
			printf("Hit %.2lf\n",dis);
			*/
		}
	}
	Light *lit = NULL;
	for(Light *now = world.headLight;now;now = now->next){
		tmp = now->Intersect(X,dis);
		if(tmp){
			lit = now;
			ret = tmp;
			/*
			printf("Lit %.6lf %.6lf %.6lf\n",X.Direction.x,X.Direction.y,X.Direction.z);
			*/
			/*
			puts("Lit");
			*/
		}
	}
	/*
	if(lit != NULL) puts("lit");
	else if(pri != NULL) puts("pri");
	else puts("Nope");
	*/
	Color result;
	if(lit != NULL) return lit->col;
	if(pri != NULL){
		result = Diffuse(X,dis,*pri);
		Vector3 pi = X.Origin + X.Direction * dis;
		Vector3 N = pri->GetNormal(pi);
		if(pri->material.refl > Eps){
			Vector3 Reflection = X.Direction - (N * (2.0 * X.Direction.Dot(N)));
			if(depth < TRACEDEPTH){
				/*Debug
					printf("Depth%d Hit Ball %.2lf\n",depth,((Sphere*)pri)->R); 
					printf("At "); pi.Print(); printf("Ref "); Reflection.Print(); puts("");	
				*/
				Reflection.Normalize();
				result = result + (pri->material.col * (Tracing(Ray(pi + Reflection * EPSILON,Reflection),depth + 1,nindx) * pri->material.refl));
				/*
					if(((Sphere*)pri)->R == 2){printf("Here end %d ",depth); result.Print();}
				*/
			}
		}
		if((pri->material.refr) > Eps && (depth < TRACEDEPTH)){
			double rindx = pri->material.rindx;
			//if(ret == -1) rindx = 1.0;
			double n = nindx / rindx;
			N = N * (double)ret;
			double cosI = -X.Direction.Dot(N);
			double cosT2 = 1.0 - n * n * (1.0 - cosI * cosI);
			if(cosT2 > Eps){
				double cosT = sqrt(cosT2);
				Vector3 T = X.Direction * n + N * (n * cosI - cosT);
				T.Normalize();
				result = result + Tracing(Ray(pi + T * EPSILON,T),depth + 1,rindx) * pri->material.refr * pri->material.col;
			}
		}
	}else result = world.background; 
	/*Debug
	if(pri && pri->type == Primitive::SPHERE){
		printf("Dep: %d\n",depth);
		Vector3 pi = X.Origin + X.Direction * dis;
		pi.Print();
		result.Print();
	}
	*/
	result.Limit();
	return result;
}

Vector3 Camera::compass(double i,double j,const int &H,const int &W){
	Vector3 term((i - (double)W / 2.0) * Dx,(j - (double)H / 2.0) * Dy,0);
	return term;
}

void Camera::TickTick(const char *msg,int done,int all){
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
	Vector3 eye(0,0,-d),v;
	start_time = clock(); real_time = start_time;
	TickTick("Start shooting...",0,1);

	for(int j = 0;j < H;++ j){
		for(int i = 0;i < W;++ i){
			Color acc(0,0,0);
			int mid = qua >> 1;
			for(int di = 1;di <= qua;++ di)
				for(int dj = 1;dj <= qua;++ dj){
					v = compass(i + (double)di / qua - (double)mid / qua + 1,j + (double)dj / qua - (double)mid / qua + 1,H,W) - eye;
					v.Normalize();
					Ray X(eye,v);
					acc = acc + tracer.Tracing(X,1,1.0);
				}
			acc = acc / (double)(qua * qua);

			bmp.pic[j * W + i].red = (int)(acc.r * 256);
			bmp.pic[j * W + i].green = (int)(acc.g * 256);
			bmp.pic[j * W + i].blue = (int)(acc.b * 256);
			bmp.pic[j * W + i].legal();
		}
		TickTick("Shooting...",j + 1,H);
	}
}
