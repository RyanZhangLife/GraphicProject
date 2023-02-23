#pragma once

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

template<class T>
void swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}




namespace GT {
	template<typename T>
	struct tVec2
	{
		T x;
		T y;

		tVec2(T _x, T _y) {
			x = _x;
			y = _y;
		}
		tVec2() {
			x = -1;
			y = -1;
		}
	};

	typedef tVec2<int> intV2;
	typedef tVec2<float> floatV2;

	typedef unsigned int uint;
	typedef unsigned char byte;

	struct GT_RECT
	{
		int m_left;
		int m_right;
		int m_top;
		int m_bottom;
		GT_RECT(
			int _left = 0,
			int _right = 0,
			int _top = 0,
			int _bottom = 0
		)
		{
			m_left = _left;
			m_right = _right;
			m_bottom = _bottom;
			m_top = _top;
		}
	};
}