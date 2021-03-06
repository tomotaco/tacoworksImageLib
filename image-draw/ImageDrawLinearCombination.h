#ifndef __IMAGE_DRAW_LINEAR_COMBINATION_H_INCLUDED
#define __IMAGE_DRAW_LINEAR_COMBINATION_H_INCLUDED

#define NOMINMAX
#include <cmath>

#include "Image.h"
#include "MathUtil.h"
#include "Vector2f.h"
#include "Rect2f.h"
#include "LinearCombinationDistance.h"

// use RGBCompositeFunctor, RGACompositeFunctor, or AlphaCompositeFunctor in RGBCompositeFunctor.h
// for TypeCompositeFunctor funcComposite

namespace ImageDrawLinearCombinationLocal
{
	// 図形ののα値を返す。
	// 範囲内はα=1.0
	// 外側の 1 pixel分に向かってαは 0.0 に近づき、さらにその外は 0.0。
	inline float getAlpha(const LinearCombinationDistance& rDistance)
	{		
		// return 1.0f;

		float valueU = rDistance.getValueU();
		float valueV = rDistance.getValueV();

		float borderValueU = rDistance.getBorderValueU();
		float borderValueV = rDistance.getBorderValueV();

		float alphaU = 0.0f;
		if ((0.0f < valueU || isZero(valueU)) && (valueU < 1.0f || isEqual(valueU, 1.0f))) {
			alphaU = 1.0f;
		} else if (- borderValueU < valueU && valueU < 0.0f) {
			alphaU = 1.0f + (valueU / borderValueU);
		} else if (1.0f < valueU && valueU < 1.0f + borderValueU) {
			alphaU = 1.0f - (valueU - 1.0f) / borderValueU;
		}

		float alphaV = 0.0f;
		if ((0.0f < valueV || isZero(valueV)) && (valueV < 1.0f || isEqual(valueV, 1.0f))) {
			alphaV = 1.0f;
		} else if (- borderValueV < valueV && valueV < 0.0f) {
			alphaV = 1.0f + (valueV / borderValueV);
		} else if (1.0f < valueV && valueV < 1.0f + borderValueV) {
			alphaV = 1.0f - (valueV - 1.0f) / borderValueV;
		}

		return std::min(alphaU, alphaV);
	}
};

// 2 つのベクトルの線形一時結合で表現できる図形を描画する
// 矩形・ひし形・平行四辺形など
namespace ImageDrawLinearCombination
{
	// RGB 専用
	template<typename TypeElement, int NumElements, typename RowTraits, typename AllocTraits, typename TypeCompositeFunctor>
	void drawLinearCombination(const Vector2f& rPoint0, const Vector2f& rPoint1, const Vector2f& rPoint2,
		TypeCompositeFunctor funcComposite, Image<Pixel<TypeElement, NumElements>, RowTraits, AllocTraits>& rImage)
	{
		Vector2f vecP0P1 = rPoint1 - rPoint0;
		Vector2f vecP0P2 = rPoint2 - rPoint0;
		float lengthVecP0ToP1 = vecP0P1.getLength();
		float lengthVecP0ToP2 = vecP0P2.getLength();

		if (isZero(lengthVecP0ToP1) || isZero(lengthVecP0ToP1)) return;

		Vector2f point3(rPoint1 + vecP0P2);
		Rect2f rectBound(rPoint0);
		rectBound.addPoint(rPoint1);
		rectBound.addPoint(rPoint2);
		rectBound.addPoint(point3);

		Rect2i rectBoundDiscrete(Point2i((int)std::floor(rectBound.getLowerX()), (int)std::floor(rectBound.getLowerY())));
		rectBoundDiscrete.addPoint(Point2i((int)std::ceil(rectBound.getUpperX()) + 1, (int)std::ceil(rectBound.getUpperY()) + 1));
		if (!rectBoundDiscrete.doesIntersect(rImage.getRect())) return;
		Rect2i rectBoundVisible(rectBoundDiscrete);
		rectBoundVisible *= rImage.getRect();

		LinearCombinationDistance distance(rPoint0, rPoint1, rPoint2,rectBoundVisible.getLower());
		for (int y = rectBoundVisible.getLowerY(); y < rectBoundVisible.getUpperY(); y ++) {
			LinearCombinationDistance distanceOnLine(distance);
			for (int x = rectBoundVisible.getLowerX(); x < rectBoundVisible.getUpperX(); x ++) {
				float alpha = ImageDrawLinearCombinationLocal::getAlpha(distanceOnLine);
			
				const Point2i& rPoint = distanceOnLine.getPosition();
				const Pixel<TypeElement, NumElements>& rColorDst = rImage.getPixel(rPoint);
				TypeElement alphaElement = TypeElement(alpha * ElementUtil::getMaxValue<unsigned char>());
				Pixel<TypeElement, NumElements> pixelDst = funcComposite(Pixel<TypeElement, 1>(alphaElement), rColorDst);
				rImage.setPixel(rPoint, pixelDst);

				distanceOnLine.moveX(1);
			}
			distance.moveY(1);
		}
	}
};

#endif

