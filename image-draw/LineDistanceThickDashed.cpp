#include "LineDistanceThickDashed.h"

LineDistanceThickDashed::LineDistanceThickDashed(const Vector2f& rPoint0, const Vector2f& rPoint1,
						   float halfWidthLine, float lengthLineDash, float lengthSpaceDash, float offsetDash,
						   const Point2i& rPosition)
	 : m_point0(rPoint0), m_point1(rPoint1),
	   m_halfWidthLine(halfWidthLine), m_lengthUnitDash(lengthLineDash), m_lengthSpaceDash(lengthSpaceDash), m_offsetDash(offsetDash),
	   m_distanceOnLine(0.0f), m_offsetDistanceOnLineX(0.0f), m_offsetDistanceOnLineY(0.0f),
	   m_distancePerpendicular(0.0f), m_offsetDistancePerpendicularX(0.0f), m_offsetDistancePerpendicularY(0.0f)
{
	Vector2f vecP0P1(rPoint1 - rPoint0);
	m_lengthLine = vecP0P1.getLength();
	assert(!isZero(m_lengthLine));
	if (isZero(m_lengthLine)) return;

	vecP0P1.normalize();
	m_vecUnitP0P1 = vecP0P1;
	m_vecUnitPerpendicular = vecP0P1.getPerpendicular();

	m_halfWidthLine = halfWidthLine;
	m_lengthLineDash = lengthLineDash;
	m_lengthSpaceDash = lengthSpaceDash;
	m_lengthUnitDash = lengthLineDash + lengthSpaceDash;

	setPosition(rPosition);
}

void LineDistanceThickDashed::setPosition(const Point2i& rPosition)
{
	Vector2f pointPFloat((float)rPosition.getX(), (float)rPosition.getY());
	Vector2f vecP0P(pointPFloat - m_point0);
	Vector2f vecUnitX(1.0f, 0.0f), vecUnitY(0.0f, 1.0f);

	m_position = rPosition;
	m_distanceOnLine = Vector2f::dotProduct(vecP0P, m_vecUnitP0P1);
	m_offsetDistanceOnLineX = Vector2f::dotProduct(vecUnitX, m_vecUnitP0P1);
	m_offsetDistanceOnLineY = Vector2f::dotProduct(vecUnitY, m_vecUnitP0P1);
	m_distancePerpendicular = Vector2f::dotProduct(vecP0P, m_vecUnitPerpendicular);
	m_offsetDistancePerpendicularX = Vector2f::dotProduct(vecUnitX, m_vecUnitPerpendicular);
	m_offsetDistancePerpendicularY = Vector2f::dotProduct(vecUnitY, m_vecUnitPerpendicular);

	m_distanceUnitDashOnLine = m_distanceOnLine + m_offsetDash;
	while (!isZero(m_distanceUnitDashOnLine) && m_distanceUnitDashOnLine < 0.0f) m_distanceUnitDashOnLine += m_lengthUnitDash;
	while (m_lengthUnitDash < m_distanceUnitDashOnLine) m_distanceUnitDashOnLine -= m_lengthUnitDash;
}
