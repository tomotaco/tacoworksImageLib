#ifndef ImageBaseH
#define ImageBaseH

#include "Point2i.h"
#include "Rect2i.h"
#include "Pixel.h"

class ImageBase
{
public:
	ImageBase() {}
	virtual ~ImageBase() {}
	
	virtual ImageBase* createCopy() const = 0;
	virtual void clear() = 0;
	virtual void setSize(unsigned int width, unsigned int height) = 0;

	virtual unsigned int getWidth() const  = 0;
	virtual unsigned int getHeight() const = 0;
	virtual ElementType getTypeElement() const = 0;
	virtual ChannelType getTypeChannel() const = 0;
	virtual unsigned int getNumBytesPerElement() const = 0;
	virtual unsigned int getNumElementsPerPixel() const = 0;
	virtual unsigned int getNumBytesPixel() const = 0;
	virtual unsigned int getNumBytesRow() const = 0;
	virtual unsigned int getNumBytesBuffer() const = 0;
	virtual void* getRawBuffer() = 0;
	virtual const void* getRawBuffer() const = 0;

	virtual void* getRawAddress(unsigned int x, unsigned int y) = 0;
	virtual void* getRawAddress(const Point2i& rPoint) = 0;
	virtual const void* getRawAddress(unsigned int x, unsigned int y) const = 0;
	virtual const void* getRawAddress(const Point2i& rPoint) const = 0;

	virtual Rect2i getRect() const = 0;
	virtual void getMinMaxValueByDouble(const Rect2i& rRect, double& rValueMin, double& rValueMax) const = 0;
	virtual double getValueElementByDouble(unsigned int x, unsigned int y, unsigned int indexElement) const = 0;
	virtual double getValueElementByDouble(const Point2i& rPoint, unsigned int indexElement) const = 0;
	virtual void setValueElementByDouble(unsigned int x, unsigned int y, unsigned int indexElement, double value) = 0;
	virtual void setValueElementByDouble(const Point2i& rPoint, unsigned int indexElement, double value) = 0;
};

#endif
