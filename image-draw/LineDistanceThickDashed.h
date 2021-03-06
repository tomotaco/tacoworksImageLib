#ifndef __LINE_DISTANCE_THICK_DASHED_H_INCLUDED
#define __LINE_DISTANCE_THICK_DASHED_H_INCLUDED

#include <cmath>

#include "Image.h"
#include "MathUtil.h"
#include "Vector2f.h"
#include "Rect2f.h"

class LineDistanceThickDashed
{
public:
	LineDistanceThickDashed(const Vector2f& rPoint0, const Vector2f& rPoint1,
		float halfWidthLine, float lengthLineDash, float lengthSpaceDash, float offsetDash,
		const Point2i& rPosition);

	LineDistanceThickDashed(const LineDistanceThickDashed& rX)
		 : 	m_point0(rX.m_point0), m_point1(rX.m_point1), 
			m_lengthLine(rX.m_lengthLine),
			m_vecUnitP0P1(rX.m_vecUnitP0P1), m_vecUnitPerpendicular(rX.m_vecUnitPerpendicular),
			m_halfWidthLine(rX.m_halfWidthLine),
			m_lengthLineDash(rX.m_lengthLineDash),
			m_lengthSpaceDash(rX.m_lengthSpaceDash),
			m_lengthUnitDash(rX.m_lengthUnitDash),
			m_offsetDash(rX.m_offsetDash),
			m_position(rX.m_position),
			m_distanceOnLine(rX.m_distanceOnLine),
			m_distanceUnitDashOnLine(rX.m_distanceUnitDashOnLine),
			m_offsetDistanceOnLineX(rX.m_offsetDistanceOnLineX),
			m_offsetDistanceOnLineY(rX.m_offsetDistanceOnLineY),
			m_distancePerpendicular(rX.m_distancePerpendicular),
			m_offsetDistancePerpendicularX(rX.m_offsetDistancePerpendicularX),
			m_offsetDistancePerpendicularY(rX.m_offsetDistancePerpendicularY) {}

	const LineDistanceThickDashed& operator=(const LineDistanceThickDashed& rX)
	{
		m_point0 = rX.m_point0;
		m_point1 = rX.m_point0;
		m_lengthLine = rX.m_lengthLine;
		m_vecUnitP0P1 = rX.m_vecUnitP0P1;
		m_vecUnitPerpendicular = rX.m_vecUnitPerpendicular;
		m_halfWidthLine = rX.m_halfWidthLine;
		m_lengthLineDash = rX.m_lengthLineDash;
		m_lengthSpaceDash = rX.m_lengthSpaceDash;
		m_lengthUnitDash = rX.m_lengthUnitDash;
		m_offsetDash = rX.m_offsetDash;

		m_position = rX.m_position;
		m_distanceOnLine = rX.m_distanceOnLine;
		m_distanceUnitDashOnLine = rX.m_distanceUnitDashOnLine;
		m_offsetDistanceOnLineX = rX.m_offsetDistanceOnLineX;
		m_offsetDistanceOnLineY = rX.m_offsetDistanceOnLineY;
		m_distancePerpendicular = rX.m_distancePerpendicular;
		m_offsetDistancePerpendicularX = rX.m_offsetDistancePerpendicularX;
		m_offsetDistancePerpendicularY = rX.m_offsetDistancePerpendicularY;
		return *this;
	}

	void setPosition(const Point2i& rPosition);
	void move(bool doesMoveX, int sign);
	void moveX(int sign);
	void moveY(int sign);
	void moveToFitDistancePerpendicular(bool doesMoveX);
	void moveXToFitDistancePerpendicular();
	void moveYToFitDistancePerpendicular();

	const Vector2f& getPoint0() const { return m_point0; }
	const Vector2f& getPoint1() const { return m_point1; }
	float getLengthLine() const { return m_lengthLine; }
	const Vector2f& getVecUnitP0P1() const { return m_vecUnitP0P1; }
	const Vector2f& getVecUnitPerpendicular() const { return m_vecUnitPerpendicular; }
	float getHaflWidthLine() const { return m_halfWidthLine; }
	float getLengthLineDash() const { return m_lengthLineDash; }
	float getLengthSpaceDash() const { return m_lengthSpaceDash; }
	float getLengthUnitDash() const { return m_lengthUnitDash; }
	float getOffsetDash() const { return m_offsetDash; }

	const Point2i& getPosition() const { return m_position; }
	float getDistanceOnLine() const { return m_distanceOnLine; }
	float getDistanceUnitDashOnLine() const { return m_distanceUnitDashOnLine; }
	float getOffsetDistanceOnLineX() const { return m_offsetDistanceOnLineX; }
	float getOffsetDistanceOnLineY() const { return m_offsetDistanceOnLineY; }
	float getDistancePerpendicular() const { return m_distancePerpendicular; }
	float getOffsetDistancePerpendicularX() const { return m_offsetDistancePerpendicularX; }
	float getOffsetDistancePerpendicularY() const { return m_offsetDistancePerpendicularY; }

private:
	void wrapDistanceUnitDashOnLine();

	Vector2f m_point0;
	Vector2f m_point1;
	float m_lengthLine;
	Vector2f m_vecUnitP0P1;
	Vector2f m_vecUnitPerpendicular;

	float m_halfWidthLine;
	float m_lengthLineDash;
	float m_lengthSpaceDash;
	float m_lengthUnitDash;
	float m_offsetDash;

	Point2i m_position;

	float m_distanceOnLine;	// P を直線 P0P1 上に下ろした垂線の足と P0 との距離
	float m_distanceUnitDashOnLine; // P を直線状に下ろした垂線の足と P0 との距離を破線単位内に収めた距離
	float m_offsetDistanceOnLineX; // x ++ したときの distacneOnLine の変化量
	float m_offsetDistanceOnLineY; // y ++ したときの distacneOnLine の変化量
	float m_distancePerpendicular;	// P と直線 P0P1 との距離
	float m_offsetDistancePerpendicularX; // x ++ したときの distacnePerpendicular の変化量
	float m_offsetDistancePerpendicularY; // y ++ したときの distacnePerpendicular の変化量
};

inline void LineDistanceThickDashed::move(bool doesMoveX, int sign)
{
	if (doesMoveX) {
		moveX(sign);
	} else {
		moveY(sign);
	}
}

inline void LineDistanceThickDashed::moveX(int sign)
{
	assert(sign == -1 || sign == 1);
	if (!(sign == -1 || sign == 1)) return;

	m_position.setX(m_position.getX() + sign);
	if (sign < 0) {
		m_distanceOnLine -= m_offsetDistanceOnLineX;
		m_distanceUnitDashOnLine -= m_offsetDistanceOnLineX;
		m_distancePerpendicular -= m_offsetDistancePerpendicularX;
	} else {
		m_distanceOnLine += m_offsetDistanceOnLineX;
		m_distanceUnitDashOnLine += m_offsetDistanceOnLineX;
		m_distancePerpendicular += m_offsetDistancePerpendicularX;
	}
	wrapDistanceUnitDashOnLine();
}

inline void LineDistanceThickDashed::moveY(int sign)
{
	assert(sign == -1 || sign == 1);
	if (!(sign == -1 || sign == 1)) return;

	m_position.setY(m_position.getY() + sign);
	if (sign < 0) {
		m_distanceOnLine -= m_offsetDistanceOnLineY;
		m_distanceUnitDashOnLine -= m_offsetDistanceOnLineY;
		m_distancePerpendicular -= m_offsetDistancePerpendicularY;
	} else {
		m_distanceOnLine += m_offsetDistanceOnLineY;
		m_distanceUnitDashOnLine += m_offsetDistanceOnLineY;
		m_distancePerpendicular += m_offsetDistancePerpendicularY;
	}
	wrapDistanceUnitDashOnLine();
}

// distancePerpendicular が 0.0 - 1.0 の間に入るよう X 座標を動かす
inline void LineDistanceThickDashed::moveToFitDistancePerpendicular(bool doesMoveX)
{
	if (doesMoveX) {
		moveXToFitDistancePerpendicular();
	} else {
		moveYToFitDistancePerpendicular();
	}
}


inline void LineDistanceThickDashed::moveXToFitDistancePerpendicular()
{
	if (m_offsetDistancePerpendicularX > 0.0f) {
		while (m_distancePerpendicular < 0.0f) moveX(1);
		while (1.0f < m_distancePerpendicular) moveX(-1);
	} else {
		while (m_distancePerpendicular < 0.0f) moveX(-1);
		while (1.0f < m_distancePerpendicular) moveX(1);
	}
}

inline void LineDistanceThickDashed::moveYToFitDistancePerpendicular()
{
	if (m_offsetDistancePerpendicularY > 0.0f) {
		while (m_distancePerpendicular < 0.0f) moveY(1);
		while (1.0f < m_distancePerpendicular) moveY(-1);
	} else {
		while (m_distancePerpendicular < 0.0f) moveY(-1);
		while (1.0f < m_distancePerpendicular) moveY(1);
	}
}

inline void LineDistanceThickDashed::wrapDistanceUnitDashOnLine()
{
	if (m_lengthUnitDash < m_distanceUnitDashOnLine) {
		m_distanceUnitDashOnLine -= m_lengthUnitDash;
	} else if (!isZero(m_distanceUnitDashOnLine) && m_distanceUnitDashOnLine < 0.0f) {
		m_distanceUnitDashOnLine += m_lengthUnitDash;
	}
}
#endif
