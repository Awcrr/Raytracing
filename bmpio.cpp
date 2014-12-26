#include "bmpio.h"

using namespace std;

void Bmp::Initialize(int H,int W){
	bmpHead.bfType = 0x4D42;
	bmpHead.bfReserved1 = 0;
	bmpHead.bfReserved2 = 0;
	bmpHead.bfOffBits = 54;

	bmpInfo.biSize = 40;
	bmpInfo.biWidth = W;
	bmpInfo.biHeight = H;
	bmpInfo.biPlanes = 1;
	bmpInfo.biBitCount = 24;
	bmpInfo.biCompression = 0;
	bmpInfo.biSizeImage = W * H * 3;
	bmpInfo.biXPelsPerMeter = 0;
	bmpInfo.biYPelsPerMeter = 0;
	bmpInfo.biClrUsed = 0;
	bmpInfo.biClrImportant = 0;
	bmpHead.bfSize = bmpInfo.biSizeImage + bmpInfo.biBitCount;
}

void Bmp::Input(char* fl){
	FILE *in = fopen(fl,"rb");
	if(!in)
		fprintf(stderr,"Image loading error: Failed to read.");
	fread(&bmpHead,1,sizeof(BITMAPFILEHEADER),in);
	fread(&bmpInfo,1,sizeof(BITMAPINFOHEADER),in);
	RGBQUAD palette;
	for(int i = 0,lim = (int)bmpInfo.biClrUsed;i < lim;++ i){
		fread(&palette.rgbBlue,1,sizeof(byte),in);
		fread(&palette.rgbGreen,1,sizeof(byte),in);
		fread(&palette.rgbRed,1,sizeof(byte),in);
	}

	for(int i = 0;i < bmpInfo.biHeight;++ i)
		for(int j = 0;j < bmpInfo.biWidth;++ j){
			fread(&pic[i][j].blue,1,sizeof(byte),in);
			fread(&pic[i][j].green,1,sizeof(byte),in);
			fread(&pic[i][j].red,1,sizeof(byte),in);
		}
	fclose(in);
}

void Bmp::Output(char* fl){
	FILE *out = fopen(fl,"wb");
	fwrite(&bmpHead,1,sizeof(BITMAPFILEHEADER),out);
	fwrite(&bmpInfo,1,sizeof(BITMAPINFOHEADER),out);

	for(int i = 0;i < bmpInfo.biHeight;++ i)
		for(int j = 0;j < bmpInfo.biWidth;++ j){
			fwrite(&pic[i][j].blue,1,sizeof(byte),out);
			fwrite(&pic[i][j].green,1,sizeof(byte),out);
			fwrite(&pic[i][j].red,1,sizeof(byte),out);
		}
	fclose(out);
}
