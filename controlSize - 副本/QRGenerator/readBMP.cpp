#include "readBMP.h"

rawRGB *readBMP(const char* filePass)
/*
 *@filePass：读取BMP文件的路径
 */
{
	//读取头结构
	BITMAPFILEHEADER *bmpFHeader = new BITMAPFILEHEADER;
	BITMAPINFOHEADER *bmpIHeader = new BITMAPINFOHEADER;
	FILE *bmpf = fopen(filePass, "rb");
	fread(bmpFHeader, sizeof(BITMAPFILEHEADER), 1, bmpf);
	fread(bmpIHeader, sizeof(BITMAPINFOHEADER), 1, bmpf);
	if (bmpIHeader->biCompression != 0)				//必须为非压缩格式
	{
		printf("this BMP is compressed, choose a lossless one");
		delete bmpFHeader;
		delete bmpIHeader;
		fclose(bmpf);
		return NULL;
	}
	if (bmpIHeader->biBitCount != 24)				//必须是全彩色格式
	{
		printf("this BMP is not full colored, convert it to that fomat");
		delete bmpFHeader;
		delete bmpIHeader;
		fclose(bmpf);
		return NULL;
	}
	else
	{
		//读取颜色数据
		rawRGB *tempRAW = new rawRGB;
		int width = tempRAW->_width = abs(bmpIHeader->biWidth);
		int height = tempRAW->_height = abs(bmpIHeader->biHeight);
		unsigned char *tempPool = new unsigned char[bmpIHeader->biSizeImage];
		tempRAW->_pixels = new rgbPix[tempRAW->_height*tempRAW->_width];
		fread(tempPool, bmpIHeader->biSizeImage, 1, bmpf);

		//建立指向颜色数据中每一位的指针以便访问
		tempRAW->rawData = tempPool;
		int dataWidth = tempRAW->_width * 3;
		if (dataWidth % 4)
			dataWidth = (dataWidth / 4 + 1) * 4;
		rgbPix *pixelsPointer = tempRAW->_pixels;
		unsigned char *tempPoolPointer = tempPool;
		if (bmpIHeader->biHeight < 0)			//这种情况是左上角坐标为（0，0）
		{
			for (int i = 0; i < height; ++i)
			{
				tempPoolPointer = tempPool + i*dataWidth;
				for (int j = 0; j < width; ++j)
				{
					//tempRAW->_pixels[i*width + j]._b = &tempPool[i*dataWidth + j * 3];
					//tempRAW->_pixels[i*width + j]._g = &tempPool[i*dataWidth + j * 3 + 1];
					//tempRAW->_pixels[i*width + j]._r = &tempPool[i*dataWidth + j * 3 + 2];
					pixelsPointer->_b = tempPoolPointer++;
					pixelsPointer->_g = tempPoolPointer++;
					pixelsPointer->_r = tempPoolPointer++;
					pixelsPointer++;
				}
			}
		}
		else									//这种情况是左下角坐标为（0，0），需要先作数据重组
		{
			/*
			for (int i = 0; i < height; ++i)
			{
			tempPoolPointer = tempPool + (height-i-1)*dataWidth;
			for (int j = 0; j < width; ++j)
			{
			//tempRAW->_pixels[i*width + j]._b = &tempPool[i*dataWidth + j * 3];a
			//tempRAW->_pixels[i*width + j]._g = &tempPool[i*dataWidth + j * 3 + 1];
			//tempRAW->_pixels[i*width + j]._r = &tempPool[i*dataWidth + j * 3 + 2];
			pixelsPointer->_b = tempPoolPointer++;
			pixelsPointer->_g = tempPoolPointer++;
			pixelsPointer->_r = tempPoolPointer++;
			pixelsPointer++;
			}
			}*/
			unsigned char *temp = new unsigned char[dataWidth];
			for (int i = 0; i < height / 2; ++i)
			{
				tempPoolPointer = tempPool + i*dataWidth;
				unsigned char *tempPoolPointer1 = tempPool + (height - i - 1)*dataWidth;
				for (int j = 0; j < dataWidth; ++j)
				{
					temp[j] = *(tempPoolPointer++);
				}
				tempPoolPointer = tempPool + i*dataWidth;
				for (int j = 0; j < dataWidth; ++j)
				{
					*(tempPoolPointer++) = *(tempPoolPointer1++);
				}
				tempPoolPointer1 = tempPool + (height - i - 1)*dataWidth;
				for (int j = 0; j < dataWidth; ++j)
				{
					*(tempPoolPointer1++) = temp[j];
				}
			}
			delete[]temp;
			for (int i = 0; i < height; ++i)
			{
				tempPoolPointer = tempPool + i*dataWidth;
				for (int j = 0; j < width; ++j)
				{
					//tempRAW->_pixels[i*width + j]._b = &tempPool[i*dataWidth + j * 3];
					//tempRAW->_pixels[i*width + j]._g = &tempPool[i*dataWidth + j * 3 + 1];
					//tempRAW->_pixels[i*width + j]._r = &tempPool[i*dataWidth + j * 3 + 2];
					pixelsPointer->_b = tempPoolPointer++;
					pixelsPointer->_g = tempPoolPointer++;
					pixelsPointer->_r = tempPoolPointer++;
					pixelsPointer++;
				}
			}
		}
		//填充结构体
		tempRAW->_head = tempRAW->_pixels;
		tempRAW->_xOffset = tempRAW->_yOffset = 0;
		tempRAW->rawDataLength = bmpIHeader->biSizeImage;
		delete bmpFHeader;
		delete bmpIHeader;
		fclose(bmpf);
		return tempRAW;
	}
}

rgbPix* getPix(rawRGB *raw, int x, int y)
/*
 *@raw：已经读取完成的rawRGB
 *@x：相对左上角的横向第x像素（从_xOffset开始）
 *@y：类似x
 */
{
	if (x + raw->_xOffset >= raw->_width || y + raw->_yOffset >= raw->_height) return NULL;// printf("out of range!!");
	if (x + raw->_xOffset < 0 || y + raw->_yOffset < 0) return NULL;// printf("out of range!!");
	return (raw->_head + y*(raw->_width) + x);
}

rawRGB *adjustImage(rawRGB *source, float size, float x, float y)
/*
*@source：被缩放平移的结构
*@size：大小为原来的多少倍
*@x，y：左上角位置移动到原来图片位置的比例（0代表复位，可以是负数）
*/
{
	int unwidth = source->_width;
	int unheight = source->_height;
	int resizedWidth = unwidth*size;
	int resizedHeight = unheight*size;
	int adjustedWidth = resizedWidth * 3;
	if (adjustedWidth % 4)
		adjustedWidth = (adjustedWidth / 4 + 1) * 4;

	int rawDataLength = adjustedWidth*resizedHeight;
	unsigned char *resizedPool = new unsigned char[rawDataLength];

	int resizedTotalPix = resizedHeight*resizedWidth;
	rgbPix *resizedPixels = new rgbPix[resizedTotalPix];
	rgbPix *pointer = resizedPixels;

	//使用临近点算法
	for (int i = 0; i < resizedHeight;i++)
	{
		unsigned char *resizedPoolPointer = resizedPool + i*adjustedWidth;
		for (int j = 0; j < resizedWidth; j++)
		{
			int ii = i / size;
			int jj = j / size;

			pointer->_b = resizedPoolPointer++;
			*pointer->_b = *source->_pixels[ii*unwidth + jj]._b;

			pointer->_g = resizedPoolPointer++;
			*pointer->_g = *source->_pixels[ii*unwidth + jj]._g;

			pointer->_r = resizedPoolPointer++;
			*pointer->_r = *source->_pixels[ii*unwidth + jj]._r;
			pointer++;
		}
	}
	//填充新的结构体
	rawRGB *newRaw = new rawRGB;
	newRaw->rawData = resizedPool;
	newRaw->rawDataLength = source->rawDataLength;
	newRaw->_height = resizedHeight;
	newRaw->_pixels = resizedPixels;
	newRaw->_width = resizedWidth;
	newRaw->_xOffset = x*resizedWidth;
	newRaw->_yOffset = y*resizedHeight;
	newRaw->_head = resizedPixels + newRaw->_xOffset + newRaw->_yOffset*resizedWidth;
	return newRaw;
}

void generateBMP(QRmap *map, const char *pass, int QRwidth)
/*
*@map：QRmap结构体
*@pass：存储路径
*@QRwidth：每个QRcode位的像素宽度
*/
{
	BITMAPFILEHEADER kFileHeader;
	kFileHeader.bfType = 0x4d42;  // "BM"
	kFileHeader.bfSize = sizeof(BITMAPFILEHEADER)+
		sizeof(BITMAPINFOHEADER)+
		map->dataBytes;
	kFileHeader.bfReserved1 = 0;
	kFileHeader.bfReserved2 = 0;
	kFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+
		sizeof(BITMAPINFOHEADER);

	BITMAPINFOHEADER kInfoHeader;
	kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	kInfoHeader.biWidth = map->width * QRwidth;
	kInfoHeader.biHeight = -((int)map->width * QRwidth);
	kInfoHeader.biPlanes = 1;
	kInfoHeader.biBitCount = 24;
	kInfoHeader.biCompression = BI_RGB;
	kInfoHeader.biSizeImage = 0;
	kInfoHeader.biXPelsPerMeter = 0;
	kInfoHeader.biYPelsPerMeter = 0;
	kInfoHeader.biClrUsed = 0;
	kInfoHeader.biClrImportant = 0;

	// Output the bmp file

	FILE *f;
	if (!(fopen_s(&f, pass, "wb")))
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

}