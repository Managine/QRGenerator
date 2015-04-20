#include "QRmap.h"

//	-------------------------------------------------------
//	DEFines
//	-------------------------------------------------------

#define QRCODE_TEXT					"http://www.ultramundum.org/index.htm";		// Text to encode into QRCode
#define OUT_FILE					"C:/test.bmp"								// Output file name
#define OUT_FILE_PIXEL_PRESCALER	15											// Prescaler (number of pixels in bmp file for each QRCode pixel, on each dimension)

#define PIXEL_COLOR_R				0											// Color of bmp pixels
#define PIXEL_COLOR_G				0
#define PIXEL_COLOR_B				0xff

// BMP defines

typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef signed long		LONG;

#define BI_RGB			0L

//	-------------------------------------------------------
QRmap* generateQRmap(const char* source, int pixelScale, int mod)
{
	char*			szSourceSring = QRCODE_TEXT;
	unsigned int	unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
	unsigned char*	pRGBData, *pSourceData, *pDestData;
	QRcode*			pQRC;
	rgbPointer *pixels;
	if (pQRC = QRcode_encodeString(source, 0, QR_ECLEVEL_H, QR_MODE_8, 1))
	{
		unWidth = pQRC->width;
		unWidthAdjusted = unWidth * pixelScale * 3;
		if (unWidthAdjusted % 4)
			unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
		unDataBytes = unWidthAdjusted * unWidth * pixelScale;

		//bool *maskData = generateMask(pQRC->version, pQRC->width);

		if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
		{
			printf("Out of memory");
			exit(-1);
		}


		memset(pRGBData, 0xff, unDataBytes);

		pixels = new rgbPointer[unWidth * pixelScale *  unWidth * pixelScale];
		rgbPointer *pointPix = pixels;

		pSourceData = pQRC->data;
		int pixPerLine = unWidth * pixelScale;

		for (y = 0; y < unWidth; y++)
		{
			pDestData = pRGBData + unWidthAdjusted * y * pixelScale;
			pointPix = pixels + y * pixPerLine * pixelScale;
			for (x = 0; x < unWidth; x++)
			{
				if (*pSourceData & 1)
				{
					for (l = 0; l < pixelScale; l++)
					{
						for (n = 0; n < pixelScale; n++)
						{
							*(pDestData +     n * 3 + unWidthAdjusted * l) = 0;
							*(pDestData + 1 + n * 3 + unWidthAdjusted * l) = 0;
							*(pDestData + 2 + n * 3 + unWidthAdjusted * l) = 0;
						}
					}
				}
				/*
				for (l = 0; l < pixelScale; l++)
				{
					for (n = 0; n < pixelScale; n++)
					{
						(pointPix + n + l*pixPerLine)->data = mask[y*unWidth + x];
					}
				}
				*/
				/*
				if (*mask == false)
				{
					for (l = 0; l < pixelScale; l++)
					{
						for (n = 0; n < pixelScale; n++)
						{
							*(pDestData + n * 3 + unWidthAdjusted * l) = 0;
							*(pDestData + 1 + n * 3 + unWidthAdjusted * l) = 0;
							*(pDestData + 2 + n * 3 + unWidthAdjusted * l) = 0xff;
						}
					}
				}
				*/
				pDestData += 3 * pixelScale;
				pointPix += pixelScale;
				pSourceData++;
			}
		}
		int widthInPix = unWidth*pixelScale;
		pointPix = pixels;
		bool *mask = generateMask(pQRC->version, pQRC->width);

		//adjusted for different kind of Halftone
		int onethird, twothird;
		if (mod == 5)
		{
			onethird = pixelScale / 5 * 2;
			twothird = pixelScale / 5 * 3;
		}
		else
		{
			onethird = pixelScale / 3;
			twothird = pixelScale / 3 * 2;
		}

		for (int i = 0; i < widthInPix; i++)
		{
			pDestData = pRGBData + unWidthAdjusted * i;
			for (int j = 0; j < widthInPix; j++)
			{
				pointPix->_b = pDestData++;
				pointPix->_g = pDestData++;
				pointPix->_r = pDestData++;
				pointPix->data = mask[i / pixelScale *unWidth + j / pixelScale];
				int lefti = i%pixelScale;
				int leftj = j%pixelScale;
				if (lefti >= onethird&&lefti < twothird && leftj >= onethird&&leftj < twothird)
					pointPix->data = 0;
				pointPix++;
			}
		}
		delete []mask;
	}
	QRmap *map = new QRmap;
	map->pixelScale = pixelScale;
	map->version = pQRC->version;
	map->pRGBData = pRGBData;
	map->dataBytes = unDataBytes;
	map->width = unWidth;
	map->pixelData = pixels;
	map->widthInPix = unWidth*pixelScale;
	free(pQRC);
	return map;
}
