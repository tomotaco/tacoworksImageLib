#ifndef ImageUtilH
#define ImageUtilH

#include <string>
#include "Image.h"
#include "ImageAccessor.h"
#include "ImageAccessorFactory.h"

namespace ImageUtil
{
	void extractRGB(const ImageRGBA8& rImageRGBA, ImageRGB8& rImageRGB);
	void extractR(const ImageRGBA8& rImageRGBA, ImageGray8& rImageGray);
	void extractG(const ImageRGBA8& rImageRGBA, ImageGray8& rImageGray);
	void extractB(const ImageRGBA8& rImageRGBA, ImageGray8& rImageGray);
	void extractA(const ImageRGBA8& rImageRGBA, ImageGray8& rImageGray);

	void compositeRGB(const ImageGray8& rImageR, const ImageGray8& rImageG, const ImageGray8& rImageB,
					  ImageRGB8& rImageRGB);

	void compositeRGBA(const ImageGray8& rImageR, const ImageGray8& rImageG, const ImageGray8& rImageB, const ImageGray8& rImageA,
					  ImageRGBA8& rImageRGBA);

	void expandRGBA(const ImageRGB8& rImageRGB, ImageRGBA8& rImageRGBA);
	void expandRGBA(const ImageGray8& rImageGray, ImageRGBA8& rImageRGBA);

	void expandRGB(const ImageGray8& rImageGray, ImageRGB8& rImageRGB);

	bool is8(const ImageBase* pImage);
	bool is16(const ImageBase* pImage);
	bool isSInt(const ImageBase* pImage);
	bool isUInt(const ImageBase* pImage);
	bool isFloat(const ImageBase* pImage);
	bool isDouble(const ImageBase* pImage);
	bool isGray(const ImageBase* pImage);
	bool isRGB(const ImageBase* pImage);
	bool isRGBA(const ImageBase* pImage);
	const char* getTextTypeImage(const ImageBase* pImage);

	std::string getTextPixel(const Point2i& rPosition, const ImageBase* pImage, bool isHexadecimal);

	template <class TImage>
	ImageBase* createAndConvertImage(const ImageBase* pImage)
	{
		if (!pImage) return NULL;

		unsigned int width = pImage->getWidth();
		unsigned int height = pImage->getHeight();
		TImage* pImageDst = new TImage(width, height);
		const ImageAccessorIF<typename TImage::TypePixel>* pAccessorIF = ImageAccessorFactory::createImageAccessor<typename TImage::TypePixel>(pImage);
		for (unsigned int y = 0; y < height; y ++) {
			for (unsigned int x = 0; x < width; x ++) {
				pImageDst->setPixel(x, y, pAccessorIF->getPixel(x, y));
			}
		}

		delete pAccessorIF;
		return static_cast<ImageBase*>(pImageDst);
	}
};

#endif
