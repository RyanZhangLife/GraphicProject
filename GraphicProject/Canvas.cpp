#include "Canvas.h"
#include <math.h>

namespace GT
{
	// Brensenham画线算法
	void Canvas::drawLine(Point pt1, Point pt2)
	{
		// 偏移量
		int disY = abs(pt2.m_y - pt1.m_y);
		int disX = abs(pt2.m_x - pt1.m_x);
		// x,y起始值
		int xNow = pt1.m_x;
		int yNow = pt1.m_y;
		// 步进值
		int stepX = 0;
		int stepY = 0;

		// 判断两个方向步进的正负
		if (pt1.m_x < pt2.m_x)
		{
			stepX = 1;
		}
		else
		{
			stepX = -1;
		}
		if (pt1.m_y < pt2.m_y)
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
			RGBA _color;
			float _scale = 0;
			if (useXStep)
			{
				_scale = (float)(xNow - pt1.m_x) / (float)(pt2.m_x - pt1.m_x);
			}else
			{
				_scale = (float)(yNow - pt1.m_y) / (float)(pt2.m_y - pt1.m_y);
			}
			_color = colorLerp(pt1.m_color, pt2.m_color, _scale);
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
	
	// 扫描线算法
	void Canvas::drawTriangle(Point pt1, Point pt2, Point pt3)
	{
		// 构建包围体
		int left = MIN(pt3.m_x, MIN(pt1.m_x, pt2.m_x));
		int top = MIN(pt3.m_y, MIN(pt1.m_y, pt2.m_y));
		int right = MAX(pt3.m_x, MAX(pt1.m_x, pt2.m_x));
		int bottom = MAX(pt3.m_y, MAX(pt1.m_y, pt2.m_y));

		left = left < 0 ? 0 : left;
		top = top < 0 ? 0 : top;
		right = right > (m_width - 1) ? m_width - 1 : right;
		bottom = bottom > (m_height - 1) ? m_height - 1 : bottom;
	}	
}