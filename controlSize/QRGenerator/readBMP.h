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

//����BMPͷ�ṹ�Ľṹ��
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

//rgbPix�е�����ָ��ֱ�ָ������ض�Ӧ��BMP�ж��������ݵ�ɫֵ
struct rgbPix
{
	unsigned char *_b, *_g, *_r, *_s, *_e;
};

//�洢��������ͼƬ��Ϣ�Ľṹ
struct rawRGB
{
	int _width, _height;	//ͼƬ��ȣ��߶�/����
	int _xOffset, _yOffset;	//��ʼλ��λ��ͼƬ��ƫ����
	unsigned char *rawData; //��ͼƬ�ļ���ֱ�Ӷ�ȡ������
	int rawDataLength;		//�����ֽ���
	rgbPix *_pixels;		//���ʸ�������ʹ�õ�ָ��
	rgbPix *_head;			//���ʵ���ʼλ�ã���_xOffset, _yOffsetͬ����

	bool *_mask;
};


rawRGB *readBMP(const char* filePass);										//��ȡBMP��ʽͼƬ��rawRGB�ṹ��

rgbPix *getPix(rawRGB *raw, int x, int y);									//����x���ᣩ��y���ݣ�λ�õõ�rgbPix�ṹָ�룬����ǷǷ������򷵻�NULL

rawRGB *adjustImage(rawRGB *source, float size, float x, float y);			//��ͼƬ���ţ�ƽ�Ƶ���Ӧλ��

void generateBMP(QRmap *map, const char *pass, int QRwidth);				//����BMP�ļ�����ӦĿ¼