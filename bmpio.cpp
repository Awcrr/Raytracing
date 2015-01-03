#include "bmpio.h"

using namespace std;

void rgb::print() { printf("RGB : %d %d %d\n",red,green,blue); }

void rgb::legal(){
	if(red > 255) red = 255;
	if(green > 255) green = 255;
	if(blue > 255) blue = 255;
}

int __BMPIO_binread(unsigned char *s,int p){
	return (int(s[p])) | (int(s[p + 1]) << 8) | (int(s[p + 2]) << 16) | (int(s[p + 3]) << 24);
}

void __BMPIO_binwrite(unsigned char *s,int p,int x){
	s[p ++] = x & BIT; s[p ++] = (x >> 8) & BIT; s[p ++] = (x >> 16) & BIT; s[p ++] = (x >> 24) & BIT;
}

void Bmp::Initialize(const int &H,const int &W){
	pic = new rgb[H * W];
	biHeight = H; biWidth = W;
	for(int i = 0,lim = H * W;i < lim;++ i) pic[i] = rgb(0,0,0);
}

void Bmp::Input(const char *fl){
	FILE *in = fopen(fl,"rb");
	if(!in){
		fprintf(stderr,"Failed to read file %s\n",fl);
		exit(0);
	}
	unsigned char header[54];
	fread(header,sizeof(unsigned char),54,in);
	biWidth = __BMPIO_binread(header,18); biHeight = __BMPIO_binread(header,22);
	unsigned char *img;
	img = new unsigned char[biWidth * biHeight * 3 + 100];
	fread(img,sizeof(unsigned char),biWidth * biHeight * 3,in);
	fclose(in);
	int cnt = 0;
	pic = new rgb[biWidth * biHeight];
	for(int i = 0;i < biWidth * biHeight;++ i){
		pic[i].blue = img[cnt ++]; pic[i].green = img[cnt ++]; pic[i].red = img[cnt ++]; 	
	}
}

void Bmp::Output(const char *fl){
	FILE *out = fopen(fl,"wb");
	if(!out){
		fprintf(stderr,"Failed to write file %s\n",fl);
		exit(0);
	}
	unsigned char header[54] = {
		0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
		54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0
	};
	__BMPIO_binwrite(header,2,biWidth * biHeight * 3 + 54);
	__BMPIO_binwrite(header,18,biWidth);
	__BMPIO_binwrite(header,22,biHeight);

	fwrite(header,sizeof(unsigned char),54,out);
	unsigned char *img; img = new unsigned char[biWidth * biHeight * 3 + 100];
	int cnt = 0;
	for(int i = 0,lim = biWidth * biHeight;i < lim;++ i){
		img[cnt ++] = pic[i].blue; img[cnt ++] = pic[i].green; img[cnt ++] = pic[i].red;
		/*
		printf("%d %d %d\n",pic[i].blue,pic[i].green,pic[i].red);
		*/
	}
	fwrite(img,sizeof(unsigned char),biWidth * biHeight * 3,out);
	fclose(out);
}
