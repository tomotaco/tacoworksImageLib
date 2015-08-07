#ifndef ImageFactoryH
#define ImageFactoryH

#include "ElementType.h"
#include "PixelType.h"
#include "Image.h"
#include "Factory.h"

namespace ImageFactory
{
	template<class TImage>
	static TImage* createImage()
	{
		return new TImage;
	}

	template<class TImage>
	static  TImage* createImage(unsigned int width, unsigned int height)
	{
		return new TImage(width, height);
	}

	ImageBase* createImage(ElementType typeElement, ChannelType typeChannel);
	ImageBase* createImage(unsigned int width, unsigned int height, ElementType typeElement, ChannelType typeChannel);
   
	ImageBase* duplicateImage(const ImageBase* pImageSrc);
};

#endif
