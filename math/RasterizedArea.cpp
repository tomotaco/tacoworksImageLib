#include <cmath>
#include <vector>
#include <algorithm>
#include "MathUtil.h"
#include "RasterizedArea.h"

const float g_epsilon = 1.0e-6f;

RasterizedArea::RasterizedArea(const std::vector<Vector2f>& rPoints)
{
	int numPoints = rPoints.size();
	if (numPoints < 2) return;

	m_boundingBox.setPoint(Point2i(roundInt(rPoints.front().getX()), roundInt(rPoints.front().getY())));
	for (int i = 1; i < numPoints; i ++) {
		m_boundingBox.addPoint(Point2i(roundInt(rPoints[i].getX()), roundInt(rPoints[i].getY())));
	}

	m_lineSegments.clear();
	for (int i = 0; i < numPoints - 1; i ++) {
			m_lineSegments.push_back(LineSegment(rPoints[i], rPoints[i + 1]));
	}
	m_lineSegments.push_back(LineSegment(rPoints[numPoints - 1], rPoints[0]));


}

RasterizedArea::~RasterizedArea()
{
	// nothing to be implemented
}

bool RasterizedArea::getRasterSegments(int y, std::vector<RasterSegment>& rRasterSegments) const
{
	std::vector<float> xHitList;
	getXHitList(y, xHitList);
	if (xHitList.size() < 2) return false;

	int xHead = roundInt(xHitList.front());
	int xTail = roundInt(xHitList.back()) + 1;

	std::vector<bool> singleRaster(xTail - xHead, false);
	for (int x = xHead; x < xTail; x ++) {
		std::vector<float>::const_iterator it = xHitList.begin();
		int count = 0;
		while (it != xHitList.end() && *it < float(x)) {
			count ++;
			it ++;
		}
		if (count % 2 == 1) singleRaster[x - xHead] = true;
	}

	int index = 0;
	int lengthRaster = singleRaster.size();
	while (index < lengthRaster)  {
		while (index < lengthRaster && singleRaster[index] == false) index ++;
		if (index == singleRaster.size()) break;
		int startIndex = xHead + index;
		while (index < lengthRaster && singleRaster[index] == true) index ++;
		int endIndex = xHead + index;
		rRasterSegments.push_back(RasterSegment(startIndex, endIndex));
	}

	return true;
}

void RasterizedArea::getXHitList(int y, std::vector<float>& rXHitList) const
{
	rXHitList.clear();
	for (int i = 0; i < m_lineSegments.size(); i ++) {
		float yVal = float(y);
		Vector2f p0 = m_lineSegments[i].getP0();
		Vector2f p1 = m_lineSegments[i].getP1();

		if (m_lineSegments[i].getMinY() < yVal && yVal < m_lineSegments[i].getMaxY()) {
			float x = p0.getX() + (p1.getX() - p0.getX()) * (yVal - p0.getY()) / (p1.getY() - p0.getY());
			rXHitList.push_back(x);
		} else {
			// スキャンライン上にある頂点は含まない、という仮の定義
//			if (fabsf(p0.getY() - float(y)) > g_epsilon) rXHitList.push_back(p0.getX());
//			if (fabsf(p1.getY() - float(y)) > g_epsilon) rXHitList.push_back(p1.getX());

			// スキャンライン上にエッジの端点があり、
			// その端点を持つ 2 つのエッジが互いにスキャンラインをまたいでいたら、
			// 交点とみなす。
			if (std::fabs(p1.getY() - yVal) < g_epsilon) {
				int indexNext = (i < m_lineSegments.size() - 1) ? i + 1 : 0;
				Vector2f p1Next = m_lineSegments[indexNext].getP1();
				if ((p0.getY() - yVal) * (p1Next.getY() - yVal) < 0 ||
					(p0.getY() - yVal > 0 && p1Next.getY() - yVal > 0)) {
					rXHitList.push_back(p1.getX());
				}
			}
		}
	}
	std::sort(rXHitList.begin(), rXHitList.end());
}
