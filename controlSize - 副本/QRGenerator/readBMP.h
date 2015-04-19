#include <string.h>
#include <errno.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>

#include "QRmap.h"

typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef signed long		LONG;
#define BI_RGB			0L

#pragma pack(push, 2)

//保存BMP头结构的结构体
typedef struct
{
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
	DWORD      biSize;
	LONG       biWidth;
	LONG       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(pop)

//rgbPix中的三个指针分别指向该像素对应的BMP中读出的数据的色值
struct rgbPix
{
	unsigned char *_b, *_g, *_r, *_s, *_e;
};

//存储读进来的图片信息的结构
struct rawRGB
{
	int _width, _height;	//图片宽度，高度/像素
	int _xOffset, _yOffset;	//起始位置位于图片的偏移量
	unsigned char *rawData; //从图片文件中直接读取的数据
	int rawDataLength;		//数据字节数
	rgbPix *_pixels;		//访问各个像素使用的指针
	rgbPix *_head;			//访问的起始位置（和_xOffset, _yOffset同步）

	bool *_mask;
};


rawRGB *readBMP(const char* filePass);										//读取BMP格式图片到rawRGB结构中

rgbPix *getPix(rawRGB *raw, int x, int y);									//根据x（横），y（纵）位置得到rgbPix结构指针，如果是非法请求则返回NULL

rawRGB *adjustImage(rawRGB *source, float size, float x, float y);			//将图片缩放，平移到对应位置

void generateBMP(QRmap *map, const char *pass, int QRwidth);				//生成BMP文件到对应目录