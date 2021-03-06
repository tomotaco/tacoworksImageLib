#ifndef __IMAGE_DRAW_UTIL_H_INCLUDED
#define __IMAGE_DRAW_UTIL_H_INCLUDED

#include <cmath>

#include "Image.h"
#include "MathUtil.h"
#include "Vector2f.h"
#include "Rect2f.h"
#include "RGBCompositeFunctor.h"

namespace ImageDrawUtil
{
	template<typename TypeImage>
	void drawLine(const Point2i& rPoint0, const Point2i& rPoint1, bool drawTail, const typename TypeImage::TypePixel& rColor, TypeImage& rImage)
	{
		int diffX = rPoint1.getX() - rPoint0.getX();
		int diffY = rPoint1.getY() - rPoint0.getY();

		if (diffX == 0 && diffY == 0) return;

		int width = rImage.getWidth();
		int height = rImage.getHeight();

		if (std::abs(diffX) > std::abs(diffY)) {
			// X 軸上
			int shift = (diffX > 0) ? 1 : -1;
			float increment = float(diffY) / float(std::abs(diffX));
			float currentY = float(rPoint0.getY());
			int bound = drawTail ? rPoint1.getX(): rPoint1.getX() + shift;
			int currentX = rPoint0.getX();
			while (currentX != bound) {
				int currentYInt = roundInt(currentY);
				if ((0 <= currentX && currentX < width &&
					0 <= currentYInt && currentYInt < height)) {
					rImage.setPixel(currentX, currentYInt, rColor);
				}

				currentX += shift;
				currentY += increment;
			}
		} else {
			// Y 軸上
			int shift = (diffY > 0) ? 1 : -1;
			float increment = float(diffX) / float(std::abs(diffY));
			float currentX = float(rPoint0.getX());
			int bound = drawTail ? rPoint1.getY(): rPoint1.getY() + shift;
			int currentY = rPoint0.getY();
			while (currentY != bound) {
				int currentXInt = roundInt(currentX);
				if ((0 <= currentXInt && currentXInt < width &&
					0 <= currentY && currentY < height)) {
					rImage.setPixel(currentXInt, currentY, rColor);
				}

				currentY += shift;
				currentX += increment;
			}
		}
	}

	template<typename TypeImage>
	void drawRect(const Rect2i& rRect, const PixelRGB8& rColor, TypeImage& rImage)
	{
		int width = rImage.getWidth();
		int height = rImage.getHeight();

		int lowerX = rRect.getLowerX();
		int upperX = rRect.getUpperX() - 1;
		int lowerY = rRect.getLowerY();
		int upperY = rRect.getUpperY() - 1;

		drawLineH(lowerX, upperX, lowerY, rColor, rImage);
		drawLineH(lowerX, upperX, upperY, rColor, rImage);
		drawLineV(lowerX, lowerY, upperY, rColor, rImage);
		drawLineV(upperX, lowerY, upperY, rColor, rImage);
	}


	template<typename TypeImage>
	void drawLineH(int lowerX, int upperX, int y, const typename TypeImage::TypePixel& rColor, TypeImage& rImage)
	{
		int width = rImage.getWidth();
		int height = rImage.getHeight();

		if (0 <= y && y < height && lowerX < width && 0 <= upperX) {
			unsigned int boundLower = (0 <= lowerX) ? lowerX : 0;
			unsigned int boundUpper = (upperX + 1 < width) ? upperX + 1: width;
			for (unsigned int x = boundLower; x < boundUpper; x ++) {
				rImage.setPixel(x, y, rColor);
			}
		}
	}

	template<typename TypeImage>
	void drawLineV(int x, int lowerY, int upperY, const typename TypeImage::TypePixel& rColor, TypeImage& rImage)
	{
		int width = rImage.getWidth();
		int height = rImage.getHeight();

		if (0 <= x && x < width && lowerY < height && 0 <= upperY) {
			unsigned int boundLower = (0 <= lowerY) ? lowerY : 0;
			unsigned int boundUpper = (upperY + 1 < height) ? upperY + 1: height;
			for (unsigned int y = boundLower; y < boundUpper; y ++) {
				rImage.setPixel(x, y, rColor);
			}
		}
	}

	template<typename TypeImage>
	void drawCross(const Point2i& rPoint, unsigned int size, const typename TypeImage::TypePixel& rColor, TypeImage& rImage)
	{
		int sizeHalf = size / 2;
		int centerX = rPoint.getX();
		int centerY = rPoint.getY();

		drawLineH(centerX - sizeHalf, centerX + sizeHalf, centerY, rColor, rImage);
		drawLineV(centerX, centerY - sizeHalf, centerY + sizeHalf, rColor, rImage);
	}
};

#endif
