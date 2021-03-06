#ifndef __LINEAR_COMBINATION_DISTANCE_H_INCLUDED
#define __LINEAR_COMBINATION_DISTANCE_H_INCLUDED

#include <cmath>

#include "Image.h"
#include "MathUtil.h"
#include "Vector2f.h"
#include "Matrix2f.h"
#include "Rect2f.h"

class LinearCombinationDistance
{
public:
	LinearCombinationDistance(const Vector2f& rPoint0, const Vector2f& rPoint1, const Vector2f& rPoint2,
		const Point2i& rPosition);

	LinearCombinationDistance(const LinearCombinationDistance& rX)
		 : m_point0(rX.m_point0), m_point1(rX.m_point1), m_point2(rX.m_point2),
		   m_matInv(rX.m_matInv),
		   m_position(rX.m_position), 
		   m_valueU(rX.m_valueU), m_offsetValueUX(rX.m_offsetValueUX), m_offsetValueUY(rX.m_offsetValueUY), m_borderValueU(rX.m_borderValueU),
		   m_valueV(rX.m_valueV), m_offsetValueVX(rX.m_offsetValueVX), m_offsetValueVY(rX.m_offsetValueVY), m_borderValueV(rX.m_borderValueV)
	{}

	const LinearCombinationDistance& operator=(const LinearCombinationDistance& rX)
	{
		m_point0 = rX.m_point0;
		m_point1 = rX.m_point1;
		m_point2 = rX.m_point2;

		m_matInv = rX.m_matInv;
		m_position = rX.m_position;
		m_valueU = rX.m_valueU;
		m_offsetValueUX = rX.m_offsetValueUX;
		m_offsetValueUY = rX.m_offsetValueUY;
		m_borderValueU = rX.m_borderValueU;
		m_valueV = rX.m_valueV;
		m_offsetValueVX = rX.m_offsetValueVX;
		m_offsetValueVY = rX.m_offsetValueVY;
		m_borderValueV = rX.m_borderValueV;

		return *this;
	}

	void setPosition(const Point2i& rPosition);
	void moveX(int sign);
	void moveY(int sign);

	const Vector2f& getPoint0() const { return m_point0; }
	const Vector2f& getPoint1() const { return m_point1; }
	const Vector2f& getPoint2() const { return m_point2; }

	const Matrix2f& getMatrixInv() const { return m_matInv; }

	const Point2i& getPosition() const { return m_position; }
	float getValueU() const { return m_valueU; }
	float getValueV() const { return m_valueV; }

	float getOffsetValueUX() const { return m_offsetValueUX; }
	float getOffsetValueUY() const { return m_offsetValueUY; }
	float getOffsetValueVX() const { return m_offsetValueVX; }
	float getOffsetValueVY() const { return m_offsetValueVY; }

	float getBorderValueU() const { return m_borderValueU; }
	float getBorderValueV() const { return m_borderValueV; }

private:
	Vector2f m_point0;
	Vector2f m_point1;
	Vector2f m_point2;

	Matrix2f m_matInv;

	Point2i m_position;

	float m_valueU; // P0P1 成分
	float m_offsetValueUX; // x ++ したときの valueU の変化量
	float m_offsetValueUY; // y ++ したときの valueU の変化量
	float m_borderValueU;

	float m_valueV; // P0P2 成分
	float m_offsetValueVX; // x ++ したときの valueV の変化量
	float m_offsetValueVY; // y ++ したときの valueV の変化量
	float m_borderValueV;
};

inline void LinearCombinationDistance::moveX(int sign)
{
	assert(sign == -1 || sign == 1);
	if (!(sign == -1 || sign == 1)) return;

	m_position.setX(m_position.getX() + sign);
	if (sign < 0) {
		m_valueU -= m_offsetValueUX;
		m_valueV -= m_offsetValueVX;
	} else {
		m_valueU += m_offsetValueUX;
		m_valueV += m_offsetValueVX;
	}
}

inline void LinearCombinationDistance::moveY(int sign)
{
	assert(sign == -1 || sign == 1);
	if (!(sign == -1 || sign == 1)) return;

	m_position.setY(m_position.getY() + sign);
	if (sign < 0) {
		m_valueU -= m_offsetValueUY;
		m_valueV -= m_offsetValueVY;
	} else {
		m_valueU += m_offsetValueUY;
		m_valueV += m_offsetValueVY;
	}
}

#endif
