#ifndef ImageRangeIteratorH
#define ImageRangeIteratorH

#include <iterator>
#include "Point2i.h"
#include "Rect2i.h"
#include "Image.h"
#include "PixelTraits.h"

template<class TImage, class PixelTraits>
class ImageRangeIterator : public std::iterator<std::bidirectional_iterator_tag, typename TImage::TypePixel>
{
public:
	typedef typename PixelTraits::TypeValue TypeValue;
	typedef typename PixelTraits::TypeReference TypeReference;
	typedef typename PixelTraits::TypePointer TypePointer;
	typedef typename PixelTraits::TypeContainer TypeContainer;
	typedef typename PixelTraits::TypeContainerReference TypeContainerReference;
	typedef typename PixelTraits::TypeContainerPointer TypeContainerPointer;

	typedef ImageRangeIterator<TImage, TraitsNonConst<typename TImage::TypePixel, TImage> > TypeIterator;
	typedef ImageRangeIterator<TImage, TraitsConst<typename TImage::TypePixel, TImage> > TypeConstIterator;

	ImageRangeIterator() : m_pImage(NULL) {}

	ImageRangeIterator(TypeContainerPointer pImage, const Rect2i& rRectRange, const Point2i& rPointStart)
	  : m_pImage(pImage), m_rectRange(rRectRange), m_position(rPointStart)
	{
		m_pPixelCurrent = pImage->getAddress(rPointStart.getX(), rPointStart.getY());
		m_isInsideRange = m_rectRange.doesInclude(rPointStart) ? true : false;
	}

	// NonConst -> Const のコンストラクタ兼、NonConst -> NonConst のコピーコンストラクタ
	ImageRangeIterator(const TypeIterator& rX)
	  : m_pImage(rX.m_pImage), m_rectRange(rX.m_rectRange), m_position(rX.m_position),
		m_pPixelCurrent(rX.m_pPixelCurrent), m_isInsideRange(rX.m_isInsideRange) {}

	// NonConst から Const な iterator を生成する際、NonConst な iterator のメンバへのアクセスを許す必要がある
	// friend TypeConstIterator;
	friend class ImageRangeIterator<TImage, TraitsConst<typename TImage::TypePixel, TImage> >;

	ImageRangeIterator& operator=(const ImageRangeIterator& rX)
	{
		m_pImage = rX.m_pImage;
		m_rectRange = rX.m_rectRange;
		m_position = rX.m_position;
		m_pPixelCurrent = rX.m_pPixelCurrent;
		m_isInsideRange = rX.m_isInsideRange;

		return *this;
	}

	TypeReference operator*() const
	{
		assert(m_isInsideRange);
		if (!m_isInsideRange) return *m_pImage->getAddress(0, 0);

		return *m_pPixelCurrent;
	}

	ImageRangeIterator& operator++()
	{
		if (!m_isInsideRange) return *this;

		int x = m_position.getX();
		int y = m_position.getY();
		if (x < m_rectRange.getUpperX() - 1) {
			x ++;
			m_pPixelCurrent ++;
		} else {
			x = m_rectRange.getLowerX();
			y ++;
			m_pPixelCurrent = m_pImage->getAddress(x, y);
			if (y > m_rectRange.getUpperY()) {
				m_isInsideRange = false;
			}
		}
		m_position.set(x, y);

		return *this;
	}
	ImageRangeIterator operator++(int)
	{
		ImageRangeIterator ret = *this;
		operator++();
		return ret;
	}
	ImageRangeIterator& operator--()
	{
		int x = m_position.getX();
		int y = m_position.getY();
		if (m_rectRange.getLowerX() < x) {
			x --;
			m_pPixelCurrent --;
		} else {
			x = m_rectRange.getUpperX() - 1;
			if (m_rectRange.getLowerY() == y) {
				m_isInsideRange = false;
			}
			y --;
			m_pPixelCurrent = m_pImage->getAddress(x, y);
		}
		m_position.set(x, y);

		return *this;
	}
	ImageRangeIterator& operator--(int)
	{
		ImageRangeIterator<TImage, PixelTraits> ret = *this;
		operator--();
		return ret;
	}

	bool operator==(const ImageRangeIterator& rX) const
	{
		return (m_pPixelCurrent == rX.m_pPixelCurrent) ? true : false;
	}
	bool operator!=(const ImageRangeIterator& rX) const
	{
		return !(*this == rX);
	}

	const Point2i& getPosition() const { return m_position; }
	unsigned int getPositionX() const { return m_position.getX(); }
	unsigned int getPositionY() const { return m_position.getY(); }

	void setPosition(const Point2i& rPosition)
	{
		if (m_rectRange.doesInclude(rPosition)) {
			m_isInsideRange = true;
		} else {
			m_isInsideRange = false;
		}
	}
	void setPosition(unsigned int x, unsigned int y)
	{
		setPosition(Point2i(x, y));
	}

	void moveUp()
	{
		unsigned int y = m_position.getY();
		if (m_rectRange.getLowerY() < y) {
			m_position.setY(y - 1);
			m_pPixelCurrent = reinterpret_cast<TypePointer>(
				reinterpret_cast<unsigned char*>(m_pPixelCurrent) - m_pImage->getNumBytesRow());
		} else {
			m_isInsideRange = false;
		}
	}
	
	void moveDown()
	{
		unsigned int y = m_position.getY();
		if (y < m_rectRange.getUpperY() - 1) {
			m_position.setY(y + 1);
			m_pPixelCurrent = reinterpret_cast<TypePointer>(
				reinterpret_cast<unsigned char*>(m_pPixelCurrent) + m_pImage->getNumBytesRow());
		} else {
			m_isInsideRange = false;
		}
	}
	void moveLeft()
	{
		unsigned int x = m_position.getX();
		if (m_rectRange.getLowerX() < x) {
			m_position.setX(x - 1);
			m_pPixelCurrent --;
		} else {
			m_isInsideRange = false;
		}
	}
	void moveRight()
	{
		unsigned int x = m_position.getX();
		if (x < m_rectRange.getUpperX() - 1) {
			m_position.setX(x + 1);
			m_pPixelCurrent ++;
		} else {
			m_isInsideRange = false;
		}
	}

	void moveLeftTop()
	{
		setPosition(m_rectRange.getLower());
	}
	void moveRightTop()
	{
		setPosition(Point2i(m_rectRange.getUpperX(), m_rectRange.getLowerY()));
	}
	void moveLeftDown()
	{
		setPosition(Point2i(m_rectRange.getUpperY(), m_rectRange.getLowerX()));
	}
	void moveRightDown()
	{
		setPosition(m_rectRange.getUpper());
	}

	bool isInsideRange() const { return m_isInsideRange; }

protected:
	TypeContainerPointer m_pImage;
	Rect2i m_rectRange;
	Point2i m_position;
	TypePointer m_pPixelCurrent;
	bool m_isInsideRange;
};

#endif
