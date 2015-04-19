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

//rgbPointer�е�����ָ��ֱ�ָ������ض�Ӧ������QRcode�����ݵ�ɫֵ
struct rgbPointer
{
	unsigned char *_b, *_g, *_r;
	bool data;				//�Ƿ��ǿ��Ա����ǵĲ���
};

//�����ά�����ݵĽṹ
struct QRmap
{
	int version;				//�汾1~40
	int pixelScale;				//ÿ��QRcodeΪ�����ؿ��
	int dataBytes;				//pRGBData�����ֽ���
	int width;					//��ά��QRcode���
	int widthInPix;				//width*pixelScale
	unsigned char* pRGBData;	//�洢����д��BMP������
	rgbPointer *pixelData;		//ָ����ɫ��Ϣ
};

//���ɶ�ά��ṹ
QRmap* generateQRmap(const char* source, int pixelScale = 15, int mod = 5);