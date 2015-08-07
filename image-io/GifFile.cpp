//#include <wtypes.h>

#include "gif_lib.h"

#include <vector>
#include <algorithm>

#include "PixelType.h"
#include "Image.h"
#include "ImageFactory.h"
#include "ImageUtil.h"

#include "GifFile.h"

template <class TPixel>
struct DecodePixelFunctor
{
public:
	DecodePixelFunctor(const std::vector<PixelRGB8>& rTableColorsMap,
		const PixelRGB8& rColorBG, bool isTransparent, const GifPixelType& rPixelTransparent)
		 : m_rTableColorsMap(rTableColorsMap),
		m_colorBG(rColorBG), m_isTransparent(isTransparent),
		m_pixelTransparent(rPixelTransparent) {}

	TPixel operator()(const GifPixelType& rPixelGif) const
	{
		return (m_isTransparent && (rPixelGif == m_pixelTransparent)) ? m_colorBG : m_rTableColorsMap[rPixelGif];
	}

private:
	const std::vector<PixelRGB8>& m_rTableColorsMap;
	const PixelRGB8 m_colorBG;
	bool m_isTransparent;
	GifPixelType m_pixelTransparent;
};

template<>
PixelRGB8 DecodePixelFunctor<PixelRGB8>::operator()(const GifPixelType& rPixelGif) const
{
	// return (m_isTransparent && (rPixelGif == m_pixelTransparent)) ? m_colorBG : m_rTableColorsMap[rPixelGif];
	return m_rTableColorsMap[rPixelGif];
}

template<>
PixelRGBA8 DecodePixelFunctor<PixelRGBA8>::operator()(const GifPixelType& rPixelGif) const
{
	const PixelRGB8& rPixel = m_rTableColorsMap[rPixelGif];
	if (m_isTransparent && (rPixelGif == m_pixelTransparent)) {
//		return PixelRGBA8(m_colorBG.getR(), m_colorBG.getG(), m_colorBG.getB(),
//				ElementUtil::getMinValue<PixelRGBA8::TypeElement>());
		return PixelRGBA8(rPixel.getR(), rPixel.getG(), rPixel.getB(),
			ElementUtil::getMinValue<PixelRGBA8::TypeElement>());
	} else {
		return PixelRGBA8(rPixel.getR(), rPixel.getG(), rPixel.getB(),
			ElementUtil::getMaxValue<PixelRGBA8::TypeElement>());
	}
}

template<class TImage>
bool readPixels(GifFileType* pTypeFileGif,
		const std::vector<PixelRGB8>& tableColorsMap, const PixelRGB8& rColorBG,
		bool& rIsTransparent, GifPixelType& rPixelTransparent,
		const Point2i& rOriginImage,
		TImage* pImage)
{
	assert(pTypeFileGif && pImage);
	if (!(pTypeFileGif && pImage)) return false;

	int widthImage = pImage->getWidth();
	int heightImage = pImage->getHeight();
	if (pTypeFileGif->Image.Interlace) {
		// Need to perform 4 passes on the images:
		const int numPass = 4;
		int tableIndexOffset[numPass] = { 0, 4, 2, 1 };
		int tableIndexSkip[numPass] = { 8, 8, 4, 2 };
		DecodePixelFunctor<typename TImage::TypePixel> funcDecodePixel(tableColorsMap, rColorBG, rIsTransparent, rPixelTransparent);
		std::vector<GifPixelType> bufferLine(pImage->getNumBytesRow());
		GifPixelType* pLine = &(bufferLine[0]);

		for (int pass = 0; pass < 4; pass ++) {
			int indexOffset = tableIndexOffset[pass];
			int indexSkip = tableIndexSkip[pass];
			int startX = rOriginImage.getX();
			int startY = rOriginImage.getY() + indexOffset;
			for (int y = startY; y < heightImage; y += indexSkip) {
				if (::DGifGetLine(pTypeFileGif, pLine, widthImage) == GIF_ERROR) return false;
				Rect2i rect(startX, y, widthImage, y + 1);
				typename TImage::iterator it = pImage->range_begin(rect);
				std::transform(pLine, pLine + widthImage, pImage->range_begin(rect), funcDecodePixel);
			}
		}
	} else {
		// Non-interlaced image
		DecodePixelFunctor<typename TImage::TypePixel> funcDecodePixel(tableColorsMap, rColorBG, rIsTransparent, rPixelTransparent);
		std::vector<GifPixelType> bufferLine(pImage->getNumBytesRow());
		GifPixelType* pLine = &(bufferLine[0]);

		int startX = rOriginImage.getX();
		int startY = rOriginImage.getY();
		for (int y = startY; y < startY + heightImage; y ++) {
			if (::DGifGetLine(pTypeFileGif, pLine, widthImage) == GIF_ERROR) return false;
			Rect2i rect(startX, y, widthImage, y + 1);
			typename TImage::iterator it = pImage->range_begin(rect);
			std::transform(pLine, pLine + widthImage, pImage->range_begin(rect), funcDecodePixel);
		}
	}
	return true;

}

#define GIF_TRANSPARENT		0x01
GifByteType szNetscape20ext[] = "\x0bNETSCAPE2.0";
#define NSEXT_LOOP      0x01        // Loop Count field code

bool parseExtention(GifFileType* pTypeFileGif, bool& rIsTransparent, GifPixelType& rPixelTransparent)
{
	GifByteType *pExtension = NULL;
	int codeExtension = 0;

	if (::DGifGetExtension(pTypeFileGif, &codeExtension, &pExtension) == GIF_ERROR) {
		// DGifGetExtension error
		return false;
	}

	bool bNetscapeExt = false;
	switch (codeExtension) {
	case COMMENT_EXT_FUNC_CODE:
		// GIF89 comment
		break;
	case GRAPHICS_EXT_FUNC_CODE:
	{
		// GIF89 graphics control
		if (!(pExtension[0] == 4)) {
			return false;
		}
		//
		int flag = pExtension[1];
		// delay  = MAKEWORD(pExtension[2], pExtension[3]);
		// dispose = (flag >> GIF_DISPOSE_SHIFT) & GIF_DISPOSE_MASK;
		if (flag & GIF_TRANSPARENT) {
			rIsTransparent = true;
			rPixelTransparent = pExtension[4];
		}
		break;
	}
	case PLAINTEXT_EXT_FUNC_CODE:
		// GIF89 plaintext
		break;
	case APPLICATION_EXT_FUNC_CODE:
	{
		// GIF89 application block;
		assert(pExtension);
		if (!pExtension) return false;
		if (std::equal(szNetscape20ext, szNetscape20ext + szNetscape20ext[0], pExtension)) {
			// Netscape 2.0 extension
			bNetscapeExt = true;
		}
		break;
	}
	default:
		// pExtension record of unknown type;
		break;
	}
	do {
		if (::DGifGetExtensionNext(pTypeFileGif, &pExtension) == GIF_ERROR) {
			// DGifGetExtensionNext error
			return false;
		}
		// Process Netscape 2.0 extension (GIF looping)
		if (pExtension && bNetscapeExt) {
			GifByteType bLength = pExtension[0];
			int iSubCode = pExtension[1] & 0x07;
			if (bLength == 3 && iSubCode == NSEXT_LOOP) {
				unsigned int uLoopCount = ((unsigned long)(pExtension[2]) & 0xff) | ((unsigned long)(pExtension[3]) << 8);
				// m_uLoopCount = uLoopCount - 1;
			}
		}
	} while (pExtension);

	return true;
}

bool loadGif(const char* pNameFile, bool useAlpha, ImageBase** ppImage)
{
	if (!pNameFile || !ppImage || *ppImage) return false;

	GifFileType* pTypeFileGif = ::DGifOpenFileName(pNameFile);
	int width = pTypeFileGif->SWidth;
	int height = pTypeFileGif->SHeight;

	const int numColorsMap = 256;
	std::vector<PixelRGB8> tableColorsMap(numColorsMap);
	PixelRGB8 colorBG;

	// Copy global color map
	if (pTypeFileGif->SColorMap) {
		// Global Color Map
		for (unsigned int index = 0; index < numColorsMap; index ++) {
			GifColorType* pColor = pTypeFileGif->SColorMap->Colors + index;
			tableColorsMap[index] = PixelRGB8(pColor->Red, pColor->Green, pColor->Blue);
		}

		GifColorType* pColor = pTypeFileGif->SColorMap->Colors + pTypeFileGif->SBackGroundColor;
		colorBG.setR(pColor->Red);
		colorBG.setG(pColor->Green);
		colorBG.setB(pColor->Blue);
	}

	GifRecordType typeRecord = UNDEFINED_RECORD_TYPE;
	GifPixelType transparent = 0;
	bool isTransparent = false;
	do {

		if (DGifGetRecordType(pTypeFileGif, &typeRecord) == GIF_ERROR) {
			break;
		}
		switch (typeRecord) {
		case IMAGE_DESC_RECORD_TYPE:
			if (::DGifGetImageDesc(pTypeFileGif) != GIF_ERROR) {
				Point2i originImage(pTypeFileGif->Image.Left, pTypeFileGif->Image.Top);

				GifColorType* pColorTable = NULL;
				if (pTypeFileGif->Image.ColorMap == NULL) {
					// No Image Color Map
					// Copy global bitmap info for display
				} else {
					// Image Has Color Map
					for (unsigned int index = 0; index < numColorsMap; index ++) {
						GifColorType* pColor = pTypeFileGif->Image.ColorMap->Colors + index;
						tableColorsMap[index] = PixelRGB8(pColor->Red, pColor->Green, pColor->Blue);
					}
				}

				const int widthImage = pTypeFileGif->Image.Width;
				const int heightImage = pTypeFileGif->Image.Height;

				if (useAlpha) {
					ImageRGBA8* pImage = new ImageRGBA8(widthImage, heightImage);
					if (!readPixels<ImageRGBA8>(pTypeFileGif, tableColorsMap, colorBG, isTransparent, transparent, originImage, pImage)) {
						delete pImage;
						::DGifCloseFile(pTypeFileGif);
						return false;
					}
					*ppImage = pImage;
				} else {
					ImageRGB8* pImage = new ImageRGB8(widthImage, heightImage);
					if (!readPixels<ImageRGB8>(pTypeFileGif, tableColorsMap, colorBG, isTransparent, transparent, originImage, pImage)) {
						delete pImage;
						::DGifCloseFile(pTypeFileGif);
						return false;
					}
					*ppImage = pImage;
				}


			}
			break;
		case EXTENSION_RECORD_TYPE:
		{
			if (!parseExtention(pTypeFileGif, isTransparent, transparent)) {
				::DGifCloseFile(pTypeFileGif);
				return false;
			}

			break;

		}
		case TERMINATE_RECORD_TYPE:
			break;
		default:			// Should be trapped by DGifGetRecordType
			break;
		}
	} while (typeRecord != TERMINATE_RECORD_TYPE);

	::DGifCloseFile(pTypeFileGif);

	return true;
}

