#ifndef __RASTERIZED_AREA_H_INCLUDED
#define __RASTERIZED_AREA_H_INCLUDED

#include <cmath>
#include <vector>
#include "Vector2f.h"
#include "Rect2i.h"

class RasterSegment {
public:
	RasterSegment() : m_left(0), m_right(0) {}
	RasterSegment(const RasterSegment& rX) : m_left(rX.m_left), m_right(rX.m_right) {}
	RasterSegment(int left, int right) : m_left(left), m_right(right) {}

	const RasterSegment& operator=(const RasterSegment& rX)
	{
		m_left = rX.m_left;
		m_right = rX.m_right;
		return *this;
	}

	int		getLeftBound() const { return m_left; }
	int		getRightBound() const { return m_right; }
	void	setLeftBound(int x) { m_left = x; }
	void	setRightBound(int x) { m_right = x; }
	int		getLength() const { return m_right - m_left; }

public:
	int m_left;		// included
	int m_right;	// not included
};

class LineSegment {
public:
	LineSegment() : m_p0(0.0f, 0.0f), m_p1(0.0f, 0.0f) {}
	LineSegment(const LineSegment& rX) : m_p0(rX.m_p0), m_p1(rX.m_p1) {}
	LineSegment(const Vector2f& rP0, const Vector2f& rP1) : m_p0(rP0), m_p1(rP1) {}

	const LineSegment& operator=(const LineSegment& rX)
	{
		m_p0 = rX.m_p0;
		m_p1 = rX.m_p1;
		return *this;
	}

	Vector2f	getP0() const { return m_p0; }
	Vector2f	getP1() const { return m_p1; }
	void		setP0(const Vector2f& rP0) { m_p0 = rP0; }
	void		setP1(const Vector2f& rP1) { m_p1 = rP1; }
	float getLength() const
	{
		float offsetX = m_p1.getX() - m_p0.getX();
		float offsetY = m_p1.getY() - m_p0.getY();
		return std::sqrt(offsetX * offsetX + offsetY * offsetY);
	}

	float		getMinX() const { return (m_p0.getX() < m_p1.getX()) ? m_p0.getX() : m_p1.getX(); }
	float		getMinY() const { return (m_p0.getY() < m_p1.getY()) ? m_p0.getY() : m_p1.getY(); }
	float		getMaxX() const { return (m_p0.getX() < m_p1.getX()) ? m_p1.getX() : m_p0.getX(); }
	float		getMaxY() const { return (m_p0.getY() < m_p1.getY()) ? m_p1.getY() : m_p0.getY(); }

private:
	Vector2f	m_p0;
	Vector2f	m_p1;
};

class RasterizedArea {
public:
	RasterizedArea(const std::vector<Vector2f>& rPoints);
	~RasterizedArea();

	const	Rect2i&	getBoundingBox() const { return m_boundingBox; }
	bool	getRasterSegments(int y, std::vector<RasterSegment>& rRasterSegments) const;

private:
	void	getXHitList(int y, std::vector<float>& rXHitList) const;

	Rect2i	m_boundingBox;
	std::vector<LineSegment>		m_lineSegments;
};

#endif // __RASTERIZED_AREA_H_INCLUDED

