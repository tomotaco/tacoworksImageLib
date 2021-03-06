#include <algorithm>
#include <sstream>
#include "PixelConvertFunctor.h"
#include "PixelInfoString.h"
#include "StaticSingleDispatcher.h"
#include "ImageUtil.h"

void ImageUtil::extractRGB(const ImageRGBA8& rImageSrc, ImageRGB8& rImageDst)
{
	rImageDst.setSize(rImageSrc.getWidth(), rImageSrc.getHeight());
	Rect2i rect = rImageSrc.getRect();
	std::transform(rImageSrc.range_begin(rect), rImageSrc.range_end(rect), rImageDst.range_begin(rect),
					PixelConvertFunctor<ImageRGBA8::TypePixel, ImageRGB8::TypePixel>());
}

void ImageUtil::extractR(const ImageRGBA8& rImageSrc, ImageGray8& rImageDst)
{
	rImageDst.setSize(rImageSrc.getWidth(), rImageSrc.getHeight());
	Rect2i rect = rImageSrc.getRect();
	std::transform(rImageSrc.range_begin(rect), rImageSrc.range_end(rect), rImageDst.range_begin(rect),
					PixelConvertFunctor<ImageRGBA8::TypePixel, ImageGray8::TypePixel,
						ExtractRChannelFunctor<ImageRGBA8::TypeElement> >());
}

void ImageUtil::extractG(const ImageRGBA8& rImageSrc, ImageGray8& rImageDst)
{
	rImageDst.setSize(rImageSrc.getWidth(), rImageSrc.getHeight());
	Rect2i rect = rImageSrc.getRect();
	std::transform(rImageSrc.range_begin(rect), rImageSrc.range_end(rect), rImageDst.range_begin(rect),
					PixelConvertFunctor<ImageRGBA8::TypePixel, ImageGray8::TypePixel,
						ExtractGChannelFunctor<ImageRGBA8::TypeElement> >());
}

void ImageUtil::extractB(const ImageRGBA8& rImageSrc, ImageGray8& rImageDst)
{
	rImageDst.setSize(rImageSrc.getWidth(), rImageSrc.getHeight());
	Rect2i rect = rImageSrc.getRect();
	std::transform(rImageSrc.range_begin(rect), rImageSrc.range_end(rect), rImageDst.range_begin(rect),
					PixelConvertFunctor<ImageRGBA8::TypePixel, ImageGray8::TypePixel,
						ExtractBChannelFunctor<ImageRGBA8::TypeElement> >());
}

void ImageUtil::extractA(const ImageRGBA8& rImageSrc, ImageGray8& rImageDst)
{
	rImageDst.setSize(rImageSrc.getWidth(), rImageSrc.getHeight());
	Rect2i rect = rImageSrc.getRect();
	std::transform(rImageSrc.range_begin(rect), rImageSrc.range_end(rect), rImageDst.range_begin(rect),
					PixelConvertFunctor<ImageRGBA8::TypePixel, ImageGray8::TypePixel,
						ExtractAChannelFunctor<ImageRGBA8::TypeElement> >());
}

void ImageUtil::compositeRGB(const ImageGray8& rImageSrcR, const ImageGray8& rImageSrcG, const ImageGray8& rImageSrcB,
				  ImageRGB8& rImageDst)
{
	// std::transform() は 2 入力までなのでここでは使えない。
	rImageDst.setSize(rImageSrcR.getWidth(), rImageSrcR.getHeight());
	Rect2i rect = rImageSrcR.getRect();
	ImageGray8::const_iterator itSrcR = rImageSrcR.range_begin(rect);
	ImageGray8::const_iterator itSrcG = rImageSrcG.range_begin(rect);
	ImageGray8::const_iterator itSrcB = rImageSrcB.range_begin(rect);
	ImageRGB8::iterator itDst = rImageDst.range_begin(rect);

	for (; itSrcR != rImageSrcR.range_end(rect); ++itSrcR, ++itSrcG, ++itSrcB, ++itDst) {
		*itDst = PixelRGB8((*itSrcR).getA(), (*itSrcG).getA(), (*itSrcB).getA());
	}
}

void ImageUtil::compositeRGBA(const ImageGray8& rImageSrcR, const ImageGray8& rImageSrcG, const ImageGray8& rImageSrcB, const ImageGray8& rImageSrcA,
				  ImageRGBA8& rImageDst)
{
	// std::transform() は 2 入力までなのでここでは使えない。
	rImageDst.setSize(rImageSrcR.getWidth(), rImageSrcR.getHeight());
	Rect2i rect = rImageSrcR.getRect();
	ImageGray8::const_iterator itSrcR = rImageSrcR.range_begin(rect);
	ImageGray8::const_iterator itSrcG = rImageSrcG.range_begin(rect);
	ImageGray8::const_iterator itSrcB = rImageSrcB.range_begin(rect);
	ImageGray8::const_iterator itSrcA = rImageSrcA.range_begin(rect);
	ImageRGBA8::iterator itDst = rImageDst.range_begin(rect);

	for (; itSrcR != rImageSrcR.range_end(rect); ++itSrcR, ++itSrcG, ++itSrcB, ++itDst) {
		*itDst = PixelRGBA8((*itSrcR).getA(), (*itSrcG).getA(), (*itSrcB).getA(), (*itSrcA).getA());
	}
}

void ImageUtil::expandRGBA(const ImageRGB8& rImageSrc, ImageRGBA8& rImageDst)
{
	rImageDst.setSize(rImageSrc.getWidth(), rImageSrc.getHeight());
	Rect2i rect = rImageSrc.getRect();
	std::transform(rImageSrc.range_begin(rect), rImageSrc.range_end(rect),
					rImageDst.range_begin(rect), PixelConvertFunctor<ImageRGB8::TypePixel, ImageRGBA8::TypePixel>());
}

void ImageUtil::expandRGBA(const ImageGray8& rImageSrc, ImageRGBA8& rImageDst)
{
	rImageDst.setSize(rImageSrc.getWidth(), rImageSrc.getHeight());
	Rect2i rect = rImageSrc.getRect();
	std::transform(rImageSrc.range_begin(rect), rImageSrc.range_end(rect),
					rImageDst.range_begin(rect), PixelConvertFunctor<ImageGray8::TypePixel, ImageRGBA8::TypePixel>());
}

void ImageUtil::expandRGB(const ImageGray8& rImageSrc, ImageRGB8& rImageDst)
{
	rImageDst.setSize(rImageSrc.getWidth(), rImageSrc.getHeight());
	Rect2i rect = rImageSrc.getRect();
	std::transform(rImageSrc.range_begin(rect), rImageSrc.range_end(rect),
					rImageDst.range_begin(rect), PixelConvertFunctor<ImageGray8::TypePixel, ImageRGB8::TypePixel>());
}

const char* ImageUtil::getTextTypeImage(const ImageBase* pImage)
{
	if (!pImage) return "Null Pointer";

	const ImageRGBA8*      pImageRGBA8      = dynamic_cast<const ImageRGBA8*>(pImage);
	const ImageRGB8*       pImageRGB8       = dynamic_cast<const ImageRGB8*>(pImage);
	const ImageGray8*      pImageGray8      = dynamic_cast<const ImageGray8*>(pImage);
	const ImageRGBA16*     pImageRGBA16     = dynamic_cast<const ImageRGBA16*>(pImage);
	const ImageRGB16*      pImageRGB16      = dynamic_cast<const ImageRGB16*>(pImage);
	const ImageGray16*     pImageGray16     = dynamic_cast<const ImageGray16*>(pImage);
	const ImageRGBASInt*   pImageRGBASInt   = dynamic_cast<const ImageRGBASInt*>(pImage);
	const ImageRGBSInt*    pImageRGBSInt    = dynamic_cast<const ImageRGBSInt*>(pImage);
	const ImageGraySInt*   pImageGraySInt   = dynamic_cast<const ImageGraySInt*>(pImage);
	const ImageRGBAUInt*   pImageRGBAUInt   = dynamic_cast<const ImageRGBAUInt*>(pImage);
	const ImageRGBUInt*    pImageRGBUInt    = dynamic_cast<const ImageRGBUInt*>(pImage);
	const ImageGrayUInt*   pImageGrayUInt   = dynamic_cast<const ImageGrayUInt*>(pImage);
	const ImageRGBAFloat*  pImageRGBAFloat  = dynamic_cast<const ImageRGBAFloat*>(pImage);
	const ImageRGBFloat*   pImageRGBFloat   = dynamic_cast<const ImageRGBFloat*>(pImage);
	const ImageGrayFloat*  pImageGrayFloat  = dynamic_cast<const ImageGrayFloat*>(pImage);
	const ImageRGBADouble* pImageRGBADouble = dynamic_cast<const ImageRGBADouble*>(pImage);
	const ImageRGBDouble*  pImageRGBDouble  = dynamic_cast<const ImageRGBDouble*>(pImage);
	const ImageGrayDouble* pImageGrayDouble = dynamic_cast<const ImageGrayDouble*>(pImage);

	if (pImageRGBA8)      return "8bitRGBA";
	if (pImageRGB8)       return "8bitRGB";
	if (pImageGray8)      return "8bitGray";
	if (pImageRGBA16)     return "16bitRGBA";
	if (pImageRGB16)      return "16bitRGB";
	if (pImageGray16)     return "16bitGray";
	if (pImageRGBASInt)   return "SIntRGBA";
	if (pImageRGBSInt)    return "SIntRGB";
	if (pImageGraySInt)   return "SIntGray";
	if (pImageRGBAUInt)   return "UIntRGBA";
	if (pImageRGBUInt)    return "UIntRGB";
	if (pImageGrayUInt)   return "UIntGray";
	if (pImageRGBAFloat)  return "FloatRGBA";
	if (pImageRGBFloat)   return "FloatRGB";
	if (pImageGrayFloat)  return "FloatGray";
	if (pImageRGBADouble) return "DoubleRGBA";
	if (pImageRGBDouble)  return "DoubleRGB";
	if (pImageGrayDouble) return "DoubleGray";

	return "Unknown Image Type";
}

bool ImageUtil::is8(const ImageBase* pImage)
{
	if (!pImage) return false;

	const ImageRGBA8*     pImageRGBA8     = dynamic_cast<const ImageRGBA8*>(pImage);
	const ImageRGB8*      pImageRGB8      = dynamic_cast<const ImageRGB8*>(pImage);
	const ImageGray8*     pImageGray8     = dynamic_cast<const ImageGray8*>(pImage);
	return (pImageRGBA8 || pImageRGB8 || pImageGray8) ? true : false;
}

bool ImageUtil::is16(const ImageBase* pImage)
{
	if (!pImage) return false;

	const ImageRGBA16*    pImageRGBA16    = dynamic_cast<const ImageRGBA16*>(pImage);
	const ImageRGB16*     pImageRGB16     = dynamic_cast<const ImageRGB16*>(pImage);
	const ImageGray16*    pImageGray16    = dynamic_cast<const ImageGray16*>(pImage);

	return (pImageRGBA16 || pImageRGB16 || pImageGray16) ? true : false;
}

bool ImageUtil::isSInt(const ImageBase* pImage)
{
	if (!pImage) return false;

	const ImageRGBASInt*    pImageRGBASInt    = dynamic_cast<const ImageRGBASInt*>(pImage);
	const ImageRGBSInt*     pImageRGBSInt     = dynamic_cast<const ImageRGBSInt*>(pImage);
	const ImageGraySInt*    pImageGraySInt    = dynamic_cast<const ImageGraySInt*>(pImage);

	return (pImageRGBASInt || pImageRGBSInt || pImageGraySInt) ? true : false;
}

bool ImageUtil::isUInt(const ImageBase* pImage)
{
	if (!pImage) return false;

	const ImageRGBAUInt*    pImageRGBAUInt    = dynamic_cast<const ImageRGBAUInt*>(pImage);
	const ImageRGBUInt*     pImageRGBUInt     = dynamic_cast<const ImageRGBUInt*>(pImage);
	const ImageGrayUInt*    pImageGrayUInt    = dynamic_cast<const ImageGrayUInt*>(pImage);

	return (pImageRGBAUInt || pImageRGBUInt || pImageGrayUInt) ? true : false;
}

bool ImageUtil::isFloat(const ImageBase* pImage)
{
	if (!pImage) return false;

	const ImageRGBAFloat* pImageRGBAFloat = dynamic_cast<const ImageRGBAFloat*>(pImage);
	const ImageRGBFloat*  pImageRGBFloat  = dynamic_cast<const ImageRGBFloat*>(pImage);
	const ImageGrayFloat* pImageGrayFloat = dynamic_cast<const ImageGrayFloat*>(pImage);

	return (pImageRGBAFloat || pImageRGBFloat || pImageGrayFloat) ? true : false;
}

bool ImageUtil::isDouble(const ImageBase* pImage)
{
	if (!pImage) return false;

	const ImageRGBADouble* pImageRGBADouble = dynamic_cast<const ImageRGBADouble*>(pImage);
	const ImageRGBDouble*  pImageRGBDouble  = dynamic_cast<const ImageRGBDouble*>(pImage);
	const ImageGrayDouble* pImageGrayDouble = dynamic_cast<const ImageGrayDouble*>(pImage);

	return (pImageRGBADouble || pImageRGBDouble || pImageGrayDouble) ? true : false;
}

bool ImageUtil::isGray(const ImageBase* pImage)
{
	if (!pImage) return false;

	const ImageGray8*     pImageGray8     = dynamic_cast<const ImageGray8*>(pImage);
	const ImageGray16*    pImageGray16    = dynamic_cast<const ImageGray16*>(pImage);
	const ImageGrayFloat* pImageGrayFloat = dynamic_cast<const ImageGrayFloat*>(pImage);

	return (pImageGray8 || pImageGray16 || pImageGrayFloat) ? true : false;
}

bool ImageUtil::isRGB(const ImageBase* pImage)
{
	if (!pImage) return false;

	const ImageRGB8*      pImageRGB8      = dynamic_cast<const ImageRGB8*>(pImage);
	const ImageRGB16*     pImageRGB16     = dynamic_cast<const ImageRGB16*>(pImage);
	const ImageRGBFloat*  pImageRGBFloat  = dynamic_cast<const ImageRGBFloat*>(pImage);

	return (pImageRGB8 || pImageRGB16 || pImageRGBFloat) ? true : false;
}

bool ImageUtil::isRGBA(const ImageBase* pImage)
{
	if (!pImage) return false;

	const ImageRGBA8*     pImageRGBA8     = dynamic_cast<const ImageRGBA8*>(pImage);
	const ImageRGBA16*    pImageRGBA16    = dynamic_cast<const ImageRGBA16*>(pImage);
	const ImageRGBAFloat* pImageRGBAFloat = dynamic_cast<const ImageRGBAFloat*>(pImage);

	return (pImageRGBA8 || pImageRGBA16 || pImageRGBAFloat) ? true : false;
}

std::string ImageUtil::getTextPixel(const Point2i& rPosition, const ImageBase* pImage, bool isHexadecimal)
{
	assert(pImage);
	if (!pImage) return "(Null Pointer)";

	int x = rPosition.getX();
	int y = rPosition.getY();
	unsigned int width = pImage->getWidth();
	unsigned int height = pImage->getHeight();

	assert(0 <= x && x < width && 0 <= y && y < height);
	if (!(0 <= x && x < width && 0 <= y && y < height)) {
		return "(Out of range)";
	}

	std::ostringstream osstr;
	osstr << getTextTypeImage(pImage) << "=(";

	if (isHexadecimal) {
		PixelInfoString<HexadecimalElementInfoStrBuilder> strInfoPixel(rPosition);
		StaticSingleDispatcher<ImageBase, TypeListImages, PixelInfoString<HexadecimalElementInfoStrBuilder> >::dispatch(pImage, strInfoPixel);
		osstr << strInfoPixel.getInfoString();
	} else {
		PixelInfoString<DecimalElementInfoStrBuilder> strInfoPixel(rPosition);
		StaticSingleDispatcher<ImageBase, TypeListImages, PixelInfoString<DecimalElementInfoStrBuilder> >::dispatch(pImage, strInfoPixel);
		osstr << strInfoPixel.getInfoString();
	}
	osstr << ")";

	return osstr.str();
}

