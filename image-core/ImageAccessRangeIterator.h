#ifndef ImageAccessRangeIteratorH
#define ImageAccessRangeIteratorH

#include <cassert>

#include "ImageRangeIterator.h"
#include "ImageAccessReferer.h"
#include "PixelAccessTraits.h"

template<class TImage, class TPixelAccess,
	class TFuncConvertGet, class TFuncConvertSet, class Traits>
class ImageAccessRangeIterator : public ImageRangeIterator<TImage, Traits>
{
public:
	typedef typename Traits::TypeValue TypeValue;
	typedef typename Traits::TypeReference TypeReference;
	typedef typename Traits::TypePointer TypePointer;
	typedef typename Traits::TypeAccessValue TypeAccessValue;
	typedef typename Traits::TypeAccessReference TypeAccessReference;
	typedef typename Traits::TypeAccessPointer TypeAccessPointer;
	typedef typename Traits::TypeContainer TypeContainer;
	typedef typename Traits::TypeContainerReference TypeContainerReference;
	typedef typename Traits::TypeContainerPointer TypeContainerPointer;

	typedef ImageAccessRangeIterator<TImage, TPixelAccess, TFuncConvertGet, TFuncConvertSet, TraitsAccessNonConst<typename TImage::TypePixel, TPixelAccess, TImage> > TypeIterator;
	typedef ImageAccessRangeIterator<TImage, TPixelAccess, TFuncConvertGet, TFuncConvertSet, TraitsAccessConst<typename TImage::TypePixel, TPixelAccess, TImage> > TypeConstIterator;

	ImageAccessRangeIterator() {}

	ImageAccessRangeIterator(TypeContainerPointer pImage, const Rect2i& rRectRange, const Point2i& rPointStart,
								const TFuncConvertGet& rFuncGet, const TFuncConvertSet& rFuncSet)
	  : ImageRangeIterator<TImage, Traits>(pImage, rRectRange, rPointStart), m_rFuncGet(rFuncGet), m_rFuncSet(rFuncSet) {}

	// NonConst -> Const のコンストラクタ兼、NonConst -> NonConst のコピーコンストラクタ
	ImageAccessRangeIterator(const TypeIterator& rX)
	  : ImageRangeIterator<TImage, Traits>(rX.m_pImage, rX.m_rectRange, rX.m_position), m_rFuncGet(rX.m_rFuncGet), m_rFuncSet(rX.m_rFuncSet) {}

	// nonconst から const な iterator を生成する際、nonconst な iterator のメンバへのアクセスを許す必要がある
	// friend TypeConstIterator;
	friend class ImageAccessRangeIterator<TImage, TPixelAccess, TFuncConvertGet, TFuncConvertSet, TraitsAccessConst<typename TImage::TypePixel, TPixelAccess, TImage> >;

	ImageAccessReferer<TImage, TPixelAccess, TFuncConvertGet, TFuncConvertSet, Traits>
	operator*() const
	{
		assert((ImageRangeIterator<TImage, Traits>::m_isInsideRange));
		if (!ImageRangeIterator<TImage, Traits>::m_isInsideRange) return ImageAccessReferer<TImage, TPixelAccess, TFuncConvertGet, TFuncConvertSet, Traits>(ImageRangeIterator<TImage, Traits>::m_pImage->getAddress(0, 0), m_rFuncGet, m_rFuncSet);

		return ImageAccessReferer<TImage, TPixelAccess, TFuncConvertGet, TFuncConvertSet, Traits>(ImageRangeIterator<TImage, Traits>::m_pPixelCurrent, m_rFuncGet, m_rFuncSet);
	}

private:
	ImageAccessRangeIterator& operator=(const ImageAccessRangeIterator& rX) {}

	const TFuncConvertGet& m_rFuncGet;
	const TFuncConvertSet& m_rFuncSet;
};
#endif
