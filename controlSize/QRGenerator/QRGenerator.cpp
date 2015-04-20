//	---------------------------------------------------------------------------
//		
//		QRGenerator
//
//		Create:			15/05/2013
//		Last update:	15/05/2013
//
//		Author:	TWOTM
//
//
//		Note:
//
//		/o ULTRAMUNDUM FOUNDATION - all rights reserved
//	---------------------------------------------------------------------------


//	-------------------------------------------------------
//	Includes
//	-------------------------------------------------------
#include "QRGenerator.h"
//	-------------------------------------------------------


//	-------------------------------------------------------
//	DEFines
//	-------------------------------------------------------

#define QRCODE_TEXT					"http://www.ultramundum.org/index.htm";		// Text to encode into QRCode
#define OUT_FILE					"D:/test.bmp"								// Output file name
#define OUT_FILE_PIXEL_PRESCALER	15											// Prescaler (number of pixels in bmp file for each QRCode pixel, on each dimension)

#define PIXEL_COLOR_R				0											// Color of bmp pixels
#define PIXEL_COLOR_G				0
#define PIXEL_COLOR_B				0xff

int _tmain(int argc, _TCHAR* argv[]){}

//	-------------------------------------------------------

//	-------------------------------------------------------
//	Main
//	-------------------------------------------------------
extern "C" __declspec(dllexport) void __stdcall generate(char* path, char* input, float x, float y, int width, float minpro, float adjust, float sacrificeRate)
{

		// Compute QRCode
		QRmap *map = generateQRmap(input,15,5);

		rawRGB *raw1 = readBMP(path);

		rawRGB *raw = adjustImage(raw1,map->widthInPix/float(width), x, y);
		//simpleBAW(raw);
		//if (type==2)
		//	lottery(raw, map->widthInPix, (int)(map->pixelScale/3),2-pattern, axis1, axis2);
		//else if (type==4)
		levelHalftone(raw, map, minpro, adjust, sacrificeRate);
		//else if (type==1)
		//	halfTone(raw, map->widthInPix, level, 5);
		//else
		//	superHalftone(raw, map, minpro);
		combine(raw, map);
		/*
		int tt = map->widthInPix;
		for (int i = 0; i < tt; ++i)
		{
			for (int j = 0; j < tt; ++j)
			{
				if (map->pixelData[i*tt + j].data)
				{
					//*(map->pixelData[i*tt + j]._b) = *(raw->_pixels[i*raw->_width + j]._b);
					//*(map->pixelData[i*tt + j]._g) = *(raw->_pixels[i*raw->_width + j]._g);
					//*(map->pixelData[i*tt + j]._r) = *(raw->_pixels[i*raw->_width + j]._r);

					rgbPix *temp = getPix(raw, j, i);
					if (temp != NULL)
					{
						*(map->pixelData[i*tt + j]._b) = *(temp->_b);
						*(map->pixelData[i*tt + j]._g) = *(temp->_g);
						*(map->pixelData[i*tt + j]._r) = *(temp->_r);
					}
				}
			}
		}
		*/
			// Prepare bmp headers
		/*
		BITMAPFILEHEADER kFileHeader;
		kFileHeader.bfType = 0x4d42;  // "BM"
		kFileHeader.bfSize =	sizeof(BITMAPFILEHEADER) +
								sizeof(BITMAPINFOHEADER) +
								map->dataBytes;
		kFileHeader.bfReserved1 = 0;
		kFileHeader.bfReserved2 = 0;
		kFileHeader.bfOffBits =	sizeof(BITMAPFILEHEADER) +
								sizeof(BITMAPINFOHEADER);

		BITMAPINFOHEADER kInfoHeader;
		kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		kInfoHeader.biWidth = map->width * OUT_FILE_PIXEL_PRESCALER;
		kInfoHeader.biHeight = -((int)map->width * OUT_FILE_PIXEL_PRESCALER);
		kInfoHeader.biPlanes = 1;
		kInfoHeader.biBitCount = 24;
		kInfoHeader.biCompression = BI_RGB;
		kInfoHeader.biSizeImage = 0;
		kInfoHeader.biXPelsPerMeter = 0;
		kInfoHeader.biYPelsPerMeter = 0;
		kInfoHeader.biClrUsed = 0;
		kInfoHeader.biClrImportant = 0;

			// Output the bmp file

		if (!(fopen_s(&f, OUT_FILE, "wb")))
			{
			fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, f);
			fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);
			fwrite(map->pRGBData, sizeof(unsigned char), map->dataBytes, f);
			
			fclose(f);
			}
		else
			{
			printf("Unable to open file");
			exit(-1);
			}
			*/
		generateBMP(map,"tmp.bmp", (int)map->pixelScale);
}
//	-------------------------------------------------------

int x(int a)
{
	return a;
}