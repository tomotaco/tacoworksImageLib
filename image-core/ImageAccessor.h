#ifndef ImageAccessorH
#define ImageAccessorH

#include "Image.h"
#include "PixelConvertFunctor.h"
#include "PixelAccessTraits.h"
#include "ImageAccessRangeIterator.h"

// 画像自身の PixelType とは異なる PixelType での読み書きを行うラッパークラス
// PixelType の変換関数はデフォルトで定義されているが、functor でオーバーライド可能(なはず)
template<class TPixelAccess>
class ImageAccessorIF
{
public:
	virtual ~ImageAccessorIF() {}

	// get のメソッドは必ずインプリせなあかん set はなくてもよい
	// (そうしないと read only 版が実装できない)
	virtual TPixelAccess getPixel(unsigned int x, unsigned int y) const = 0;
	virtual TPixelAccess getPixel(const Point2i& rPoint) const = 0;
	virtual void setPixel(unsigned int x, unsigned int y, const TPixelAccess& rPixelAccess) {}
	virtual void setPixel(const Point2i& rPoint, const TPixelAccess& rPixelAccess) {}
	virtual void clear() {}
	virtual void setSize(unsigned int width, unsigned int height) {}
	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const = 0;
	virtual Rect2i getRect() const = 0;
};

template<class TImage, class TPixelAccess,
	class TFuncConvertGet = PixelConvertFunctor<typename TImage::TypePixel, TPixelAccess>,
	class TFuncConvertSet = PixelConvertFunctor<TPixelAccess, typename TImage::TypePixel> >
class ImageAccessor : public ImageAccessorIF<TPixelAccess>
{
public:
	typedef TImage TypeContainer;
	typedef ImageAccessRangeIterator<TypeContainer, TPixelAccess, TFuncConvertGet, TFuncConvertSet, TraitsAccessNonConst<typename TImage::TypePixel, TPixelAccess, TypeContainer> > TypeIterator;
	typedef TypeIterator iterator;
	typedef ImageAccessRangeIterator<TypeContainer, TPixelAccess, TFuncConvertGet, TFuncConvertSet, TraitsAccessConst<typename TImage::TypePixel, TPixelAccess, TypeContainer> > TypeConstIterator;
	typedef TypeConstIterator const_iterator;

	ImageAccessor(TImage* pImage) : m_pImage(pImage)
	{
		assert(pImage);
	}
	virtual ~ImageAccessor() {}

	virtual TPixelAccess getPixel(unsigned int x, unsigned int y) const
	{
		TPixelAccess pixelAccess;
		m_funcGet(m_pImage->getPixel(x, y), pixelAccess);
		return pixelAccess;
	}
	virtual TPixelAccess getPixel(const Point2i& rPoint) const
	{
		return getPixel(rPoint.getX(), rPoint.getY());
	}

	virtual void setPixel(unsigned int x, unsigned int y, const TPixelAccess& rPixelAccess)
	{
		typename TImage::TypePixel pixelImage;
		m_funcSet(rPixelAccess, pixelImage);
		m_pImage->setPixel(x, y, pixelImage);
	}
	virtual void setPixel(const Point2i& rPoint, const TPixelAccess& rPixelAccess)
	{
		setPixel(rPoint.getX(), rPoint.getY(), rPixelAccess);
	}

	virtual void clear() { m_pImage->clear(); }
	virtual void setSize(unsigned int width, unsigned int height) { m_pImage->setSize(width, height); }
	virtual unsigned int getWidth() const { return m_pImage->getWidth(); }
	virtual unsigned int getHeight() const { return m_pImage->getHeight(); }
	virtual Rect2i getRect() const { return m_pImage->getRect(); }

	TypeIterator range_begin(const Rect2i& rRect)
	{
		Point2i pointStart(rRect.getLower());
		return TypeIterator(m_pImage, rRect, pointStart, m_funcGet, m_funcSet);
	}
	TypeConstIterator range_begin(const Rect2i& rRect) const
	{
		Point2i pointStart(rRect.getLower());
		return TypeConstIterator(m_pImage, rRect, pointStart, m_funcGet, m_funcSet);
	}
	TypeIterator range_end(const Rect2i& rRect)
	{
		Point2i pointEnd(rRect.getLowerX(), rRect.getUpperY());
		return TypeIterator(m_pImage, rRect, pointEnd, m_funcGet, m_funcSet);
	}
	TypeConstIterator range_end(const Rect2i& rRect) const
	{
		Point2i pointEnd(rRect.getLowerX(), rRect.getUpperY());
		return TypeConstIterator(m_pImage, rRect, pointEnd, m_funcGet, m_funcSet);
	}

private:
	TFuncConvertGet m_funcGet;
	TFuncConvertSet m_funcSet;

	TImage* m_pImage;
};

// 画像に対して read only なアクセスしか提供しない版
// const な画像を渡すことができる
template<class TImage, class TPixelAccess,
	class TFuncConvertGet = PixelConvertFunctor<typename TImage::TypePixel, TPixelAccess>,
	class TFuncConvertSet = PixelConvertFunctor<TPixelAccess, typename TImage::TypePixel> >
class ImageAccessorReadOnly : public ImageAccessorIF<TPixelAccess>
{
public:

	typedef TImage TypeContainer;
	typedef ImageAccessRangeIterator<TypeContainer, TPixelAccess, TFuncConvertGet, TFuncConvertSet, TraitsAccessConst<typename TImage::TypePixel, TPixelAccess, TypeContainer> > TypeConstIterator;
	typedef TypeConstIterator const_iterator;

	ImageAccessorReadOnly(const TImage* pImage) : m_pImage(pImage)
	{
		assert(pImage);
	}
	virtual ~ImageAccessorReadOnly() {}

	virtual TPixelAccess getPixel(unsigned int x, unsigned int y) const
	{
		TPixelAccess pixelAccess;
		m_funcGet(m_pImage->getPixel(x, y), pixelAccess);
		return pixelAccess;
	}
	virtual TPixelAccess getPixel(const Point2i& rPoint) const
	{
		return getPixel(rPoint.getX(), rPoint.getY());
	}

	virtual unsigned int getWidth() const { return m_pImage->getWidth(); }
	virtual unsigned int getHeight() const { return m_pImage->getHeight(); }
	virtual Rect2i getRect() const { return m_pImage->getRect(); }

	// iterator も const_iterator しか用意しない
	TypeConstIterator range_begin(const Rect2i& rRect) const
	{
		Point2i pointStart(rRect.getLower());
		return TypeConstIterator(m_pImage, rRect, pointStart, m_funcGet, TFuncConvertSet());
	}
	TypeConstIterator range_end(const Rect2i& rRect) const
	{
		Point2i pointEnd(rRect.getLowerX(), rRect.getUpperY());
		return TypeConstIterator(m_pImage, rRect, pointEnd, m_funcGet, TFuncConvertSet());
	}

private:
	TFuncConvertGet m_funcGet;
	const TImage* m_pImage;
};

#endif
