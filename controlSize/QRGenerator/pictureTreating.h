#include "readBMP.h"
#include <time.h>
#include <cmath>

#define sinned 1
#define linear 2

void simpleBAW(rawRGB *data);		//将rawRGB格式的图像转化为黑白
void halfTone(rawRGB *data, int width, int level, int halftoneWidth);		//简单半调色
/*
*@data：指向待处理rawRGB结构的指针
*@width：处理区域的宽度
*@level：明暗级别(0~255)
*@halftoneWidth：单个半调格子的像素宽度
*/

void lottery(rawRGB *data, int width, int halftoneWidth, int pattern, float axis1, float axis2 = -1);		//随机半调色算法
/*
*@data：指向待处理rawRGB结构的指针
*@width：处理区域的宽度
*@halftoneWidth：单个半调格子的像素宽度
*@pattern：处理概率使用的函数
*@axis1：越大黑白越分明，缺少过度(>0)
*@axis2：亮度，越大越亮，在0到1.0之间。缺省状态时自动取值。
*/

void superHalftone(rawRGB *data, QRmap *map, float minPro=0.15);		//超级半调色
/*
*@data：指向待处理rawRGB结构的指针
*@map：二维码结构
*@minPro：最小填充率（小于一的数，默认值0.15），数值越大识别越好但图像越差
*/

void levelHalftone(rawRGB *data, QRmap *map, float minPro = 0.0, float adjust = 0.0, float sacrificeRate = 0.0);		//分级半调色
/*
*@data：指向待处理rawRGB结构的指针
*@map：二维码结构
*@minPro：最小填充率,加入一定比例的辅助黑白点提高识别率（小于一的数，默认值0.0），数值越大识别越好但图像越差
*@adjust：是否调整图像提高识别率（只做调整而不强制增加黑白点，0.0为否，小于一的数表示调整幅度）
*@sacrifice：是否牺牲一些突兀的点（中心与四周颜色不符）
*/


void colorHalftone(rawRGB *data, QRmap *map, float minPro = 0.15);		//分级半调色
/*
*@data：指向待处理rawRGB结构的指针
*@map：二维码结构
*@minPro：最小填充率（小于一的数，默认值0.15），数值越大识别越好但图像越差
*/


void combine(rawRGB *raw, QRmap *map);			//将QRmap中数据像素和rawRGB中图像进行混合
/*
*@raw：指向rawRGB的指针
*@map：指向QRmap的指针
*/