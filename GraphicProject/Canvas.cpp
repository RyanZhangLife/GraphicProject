#include "Canvas.h"
#include <math.h>
#include <vector>
#include <algorithm>

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
		RGBA _color;
		for (int i{ 0 }; i < sumStep; ++i)
		{
			float _scale = 0;
			if (useXStep)
			{
				_scale = (float)(xNow - pt1.m_x) / (float)(pt2.m_x - pt1.m_x);
			}else
			{
				_scale = (float)(yNow - pt1.m_y) / (float)(pt2.m_y - pt1.m_y);
			}
			if (m_enableTexture)
			{
				floatV2 _uv = uvLerp(pt1.m_uv, pt2.m_uv, _scale);
				if (m_texture)
				{
					_color = m_texture->getColorByUV(_uv.x, _uv.y);
				}
				else
				{
					_color = colorLerp(pt1.m_color, pt2.m_color, _scale);
				}
			}
			else
			{
				_color = colorLerp(pt1.m_color, pt2.m_color, _scale);
			}
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
	
	// 扫描线算法绘制三角形
	void Canvas::scaningDrawTriangle(Point pt1, Point pt2, Point pt3)
	{
		RGBA _color(255, 0, 0);
		// 构建包围体
		int left = MIN(pt3.m_x, MIN(pt1.m_x, pt2.m_x));
		int top = MIN(pt3.m_y, MIN(pt1.m_y, pt2.m_y));
		int right = MAX(pt3.m_x, MAX(pt1.m_x, pt2.m_x));
		int bottom = MAX(pt3.m_y, MAX(pt1.m_y, pt2.m_y));

		left = left < 0 ? 0 : left;
		top = top < 0 ? 0 : top;
		right = right > (m_width - 1) ? m_width - 1 : right;
		bottom = bottom > (m_height - 1) ? m_height - 1 : bottom;

		// 计算直线斜率参数值
		float k1 = (float)(pt2.m_y - pt3.m_y) / (float)(pt2.m_x - pt3.m_x);
		float k2 = (float)(pt1.m_y - pt3.m_y) / (float)(pt1.m_x - pt3.m_x);
		float k3 = (float)(pt2.m_y - pt1.m_y) / (float)(pt2.m_x - pt1.m_x);

		// 计算直线 b 值
		float b1 = (float)pt2.m_y - k1 * (float)pt2.m_x;
		float b2 = (float)pt3.m_y - k2 * (float)pt3.m_x;
		float b3 = (float)pt1.m_y - k3 * (float)pt1.m_x;

		// 循环判断
		for (int x = left; x <= right; ++x) {
			for (int y = top; y <= bottom; ++y) {
				// 判断点是否在三角形范围内
				float judge1 = (y - (k1 * x + b1)) * (pt1.m_y - (k1 * pt1.m_x + b1));
				float judge2 = (y - (k2 * x + b2)) * (pt2.m_y - (k2 * pt2.m_x + b2));
				float judge3 = (y - (k3 * x + b3)) * (pt3.m_y - (k3 * pt3.m_x + b3));

				if (judge1 >= 0 && judge2 >= 0 && judge3 >= 0) {
					drawPoint(x, y, _color);
				}
			}
		}
	}

	// 绘制平底三角形算法
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

		// 颜色插值
		RGBA colorStart1;
		RGBA colorStart2;
		RGBA colorEnd1;
		RGBA colorEnd2;

		floatV2 uvStart1;
		floatV2 uvStart2;
		floatV2 uvEnd1;
		floatV2 uvEnd2;

		if (pt.m_y < ptFlat1.m_y)
		{
			yStart = pt.m_y;
			yEnd = ptFlat1.m_y;

			colorStart1 = pt.m_color;
			colorEnd1 = ptFlat1.m_color;
			colorStart2 = pt.m_color;
			colorEnd2 = ptFlat2.m_color;

			uvStart1 = pt.m_uv;
			uvEnd1 = ptFlat1.m_uv;
			uvStart2 = pt.m_uv;
			uvEnd2 = ptFlat2.m_uv;
		}
		else 
		{
			yStart = ptFlat1.m_y;
			yEnd = pt.m_y;

			colorStart1 = ptFlat1 .m_color;
			colorEnd1 = pt.m_color;
			colorStart2 = ptFlat2.m_color;
			colorEnd2 = pt.m_color;

			uvStart1 = ptFlat1.m_uv;
			uvStart2 = ptFlat2.m_uv;
			uvEnd1 = pt.m_uv;
			uvEnd2 = pt.m_uv;
		}
		float yColorStep = 1.0 / (float)(yEnd - yStart);
		int yColorStart = yStart;

		if (yStart < 0)
		{
			yStart = 0;
		}
		if (yEnd > m_height)
		{
			yEnd = m_height - 1;
		}

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
			// 剪裁x1
			if (x1 < 0)
			{
				x1 = 0;
			}
			if (x1 > m_width)
			{
				x1 = m_width - 1;
			}

			if (k2 == 0)
			{
				x2 = ptFlat1.m_x;
			}
			else
			{
				x2 = ((float)y - b2) / k2;
			}

			// 剪裁x2
			if (x2 < 0)
			{
				x2 = 0;
			}
			if (x2 > m_width)
			{
				x2 = m_width - 1;
			}

			float s = (float)(y - yColorStart) * yColorStep;

			RGBA _color1 = colorLerp(colorStart1, colorEnd1, s);
			RGBA _color2 = colorLerp(colorStart2, colorEnd2, s);

			floatV2 _uv1 = uvLerp(uvStart1, uvEnd1, s);
			floatV2 _uv2 = uvLerp(uvStart2, uvEnd2, s);

			Point pt1(x1, y, _color1, _uv1);
			Point pt2(x2, y, _color2, _uv2);
			drawLine(pt1, pt2);
		}
	}

	floatV2 Canvas::uvLerp(floatV2 _uv1, floatV2 _uv2, float _scale)
	{
		floatV2 _uv;
		_uv.x = _uv1.x + (_uv2.x - _uv1.x) * _scale;
		_uv.y = _uv1.y + (_uv2.y - _uv1.y) * _scale;
		return _uv;
	}

	// 通用三角形绘制算法
	void Canvas::drawTriangle(Point pt1, Point pt2, Point pt3)
	{
		std::vector<Point> pVec{ pt1,pt2,pt3 };
		
		GT_RECT _rect(0, m_width, 0, m_height);
		// 判断三角形是否与屏幕相交
		while (true)
		{
			if (judgeInRect(pt1, _rect) || judgeInRect(pt2, _rect) || judgeInRect(pt3, _rect))
			{
				break;
			}
			Point rpt1(0, 0, RGBA());
			Point rpt2(m_width, 0, RGBA());
			Point rpt3(0, m_height, RGBA());
			Point rpt4(m_width, m_height, RGBA());
			if (judgeInTriangle(rpt1, pVec) ||
				judgeInTriangle(rpt2, pVec) ||
				judgeInTriangle(rpt3, pVec) ||
				judgeInTriangle(rpt4, pVec))
			{
				break;
			}
			return;
		}

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

		float s = (float)(npt.m_y - ptMin.m_y) / (float)(ptMax.m_y - ptMin.m_y);
		npt.m_color = colorLerp(ptMin.m_color, ptMax.m_color, s);
		npt.m_uv = uvLerp(ptMin.m_uv, ptMax.m_uv, s);

		drawTriangleFlat(ptMid, npt, ptMax);
		drawTriangleFlat(ptMid, npt, ptMin);

		return;
	}

	// 判断pt是否在范围Rect内
	bool Canvas::judgeInRect(Point pt, GT_RECT _rect)
	{
		if (pt.m_x > _rect.m_left && pt.m_x<_rect.m_right && pt.m_y>_rect.m_top && pt.m_y < _rect.m_bottom)
		{
			return true;
		}
		return false;
	}

	bool Canvas::judgeInTriangle(Point pt, std::vector<Point> _ptArray)
	{
		Point pt1 = _ptArray[0];
		Point pt2 = _ptArray[1];
		Point pt3 = _ptArray[2];

		int x = pt.m_x;
		int y = pt.m_y;

		// 计算直线斜率参数值
		float k1 = (float)(pt2.m_y - pt3.m_y) / (float)(pt2.m_x - pt3.m_x);
		float k2 = (float)(pt1.m_y - pt3.m_y) / (float)(pt1.m_x - pt3.m_x);
		float k3 = (float)(pt2.m_y - pt1.m_y) / (float)(pt2.m_x - pt1.m_x);

		// 计算直线 b 值
		float b1 = (float)pt2.m_y - k1 * (float)pt2.m_x;
		float b2 = (float)pt3.m_y - k2 * (float)pt3.m_x;
		float b3 = (float)pt1.m_y - k3 * (float)pt1.m_x;

		// 判断点是否在三角形范围内
		float judge1 = (y - (k1 * x + b1)) * (pt1.m_y - (k1 * pt1.m_x + b1));
		float judge2 = (y - (k2 * x + b2)) * (pt2.m_y - (k2 * pt2.m_x + b2));
		float judge3 = (y - (k3 * x + b3)) * (pt3.m_y - (k3 * pt3.m_x + b3));

		if (judge1 >= 0 && judge2 >= 0 && judge3 >= 0) {
			return true;
		}
		return false;
	}
	void Canvas::drawImage(int _x, int _y, Image* _image) 
	{
		for (int u = 0; u < _image->getWidth() ; ++u)
		{
			for (int v = 0; v < _image->getHeight(); ++v)
			{
				RGBA _srcColor = _image->getColor(u, v);
				if (!m_useBlend)
				{
					drawPoint(_x + u, _y + v, _srcColor);
				}
				else
				{
					RGBA _dstColor = getColor(_x + u, _y + v);
					float _srcAlpha = (float)_srcColor.m_a / 255.0;
					RGBA _finalColor = colorLerp(_dstColor, _srcColor, _image->getAlpha()*_srcAlpha);
					drawPoint(_x + u, _y + v, _finalColor);
				}
			}
		}
	}
	// 设置Canvas Alpha最小值
	void Canvas::setAlphaLimit(byte _limit)
	{
		m_alphaLimit = _limit;
	}
	// 开启Alpha混合
	void Canvas::setBlend(bool _useBlend)
	{
		m_useBlend = _useBlend;
	}
	// 开启纹理贴图
	void Canvas::enableTexture(bool _enable)
	{
		m_enableTexture = _enable;
	}
	//绑定纹理贴图
	void Canvas::bindTexture(const Image* _image)
	{
		m_texture = _image;
	}
}