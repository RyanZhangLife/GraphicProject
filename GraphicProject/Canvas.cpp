#include "Canvas.h"
#include <math.h>

namespace GT
{
	// Brensenham�����㷨
	void Canvas::drawLine(intV2 pt1, intV2 pt2, RGBA _color)
	{
		// ƫ����
		int disY = abs(pt2.y - pt1.y);
		int disX = abs(pt2.x - pt1.x);
		// x,y��ʼֵ
		int xNow = pt1.x;
		int yNow = pt1.y;
		// ����ֵ
		int stepX = 0;
		int stepY = 0;

		// �ж��������򲽽�������
		if (pt1.x < pt2.x)
		{
			stepX = 1;
		}
		else
		{
			stepX = -1;
		}
		if (pt1.y < pt2.y)
		{
			stepY = 1;
		}
		else
		{
			stepY = -1;
		}

		// ������������
		int		sumStep		= disX;
		bool	useXStep	= true;
		if (disX < disY)
		{
			sumStep		=	 disY;
			useXStep	=	 false;
			swap(disX, disY);
		}

		//��ʼ��
		int p = 2 * disY - disX;

		for (int i{ 0 }; i < sumStep; ++i)
		{
			drawPoint(xNow, yNow, _color);
			// ������������
			if (p >= 0)
			{
				if (useXStep)
				{
					yNow += stepY;
				}
				else 
				{
					xNow += stepX;
				}
				p = p - 2 * disX;
			}
			// ����������
			if (useXStep)
			{
				xNow += stepX;
			}
			else
			{
				yNow += stepX;
			}

			p = p + 2 * disY;
		}
	}
}