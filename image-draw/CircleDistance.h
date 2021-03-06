#ifndef __CIRCLE_DISTANCE_H_INCLUDED
#define __CIRCLE_DISTANCE_H_INCLUDED

#include <cmath>

#include "Image.h"
#include "MathUtil.h"
#include "Vector2f.h"
#include "Rect2f.h"

class CircleDistance
{
public:
	CircleDistance(const Vector2f& rPointCenter, float radius, const Point2i& rPosition);

	CircleDistance(const CircleDistance& rX)
		: m_pointCenter(rX.m_pointCenter),
			m_radius(rX.m_radius), m_radiusSquared(rX.m_radiusSquared), m_radiusBoundarySquared(rX.m_radiusBoundarySquared), m_widthBoundarySquared(rX.m_widthBoundarySquared),
			m_position(rX.m_position),
			m_distanceSquaredFromCenter(rX.m_distanceSquaredFromCenter) {}

	const CircleDistance& operator=(const CircleDistance& rX)
	{
		m_pointCenter = rX.m_pointCenter;
		m_radius = rX.m_radius;
		m_radiusSquared = rX.m_radiusSquared;
		m_radiusBoundarySquared = rX.m_radiusBoundarySquared;
		m_widthBoundarySquared = rX.m_widthBoundarySquared;
		m_position = rX.m_position;
		m_distanceSquaredFromCenter = rX.m_distanceSquaredFromCenter;
		return *this;
	}

	void setPosition(const Point2i& rPosition);
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void moveX(int sign);
	void moveY(int sign);

	const Vector2f& getPointCenter() const { return m_pointCenter; }
	float getRadius() const { return m_radius; }
	float getRadiusSquared() const { return m_radiusSquared; }
	float getRadiusBoundarySquared() const { return m_radiusBoundarySquared; }
	float getWidthBoundarySquared() const { return m_widthBoundarySquared; }
	const Point2i& getPosition() const { return m_position; }
	float getDistanceSquaredFromCenter() const { return m_distanceSquaredFromCenter; }

private:
	float calcOffsetDistanceXSquared() const;
	float calcOffsetDistanceYSquared() const;

	Vector2f m_pointCenter;
	float m_radius;
	float m_radiusSquared;
	float m_radiusBoundarySquared;
	float m_widthBoundarySquared;
	Point2i m_position;
	float m_distanceSquaredFromCenter;
};

inline void CircleDistance::moveLeft()
{
	// offsetDistance は m_position の現在位置によって値が変わるので、先に計算
	m_position.setX(m_position.getX() - 1);
	m_distanceSquaredFromCenter -= calcOffsetDistanceXSquared();
}

inline void CircleDistance::moveRight()
{
	// offsetDistance は m_position の現在位置によって値が変わるので、先に計算
	m_distanceSquaredFromCenter += calcOffsetDistanceXSquared();
	m_position.setX(m_position.getX() + 1);
}

inline void CircleDistance::moveUp()
{
	// offsetDistance は m_position の現在位置によって値が変わるので、先に計算
	m_position.setY(m_position.getY() - 1);
	m_distanceSquaredFromCenter -= calcOffsetDistanceYSquared();
}

inline void CircleDistance::moveDown()
{
	// offsetDistance は m_position の現在位置によって値が変わるので、先に計算
	m_distanceSquaredFromCenter += calcOffsetDistanceYSquared();
	m_position.setY(m_position.getY() + 1);
}

inline void CircleDistance::moveX(int sign)
{
	if (sign > 0) moveRight(); else moveLeft();
}

inline void CircleDistance::moveY(int sign)
{
	if (sign > 0) moveDown(); else moveUp();
}

inline float CircleDistance::calcOffsetDistanceXSquared() const
{
	return (float(m_position.getX()) - m_pointCenter.getX()) * 2.0f + 1.0f;
}

inline float CircleDistance::calcOffsetDistanceYSquared() const
{
	return (float(m_position.getY()) - m_pointCenter.getY()) * 2.0f + 1.0f;
}

#endif
