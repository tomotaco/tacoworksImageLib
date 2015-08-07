#ifndef RGBACompositeFunctorH
#define RGBACompositeFunctorH

#include <functional>
#include "Pixel.h"
#include "IntensityBlendFunctor.h"
#include "AlphaBlendFunctor.h"

template <typename TypeElement>
struct RGBACompositeFunctor : public std::binary_function<typename Pixel<TypeElement, 1>,
										typename Pixel<TypeElement, 4>, typename Pixel<TypeElement,4> >
{
	RGBACompositeFunctor(const Pixel<TypeElement, 3>& rColor) : m_color(rColor) {}

	Pixel<TypeElement, 4> operator()(const Pixel<TypeElement, 1>& rPixelGray, const Pixel<TypeElement, 4>& rPixelColor) const
	{
		TypeElement alpha = rPixelGray.getA();
		IntensityBlendFunctor<TypeElement> functorBlendIntensityr;
		TypeElement colR = functorBlendIntensityr(alpha, rPixelColor.getR(), m_color.getR());
		TypeElement colG = functorBlendIntensityr(alpha, rPixelColor.getG(), m_color.getG());
		TypeElement colB = functorBlendIntensityr(alpha, rPixelColor.getB(), m_color.getB());

		AlphaBlendFunctor<TypeElement> functorBlendAlpha;
		TypeElement colA = functorBlendAlpha(alpha, rPixelColor.getA());

		return Pixel<TypeElement, 4>(colR, colG, colB, colA);
	}

	Pixel<TypeElement, 3> m_color;
};

#endif