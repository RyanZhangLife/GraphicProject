#include "Canvas.h"
#include <math.h>

namespace GT
{
	// Brensenham画线算法
	void Canvas::drawLine(intV2 pt1, intV2 pt2, RGBA _color)
	{
		// 偏移量
		int disY = abs(pt2.y - pt1.y);
		int disX = abs(pt2.x - pt1.x);
		// x,y起始值
		int xNow = pt1.x;
		int yNow = pt1.y;
		// 步进值
		int stepX = 0;
		int stepY = 0;

		// 判断两个方向步进的正负
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

		// 决定步进方向
		int		sumStep		= disX;
		bool	useXStep	= true;
		if (disX < disY)
		{
			sumStep		=	 disY;
			useXStep	=	 false;
			swap(disX, disY);
		}

		//初始化
		int p = 2 * disY - disX;

		for (int i{ 0 }; i < sumStep; ++i)
		{
			drawPoint(xNow, yNow, _color);
			// 步进辅助坐标
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
			// 步进主坐标
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