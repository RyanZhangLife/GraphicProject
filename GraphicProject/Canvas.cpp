#include "Canvas.h"
#include <math.h>
#include <vector>
#include <algorithm>

namespace GT
{
	// Brensenham�����㷨
	void Canvas::drawLine(Point pt1, Point pt2)
	{
		// ƫ����
		int disY = abs(pt2.m_y - pt1.m_y);
		int disX = abs(pt2.m_x - pt1.m_x);
		// x,y��ʼֵ
		int xNow = pt1.m_x;
		int yNow = pt1.m_y;
		// ����ֵ
		int stepX = 0;
		int stepY = 0;

		// �ж��������򲽽�������
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
	
	// ɨ�����㷨����������
	void Canvas::scaningDrawTriangle(Point pt1, Point pt2, Point pt3)
	{
		RGBA _color(255, 0, 0);
		// ������Χ��
		int left = MIN(pt3.m_x, MIN(pt1.m_x, pt2.m_x));
		int top = MIN(pt3.m_y, MIN(pt1.m_y, pt2.m_y));
		int right = MAX(pt3.m_x, MAX(pt1.m_x, pt2.m_x));
		int bottom = MAX(pt3.m_y, MAX(pt1.m_y, pt2.m_y));

		left = left < 0 ? 0 : left;
		top = top < 0 ? 0 : top;
		right = right > (m_width - 1) ? m_width - 1 : right;
		bottom = bottom > (m_height - 1) ? m_height - 1 : bottom;

		// ����ֱ��б�ʲ���ֵ
		float k1 = (float)(pt2.m_y - pt3.m_y) / (float)(pt2.m_x - pt3.m_x);
		float k2 = (float)(pt1.m_y - pt3.m_y) / (float)(pt1.m_x - pt3.m_x);
		float k3 = (float)(pt2.m_y - pt1.m_y) / (float)(pt2.m_x - pt1.m_x);

		// ����ֱ�� b ֵ
		float b1 = (float)pt2.m_y - k1 * (float)pt2.m_x;
		float b2 = (float)pt3.m_y - k2 * (float)pt3.m_x;
		float b3 = (float)pt1.m_y - k3 * (float)pt1.m_x;

		// ѭ���ж�
		for (int x = left; x <= right; ++x) {
			for (int y = top; y <= bottom; ++y) {
				// �жϵ��Ƿ��������η�Χ��
				float judge1 = (y - (k1 * x + b1)) * (pt1.m_y - (k1 * pt1.m_x + b1));
				float judge2 = (y - (k2 * x + b2)) * (pt2.m_y - (k2 * pt2.m_x + b2));
				float judge3 = (y - (k3 * x + b3)) * (pt3.m_y - (k3 * pt3.m_x + b3));

				if (judge1 >= 0 && judge2 >= 0 && judge3 >= 0) {
					drawPoint(x, y, _color);
				}
			}
		}
	}

	// ����ƽ���������㷨
	void Canvas::drawTriangleFlat(Point ptFlat1, Point ptFlat2, Point pt)
	{
		float k1 = 0.0;
		float k2 = 0.0;

		if (ptFlat1.m_x != pt.m_x)
		{
			k1 = (float)(ptFlat1.m_y - pt.m_y) / (float)(ptFlat1.m_x - pt.m_x);
		}
		if (ptFlat1.m_x != pt.m_x)
		{
			k2 = (float)(ptFlat2.m_y - pt.m_y) / (float)(ptFlat2.m_x - pt.m_x);
		}
		float b1 = (float)pt.m_y - (float)pt.m_x * k1;
		float b2 = (float)pt.m_y - (float)pt.m_x * k2;

		int yStart = MIN(pt.m_y, ptFlat1.m_y);
		int yEnd = MAX(pt.m_y, ptFlat1.m_y);

		for (int y = yStart; y <= yEnd; ++y)
		{
			int x1{ 0 };
			int x2{ 0 };
			if (k1 == 0)
			{
				x1 = ptFlat1.m_x;
			}
			else
			{
				x1 = ((float)y - b1) / k1;
			}
			if (k2 == 0)
			{
				x2 = ptFlat1.m_x;
			}
			else
			{
				x2 = ((float)y - b2) / k2;
			}
			Point pt1(x1, y, RGBA(255, 0, 0));
			Point pt2(x2, y, RGBA(255, 0, 0));
			drawLine(pt1, pt2);
		}
	}

	// ͨ�������λ����㷨
	void Canvas::drawTriangle(Point pt1, Point pt2, Point pt3)
	{
		std::vector<Point> pVec{ pt1,pt2,pt3 };
		std::sort(pVec.begin(), pVec.end(), [](const Point& pt1, const Point& pt2) {return pt1.m_y > pt2.m_y; });

		Point ptMax = pVec[0];
		Point ptMid = pVec[1];
		Point ptMin = pVec[2];

		if (ptMax.m_y == ptMid.m_y)
		{
			drawTriangleFlat(ptMax, ptMid, ptMin);
			return;
		}
		if (ptMin.m_y == ptMid.m_y)
		{
			drawTriangleFlat(ptMin, ptMid, ptMax);
			return;
		}
		float k = 0.0;
		if (ptMax.m_x != ptMin.m_x)
		{
			k = (float)(ptMax.m_y - ptMin.m_y) / (float)(ptMax.m_x - ptMin.m_x);
		}
		float b = (float)ptMax.m_y - (float)ptMax.m_x * k;

		Point npt(0, 0, RGBA(255, 0, 0));
		npt.m_y = ptMid.m_y;
		if (k == 0)
		{
			npt.m_x = ptMax.m_x;
		}
		else
		{
			npt.m_x = ((float)npt.m_y - b) / k;
		}

		drawTriangleFlat(ptMid, npt, ptMax);
		drawTriangleFlat(ptMid, npt, ptMin);

		return;
	}
}