#ifndef __RECT_2F_H_INCLUDED
#define __RECT_2F_H_INCLUDED

#include "Vector2f.h"

class Rect2f
{
public:
	Rect2f() {}
	Rect2f(const Rect2f& rX) :
		m_lower(rX.m_lower), m_upper(rX.m_upper) {}
	Rect2f(const Vector2f& rPoint);
	Rect2f(const Vector2f& rPoint0, const Vector2f& rPoint1);
	Rect2f(float lowerX, float lowerY, float upperX, float upperY);

	const Rect2f& operator=(const Rect2f& rX);
	const Rect2f& operator+=(const Rect2f& rX); 
	const Rect2f& operator*=(const Rect2f& rX); 

	bool operator==(const Rect2f& rX) const;
	bool operator!=(const Rect2f& rX) const;

	Vector2f getCenter() const { return (m_lower + m_upper) * 0.5f; }

	const Vector2f& getLower() const { return m_lower; }
	const Vector2f& getUpper() const { return m_upper; }
	float getLowerX() const { return m_lower.getX(); }
	float getLowerY() const { return m_lower.getY(); }
	float getUpperX() const { return m_upper.getX(); }
	float getUpperY() const { return m_upper.getY(); }

	float getSizeX() const { return m_upper.getX() - m_lower.getX(); }
	float getSizeY() const { return m_upper.getY() - m_lower.getY(); }

	void setPoint(const Vector2f& rPoint);
	void addPoint(const Vector2f& rPoint);
	void expandX(float x);
	void expandY(float y);
	void shrinkLowerX(float x);
	void shrinkLowerY(float y);
	void shrinkUpperX(float x);
	void shrinkUpperY(float y);

	void move(const Vector2f& rOffset);

	bool doesIntersect(const Rect2f& rRect) const;
	bool doesInclude(const Rect2f& rRect) const;
	bool doesInclude(const Vector2f& rPoint) const;

private:
	Vector2f m_lower;
	Vector2f m_upper;
};

inline Rect2f::Rect2f(const Vector2f& rPoint)
{
	setPoint(rPoint);
}

inline Rect2f::Rect2f(const Vector2f& rPoint0, const Vector2f& rPoint1)
{
	setPoint(rPoint0);
	addPoint(rPoint1);
}

inline Rect2f::Rect2f(float lowerX, float lowerY, float upperX, float upperY)
: m_lower(lowerX, lowerY), m_upper(upperX, upperY)
{
	// nothing to be implemented
}

inline void Rect2f::setPoint(const Vector2f& rPoint)
{
	m_lower = rPoint;
	m_upper = rPoint;
}

inline void Rect2f::addPoint(const Vector2f& rPoint)
{
	expandX(rPoint.getX());
	expandY(rPoint.getY());
}

inline void Rect2f::expandX(float x)
{
	if (x < m_lower.getX()) m_lower.setX(x);
	if (m_upper.getX() < x) m_upper.setX(x);
}

inline void Rect2f::expandY(float y)
{
	if (y < m_lower.getY()) m_lower.setY(y);
	if (m_upper.getY() < y) m_upper.setY(y);
}

inline void Rect2f::shrinkLowerX(float x)
{
	if (m_lower.getX() < x) m_lower.setX(x);
}

inline void Rect2f::shrinkLowerY(float y)
{
	if (m_lower.getY() < y) m_lower.setY(y);
}

inline void Rect2f::shrinkUpperX(float x)
{
	if (x < m_upper.getX()) m_upper.setX(x);
}

inline void Rect2f::shrinkUpperY(float y)
{
	if (y < m_upper.getY()) m_upper.setY(y);
}

inline void Rect2f::move(const Vector2f& rOffset)
{
	m_lower += rOffset;
	m_upper += rOffset;
}

inline const Rect2f& Rect2f::operator=(const Rect2f& rX)
{
	m_lower = rX.m_lower;
	m_upper = rX.m_upper;

	return *this;
}

inline const Rect2f& Rect2f::operator+=(const Rect2f& rX)
{
	addPoint(rX.getLower());
	addPoint(rX.getUpper());

	return *this;
}

inline const Rect2f& Rect2f::operator*=(const Rect2f& rX)
{
	shrinkLowerX(rX.getLowerX());
	shrinkLowerY(rX.getLowerY());
	shrinkUpperX(rX.getUpperX());
	shrinkUpperY(rX.getUpperY());

	return *this;
}

inline bool Rect2f::operator==(const Rect2f& rX) const
{
	return (m_lower == rX.m_lower && m_upper == rX.m_upper) ? true : false;
}

inline bool Rect2f::operator!=(const Rect2f& rX) const
{
	return !(*this == rX);
}

inline bool Rect2f::doesIntersect(const Rect2f& rRect) const
{
	if (isZero(getSizeX()) && isZero(rRect.getSizeX())) {
		if (!isEqual(m_lower.getX(), rRect.getLowerX())) return false;
	} else if (m_lower.getX() < rRect.getLowerX()) {
		if (m_upper.getX() < rRect.getLowerX()) return false;
	} else {
		if (rRect.getUpperX() < m_lower.getX()) return false;
	}

	if (isZero(getSizeY()) && isZero(rRect.getSizeY())) {
		if (!isEqual(m_lower.getY(), rRect.getLowerY())) return false;
	} else if (m_lower.getY() < rRect.getLowerY()) {
		if (m_upper.getY() < rRect.getLowerY()) return false;
	} else {
		if (rRect.getUpperY() < m_lower.getY()) return false;
	}

	return true;
}

inline bool Rect2f::doesInclude(const Rect2f& rRect) const
{
	if (m_lower.getX() < rRect.getLowerX() && rRect.getUpperX() < m_upper.getX() &&
		m_lower.getY() < rRect.getLowerY() && rRect.getUpperY() < m_upper.getY()) {
		return true;
	} else {
		return false;
	}
}

inline bool Rect2f::doesInclude(const Vector2f& rPoint) const
{
	if (m_lower.getX() < rPoint.getX() && rPoint.getX() < m_upper.getX() &&
		m_lower.getY() < rPoint.getY() && rPoint.getY() < m_upper.getY()) {
		return true;
	} else {
		return false;
	}
}

inline Rect2f operator*(const Rect2f& rX, const Rect2f& rY)
{
	Rect2f rectResult(rX);
	rectResult *= rY;

	return rectResult;
}

inline Rect2f operator+(const Rect2f& rX, const Rect2f& rY)
{
	Rect2f rectResult(rX);
	rectResult += rY;

	return rectResult;
}

inline std::ostream& operator<<(std::ostream& rOst, const Rect2f& rX)
{
	rOst << " Rect2f: (" << rX.getLower() << " - " << rX.getUpper() << ")";
	return rOst;
}

#endif // __RECT_2F_H_INCLUDED
