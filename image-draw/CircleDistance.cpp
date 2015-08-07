#include "CircleDistance.h"

CircleDistance::CircleDistance(const Vector2f& rPointCenter, float radius, const Point2i& rPosition)
	 : m_pointCenter(rPointCenter), m_radius(radius)
{
	m_radiusSquared = radius * radius;
	m_radiusBoundarySquared = (radius + 1.0f) * (radius + 1.0f);
	m_widthBoundarySquared = m_radiusBoundarySquared - m_radiusSquared;
	setPosition(rPosition);
}

void CircleDistance::setPosition(const Point2i& rPosition)
{
	Vector2f pointPFloat(float(rPosition.getX()), float(rPosition.getY()));
	Vector2f vecRaduis(pointPFloat - m_pointCenter);

	m_position = rPosition;
	m_distanceSquaredFromCenter = vecRaduis.getLengthSquared();
}
