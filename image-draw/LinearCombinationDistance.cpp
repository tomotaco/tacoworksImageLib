#include "LinearCombinationDistance.h"

LinearCombinationDistance::LinearCombinationDistance(const Vector2f& rPoint0, const Vector2f& rPoint1, const Vector2f& rPoint2,
													 const Point2i& rPosition)
	 : m_point0(rPoint0), m_point1(rPoint1), m_point2(rPoint2),
	   m_valueU(0.0f), m_offsetValueUX(0.0f), m_offsetValueUY(0.0f), m_borderValueU(0.0f),
	   m_valueV(0.0f), m_offsetValueVX(0.0f), m_offsetValueVY(0.0f), m_borderValueV(0.0f)
{
	Vector2f vecP0P1(rPoint1 - rPoint0);
	Vector2f vecP0P2(rPoint2 - rPoint0);
	float lengthP0P1 = vecP0P1.getLength();
	float lengthP0P2 = vecP0P2.getLength();
	float crossP1P0P2 = Vector2f::crossProduct(vecP0P1, vecP0P2);
	assert(!isZero(lengthP0P1) && !isZero(lengthP0P2) && !isZero(crossP1P0P2));
	if (!(!isZero(lengthP0P1) && !isZero(lengthP0P2) && !isZero(crossP1P0P2))) return;

	Vector2f vecUnitP0P1(vecP0P1);
	Vector2f vecUnitP0P2(vecP0P2);
	vecUnitP0P1.normalize();
	vecUnitP0P2.normalize();

	m_matInv.setElement(0, 0, vecP0P1.getX());
	m_matInv.setElement(1, 0, vecP0P1.getY());
	m_matInv.setElement(0, 1, vecP0P2.getX());
	m_matInv.setElement(1, 1, vecP0P2.getY());

	m_matInv.invert();

	Vector2f vecUnitX(1.0f, 0.0f), vecUnitY(0.0f, 1.0f);

	Vector2f offsetUVByX = m_matInv * vecUnitX;
	m_offsetValueUX = offsetUVByX.getX();
	m_offsetValueVX = offsetUVByX.getY();

	Vector2f offsetUVByY = m_matInv * vecUnitY;
	m_offsetValueUY = offsetUVByY.getX();
	m_offsetValueVY = offsetUVByY.getY();

	m_borderValueU = 1.0f / std::abs(Vector2f::crossProduct(vecP0P1, vecUnitP0P2));
	m_borderValueV = 1.0f / std::abs(Vector2f::crossProduct(vecP0P2, vecUnitP0P1));

	setPosition(rPosition);
}

void LinearCombinationDistance::setPosition(const Point2i& rPosition)
{
	Vector2f pointPFloat((float)rPosition.getX(), (float)rPosition.getY());
	Vector2f vecP0P(pointPFloat - m_point0);

	m_position = rPosition;

	Vector2f vecUV = m_matInv * vecP0P;

	m_valueU = vecUV.getX();
	m_valueV = vecUV.getY();
}
