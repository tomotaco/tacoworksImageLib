#ifndef PixelConvertFunctorH
#define PixelConvertFunctorH

#include <functional>

#include "Pixel.h"
#include "ConvertChannelsFunctor.h"
#include "ConvertMultipleElementsFunctor.h"

// 最も汎用的なピクセル変換 functor
// 内部の呼び出しも全て functor で統一した版
template<class TPixelSrc, class TPixelDst,
	class TConvertChannelsFunc = ConvertChannelsFunctor<typename TPixelSrc::TypeElement>,
	class TConvertElementsFunc = ConvertMultipleElementsFunctor<typename TPixelSrc::TypeElement, typename TPixelDst::TypeElement> >
struct PixelConvertFunctor : public std::unary_function<TPixelSrc, TPixelDst>
{
	// 入出力ともに引数に来る版
	void operator()(const TPixelSrc& rPixelSrc, TPixelDst& rPixelDst) const
	{
		Pixel<typename TPixelSrc::TypeElement, TPixelDst::numElements> pixelTmp;
		m_funcConvertChannels(rPixelSrc, pixelTmp);
		m_funcConvertElements(pixelTmp, rPixelDst);
	}

	// 出力は返値で返す版。STL の algorithm に渡せるスタイル
	TPixelDst operator()(const TPixelSrc& rPixelSrc) const
	{
		Pixel<typename TPixelSrc::TypeElement, TPixelDst::numElements> pixelTmp;
		m_funcConvertChannels(rPixelSrc, pixelTmp);
		TPixelDst pixelDst;
		m_funcConvertElements(pixelTmp, pixelDst);

		return pixelDst;
	}

	TConvertChannelsFunc m_funcConvertChannels;
	TConvertElementsFunc m_funcConvertElements;
};

#endif
