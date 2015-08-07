#ifndef __FINDER_LABEL_INFO_H_INCLUDED
#define __FINDER_LABEL_INFO_H_INCLUDED

#include "Point2i.h"

class FindLabelInfo
{
public:
	FindLabelInfo() : m_directionSearchY(0) {}
	FindLabelInfo(const Point2i& rPosition, int directionSearchY)
		 : m_position(rPosition), m_directionSearchY(directionSearchY) {}
	FindLabelInfo(const FindLabelInfo& rX)
		 : m_position(rX.m_position), m_directionSearchY(rX.m_directionSearchY) {}

	const FindLabelInfo& operator=(const FindLabelInfo& rX)
	{
		m_position = rX.m_position;
		m_directionSearchY = rX.m_directionSearchY;

		return *this;
	}

	bool operator==(const FindLabelInfo& rX) const
	{
		return (m_position == rX.m_position && m_directionSearchY == rX.m_directionSearchY) ? true : false;
	}

	bool operator!=(const FindLabelInfo& rX) const
	{
		return !(*this == rX);
	}

	bool operator<(const FindLabelInfo& rX) const
	{
		if (m_position.getY() < rX.m_position.getY()) return true;
		if (m_position.getY() > rX.m_position.getY()) return false;

		return (m_position.getX() < rX.m_position.getX()) ? true : false;
	}

	const Point2i& getPosition() const { return m_position; }
	int getDirectionSearchY() const { return m_directionSearchY; }

	void setPosition(const Point2i& rPosition) { m_position = rPosition; }
	void setDirectionSearchY(int y) { m_directionSearchY = y; }

private:
	Point2i m_position;
	int m_directionSearchY;
};

#endif
