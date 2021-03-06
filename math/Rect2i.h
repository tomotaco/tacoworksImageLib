#ifndef __RECT_2I_H_INCLUDED
#define __RECT_2I_H_INCLUDED

#include "Point2i.h"

class Rect2i
{
public:
	Rect2i() {}
	Rect2i(const Rect2i& rX) :
		m_lower(rX.m_lower), m_upper(rX.m_upper) {}
	Rect2i(const Point2i& rPoint);
	Rect2i(const Point2i& rPoint0, const Point2i& rPoint1);
	Rect2i(int lowerX, int lowerY, int upperX, int upperY);

	const Rect2i& operator=(const Rect2i& rX);
	const Rect2i& operator+=(const Rect2i& rX); 
	const Rect2i& operator*=(const Rect2i& rX);

	bool operator==(const Rect2i& rX) const;
	bool operator!=(const Rect2i& rX) const;

	const Point2i& getLower() const { return m_lower; }
	const Point2i& getUpper() const { return m_upper; }
	int getLowerX() const { return m_lower.getX(); }
	int getLowerY() const { return m_lower.getY(); }
	int getUpperX() const { return m_upper.getX(); }
	int getUpperY() const { return m_upper.getY(); }

	int getSizeX() const { return m_upper.getX() - m_lower.getX(); }
	int getSizeY() const { return m_upper.getY() - m_lower.getY(); }
	int getWidth() const { return getSizeX(); }
	int getHeight() const { return getSizeY(); }

	Point2i getCenter() const;

	void setPoint(const Point2i& rPoint);
	void addPoint(const Point2i& rPoint);
	void expandX(int x);
	void expandY(int y);
	void shrinkLowerX(int x);
	void shrinkLowerY(int y);
	void shrinkUpperX(int x);
	void shrinkUpperY(int y);

	void move(const Point2i& rOffset);

	bool doesIntersect(const Rect2i& rRect) const;
	bool doesInclude(const Rect2i& rRect) const;
	bool doesInclude(const Point2i& rPoint) const;

private:
	Point2i m_lower;
	Point2i m_upper;
};

inline Rect2i::Rect2i(const Point2i& rPoint)
{
	setPoint(rPoint);
}

inline Rect2i::Rect2i(const Point2i& rPoint0, const Point2i& rPoint1)
{
	setPoint(rPoint0);
	addPoint(rPoint1);
}

inline Rect2i::Rect2i(int lowerX, int lowerY, int upperX, int upperY)
: m_lower(lowerX, lowerY), m_upper(upperX, upperY)
{
	// nothing to be implemented
}

inline Point2i Rect2i::getCenter() const
{
	Point2i point = m_lower + m_upper;
	return Point2i(point.getX() / 2, point.getY() / 2);
}
inline void Rect2i::setPoint(const Point2i& rPoint)
{
	m_lower = rPoint;
	m_upper = rPoint;
}

inline void Rect2i::addPoint(const Point2i& rPoint)
{
	expandX(rPoint.getX());
	expandY(rPoint.getY());
}

inline void Rect2i::expandX(int x)
{
	// Lower なら x は範囲内
	if (x < m_lower.getX()) m_lower.setX(x);

	// Upper なら x は範囲の外側
	if (m_upper.getX() < x) m_upper.setX(x);
}

inline void Rect2i::expandY(int y)
{
	// Lower なら y は範囲内
	if (y < m_lower.getY()) m_lower.setY(y);

	// Upper なら y は範囲の外側
	if (m_upper.getY() < y) m_upper.setY(y);
}

inline void Rect2i::shrinkLowerX(int x)
{
	if (m_lower.getX() < x) m_lower.setX(x);
}

inline void Rect2i::shrinkLowerY(int y)
{
	if (m_lower.getY() < y) m_lower.setY(y);
}

inline void Rect2i::shrinkUpperX(int x)
{
	// Upper なら x は範囲の外側
	if (x < m_upper.getX()) m_upper.setX(x);
}

inline void Rect2i::shrinkUpperY(int y)
{
	// Upper なら y は範囲の外側
	if (y < m_upper.getY()) m_upper.setY(y);
}

inline void Rect2i::move(const Point2i& rOffset)
{
	m_lower += rOffset;
	m_upper += rOffset;
}


inline const Rect2i& Rect2i::operator=(const Rect2i& rX)
{
	m_lower = rX.m_lower;
	m_upper = rX.m_upper;

	return *this;
}

inline const Rect2i& Rect2i::operator+=(const Rect2i& rX)
{
	addPoint(rX.getLower());
	addPoint(rX.getUpper());

	return *this;
}

inline const Rect2i& Rect2i::operator*=(const Rect2i& rX)
{
	shrinkLowerX(rX.getLowerX());
	shrinkLowerY(rX.getLowerY());
	shrinkUpperX(rX.getUpperX());
	shrinkUpperY(rX.getUpperY());

	return *this;
}

inline bool Rect2i::operator==(const Rect2i& rX) const
{
	return (m_lower == rX.m_lower && m_upper == rX.m_upper) ? true : false;
}

inline bool Rect2i::operator!=(const Rect2i& rX) const
{
	return !(*this == rX);
}

inline bool Rect2i::doesIntersect(const Rect2i& rRect) const
{
	if (m_lower.getX() < rRect.getLowerX()) {
		if (m_upper.getX() < rRect.getLowerX()) return false;
	} else {
		if (rRect.getUpperX() < m_lower.getX()) return false;
	}

	if (m_lower.getY() < rRect.getLowerY()) {
		if (m_upper.getY() < rRect.getLowerY()) return false;
	} else {
		if (rRect.getUpperY() < m_lower.getY()) return false;
	}

	return true;
}

inline bool Rect2i::doesInclude(const Rect2i& rRect) const
{
	if (m_lower.getX() <= rRect.getLowerX() && rRect.getUpperX() <= m_upper.getX() &&
		m_lower.getY() <= rRect.getLowerY() && rRect.getUpperY() <= m_upper.getY()) {
		return true;
	} else {
		return false;
	}
}

inline bool Rect2i::doesInclude(const Point2i& rPoint) const
{
	if (m_lower.getX() <= rPoint.getX() && rPoint.getX() < m_upper.getX() &&
		m_lower.getY() <= rPoint.getY() && rPoint.getY() < m_upper.getY()) {
		return true;
	} else {
		return false;
	}
}

inline Rect2i operator*(const Rect2i& rX, const Rect2i& rY)
{
	Rect2i rectResult(rX);
	rectResult *= rY;

	return rectResult;
}

inline Rect2i operator+(const Rect2i& rX, const Rect2i& rY)
{
	Rect2i rectResult(rX);
	rectResult += rY;

	return rectResult;
}

#endif // __RECT_2F_H_INCLUDED