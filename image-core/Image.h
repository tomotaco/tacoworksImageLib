#ifndef ImageH
#define ImageH

#include <algorithm>
#include "Point2i.h"
#include "Rect2i.h"
#include "Pixel.h"
#include "PixelType.h"

#include "ImageBase.h"
#include "ImageTraits.h"
#include "ImageTraitsAllocatedByNew.h"
#include "ImageRangeIterator.h"

#include "TypeList.h"

// 画像クラス
template<class TPixel, class RowTraits = ImageTraitsRowNotAligned,
						class AllocTraits = ImageTraitsAllocatedByNew<TPixel, RowTraits> >
class Image : public ImageBase, public AllocTraits
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
	typedef RowTraits TypeTraitsRow;
	typedef AllocTraits TypeTraitsAlloc;

	static ImageBase* createImage() { return static_cast<ImageBase*>(new TypeContainer); }
	static ImageBase* duplicateImage(const ImageBase* pX) { return dynamic_cast<const TypeContainer*>(pX)->createCopy(); }

	Image() : m_width(0), m_height(0), m_numBytesRow(0), m_pBuffer(NULL) {}
	Image(unsigned int width, unsigned height)
		 : m_width(0), m_height(0), m_numBytesRow(0), m_pBuffer(NULL)
	{
		setSize(width, height);
		fillPixel(TPixel());
	}

	Image(const Image<TPixel, RowTraits, AllocTraits>& rX)
		 : m_width(0), m_height(0), m_numBytesRow(0), m_pBuffer(NULL)
	{
		setSize(rX.m_width, rX.m_height);
		std::copy(rX.m_pBuffer, rX.m_pBuffer + rX.getNumBytesBuffer(), m_pBuffer);
	}

	virtual ~Image()
	{
		clear();
	}

	const Image<TPixel, RowTraits, AllocTraits>& operator=(const Image<TPixel, RowTraits, AllocTraits>& rX)
	{
		setSize(rX.m_width, rX.m_height);
		std::copy(rX.m_pBuffer, rX.m_pBuffer + rX.getNumBytesBuffer(), m_pBuffer);

		return *this;
	}

	virtual ImageBase* createCopy() const { return static_cast<ImageBase*>(new TypeContainer(*this)); }
	virtual void clear();
	virtual void setSize(unsigned int width, unsigned int height);

	virtual unsigned int getWidth() const { return m_width; }
	virtual unsigned int getHeight() const { return m_height; }
	virtual ElementType getTypeElement() const { return ElementUtil::getType<TypeElement>(); }
	virtual ChannelType getTypeChannel() const { return getTypeChannelFromNumElements(numElements);}
	virtual unsigned int getNumBytesPerElement() const { return sizeof(TypeElement); }
	virtual unsigned int getNumElementsPerPixel() const { return numElements; }

	virtual unsigned int getNumBytesPixel() const
	{
		return sizeof(TPixel);
	}

	virtual unsigned int getNumBytesRow() const
	{
		return m_numBytesRow;
	}

	virtual unsigned int getNumBytesBuffer() const
	{
		return m_height * getNumBytesRow();
	}

	virtual void* getRawBuffer() { return reinterpret_cast<void*>(m_pBuffer); }
	virtual const void* getRawBuffer() const { return reinterpret_cast<const void*>(m_pBuffer); }
	virtual void* getRawAddress(unsigned int x, unsigned int y)
	{
		return reinterpret_cast<void*>(getAddress(x, y));
	}
	virtual void* getRawAddress(const Point2i& rPoint)
	{
		return getRawAddress(rPoint.getX(), rPoint.getY());
	}

	virtual const void* getRawAddress(unsigned int x, unsigned int y) const
	{
		return reinterpret_cast<const void*>(getAddress(x, y));
	}
	virtual const void* getRawAddress(const Point2i& rPoint) const
	{
		return getRawAddress(rPoint.getX(), rPoint.getY());
	}

	virtual double getValueElementByDouble(unsigned int x, unsigned int y, unsigned int indexElement) const
	{
		assert(indexElement < numElements);
		if (!(indexElement < numElements)) return 0.0f;
		return getPixel(x, y).getElement(indexElement);
	}

	virtual double getValueElementByDouble(const Point2i& rPoint, unsigned int indexElement) const
	{
		return getValueElementByDouble(rPoint.getX(), rPoint.getY(), indexElement);
	}

	virtual void setValueElementByDouble(unsigned int x, unsigned int y, unsigned int indexElement, double value)
	{
		assert(indexElement < numElements);
		if (!(indexElement < numElements)) return;
		TPixel pixel = getPixel(x, y);
		pixel.setElement(indexElement, TypeElement(value));
		setPixel(x, y, pixel);
	}

	virtual void setValueElementByDouble(const Point2i& rPoint, unsigned int indexElement, double value)
	{
		return setValueElementByDouble(rPoint.getX(), rPoint.getY(), indexElement, value);
	}

	const TypeElement* getBuffer() const { return reinterpret_cast<const TypeElement*>(m_pBuffer); }
	TypeElement* getBuffer() { return reinterpret_cast<TypeElement*>(m_pBuffer); }

	const TPixel* getAddress(unsigned int x, unsigned int y) const
	{
		return reinterpret_cast<const TPixel*>(m_pBuffer + (y * getNumBytesRow() + x * getNumBytesPixel()));
	}
	const TPixel* getAddress(const Point2i& rPoint) const
	{
		return getAddress(rPoint.getX(), rPoint.getY());
	}

	TPixel* getAddress(unsigned int x, unsigned int y)
	{
		return reinterpret_cast<TPixel*>(m_pBuffer + (y * getNumBytesRow() + x * getNumBytesPixel()));
	}
	TPixel* getAddress(const Point2i& rPoint)
	{
		return getAddress(rPoint.getX(), rPoint.getY());
	}

	TPixel getPixel(unsigned int x, unsigned int y) const
	{
		return *getAddress(x, y);
	}
	TPixel getPixel(const Point2i& rPoint) const
	{
		return getPixel(rPoint.getX(), rPoint.getY());
	}

	void setPixel(unsigned int x, unsigned int y, const TPixel& rPixel)
	{
		TPixel* pPixel = getAddress(x, y);
		*pPixel = rPixel;
	}
	void setPixel(const Point2i& rPoint, const TPixel& rPixel)
	{
		setPixel(rPoint.getX(), rPoint.getY(), rPixel);
	}

	void fillPixel(const TPixel& rPixel); // 領域指定が必要→range_iterator でやるべし

	virtual Rect2i getRect() const { return Rect2i(0, 0, m_width, m_height); }

	TypeIterator range_begin(const Rect2i& rRect)
	{
		Point2i pointStart(rRect.getLower());
		return TypeIterator(this, rRect, pointStart);
	}
	TypeConstIterator range_begin(const Rect2i& rRect) const
	{
		Point2i pointStart(rRect.getLower());
		return TypeConstIterator(this, rRect, pointStart);
	}
	TypeIterator range_end(const Rect2i& rRect)
	{
		Point2i pointEnd(rRect.getLowerX(), rRect.getUpperY());
		return TypeIterator(this, rRect, pointEnd);
	}
	TypeConstIterator range_end(const Rect2i& rRect) const
	{
		Point2i pointEnd(rRect.getLowerX(), rRect.getUpperY());
		return TypeConstIterator(this, rRect, pointEnd);
	}

	virtual void getMinMaxValueByDouble(const Rect2i& rRect, double& rValueMin, double& rValueMax) const
	{
		TypeElement valueMin = TypeElement(), valueMax = TypeElement();
		getMinMaxValue(rRect, valueMin, valueMax);
		rValueMin = valueMin;
		rValueMax = valueMax;
	}

	void getMinMaxValue(const Rect2i& rRect, TypeElement& rValueMin, TypeElement& rValueMax) const
	{
		TypeElement valueMin = ElementUtil::getMaxValue<TypeElement>();
		TypeElement valueMax = ElementUtil::getMinValue<TypeElement>();
		typename Image<TPixel, RowTraits, AllocTraits>::const_iterator it = range_begin(rRect);
		for (; it != range_end(rRect); ++ it) {
			TPixel pixel = *it;
			TypeElement valueMinPixel = pixel.getMinValueOfElements();
			if (valueMinPixel < valueMin) valueMin = valueMinPixel;
			TypeElement valueMaxPixel = pixel.getMaxValueOfElements();
			if (valueMax < valueMaxPixel) valueMax = valueMaxPixel;
		}

		rValueMin = valueMin;
		rValueMax = valueMax;
	}

protected:
	void updateNumBytesRow()
	{
		m_numBytesRow = RowTraits::getNumBytesRow(m_width, getNumBytesPixel());
	}

	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_numBytesRow;
	unsigned char* m_pBuffer;

};

template<class TPixel, class RowTraits, class AllocTraits>
inline void Image<TPixel, RowTraits, AllocTraits>::setSize(unsigned int width, unsigned int height)
{
	clear();

	m_width = width;
	m_height = height;
	updateNumBytesRow();

	m_pBuffer = AllocTraits::allocImage(m_width, m_height);
}

template<class TPixel, class RowTraits, class AllocTraits>
inline void Image<TPixel, RowTraits, AllocTraits>::clear()
{
	if (m_pBuffer) {
		AllocTraits::freeImage(m_pBuffer);
		m_pBuffer = NULL;
		m_width = 0;
		m_height = 0;
		m_numBytesRow = 0;
	}
}

template<class TPixel, class RowTraits, class AllocTraits>
inline void Image<TPixel, RowTraits, AllocTraits>::fillPixel(const TPixel& rPixel)
{
	for (unsigned int y = 0; y < m_height; y ++) {
		TPixel* pPtr = getAddress(0, y);
		for (unsigned int x = 0; x < m_width; x ++) {
			*pPtr ++ = rPixel;
		}
	}
}

#ifdef _WIN32
typedef ImageTraitsRowAlignedBy32Bit ImageTraitsRowDefault;
#else
typedef ImageTraitsRowNotAligned ImageTraitsRowDefault;
#endif

typedef Image<PixelRGBA8>									ImageRGBA8;
typedef Image<PixelRGB8, ImageTraitsRowDefault>         ImageRGB8;
typedef Image<PixelGray8, ImageTraitsRowDefault>		ImageGray8;
typedef Image<PixelRGBA16>									ImageRGBA16;
typedef Image<PixelRGB16, ImageTraitsRowDefault>		ImageRGB16;
typedef Image<PixelGray16, ImageTraitsRowDefault>       ImageGray16;

typedef Image<PixelRGBASInt>								ImageRGBASInt;
typedef Image<PixelRGBSInt>									ImageRGBSInt;
typedef Image<PixelGraySInt>								ImageGraySInt;
typedef Image<PixelRGBAUInt>								ImageRGBAUInt;
typedef Image<PixelRGBUInt>									ImageRGBUInt;
typedef Image<PixelGrayUInt>								ImageGrayUInt;
typedef Image<PixelRGBAFloat>								ImageRGBAFloat;
typedef Image<PixelRGBFloat>								ImageRGBFloat;
typedef Image<PixelGrayFloat>								ImageGrayFloat;
typedef Image<PixelRGBADouble>								ImageRGBADouble;
typedef Image<PixelRGBDouble>								ImageRGBDouble;
typedef Image<PixelGrayDouble>								ImageGrayDouble;

typedef TypeList18(ImageRGBA8, ImageRGB8, ImageGray8,
	ImageRGBA16, ImageRGB16, ImageGray16,
	ImageRGBASInt, ImageRGBSInt, ImageGraySInt,
	ImageRGBAUInt, ImageRGBUInt, ImageGrayUInt,
	ImageRGBAFloat, ImageRGBFloat, ImageGrayFloat,
	ImageRGBADouble, ImageRGBDouble, ImageGrayDouble) TypeListImages;

#endif
