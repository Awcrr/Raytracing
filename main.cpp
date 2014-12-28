#include "common.h"
#include "bmpio.h"
#include "tracer.h"
#include "world.h"

using namespace std;

World world;
Bmp bmp;
Tracer tracer;
Camera camera;

char param[Cmd_length],res[Cmd_length];

int main(){
	scanf("%s%s",param,res);
	world.CreateWorld(param);
	bmp.Initialize(camera.iheight,camera.iwidth);
	camera.Shooting();
	bmp.Output(res);
	return 0;
}
