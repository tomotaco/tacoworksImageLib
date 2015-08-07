#ifndef RGBCompositeFunctorH
#define RGBCompositeFunctorH

#include <functional>
#include "Pixel.h"
#include "IntensityBlendFunctor.h"

template <typename TypeElement>
struct RGBCompositeFunctor : public std::binary_function<typename Pixel<TypeElement, 1>,
										typename Pixel<TypeElement, 3>, typename Pixel<TypeElement, 3> >
{
	RGBCompositeFunctor(const Pixel<TypeElement, 3>& rColor) : m_color(rColor) {}

	Pixel<TypeElement, 3> operator()(const Pixel<TypeElement, 1>& rPixelGray, const Pixel<TypeElement, 3>& rPixelColor) const
	{
		TypeElement alpha = rPixelGray.getA();
		IntensityBlendFunctor<TypeElement> functorBlendIntensityr;
		TypeElement colR = functorBlendIntensityr(alpha, rPixelColor.getR(), m_color.getR());
		TypeElement colG = functorBlendIntensityr(alpha, rPixelColor.getG(), m_color.getG());
		TypeElement colB = functorBlendIntensityr(alpha, rPixelColor.getB(), m_color.getB());

		return Pixel<TypeElement, 3>(colR, colG, colB);
	}

	Pixel<TypeElement, 3> m_color;
};

#endif