#pragma once
#include <Windows.h>
#include "GTMATH.hpp"

namespace GT
{
	struct RGBA
	{
		byte m_b;
		byte m_g;
		byte m_r;
		byte m_a;

		RGBA(byte _r = 255,
			byte _g = 255, 
			byte _b = 255, 
			byte _a = 255)
		{
			m_r = _r;
			m_g = _g;
			m_b = _b;
			m_a = _a;
		}
	};
	class Canvas
	{
	private:
		int m_width;
		int m_height;
		RGBA* m_buffer;

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
	};
}


