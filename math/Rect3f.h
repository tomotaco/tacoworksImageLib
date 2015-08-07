#ifndef __RECT_3F_H_INCLUDED
#define __RECT_3F_H_INCLUDED

#include "Vector3f.h"

class Rect3f
{
public:
	Rect3f() {}
	Rect3f(const Rect3f& rX) :
		m_lower(rX.m_lower), m_upper(rX.m_upper) {}
	Rect3f(const Vector3f& rPoint);

	const Rect3f& operator=(const Rect3f& rX);
	const Rect3f& operator+=(const Rect3f& rX); 
	const Rect3f& operator*=(const Rect3f& rX); 

	bool operator==(const Rect3f& rX) const;
	bool operator!=(const Rect3f& rX) const;

	const Vector3f& getLower() const { return m_lower; }
	const Vector3f& getUpper() const { return m_upper; }
	float getLowerX() const { return m_lower.getX(); }
	float getLowerY() const { return m_lower.getY(); }
	float getLowerZ() const { return m_lower.getZ(); }
	float getUpperX() const { return m_upper.getX(); }
	float getUpperY() const { return m_upper.getY(); }
	float getUpperZ() const { return m_upper.getZ(); }

	float getSizeX() const { return m_upper.getX() - m_lower.getX(); }
	float getSizeY() const { return m_upper.getY() - m_lower.getY(); }
	float getSizeZ() const { return m_upper.getZ() - m_lower.getZ(); }

	Vector3f getCenter() const { return (m_lower + m_upper) * 0.5f; }

	void setPoint(const Vector3f& rPoint);
	void addPoint(const Vector3f& rPoint);
	void expandX(float x);
	void expandY(float y);
	void expandZ(float z);
	void shrinkLowerX(float x);
	void shrinkLowerY(float y);
	void shrinkLowerZ(float z);
	void shrinkUpperX(float x);
	void shrinkUpperY(float y);
	void shrinkUpperZ(float z);

	void move(const Vector3f& rOffset);

	bool doesIntersect(const Rect3f& rRect) const;
	bool doesInclude(const Rect3f& rRect) const;
	bool doesInclude(const Vector3f& rPoint) const;

private:
	Vector3f m_lower;
	Vector3f m_upper;
};

inline Rect3f::Rect3f(const Vector3f& rPoint)
: m_lower(rPoint), m_upper(rPoint)
{
	// nothing to be implemented
}

inline void Rect3f::setPoint(const Vector3f& rPoint)
{
	m_lower = rPoint;
	m_upper = rPoint;
}

inline void Rect3f::addPoint(const Vector3f& rPoint)
{
	expandX(rPoint.getX());
	expandY(rPoint.getY());
	expandZ(rPoint.getZ());
}

inline void Rect3f::expandX(float x)
{
	if (x < m_lower.getX()) m_lower.setX(x);
	if (m_upper.getX() < x) m_upper.setX(x);
}

inline void Rect3f::expandY(float y)
{
	if (y < m_lower.getY()) m_lower.setY(y);
	if (m_upper.getY() < y) m_upper.setY(y);
}

inline void Rect3f::expandZ(float z)
{
	if (z < m_lower.getZ()) m_lower.setZ(z);
	if (m_upper.getZ() < z) m_upper.setZ(z);
}

inline void Rect3f::shrinkLowerX(float x)
{
	if (m_lower.getX() < x) m_lower.setX(x);
}

inline void Rect3f::shrinkLowerY(float y)
{
	if (m_lower.getY() < y) m_lower.setY(y);
}

inline void Rect3f::shrinkLowerZ(float z)
{
	if (m_lower.getZ() < z) m_lower.setZ(z);
}

inline void Rect3f::shrinkUpperX(float x)
{
	if (x < m_upper.getX()) m_upper.setX(x);
}

inline void Rect3f::shrinkUpperY(float y)
{
	if (y < m_upper.getY()) m_upper.setY(y);
}

inline void Rect3f::shrinkUpperZ(float z)
{
	if (z < m_upper.getZ()) m_upper.setZ(z);
}

inline void Rect3f::move(const Vector3f& rOffset)
{
	m_lower += rOffset;
	m_upper += rOffset;
}

inline const Rect3f& Rect3f::operator=(const Rect3f& rX)
{
	m_lower = rX.m_lower;
	m_upper = rX.m_upper;

	return *this;
}

inline const Rect3f& Rect3f::operator+=(const Rect3f& rX)
{
	addPoint(rX.getLower());
	addPoint(rX.getUpper());

	return *this;
}

inline const Rect3f& Rect3f::operator*=(const Rect3f& rX)
{
	shrinkLowerX(rX.getLowerX());
	shrinkLowerY(rX.getLowerY());
	shrinkLowerZ(rX.getLowerZ());
	shrinkUpperX(rX.getUpperX());
	shrinkUpperY(rX.getUpperY());
	shrinkUpperZ(rX.getUpperZ());

	return *this;
}

inline bool Rect3f::operator==(const Rect3f& rX) const
{
	return (m_lower == rX.m_lower && m_upper == rX.m_upper) ? true : false;
}

inline bool Rect3f::operator!=(const Rect3f& rX) const
{
	return !(*this == rX);
}


inline bool Rect3f::doesIntersect(const Rect3f& rRect) const
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
	if (isZero(getSizeZ()) && isZero(rRect.getSizeZ())) {
		if (!isEqual(m_lower.getZ(), rRect.getLowerZ())) return false;
	} else if (m_lower.getZ() < rRect.getLowerZ()) {
		if (m_upper.getZ() < rRect.getLowerZ()) return false;
	} else {
		if (rRect.getUpperZ() < m_lower.getZ()) return false;
	}

	return true;
}

inline bool Rect3f::doesInclude(const Rect3f& rRect) const
{
	if (m_lower.getX() < rRect.getLowerX() && rRect.getUpperX() < m_upper.getX() &&
		m_lower.getY() < rRect.getLowerY() && rRect.getUpperY() < m_upper.getY() &&
		m_lower.getZ() < rRect.getLowerZ() && rRect.getUpperZ() < m_upper.getZ()) {
		return true;
	} else {
		return false;
	}
}

inline bool Rect3f::doesInclude(const Vector3f& rPoint) const
{
	if (m_lower.getX() < rPoint.getX() && rPoint.getX() < m_upper.getX() &&
		m_lower.getY() < rPoint.getY() && rPoint.getY() < m_upper.getY() &&
		m_lower.getZ() < rPoint.getZ() && rPoint.getZ() < m_upper.getZ()) {
		return true;
	} else {
		return false;
	}
}

inline Rect3f operator*(const Rect3f& rX, const Rect3f& rY)
{
	Rect3f rectResult(rX);
	rectResult *= rY;

	return rectResult;
}

inline Rect3f operator+(const Rect3f& rX, const Rect3f& rY)
{
	Rect3f rectResult(rX);
	rectResult += rY;

	return rectResult;
}

#endif // __RECT_3F_H_INCLUDED