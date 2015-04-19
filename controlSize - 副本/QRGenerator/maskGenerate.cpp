#include "maskGenerate.h"


int APcentres[41] = { 0, 0, 18, 22, 26, 30, 34, 22, 24, 26, 28, 30, 32, 34, 26, 26, 26, 30, 30, 30, 34, 28, 26, 30, 28, 32, 30, 34, 26, 30, 26, 30, 34, 30, 34, 30, 24, 28, 32, 26, 30 };
void markPDP(bool *mask, int width, int x, int y)			//x,y denote to the top-left ordinate of the Position Detection Pattern
{
	bool *tempAdd = mask + y*width + x;
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			*(tempAdd + j) = 0;
		}
		tempAdd += width;
	}
}

void markAP(bool *mask, int width, int x, int y)			//x,y denote to the top-left ordinate of the Position Detection Pattern
{
	bool *tempAdd = mask + y*width + x;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			*(tempAdd + j) = 0;
		}
		tempAdd += width;
	}
}

bool *generateMask(int QRversion, int width)
{
	int total = width*width;
	bool *tempMap = new bool[total];
	bool *point = tempMap;
	//mask the line
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (i == 6 || j == 6)
				*point = 0;
			else
				*point = 1;
			point++;
		}
	}

	markPDP(tempMap, width, 0, 0);
	markPDP(tempMap, width, 0, width-8);
	markPDP(tempMap, width, width-8, 0);
	if (QRversion == 1) ;
	else if (QRversion>1 && QRversion < 7)
	{
		markAP(tempMap, width, 8 + 4 * QRversion, 8 + 4 * QRversion);
	}
	else if (QRversion>6 && QRversion < 14)
	{
		//markAP(tempMap, width, 6 + 2 * QRversion, 6 + 2 * QRversion);   //////////////////////////////////////////////////is it necessary?
		markAP(tempMap, width, 4, 6 + 2 * QRversion);
		markAP(tempMap, width, 6 + 2 * QRversion, 4);

		markAP(tempMap, width, 8 + 4 * QRversion, 8 + 4 * QRversion);   //////////////////////////////////////////////////is it necessary?
		markAP(tempMap, width, 8 + 4 * QRversion, 6 + 2 * QRversion);
		markAP(tempMap, width, 6 + 2 * QRversion, 8 + 4 * QRversion);
	}
	else if (QRversion>1 && QRversion < 41)
	{
		int second = APcentres[QRversion];
		int last = 10 + QRversion * 4;
		int total;
		if (QRversion <= 20)	total = 4;
		else if (QRversion <= 27)	total = 5;
		else if (QRversion <= 34)	total = 6;
		else if (QRversion <= 40)	total = 7;
		int interval = (last - second) / (total - 2);
		int *coordinates = new int[total];
		coordinates[0] = 6;
		int i = 1;
		while (second <= last)
		{
			coordinates[i++] = second;
			second += interval;
		}
		for (int p = 0; p < total; ++p)
		{
			for (int q = 0; q < total; ++q)
			{
				markAP(tempMap, width, coordinates[p]-2, coordinates[q]-2);
			}
		}
		delete[]coordinates;
	}
	else
	{
		printf("not surported version of QRcode");
	}
	return tempMap;
}