#include "pictureTreating.h"

#define MINPROPOTION 0.15
#define SACRIFICEPRO 0.3

void simpleBAW(rawRGB *data)
{
	int totalPix = data->_height*data->_width;
	rgbPix *pixelPointer = data->_pixels;
	for (int i = 0; i < totalPix; ++i)
	{
		*pixelPointer->_b = *pixelPointer->_r = *pixelPointer->_g = (*pixelPointer->_b + *pixelPointer->_r + *pixelPointer->_g) / 3;
		++pixelPointer;
		}
}

void halfTone(rawRGB *data, int width, int level, int halftoneWidth)
/*
 *@data：指向待处理rawRGB结构的指针
 *@width：处理区域的宽度
 *@level：明暗级别
 *@halftoneWidth：单个半调格子的像素宽度
 */

{
	if (level < 0) level = 0;
	if (level > 255) level = 255;
	int totalPix = data->_height*data->_width;
	//生成黑白图
	rgbPix *pixelPointer = data->_pixels;
	for (int i = 0; i < totalPix; ++i)
	{
		*pixelPointer->_b = *pixelPointer->_r = *pixelPointer->_g = (*pixelPointer->_b + *pixelPointer->_r + *pixelPointer->_g) / 3;
		++pixelPointer;
	}
	//计算每个半调格子的颜色深度
	int halftonePix = width / halftoneWidth;
	for (int i = 0; i < halftonePix; ++i)
	{
		for (int j = 0; j < halftonePix; ++j)
		{
			int total = 0;
			rgbPix *temp = getPix(data, j*halftoneWidth, i*halftoneWidth);
			if (temp != NULL)
			{
				for (int p = 0; p < halftoneWidth; p++)
				{
					for (int q = 0; q < halftoneWidth; q++)
					{
						total += *(temp->_b);
						temp++;
						if (temp >= data->_pixels + totalPix)
							break;
					}
					temp += (data->_width - halftoneWidth);
					if (temp >= data->_pixels + totalPix)
						break;
				}
				int everage = total / halftoneWidth / halftoneWidth;
				temp = getPix(data, j*halftoneWidth, i*halftoneWidth);
				//for (int p = 0; p < halftoneWidth; p++)
				//{
				//for (int q = 0; q < halftoneWidth; q++)
				//{
				*temp->_b = *temp->_g = everage;
				*temp->_r = 0;					//place to calculate the halftone axis
				//temp++;
				//}
				//temp += (data->_width - halftoneWidth);
				//}
			}
		}
	}


	//生成半调格子们
	for (int i = 0; i < halftonePix; ++i)
	{
		for (int j = 0; j < halftonePix; ++j)
		{
			rgbPix *value = getPix(data, j*halftoneWidth, i*halftoneWidth);
			if (value != NULL)
			{
				int bonus = (*value->_b) / 9;
				for (int p = -1; p < 2; p++)
				{
					for (int q = -1; q < 2; q++)
					{
						int newX = j + q;
						int newY = i + p;
						if (!(newX < 0 || newX >= halftonePix || newY < 0 || newY >= halftonePix))
						{
							rgbPix *temp = getPix(data, newX*halftoneWidth, newY*halftoneWidth);
							if (temp!=NULL)
								(*temp->_r) += bonus;
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < halftonePix; ++i)
	{
		for (int j = 0; j < halftonePix; ++j)
		{
			rgbPix *value = getPix(data, j*halftoneWidth, i*halftoneWidth);
			if (value != NULL)
			{
				if (*value->_r>level)
				{
					for (int p = 0; p < halftoneWidth; p++)
					{
						for (int q = 0; q < halftoneWidth; q++)
						{
							*value->_b = *value->_r = *value->_g = 255;
							value++;
							if (value >= data->_pixels + totalPix)
								break;
						}
						value += (data->_width - halftoneWidth);
						if (value >= data->_pixels + totalPix)
							break;
					}
				}
				else
				{
					for (int p = 0; p < halftoneWidth; p++)
					{
						for (int q = 0; q < halftoneWidth; q++)
						{
							*value->_b = *value->_r = *value->_g = 0;
							value++;
							if (value >= data->_pixels + totalPix)
								break;
						}
						value += (data->_width - halftoneWidth);
						if (value >= data->_pixels + totalPix)
							break;
					}
				}
			}
		}
	}
}



void lottery(rawRGB *data, int width, int halftoneWidth, int pattern, float axis1, float axis2)
/*
 *@data：指向待处理rawRGB结构的指针
 *@width：处理区域的宽度
 *@halftoneWidth：单个半调格子的像素宽度
 *@pattern：处理概率使用的函数
 *@axis1：越大黑白越分明，缺少过度
 *@axis2：亮度，越大越亮，在0到1.0之间。缺省状态时自动取值。
 */
{
	RAND_MAX;
	int totalPix = data->_height*data->_width;
	//生成黑白图
	rgbPix *pixelPointer = data->_pixels;
	for (int i = 0; i < totalPix; ++i)
	{
		*pixelPointer->_b = *pixelPointer->_r = *pixelPointer->_g = (*pixelPointer->_b + *pixelPointer->_r + *pixelPointer->_g) / 3;
		++pixelPointer;
	}
	//计算每个半调格子的颜色深度
	int halftonePix = width / halftoneWidth;
	for (int i = 0; i < halftonePix; ++i)
	{
		for (int j = 0; j < halftonePix; ++j)
		{
			int total = 0;
			rgbPix *temp = getPix(data, j*halftoneWidth, i*halftoneWidth);
			if (temp != NULL)
			{
				for (int p = 0; p < halftoneWidth; p++)
				{
					for (int q = 0; q < halftoneWidth; q++)
					{
						total += *(temp->_b);
						temp++;
						if (temp >= data->_pixels + totalPix)
							break;
					}
					temp += (data->_width - halftoneWidth);
					if (temp >= data->_pixels + totalPix)
						break;
				}
				int everage = total / halftoneWidth / halftoneWidth;
				temp = getPix(data, j*halftoneWidth, i*halftoneWidth);
				//for (int p = 0; p < halftoneWidth; p++)
				//{
				//for (int q = 0; q < halftoneWidth; q++)
				//{
				*temp->_b = *temp->_g = everage;
				*temp->_r = 0;					//place to calculate the halftone axis
				//temp++;
				//}
				//temp += (data->_width - halftoneWidth);
				//}
			}
		}
	}

	//生成半调格子们
	for (int i = 0; i < halftonePix; ++i)
	{
		for (int j = 0; j < halftonePix; ++j)
		{
			rgbPix *value = getPix(data, j*halftoneWidth, i*halftoneWidth);
			if (value != NULL)
			{
				int bonus = (*value->_b) / 9;
				for (int p = -1; p < 2; p++)
				{
					for (int q = -1; q < 2; q++)
					{
						int newX = j + q;
						int newY = i + p;
						if (!(newX < 0 || newX >= halftonePix || newY < 0 || newY >= halftonePix))
						{
							rgbPix *temp = getPix(data, newX*halftoneWidth, newY*halftoneWidth);
							if (temp != NULL)
								(*temp->_r) += bonus;
						}
					}
				}
			}
		}
	}
	srand((unsigned)time(0));
	float a;
	if(pattern == sinned) a= 16384 * axis1;
	else a = 128 * axis1;
	float b;
	if (axis2 <-0.01 || axis2>1.01)
		b = 16384 - a;
	else
		b = 32768 * (axis1*axis2 + axis2 - axis1);// a * (axis2 - 0.5) * 4;
	for (int i = 0; i < halftonePix; ++i)
	{
		for (int j = 0; j < halftonePix; ++j)
		{
			rgbPix *value = getPix(data, j*halftoneWidth, i*halftoneWidth);
			if (value != NULL)
			{
				//
				if (pattern == sinned)
				{
					if (rand() <(1 - cos(((float)(*value->_r))*3.14 / 256.0)) * a + b)
					{
						for (int p = 0; p < halftoneWidth; p++)
						{
							for (int q = 0; q < halftoneWidth; q++)
							{
								*value->_b = *value->_r = *value->_g = 255;
								value++;
								if (value >= data->_pixels + totalPix)
									break;
							}
							value += (data->_width - halftoneWidth);
							if (value >= data->_pixels + totalPix)
								break;
						}
					}
					else
					{
						for (int p = 0; p < halftoneWidth; p++)
						{
							for (int q = 0; q < halftoneWidth; q++)
							{
								*value->_b = *value->_r = *value->_g = 0;
								value++;
								if (value >= data->_pixels + totalPix)
									break;
							}
							value += (data->_width - halftoneWidth);
							if (value >= data->_pixels + totalPix)
								break;
						}
					}
				}
				else if (pattern == linear)
				{
					if (rand() < (*value->_r)*a + b)
					{
						for (int p = 0; p < halftoneWidth; p++)
						{
							for (int q = 0; q < halftoneWidth; q++)
							{
								*value->_b = *value->_r = *value->_g = 255;
								value++;
								if (value >= data->_pixels + totalPix)
									break;
							}
							value += (data->_width - halftoneWidth);
							if (value >= data->_pixels + totalPix)
								break;
						}
					}
					else
					{
						for (int p = 0; p < halftoneWidth; p++)
						{
							for (int q = 0; q < halftoneWidth; q++)
							{
								*value->_b = *value->_r = *value->_g = 0;
								value++;
								if (value >= data->_pixels + totalPix)
									break;
							}
							value += (data->_width - halftoneWidth);
							if (value >= data->_pixels + totalPix)
								break;
						}
					}
				}
			}
		}
	}
}


void sortThisBlock(int *ranking, int number,int brt,int total)
{
	int i = 0;
	while (ranking[i] != -1 && brt > ranking[i]/(256*256))
		i++;
	for (int j = total-1; j > i; --j)
	{
		ranking[j] = ranking[j - 1];
	}
	ranking[i] = brt * 256 * 256 + number;
}
void superHalftone(rawRGB *data, QRmap *map, float minPro)
{
	int totalPix = data->_height*data->_width;
	//生成黑白图
	rgbPix *pixelPointer = data->_pixels;
	for (int i = 0; i < totalPix; ++i)
	{
		*pixelPointer->_b = *pixelPointer->_r = *pixelPointer->_g = (*pixelPointer->_b + *pixelPointer->_r + *pixelPointer->_g) / 3;
		++pixelPointer;
	}
	//计算每个半调格子的颜色深度
	int halftonePix = map->widthInPix / map->pixelScale;
	int totalPixInBlock = map->pixelScale * map->pixelScale;
	for (int i = 0; i < halftonePix; ++i)
	{
		for (int j = 0; j < halftonePix; ++j)
		{
			int total = 0;
			rgbPix *temp = getPix(data, j*map->pixelScale, i*map->pixelScale);
			if (temp != NULL)
			{
				for (int p = 0; p < map->pixelScale; p++)
				{
					for (int q = 0; q < map->pixelScale; q++)
					{
						total += *(temp->_b);
						temp++;
						if (temp >= data->_pixels + totalPix)
							break;
					}
					temp += (data->_width - map->pixelScale);
					if (temp >= data->_pixels + totalPix)
						break;
				}
				int everage = total / map->pixelScale / map->pixelScale;
				temp = getPix(data, j*map->pixelScale, i*map->pixelScale);
				//for (int p = 0; p < halftoneWidth; p++)
				//{
				//for (int q = 0; q < halftoneWidth; q++)
				//{
				*temp->_b = everage;
				*temp->_r = 0;					//place to calculate the halftone axis
				//temp++;
				//}
				//temp += (data->_width - halftoneWidth);
				//}
			}
		}
	}

	srand((unsigned)time(0));
	//生成半调格子们
	for (int i = 0; i < halftonePix; ++i)
	{
		for (int j = 0; j < halftonePix; ++j)
		{
			rgbPix *value = getPix(data, j*map->pixelScale, i*map->pixelScale);
			if (value != NULL&&getPix(data, j*map->pixelScale + map->pixelScale - 1, i*map->pixelScale + map->pixelScale - 1) != NULL)
			{
				int idealWhite = *(value->_b) *totalPixInBlock / 256;
				int idealBlack = map->pixelScale*map->pixelScale - idealWhite;
				int aim = *(map->pixelData[i*map->pixelScale*map->widthInPix + j*map->pixelScale]._b);
				bool *filled = new bool[map->pixelScale*map->pixelScale];			//记录是否已经填充的二维数组
				for (int w = 0; w < totalPixInBlock; w++) filled[w] = 0;
				int *ranking = new int[totalPixInBlock];																	//按亮度排序数组
				for (int k = 0; k < totalPixInBlock; ++k)
				{
					ranking[k] = -1;
				}
				for (int p = 0; p < map->pixelScale; p++)
				{
					for (int q = 0; q < map->pixelScale; q++)
					{
						if ((value = getPix(data, (j)*map->pixelScale + q, (i)*map->pixelScale + p)) != NULL)
						{
							if (*(value->_g)>250)								//这个250和下面的5可以后期再调整，为了防止有些图的最亮和最暗不是最大值
							{
								*value->_g = *value->_b = *value->_r = 255;
								filled[p*map->pixelScale + q] = 1;
								idealWhite--;
							}
							else if (*(value->_g) < 5)
							{
								*value->_g = *value->_b = *value->_r = 0;
								filled[p*map->pixelScale + q] = 1;
								idealBlack--;
							}
							sortThisBlock(ranking, p*map->pixelScale + q, *(value->_g), totalPixInBlock);
						}
					}
				}
				//调整中间点
				if ((value = getPix(data, (j)*map->pixelScale + map->pixelScale / 2, (i)*map->pixelScale + map->pixelScale / 2)) != NULL)
				{
					if (filled[map->pixelScale / 2 * map->pixelScale + map->pixelScale / 2])
					{
						if (*(value)->_g != aim)
						{
							if (*value->_g == 0)
							{
								*value->_g = *value->_b = *value->_r = aim;
								idealWhite--;
								idealBlack++;
							}
							else
							{
								*value->_g = *value->_b = *value->_r = aim;
								idealWhite++;
								idealBlack--;
							}
						}
					}
					else
					{
						if (aim != 0)
						{
							*value->_g = *value->_b = *value->_r = aim;
							idealWhite--;
						}
						else
						{
							*value->_g = *value->_b = *value->_r = aim;
							idealBlack--;
						}
						filled[map->pixelScale / 2 * map->pixelScale + map->pixelScale / 2] = 1;
					}
					//填充剩下的格子
					int minTarget = minPro*map->pixelScale*map->pixelScale;
					int top = 4 * map->pixelScale / 5;
					int bot = 1 * map->pixelScale / 5;
					if (aim == 0)
					{
						int k = 0;
						int x, y;
						for (y = bot; y<top; ++y)
						{
							for (x = bot; x<top; ++x)
							{
								if (filled[y*map->pixelScale + x] && *getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y)->_b == aim)
									--minTarget;
							}
						}
						while (minTarget>0)
						{
							if (k >= totalPixInBlock) break;
							x = ranking[k] % (256 * 256) % map->pixelScale;
							y = ranking[k] % (256 * 256) / map->pixelScale;
							if (!filled[y*map->pixelScale + x] && x >= bot&&y >= bot&&x<top&&y<top)
							{
								--idealBlack;
								--minTarget;
								(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
								*value->_g = *value->_b = *value->_r = 0;
								filled[y*map->pixelScale + x] = 1;
							}
							++k;
						}
						if (minTarget>0)
						{
							k = 0;
							while (minTarget>0)
							{
								if (k >= totalPixInBlock) break;
								x = ranking[k] % (256 * 256) % map->pixelScale;
								y = ranking[k] % (256 * 256) / map->pixelScale;
								if (x >= bot&&y >= bot&&x<top&&y<top)
								{
									(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
									if (*value->_b == 255)
									{
										--idealBlack;
										++idealWhite;
										--minTarget;
										*value->_g = *value->_b = *value->_r = 0;
									}
								}
								++k;
							}
						}
						if (idealBlack >= 0)							//good condition
						{
							k = 0;
							while (idealBlack>0)
							{
								if (k >= totalPixInBlock) break;
								x = ranking[k] % (256 * 256) % map->pixelScale;
								y = ranking[k] % (256 * 256) / map->pixelScale;
								if (!filled[y*map->pixelScale + x])
								{
									--idealBlack;
									(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
									*value->_g = *value->_b = *value->_r = 0;
									filled[y*map->pixelScale + x] = 1;
								}
								++k;
							}
							for (y = 0; y < map->pixelScale; y++)
							{
								for (x = 0; x < map->pixelScale; ++x)
								{
									(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
									if (!filled[y*map->pixelScale + x])
									{
										*value->_g = *value->_b = *value->_r = 255;
									}
								}
							}
						}
						else											//bad condition, sacrifice detail for the brightness, and sacrifice how much?
						{
							int change = (-idealBlack) - SACRIFICEPRO*idealWhite;
							idealWhite -= change;
							if (change > 0)
							{
								k = map->pixelScale*map->pixelScale - 1;
								while (1)//idealWhite!=0||change!=0)
								{
									if (k < 0) break;
									x = ranking[k] % (256 * 256) % map->pixelScale;
									y = ranking[k] % (256 * 256) / map->pixelScale;
									if (!filled[y*map->pixelScale + x])
									{
										(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
										*value->_g = *value->_b = *value->_r = 255;
									}
									else if (change>0)
									{
										change--;
										(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
										*value->_g = *value->_b = *value->_r = 255;
									}
									k--;
								}
							}
							else
							{
								for (y = 0; y < map->pixelScale; y++)
								{
									for (x = 0; x < map->pixelScale; ++x)
									{
										if (!filled[y*map->pixelScale + x])
										{
											(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
											*value->_g = *value->_b = *value->_r = 255;
										}
									}
								}
							}
						}
					}
					else
					{
						int k = map->pixelScale*map->pixelScale - 1;
						int x, y;
						for (y = bot; y<top; ++y)
						{
							for (x = bot; x<top; ++x)
							{
								if (filled[y*map->pixelScale + x] && *getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y)->_b == aim)
									--minTarget;
							}
						}
						while (minTarget>0)
						{
							if (k == -1) break;
							x = ranking[k] % (256 * 256) % map->pixelScale;
							y = ranking[k] % (256 * 256) / map->pixelScale;
							if (!filled[y*map->pixelScale + x] && x >= bot&&y >= bot&&x<top&&y<top)
							{
								--idealWhite;
								--minTarget;
								(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
								*value->_g = *value->_b = *value->_r = 255;
								filled[y*map->pixelScale + x] = 1;
							}
							--k;
						}
						if (minTarget>0)
						{
							k = map->pixelScale*map->pixelScale - 1;
							while (minTarget>0)
							{
								if (k == -1) break;
								x = ranking[k] % (256 * 256) % map->pixelScale;
								y = ranking[k] % (256 * 256) / map->pixelScale;
								if (x >= bot&&y >= bot&&x<top&&y<top)
								{
									(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
									if (*value->_b == 0)
									{
										++idealBlack;
										--idealWhite;
										--minTarget;
										*value->_g = *value->_b = *value->_r = 255;
									}
								}
								--k;
							}
						}
						if (idealWhite >= 0)							//good condition
						{
							k = map->pixelScale*map->pixelScale - 1;
							while (idealWhite>0)
							{
								if (k == -1) break;
								x = ranking[k] % (256 * 256) % map->pixelScale;
								y = ranking[k] % (256 * 256) / map->pixelScale;
								if (!filled[y*map->pixelScale + x])
								{
									--idealWhite;
									(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
									*value->_g = *value->_b = *value->_r = 255;
									filled[y*map->pixelScale + x] = 1;
								}
								--k;
							}
							for (y = 0; y < map->pixelScale; y++)
							{
								for (x = 0; x < map->pixelScale; ++x)
								{
									if (!filled[y*map->pixelScale + x])
									{
										(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
										*value->_g = *value->_b = *value->_r = 0;
									}
								}
							}
						}
						else											//bad condition, sacrifice detail for the brightness, and sacrifice how much?
						{
							int change = (-idealWhite) - SACRIFICEPRO*idealBlack;
							idealBlack -= change;
							if (change > 0)
							{
								k = 0;
								while (1)//idealBlack != 0 || change != 0)
								{
									if (k >= totalPixInBlock) break;
									x = ranking[k] % (256 * 256) % map->pixelScale;
									y = ranking[k] % (256 * 256) / map->pixelScale;
									if (!filled[y*map->pixelScale + x])
									{
										(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
										*value->_g = *value->_b = *value->_r = 0;
									}
									else if (change>0)
									{
										change--;
										(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
										*value->_g = *value->_b = *value->_r = 0;
									}
									k++;
								}
							}
							else
							{
								for (y = 0; y < map->pixelScale; y++)
								{
									for (x = 0; x < map->pixelScale; ++x)
									{
										if (!filled[y*map->pixelScale + x])
										{
											(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
											*value->_g = *value->_b = *value->_r = 0;
										}
									}
								}
							}
						}
					}
				}
				delete[]filled;
				delete[]ranking;
			}
		}
	}
	int i = 0;
}

void sortSacrifice(rgbPix **p, rgbPix *tp, int total)
{
	int i = 0;
	rgbPix **tempp = p;
	while (p[i] != NULL && *tp->_s<*p[i]->_s)
		i++;
	for (int j = total - 1; j > i; --j)
	{
		p[j] = p[j - 1];
	}
	p[i] = tp;
}

int toSacrifice(rgbPix **p, rgbPix *tp, int total)
{
	for (int i = 0; i < total; i++)
	{
		if (p[i] == tp) return *p[i]->_s;
	}
	return false;
}

void levelHalftone(rawRGB *data, QRmap *map, float minPro, float adjust, float sacrificeRate)
{
	int totalPix = data->_height*data->_width;
	//生成黑白图
	rgbPix *pixelPointer = data->_pixels;
	for (int i = 0; i < totalPix; ++i)
	{
		*pixelPointer->_b = *pixelPointer->_r = *pixelPointer->_g = (*pixelPointer->_b + *pixelPointer->_r + *pixelPointer->_g) / 3;
		++pixelPointer;
	}
	//计算每个半调格子的颜色深度
	int halftonePix = map->widthInPix / map->pixelScale;
	int totalPixInBlock = map->pixelScale * map->pixelScale;
	int everages[5][5];
	for (int i = 0; i < halftonePix; ++i)
	{
		for (int j = 0; j < halftonePix; ++j)
		{
			int total = 0;
			rgbPix *temp = getPix(data, j*map->pixelScale, i*map->pixelScale);
			if (temp != NULL&&getPix(data, j*map->pixelScale + map->pixelScale - 1, i*map->pixelScale + map->pixelScale - 1) != NULL)
			{
				for (int r = 0; r < 5; ++r)
				{
					for (int s = 0; s < 5; ++s)
					{
						temp = getPix(data, j*map->pixelScale + s*map->pixelScale/5, i*map->pixelScale+r*map->pixelScale/5);
						for (int p = 0; p < map->pixelScale/5; p++)
						{
							for (int q = 0; q < map->pixelScale/5; q++)
							{
								total += *(temp->_b);
								temp++;
								if (temp >= data->_pixels + totalPix)
									break;
							}
							//temp += (data->_width - map->pixelScale/5);
							temp = getPix(data, j*map->pixelScale + s*map->pixelScale / 5, i*map->pixelScale + r*map->pixelScale / 5 + p);
							if (temp >= data->_pixels + totalPix)
								break;
						}
						int everage = total * 25 / map->pixelScale / map->pixelScale;
						temp = getPix(data, j*map->pixelScale + s*map->pixelScale / 5, i*map->pixelScale + r*map->pixelScale / 5);
						temp->_e = new unsigned char;
						*temp->_g = *temp->_e = everage;
						total = 0;
					}
				}
			}
		}
	}

	srand((unsigned)time(0));
	//生成半调格子们
	int top = 4 * map->pixelScale / 5;
	int bot = 1 * map->pixelScale / 5;
	int mid1 = 2 * map->pixelScale / 5;
	int mid2 = 3 * map->pixelScale / 5;
	int x, y;
	int *ideal = new int[25];
	int blockNum = halftonePix*halftonePix;
	int sacrificeNum = blockNum*0.05*sacrificeRate;
	rgbPix **sacrifice = new rgbPix*[blockNum];
	for (int i = 0; i < blockNum; ++i) sacrifice[i] = NULL;

	//给各个格子排序到sacrifice数组，用来决定牺牲哪些格子
	for (int i = 0; i < halftonePix; ++i)
	{
		for (int j = 0; j < halftonePix; ++j)
		{
			int total = 0;
			rgbPix *value = getPix(data, j*map->pixelScale, i*map->pixelScale);
			if (value != NULL&&getPix(data, j*map->pixelScale + map->pixelScale - 1, i*map->pixelScale + map->pixelScale - 1) != NULL)
			{
				int aim = *(map->pixelData[i*map->pixelScale*map->widthInPix + j*map->pixelScale]._b);
				for (int p = 0; p < bot; ++p)
				{
					value = getPix(data, j*map->pixelScale + mid1, i*map->pixelScale + mid1 + p);
					for (int q = 0; q < bot; ++q)
					{
						total += *value->_r;
						value++;
					}
				}
				int everage = total * 25 / totalPixInBlock;
				rgbPix *ptemp = getPix(data, j*map->pixelScale, i*map->pixelScale);
				ptemp->_s = new unsigned char;
				if (map->pixelData[(i*map->pixelScale)*map->widthInPix + j*map->pixelScale].data == true)
				{
					*ptemp->_s = rand() % 4 + 1 + (everage>aim ? everage - aim : aim - everage)/2;
				}
				else
					*ptemp->_s = 0;
				sortSacrifice(sacrifice, ptemp, blockNum);
			}
		}
	}
	int temp = 0;
	for (int i = 0; i < halftonePix; ++i)
	{
		for (int j = 0; j < halftonePix; ++j)
		{
			rgbPix *value = getPix(data, j*map->pixelScale, i*map->pixelScale);
			if (value!=NULL&&getPix(data, j*map->pixelScale + map->pixelScale - 1, i*map->pixelScale + map->pixelScale - 1) != NULL)
			{
				if (temp = toSacrifice(sacrifice, value, sacrificeNum))
				{
					for (int p = mid1; p < mid2; ++p)
					{
						for (int q = mid1; q < mid2; ++q)
						{
							map->pixelData[(i*map->pixelScale + p)*map->widthInPix + q + j*map->pixelScale].data = 1;
						}
					}
					int *ranking = new int[totalPixInBlock];																	//按亮度排序数组

					for (int k = 0; k < totalPixInBlock; ++k)
					{
						ranking[k] = -1;
					}
					for (int p = 0; p < map->pixelScale; p++)
					{
						for (int q = 0; q < map->pixelScale; q++)
						{
							if ((value = getPix(data, (j)*map->pixelScale + q, (i)*map->pixelScale + p)) != NULL)
							{
								if (q%bot==0&&p%bot==0)
									ideal[q / bot + p / bot * 5] = (*value->_b)*totalPixInBlock / 25 / 255;

								sortThisBlock(ranking, p*map->pixelScale + q, (*(value->_r))
									,
									totalPixInBlock);
							}
						}
					}
					for (int k = totalPixInBlock - 1; k >= 0; --k)
						{
							int index = ranking[k] % (256 * 256);
							x = index % map->pixelScale;
							y = index / map->pixelScale;
							int index1 = x / bot + y / bot * 5;
								if (ideal[index1]>0)
								{
									ideal[index1]--;
									(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
									*value->_g = *value->_b = *value->_r = 255;
								}
								else
								{
									(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
									*value->_g = *value->_b = *value->_r = 0;
								}
							
						}
					
				}
				else
				{
					int aim = *(map->pixelData[i*map->pixelScale*map->widthInPix + j*map->pixelScale]._b);
					int axis = (aim < 128 ? 1 : -1) * adjust;
					int least = minPro*totalPixInBlock;
					int *ranking = new int[totalPixInBlock];																	//按亮度排序数组

					for (int k = 0; k < totalPixInBlock; ++k)
					{
						ranking[k] = -1;
					}
					for (int p = 0; p < map->pixelScale; p++)
					{
						for (int q = 0; q < map->pixelScale; q++)
						{
							if ((value = getPix(data, (j)*map->pixelScale + q, (i)*map->pixelScale + p)) != NULL)
							{
								if (p%bot == 0 && q%bot == 0)
								{
									if (aim>128)
									{
										ideal[q / bot + p / bot * 5] = (*value->_b)*totalPixInBlock / 25 / 255;
									}
									else
									{
										ideal[q / bot + p / bot * 5] = (totalPixInBlock - (*value->_b)*totalPixInBlock / 255) / 25;
									}
								}
								if (p >= bot&&p<top&&q >= bot&&q<top)
								{
									sortThisBlock(ranking, p*map->pixelScale + q, (*(value->_r))
										+ map->pixelScale * adjust + ((p>map->pixelScale / 2 ? p - map->pixelScale / 2 : map->pixelScale / 2 - p) + (q>map->pixelScale / 2 ? q - map->pixelScale / 2 : map->pixelScale / 2 - q))*axis
										,
										totalPixInBlock);
								}
								else
								{
									sortThisBlock(ranking, p*map->pixelScale + q, (*(value->_r))
										,
										totalPixInBlock);
								}
							}
						}
					}
					if (aim > 128)
					{
						for (int k = totalPixInBlock - 1; k >= 0; --k)
						{
							int index = ranking[k] % (256 * 256);
							x = index % map->pixelScale;
							y = index / map->pixelScale;
							int index1 = x / bot + y / bot * 5;
							if (!(x >= bot&&x < top&&y >= bot&&y < top))
							{
								if (ideal[index1]>0)
								{
									ideal[index1]--;
									(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
									*value->_g = *value->_b = *value->_r = 255;
								}
								else
								{
									(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
									*value->_g = *value->_b = *value->_r = 0;
								}
							}
							else if (!(x >= mid1&&x < mid2&&y >= mid1&&y < mid2) && (ideal[index1]>0 || least>0))
							{
								ideal[index1]--;
								least--;
								(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
								*value->_g = *value->_b = *value->_r = 255;
							}
							else if (x >= mid1&&x < mid2&&y >= mid1&&y < mid2)
							{
								(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
								*value->_g = *value->_b = *value->_r = 255;
							}
							else
							{
								(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
								*value->_g = *value->_b = *value->_r = 0;
							}
						}
					}
					else
					{
						for (int k = 0; k < totalPixInBlock; ++k)
						{
							int index = ranking[k] % (256 * 256);
							x = index % map->pixelScale;
							y = index / map->pixelScale;
							int index1 = x / bot + y / bot * 5;
							if (!(x >= bot&&x < top&&y >= bot&&y < top))
							{
								if (ideal[index1]>0)
								{
									ideal[index1]--;
									(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
									*value->_g = *value->_b = *value->_r = 0;
								}
								else
								{
									(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
									*value->_g = *value->_b = *value->_r = 255;
								}
							}
							else if (!(x >= mid1&&x < mid2&&y >= mid1&&y < mid2) && (ideal[index1]>0 || least>0))
							{
								ideal[index1]--;
								least--;
								(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
								*value->_g = *value->_b = *value->_r = 0;
							}
							else if (x >= mid1&&x < mid2&&y >= mid1&&y < mid2)
							{
								(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
								*value->_g = *value->_b = *value->_r = 0;
							}
							else
							{
								(value = getPix(data, (j)*map->pixelScale + x, (i)*map->pixelScale + y));
								*value->_g = *value->_b = *value->_r = 255;
							}
						}
					}
				}
			}
		}
	}
	int i = 0;
}

void combine(rawRGB *raw, QRmap *map)
/*
 *@raw：指向rawRGB的指针
 *@map：指向QRmap的指针
 */
{
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
}