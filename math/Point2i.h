#ifndef __POINT_2I_H_INCLUDED
#define __POINT_2I_H_INCLUDED

#include <cmath>
#include "MathUtil.h"

class	Point2i {
public:
	Point2i() : m_x(0), m_y(0) {}
	Point2i(const Point2i& rV) : m_x(rV.m_x), m_y(rV.m_y) {}
	Point2i(int x, int y) : m_x(x), m_y(y) {}

	void clear() { m_x = 0; m_y = 0; }
	int getX() const { return m_x; }
	int getY() const { return m_y; }
	void setX(int x) { m_x = x; }
	void setY(int y) { m_y = y; }
	void set(int x, int y) { m_x = x; m_y = y; }

	const Point2i& operator=(const Point2i& rV);
	const Point2i& operator+=(const Point2i& rV);
	const Point2i& operator-=(const Point2i& rV);
	Point2i operator-() const;

	bool operator==(const Point2i& rV) const;
	bool operator!=(const Point2i& rV) const;

private:
	int	m_x;
	int	m_y;

};

inline const Point2i& Point2i::operator=(const Point2i& rV)
{
	m_x = rV.m_x;
	m_y = rV.m_y;
	return *this;
}

inline const Point2i& Point2i::operator+=(const Point2i& rV)
{
	m_x += rV.m_x;
	m_y += rV.m_y;
	return *this;

}

inline const Point2i& Point2i::operator-=(const Point2i& rV)
{
	m_x -= rV.m_x;
	m_y -= rV.m_y;
	return *this;

}

inline Point2i Point2i::operator-() const
{
  return Point2i(-m_x, -m_y);
}

inline bool Point2i::operator==(const Point2i& rV) const
{
	return (m_x == rV.m_x) && (m_y == rV.m_y);
}

inline bool Point2i::operator!=(const Point2i& rV) const
{
	return !(*this == rV);
}

// 二項演算子
inline Point2i operator+(const Point2i& rV1, const Point2i& rV2)
{
	Point2i v = rV1;
	v += rV2;

	return v;
}

inline Point2i operator-(const Point2i& rV1, const Point2i& rV2)
{
	Point2i v = rV1;
	v -= rV2;

	return v;
}

#endif // __POINT_2I_H_INCLUDED
