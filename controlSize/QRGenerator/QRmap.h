#include "stdafx.h"
#include <string.h>
#include <errno.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include "qrencode.h"
#include "maskGenerate.h"

//rgbPointer中的三个指针分别指向该像素对应的生成QRcode的数据的色值
struct rgbPointer
{
	unsigned char *_b, *_g, *_r;
	bool data;				//是否是可以被覆盖的部分
};

//保存二维码数据的结构
struct QRmap
{
	int version;				//版本1~40
	int pixelScale;				//每个QRcode为的像素宽度
	int dataBytes;				//pRGBData数据字节数
	int width;					//二维码QRcode宽度
	int widthInPix;				//width*pixelScale
	unsigned char* pRGBData;	//存储用于写入BMP的数据
	rgbPointer *pixelData;		//指向颜色信息
};

//生成二维码结构
QRmap* generateQRmap(const char* source, int pixelScale = 15, int mod = 5);