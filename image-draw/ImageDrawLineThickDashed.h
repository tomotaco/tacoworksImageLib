#ifndef __IMAGE_DRAW_LINE_THICK_DASHED_H_INCLUDED
#define __IMAGE_DRAW_LINE_THICK_DASHED_H_INCLUDED

#include <cmath>

#include "Image.h"
#include "MathUtil.h"
#include "Vector2f.h"
#include "Rect2f.h"
#include "LineDistanceThickDashed.h"

namespace ImageDrawLineThickDashedLocal
{
	// 破線のα値を返す。
	// halfWidth, lengthLineDash, で定義された範囲内はα=1.0
	// 外側の 1 pixel分に向かってαは 0.0 に近づき、さらにその外は 0.0。
	inline float getAlpha(const LineDistanceThickDashed& rDistance, float halfWidth)
	{		
		// return 1.0f; // アルファ値は無関係に描画領域だけ表示したいときはここをコメントアウトする

		float distancePerpendicular = std::abs(rDistance.getDistancePerpendicular());

		float alphaPerpendicular = 0.0f;
		if (distancePerpendicular < halfWidth) {
			alphaPerpendicular = 1.0f;
		} else if (distancePerpendicular < halfWidth + 1.0f) {
			alphaPerpendicular = 1.0f - (distancePerpendicular - halfWidth);
		}

		float distanceOnLine = rDistance.getDistanceOnLine();
		float alphaOnLine = 0.0f;
		if ((0.0f < distanceOnLine && distanceOnLine < rDistance.getLengthLine()) ||
			isZero(distanceOnLine) || isEqual(distanceOnLine, rDistance.getLengthLine())) {
			alphaOnLine = 1.0f;
		} else if (-1.0f < distanceOnLine && distanceOnLine < 0.0f) {
			alphaOnLine = 1.0f + distanceOnLine;
		} else if (rDistance.getLengthLine() < distanceOnLine && distanceOnLine < rDistance.getLengthLine() + 1.0f) {
			alphaOnLine = 1.0f - (distanceOnLine - rDistance.getLengthLine());
		}

		float distanceUnitDashOnLine = rDistance.getDistanceUnitDashOnLine();
		float lengthLineDash = rDistance.getLengthLineDash();
		float lengthUnitDash = rDistance.getLengthUnitDash();
		float alphaDash = 0.0f;
		if ((0.0f < distanceUnitDashOnLine && distanceUnitDashOnLine < lengthLineDash) ||
			isZero(distanceUnitDashOnLine) || isEqual(distanceUnitDashOnLine, lengthLineDash)) {
			alphaDash = 1.0f;
		} else if ((lengthLineDash < distanceUnitDashOnLine && distanceUnitDashOnLine < lengthLineDash + 1.0f) ||
			isEqual(lengthLineDash, distanceUnitDashOnLine)) {
			alphaDash = 1.0f - (distanceUnitDashOnLine - lengthLineDash);
		} else if ((lengthUnitDash - 1.0f < distanceUnitDashOnLine && distanceUnitDashOnLine < lengthUnitDash) ||
			isEqual(distanceUnitDashOnLine, lengthUnitDash)) {
			alphaDash = 1.0f - (lengthUnitDash - distanceUnitDashOnLine);
		}
		return ::min3(alphaPerpendicular, alphaOnLine, alphaDash);
	}

	template<typename TypeElement, int NumElements, typename RowTraits, typename AllocTraits, typename TypeCompositeFunctor>
	inline void plotSegment(int start, int end, bool doesMoveX, int sign, float halfWidth,
		const Rect2i& rRectBound, const LineDistanceThickDashed& rDistance,
		TypeCompositeFunctor funcComposite,
		Image<Pixel<TypeElement, NumElements>, RowTraits, AllocTraits>& rImage)
	{
		LineDistanceThickDashed distance(rDistance);
		for (int j = start; j < end; j ++) {
			if (rRectBound.doesInclude(distance.getPosition())) {
				float alpha = ImageDrawLineThickDashedLocal::getAlpha(distance, halfWidth);
				const Point2i& rPoint = distance.getPosition();
				const Pixel<TypeElement, NumElements>& rColorDst = rImage.getPixel(rPoint);
				TypeElement alphaElement = TypeElement(alpha * ElementUtil::getMaxValue<unsigned char>());
				Pixel<TypeElement, NumElements> pixelDst = funcComposite(Pixel<TypeElement, 1>(alphaElement), rColorDst);
				rImage.setPixel(rPoint, pixelDst);
			}
			distance.move(doesMoveX, sign);
		}
	}

	template<typename TypeElement, int NumElements, typename RowTraits, typename AllocTraits, typename TypeCompositeFunctor>
	inline void drawBelt(const LineDistanceThickDashed& rDistance, bool doesMoveX, int shift, int count,
		float halfWidth, int widthDiscrete, const Rect2i& rRectBound,
		TypeCompositeFunctor funcComposite,
		Image<Pixel<TypeElement, NumElements>, RowTraits, AllocTraits>& rImage)
	{
		LineDistanceThickDashed distance(rDistance);
		for (unsigned int i = 0; i < count; i ++) {
			plotSegment<TypeElement, NumElements, RowTraits, AllocTraits>(0, widthDiscrete, !doesMoveX, -1,
				halfWidth, rRectBound, distance, funcComposite, rImage);

			LineDistanceThickDashed distanceOpposite(distance);
			distanceOpposite.move(!doesMoveX, 1);
			plotSegment<TypeElement, NumElements, RowTraits, AllocTraits>(1, widthDiscrete, !doesMoveX, 1,
				halfWidth, rRectBound, distanceOpposite, funcComposite, rImage);

			distance.move(doesMoveX, shift);
			distance.moveToFitDistancePerpendicular(!doesMoveX);
		}
	}
}

namespace ImageDrawLineThickDashed
{
	template<typename TypeElement, int NumElements, typename RowTraits, typename AllocTraits, typename TypeCompositeFunctor>
	void drawLineThickDashed(const Vector2f& rPoint0, const Vector2f& rPoint1,
		float width, float lengthLineDash, float lengthSpaceDash, float offsetDash,
		TypeCompositeFunctor funcComposite,
		Image<Pixel<TypeElement, NumElements>, RowTraits, AllocTraits>& rImage)
	{
		if (rPoint0 == rPoint1) return;
		Rect2f rectBound(rPoint0);
		rectBound.addPoint(rPoint1);
		float halfWidth = width * 0.5f;
		rectBound.expandX(rectBound.getLowerX() - width);
		rectBound.expandY(rectBound.getLowerY() - width);
		rectBound.expandX(rectBound.getUpperX() + width);
		rectBound.expandY(rectBound.getUpperY() + width);
		Rect2i rectBoundDiscrete(Point2i((int)std::floor(rectBound.getLowerX()), (int)std::floor(rectBound.getLowerY())));
		rectBoundDiscrete.addPoint(Point2i((int)std::ceil(rectBound.getUpperX()) + 1, (int)std::ceil(rectBound.getUpperY()) + 1));

		if (!rectBoundDiscrete.doesIntersect(rImage.getRect())) return;
		Rect2i rectBoundVisible(rectBoundDiscrete);
		rectBoundVisible *= rImage.getRect();

		Point2i pointP(roundInt(rPoint0.getX()), roundInt(rPoint0.getY()));
		LineDistanceThickDashed distance(rPoint0, rPoint1, halfWidth, lengthLineDash, lengthSpaceDash, offsetDash, pointP);

		float diffX = rPoint1.getX() - rPoint0.getX();
		float diffY = rPoint1.getY() - rPoint0.getY();

		int widthDiscrete = int(halfWidth) + 4;

		bool doesAxisX = (std::abs(diffX) > std::abs(diffY)) ? true : false;
		int diff = int(doesAxisX ? diffX : diffY);
		int point0Coord = roundInt(doesAxisX ? rPoint0.getX() : rPoint0.getY());
		int point1Coord = roundInt(doesAxisX ? rPoint1.getX() : rPoint1.getY());

		// distancePerpendicular が 0.0f - 1.0f の間に納まるように調整
		distance.moveToFitDistancePerpendicular(!doesAxisX);

		int shift = diff > 0.0f ? 1 : -1;
		{
			LineDistanceThickDashed distanceBack(distance);
			distanceBack.move(doesAxisX, - shift);
			distanceBack.moveToFitDistancePerpendicular(!doesAxisX);
			int count = widthDiscrete - 1;

			ImageDrawLineThickDashedLocal::drawBelt(distanceBack, doesAxisX, - shift, count, halfWidth, widthDiscrete,
				rectBoundVisible, funcComposite, rImage);

		}
		int count = (point1Coord - point0Coord) * shift + widthDiscrete + 1;
		ImageDrawLineThickDashedLocal::drawBelt(distance, doesAxisX, shift, count, halfWidth, widthDiscrete,
			rectBoundVisible, funcComposite, rImage);
	}
};

#endif

