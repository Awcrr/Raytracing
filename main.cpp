#include "common.h"
#include "bmpio.h"
#include "tracer.h"
#include "world.h"

using namespace std;

World world;
Bmp bmp;
Tracer tracer;
Camera camera;

int main(){
	world.CreateWorld("Parameters.txt");
	bmp.Initialize();
	camera.shooting();
	bmp.output("result.bmp");
	return 0;
}
