#pragma once
#include <Windows.h>
#include "GTMATH.hpp"
#include <vector>
#include "Image.h"

namespace GT
{
	class Point
	{
	public:
		int m_x;
		int m_y;
		RGBA m_color;
		Point(int _x, int _y, RGBA _color)
		{
			m_x = _x;
			m_y = _y;
			m_color = _color;
		}
		~Point()
		{

		}
	};

	class Canvas
	{
	private:
		int m_width;
		int m_height;
		RGBA* m_buffer;

		byte m_alphaLimit; // Canvas的透明度最小值

	public:
		Canvas(int _width, int _height, void* _buffer) 
		{
			if (_width <= 0 || _height <= 0) {
				m_width = -1;
				m_height = -1;
				m_buffer = nullptr;
			}
			m_width = _width;
			m_height = _height;
			m_buffer = (RGBA*)_buffer;
		}
		~Canvas() 
		{
			// buffer存在外部，无需在Canvas内部释放
		}

		// 清洗画卷
		void clear()
		{
			if (m_buffer != nullptr)
			{
				memset(m_buffer, 0, sizeof(RGBA) * m_width * m_height);
			}
		}

		// 画点操作
		void drawPoint(int x, int y, RGBA _color) 
		{
			if (x < 0 || x > m_width || y < 0 || y > m_height) {
				return;
			}
			m_buffer[y * m_width + x] = _color;
		}

		// 颜色的插值计算
		inline RGBA colorLerp(RGBA _color1, RGBA _color2, float _scale)
		{
			RGBA _color;
			_color.m_r = _color1.m_r + (float)(_color2.m_r - _color1.m_r) * _scale;
			_color.m_g = _color1.m_g + (float)(_color2.m_g - _color1.m_g) * _scale;
			_color.m_b = _color1.m_b + (float)(_color2.m_b - _color1.m_b) * _scale;
			_color.m_a = _color1.m_a + (float)(_color2.m_a - _color1.m_a) * _scale;
			return _color;
		}

		// Brensenham画线算法
		void drawLine(Point pt1, Point pt2);
		// 扫描线绘制三角形算法
		void scaningDrawTriangle(Point pt1, Point pt2, Point pt3);
		void drawTriangleFlat(Point ptFlat1, Point ptFlat2, Point pt);
		// 通用三角形绘制算法
		void drawTriangle(Point pt1, Point pt2, Point pt3);
		// 判断点是否在长方体范围内
		bool judgeInRect(Point pt, GT_RECT _rect);
		// 判断点是否在三角形内
		bool judgeInTriangle(Point pt, std::vector<Point> _ptArray);

		// 图片操作
		void drawImage(int _x, int _y, Image* _image);
		void setAlphaLimit(byte _limit);
	};
}


