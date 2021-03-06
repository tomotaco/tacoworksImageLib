#ifndef __IMAGE_DRAW_CIRCLE_H_INCLUDED
#define __IMAGE_DRAW_CIRCLE_H_INCLUDED

#include <cmath>

#include "Image.h"
#include "MathUtil.h"
#include "Vector2f.h"
#include "Rect2f.h"
#include "CircleDistance.h"

namespace
{
	float getAlpha(const CircleDistance& rDistance)
	{
		float raduisSquared = rDistance.getRadiusSquared();
		float distanceSquared = rDistance.getDistanceSquaredFromCenter();
		float raduisBoundarySquared = rDistance.getRadiusBoundarySquared();
		float widthBoundarySquared = rDistance.getWidthBoundarySquared();

		if (distanceSquared < raduisSquared || isEqual(distanceSquared, raduisSquared)) {
			return 1.0f;
		} else if (distanceSquared < raduisBoundarySquared) {
			return 1.0f - std::sqrt((distanceSquared - raduisSquared) / widthBoundarySquared);
		} else {
			return 0.0f;
		}
	}
}

namespace ImageDrawCircle
{
	template<typename TypeElement, int NumElements, typename RowTraitsColor, typename AllocTraitsColor, typename TypeCompositeFunctor>
	void drawCircleFilled(const Vector2f& rPointCenter, float size,
		TypeCompositeFunctor funcComposite,
		Image<Pixel<TypeElement, NumElements>, RowTraitsColor, AllocTraitsColor>& rImageDst)
	{
		float halfSize = size * 0.5f;
		Vector2f vecHalf(halfSize, halfSize);
		Rect2f rectCircleFloat(rPointCenter - vecHalf, rPointCenter + vecHalf);
		Point2i pointLeftTopCircle(int(std::floor(rectCircleFloat.getLowerX())), int(std::floor(rectCircleFloat.getLowerY())));
		Point2i pointRightBottomCircle(int(std::ceil(rectCircleFloat.getUpperX())) + 1, int(std::ceil(rectCircleFloat.getUpperY())) + 1);

		Rect2i rectCircle(pointLeftTopCircle, pointRightBottomCircle);
		Rect2i rectCircleDraw(rectCircle);
		rectCircleDraw *= rImageDst.getRect();

		CircleDistance distance(rPointCenter, halfSize, rectCircleDraw.getLower());		
		int boundBottom = rectCircleDraw.getUpperY();
		while (distance.getPosition().getY() < boundBottom) {
			// 左→右
			int boundRight = rectCircleDraw.getUpperX();
			while (distance.getPosition().getX() < boundRight) {
				float alpha = getAlpha(distance);
				const Point2i& rPoint = distance.getPosition();
				const Pixel<TypeElement, NumElements>& rColorDst = rImageDst.getPixel(rPoint);
				TypeElement alphaElement = TypeElement(alpha * ElementUtil::getMaxValue<unsigned char>());
				Pixel<TypeElement, NumElements> pixelDst = funcComposite(Pixel<TypeElement, 1>(alphaElement), rColorDst);
				rImageDst.setPixel(rPoint, pixelDst);

				distance.moveRight();
			}
			distance.moveDown();
			distance.moveLeft();
			if (!(distance.getPosition().getY() < boundBottom)) break;

			// 右→左
			int boundLeft = rectCircleDraw.getLowerX();
			while (boundLeft <= distance.getPosition().getX()) {
				float alpha = getAlpha(distance);
				const Point2i& rPoint = distance.getPosition();
				const Pixel<TypeElement, NumElements>& rColorDst = rImageDst.getPixel(rPoint);
				TypeElement alphaElement = TypeElement(alpha * ElementUtil::getMaxValue<unsigned char>());
				Pixel<TypeElement, NumElements> pixelDst = funcComposite(Pixel<TypeElement, 1>(alphaElement), rColorDst);
				rImageDst.setPixel(rPoint, pixelDst);

				distance.moveLeft();
			}
			distance.moveDown();
			distance.moveRight();
		}

	}
};

#endif
