#ifndef ImageExternalH
#define ImageExternalH

#include "ImageTraitsNoAllocate.h"
#include "TypeList.h"

// 画像クラス
template<class TPixel, class RowTraits = ImageTraitsRowNotAligned,
						class AllocTraits = ImageTraitsNoAllocate>
class ImageExternal : public Image<TPixel, RowTraits, AllocTraits>
{
public:
	typedef TPixel TypePixel;
	typedef typename TPixel::TypeElement TypeElement;
	enum {
		numElements = TPixel::numElements
	};

	typedef Image<TPixel, RowTraits, AllocTraits> TypeContainer;
	typedef ImageRangeIterator<TypeContainer, TraitsNonConst<TPixel, TypeContainer> > TypeIterator;
	typedef TypeIterator iterator;
	typedef ImageRangeIterator<TypeContainer, TraitsConst<TPixel, TypeContainer> > TypeConstIterator;
	typedef TypeConstIterator const_iterator;

	ImageExternal(unsigned int width, unsigned height, unsigned char* pAddrExternal)
	{
		Image<TPixel, RowTraits, AllocTraits>::m_width = width;
		Image<TPixel, RowTraits, AllocTraits>::m_height = height;
		Image<TPixel, RowTraits, AllocTraits>::updateNumBytesRow();

		Image<TPixel, RowTraits, AllocTraits>::m_pBuffer = pAddrExternal;
	}

	virtual ~ImageExternal()
	{
		// m_pBuffer は外部の画像を指しているので、ここではポインタを NULL にセットするだけ
		clear();
	}

private:
	// クラスの外からは使わせない
	virtual void clear() { Image<TPixel, RowTraits, AllocTraits>::m_pBuffer = NULL; }
	virtual void setSize(unsigned int width, unsigned int height) {}
};

typedef ImageExternal<PixelRGBA8>									ImageExternalRGBA8;
typedef ImageExternal<PixelRGB8, ImageTraitsRowAlignedBy32Bit>		ImageExternalRGB8;
typedef ImageExternal<PixelGray8, ImageTraitsRowAlignedBy32Bit>		ImageExternalGray8;
typedef ImageExternal<PixelRGBA16>									ImageExternalRGBA16;
typedef ImageExternal<PixelRGB16, ImageTraitsRowAlignedBy32Bit>		ImageExternalRGB16;
typedef ImageExternal<PixelGray16, ImageTraitsRowAlignedBy32Bit>	ImageExternalGray16;
typedef ImageExternal<PixelRGBASInt>								ImageExternalRGBASInt;
typedef ImageExternal<PixelRGBSInt>									ImageExternalRGBSInt;
typedef ImageExternal<PixelGraySInt>								ImageExternalGraySInt;
typedef ImageExternal<PixelRGBAUInt>								ImageExternalRGBAUInt;
typedef ImageExternal<PixelRGBUInt>									ImageExternalRGBUInt;
typedef ImageExternal<PixelGrayUInt>								ImageExternalGrayUInt;
typedef ImageExternal<PixelRGBAFloat>								ImageExternalRGBAFloat;
typedef ImageExternal<PixelRGBFloat>								ImageExternalRGBFloat;
typedef ImageExternal<PixelGrayFloat>								ImageExternalGrayFloat;
typedef ImageExternal<PixelRGBADouble>								ImageExternalRGBADouble;
typedef ImageExternal<PixelRGBDouble>								ImageExternalRGBDouble;
typedef ImageExternal<PixelGrayDouble>								ImageExternalGrayDouble;

typedef TypeList18(ImageExternalRGBA8, ImageExternalRGB8, ImageExternalGray8,
	ImageExternalRGBA16, ImageExternalRGB16, ImageExternalGray16,
	ImageExternalRGBASInt, ImageExternalRGBSInt, ImageExternalGraySInt,
	ImageExternalRGBAUInt, ImageExternalRGBUInt, ImageExternalGrayUInt,
	ImageExternalRGBAFloat, ImageExternalRGBFloat, ImageExternalGrayFloat,
	ImageExternalRGBADouble, ImageExternalRGBDouble, ImageExternalGrayDouble) TypeListImagesExternal;

#endif
