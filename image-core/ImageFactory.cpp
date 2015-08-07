#include "ImageFactory.h"

namespace {

Factory<ImageBase, std::pair<ElementType, ChannelType> > g_factory;

Factory<ImageBase, std::pair<ElementType, ChannelType> >& getFactory()
{
	if (g_factory.isEmpty()) {
		g_factory.registerCreator(std::make_pair(ELEMENT_8, CHANNEL_GRAY), ImageGray8::createImage, ImageGray8::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_8, CHANNEL_RGB), ImageRGB8::createImage, ImageRGB8::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_8, CHANNEL_RGBA), ImageRGBA8::createImage, ImageRGBA8::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_16, CHANNEL_GRAY), ImageGray16::createImage, ImageGray16::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_16, CHANNEL_RGB), ImageRGB16::createImage, ImageRGB16::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_16, CHANNEL_RGBA), ImageRGBA16::createImage, ImageRGBA16::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_32S, CHANNEL_GRAY), ImageGraySInt::createImage, ImageGraySInt::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_32S, CHANNEL_RGB), ImageRGBSInt::createImage, ImageRGBSInt::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_32S, CHANNEL_RGBA), ImageRGBASInt::createImage, ImageRGBASInt::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_32U, CHANNEL_GRAY), ImageGrayUInt::createImage, ImageGrayUInt::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_32U, CHANNEL_RGB), ImageRGBUInt::createImage, ImageRGBUInt::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_32U, CHANNEL_RGBA), ImageRGBAUInt::createImage, ImageRGBAUInt::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_FLOAT, CHANNEL_GRAY), ImageGrayFloat::createImage, ImageGrayFloat::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_FLOAT, CHANNEL_RGB), ImageRGBFloat::createImage, ImageRGBFloat::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_FLOAT, CHANNEL_RGBA), ImageRGBAFloat::createImage, ImageRGBAFloat::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_DOUBLE, CHANNEL_GRAY), ImageGrayDouble::createImage, ImageGrayDouble::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_DOUBLE, CHANNEL_RGB), ImageRGBDouble::createImage, ImageRGBDouble::duplicateImage);
		g_factory.registerCreator(std::make_pair(ELEMENT_DOUBLE, CHANNEL_RGBA), ImageRGBADouble::createImage, ImageRGBADouble::duplicateImage);
	}

	return g_factory;
}
   
}

ImageBase* ImageFactory::createImage(ElementType typeElement, ChannelType typeChannel)
{
	Factory<ImageBase, std::pair<ElementType, ChannelType> >& rFactory = getFactory();
	return rFactory.createObject(std::make_pair(typeElement, typeChannel));
}

ImageBase* ImageFactory::createImage(unsigned int width, unsigned int height, ElementType typeElement, ChannelType typeChannel)
{
	ImageBase* pImageBase = createImage(typeElement, typeChannel);
	if (!pImageBase) return NULL;
	pImageBase->setSize(width, height);

	return pImageBase;
}

ImageBase* ImageFactory::duplicateImage(const ImageBase* pImageSrc)
{
	if (!pImageSrc) return NULL;
	Factory<ImageBase, std::pair<ElementType, ChannelType> >& rFactory = getFactory();
	return rFactory.duplicateObject(std::make_pair(pImageSrc->getTypeElement(), pImageSrc->getTypeChannel()), pImageSrc);
}
