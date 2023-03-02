#pragma once
#include <Windows.h>
#include "GTMATH.hpp"
#include <vector>
#include "Image.h"

namespace GT
{
	struct Point
	{
	public:
		int m_x;
		int m_y;
		RGBA m_color;
		floatV2 m_uv; // ͼƬ��uv��������
		Point(int _x = 0, int _y = 0, RGBA _color = RGBA(0, 0, 0, 0), floatV2 _uv = floatV2(0.0, 0.0))
		{
			m_x = _x;
			m_y = _y;
			m_color = _color;
			m_uv = _uv;
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

		byte m_alphaLimit; // Canvas��͸������Сֵ
		bool m_useBlend; // �Ƿ���Alpha���
		bool m_enableTexture; // �Ƿ�����������ͼ
		const Image* m_texture; // ������ͼ��ͼƬ
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
			m_useBlend = false;
			m_enableTexture = false;
		}
		~Canvas() 
		{
			// buffer�����ⲿ��������Canvas�ڲ��ͷ�
		}

		// ��ϴ����
		void clear()
		{
			if (m_buffer != nullptr)
			{
				memset(m_buffer, 0, sizeof(RGBA) * m_width * m_height);
			}
		}

		// �������
		void drawPoint(int x, int y, RGBA _color) 
		{
			if (x < 0 || x > m_width || y < 0 || y > m_height) {
				return;
			}
			m_buffer[y * m_width + x] = _color;
		}

		// ��ȡ���϶�Ӧ����ɫ
		RGBA getColor(int x, int y)
		{
			if (x < 0 || x > m_width || y < 0 || y > m_height) {
				return RGBA(0, 0, 0);
			}
			return m_buffer[y * m_width + x];
		}

		// ��ɫ�Ĳ�ֵ����
		inline RGBA colorLerp(RGBA _color1, RGBA _color2, float _scale)
		{
			RGBA _color;
			_color.m_r = _color1.m_r + (float)(_color2.m_r - _color1.m_r) * _scale;
			_color.m_g = _color1.m_g + (float)(_color2.m_g - _color1.m_g) * _scale;
			_color.m_b = _color1.m_b + (float)(_color2.m_b - _color1.m_b) * _scale;
			_color.m_a = _color1.m_a + (float)(_color2.m_a - _color1.m_a) * _scale;
			return _color;
		}

		floatV2 uvLerp(floatV2 _uv1, floatV2 _uv2, float _scale);
		// Brensenham�����㷨
		void drawLine(Point pt1, Point pt2);
		// ɨ���߻����������㷨
		void scaningDrawTriangle(Point pt1, Point pt2, Point pt3);
		void drawTriangleFlat(Point ptFlat1, Point ptFlat2, Point pt);
		// ͨ�������λ����㷨
		void drawTriangle(Point pt1, Point pt2, Point pt3);
		// �жϵ��Ƿ��ڳ����巶Χ��
		bool judgeInRect(Point pt, GT_RECT _rect);
		// �жϵ��Ƿ�����������
		bool judgeInTriangle(Point pt, std::vector<Point> _ptArray);

		// ͼƬ����
		void drawImage(int _x, int _y, Image* _image);
		void setAlphaLimit(byte _limit);


		void setBlend(bool _useBlend);
		void enableTexture(bool _enable);
		void bindTexture(const Image* _image);
	};
}


