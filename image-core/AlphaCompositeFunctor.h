#ifndef AlphaCompositeFunctorH
#define AlphaCompositeFunctorH

#include <functional>
#include "Pixel.h"
#include "AlphaBlendFunctor.h"

template <typename TypeElement>
struct AlphaCompositeFunctor : public std::binary_function<typename Pixel<TypeElement, 1>,
										typename Pixel<TypeElement, 1>, typename Pixel<TypeElement, 1> >
{
	Pixel<TypeElement, 1> operator()(const Pixel<TypeElement, 1>& rPixelGray, const Pixel<TypeElement,1>& rPixelColor) const
	{
		AlphaBlendFunctor<TypeElement> functorBlendAlpha;
		TypeElement colA = functorBlendAlpha(rPixelGray.getA(), rPixelColor.getA());

		return Pixel<TypeElement, 1>(colA);
	}
};

#endif