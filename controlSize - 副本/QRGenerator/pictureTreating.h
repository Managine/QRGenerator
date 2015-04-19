#include "readBMP.h"
#include <time.h>
#include <cmath>

#define sinned 1
#define linear 2

void simpleBAW(rawRGB *data);		//��rawRGB��ʽ��ͼ��ת��Ϊ�ڰ�
void halfTone(rawRGB *data, int width, int level, int halftoneWidth);		//�򵥰��ɫ
/*
*@data��ָ�������rawRGB�ṹ��ָ��
*@width����������Ŀ��
*@level����������(0~255)
*@halftoneWidth������������ӵ����ؿ��
*/

void lottery(rawRGB *data, int width, int halftoneWidth, int pattern, float axis1, float axis2 = -1);		//������ɫ�㷨
/*
*@data��ָ�������rawRGB�ṹ��ָ��
*@width����������Ŀ��
*@halftoneWidth������������ӵ����ؿ��
*@pattern���������ʹ�õĺ���
*@axis1��Խ��ڰ�Խ������ȱ�ٹ���(>0)
*@axis2�����ȣ�Խ��Խ������0��1.0֮�䡣ȱʡ״̬ʱ�Զ�ȡֵ��
*/

void superHalftone(rawRGB *data, QRmap *map, float minPro=0.15);		//�������ɫ
/*
*@data��ָ�������rawRGB�ṹ��ָ��
*@map����ά��ṹ
*@minPro����С����ʣ�С��һ������Ĭ��ֵ0.15������ֵԽ��ʶ��Խ�õ�ͼ��Խ��
*/

void levelHalftone(rawRGB *data, QRmap *map, float minPro = 0.0, float adjust = 0.0, float sacrificeRate = 0.0);		//�ּ����ɫ
/*
*@data��ָ�������rawRGB�ṹ��ָ��
*@map����ά��ṹ
*@minPro����С�����,����һ�������ĸ����ڰ׵����ʶ���ʣ�С��һ������Ĭ��ֵ0.0������ֵԽ��ʶ��Խ�õ�ͼ��Խ��
*@adjust���Ƿ����ͼ�����ʶ���ʣ�ֻ����������ǿ�����Ӻڰ׵㣬0.0Ϊ��С��һ������ʾ�������ȣ�
*@sacrifice���Ƿ�����һЩͻأ�ĵ㣨������������ɫ������
*/


void colorHalftone(rawRGB *data, QRmap *map, float minPro = 0.15);		//�ּ����ɫ
/*
*@data��ָ�������rawRGB�ṹ��ָ��
*@map����ά��ṹ
*@minPro����С����ʣ�С��һ������Ĭ��ֵ0.15������ֵԽ��ʶ��Խ�õ�ͼ��Խ��
*/


void combine(rawRGB *raw, QRmap *map);			//��QRmap���������غ�rawRGB��ͼ����л��
/*
*@raw��ָ��rawRGB��ָ��
*@map��ָ��QRmap��ָ��
*/