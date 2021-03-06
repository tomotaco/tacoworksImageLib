#ifndef __LABEL_RECT_H_INCLUDED
#define __LABEL_RECT_H_INCLUDED

#include "Vector2f.h"
#include "Rect2i.h"

// 点群のバウンディングボックスと重心を返す
class LabelRect
{
public:
	LabelRect() : m_numPoints(0) {}
	LabelRect(const LabelRect& rX) :
		m_boundingBox(rX.m_boundingBox), m_numPoints(rX.m_numPoints),
		m_pointSum(rX.m_pointSum), m_pointCOG(rX.m_pointCOG) {}

	const LabelRect& operator=(const LabelRect& rX)
	{
		m_boundingBox = rX.m_boundingBox;
		m_numPoints = rX.m_numPoints;
		m_pointSum = rX.m_pointSum;
		m_pointCOG = rX.m_pointCOG;

		return *this;
	}

	void clear()
	{
		m_boundingBox.setPoint(Point2i(0, 0));
		m_numPoints = 0;
		m_pointSum.set(0.0f, 0.0f);
	}
	
	void addPoint(const Point2i& rPoint)
	{
		m_pointSum += Vector2f(float(rPoint.getX()), float(rPoint.getY()));

		if (m_numPoints > 0) {
			m_boundingBox.addPoint(rPoint);
		} else {
			m_boundingBox.setPoint(rPoint);
		}

		m_numPoints ++;
	}

	// 重心は明示的に updateCOG() を呼ばないと更新されない
	void updateCOG()
	{
		m_pointCOG = (m_numPoints > 0) ? (m_pointSum / float(m_numPoints)) : Vector2f(0.0f, 0.0f);
	}

	const Rect2i& getBoundingBox() const { return m_boundingBox; }
	unsigned int getNumPoints() const { return m_numPoints; }
	const Vector2f& getCOG() const { return m_pointCOG; }

private:
	Rect2i m_boundingBox;
	unsigned int m_numPoints;
	Vector2f m_pointSum;
	Vector2f m_pointCOG;
};

#endif
