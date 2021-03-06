#include <memory.h>

//extern "C" {
#include "targa.h"
//}

#include "Image.h"
#include "ImageFactory.h"
#include "ImageAccessorFactory.h"
#include "ImageUtil.h"
#include "TargaFile.h"

namespace {

template<class TImage>
ImageBase* copyFromTGAImage(const tga_image& rImageTGA)
{
	TImage* pImage = ImageFactory::createImage<TImage>(rImageTGA.width, rImageTGA.height);
	for (unsigned int y = 0; y < rImageTGA.height; y ++) {
		for (unsigned int x = 0; x < rImageTGA.width; x ++) {
			unsigned char* pPixel = ::tga_find_pixel(&rImageTGA, x, y);
			unsigned char valueR = 0, valueG = 0, valueB = 0, valueA = 0;
			::tga_unpack_pixel(pPixel, rImageTGA.pixel_depth,
								(TImage::numElements == 1) ? &valueA : &valueB,
								(TImage::numElements > 1)  ? &valueG : NULL,
								(TImage::numElements > 1)  ? &valueR : NULL,
								(TImage::numElements == 4) ? &valueA : NULL);
			if (TImage::numElements == 1) {
#ifdef WIN64
				pImage->setPixel(x, y, typename TImage::TypePixel(valueA));
#else
				pImage->setPixel(x, y, TImage::TypePixel(valueA));
#endif
			} else if (TImage::numElements == 3) {
#ifdef WIN64
				pImage->setPixel(x, y, typename TImage::TypePixel(valueR, valueG, valueB));
#else
				pImage->setPixel(x, y, TImage::TypePixel(valueR, valueG, valueB));
#endif
			} else if (TImage::numElements == 4) {
#ifdef WIN64
				pImage->setPixel(x, y, typename TImage::TypePixel(valueR, valueG, valueB, valueA));
#else
				pImage->setPixel(x, y, TImage::TypePixel(valueR, valueG, valueB, valueA));
#endif
			} else {
				assert(0);
			}
		}
	}
	return static_cast<ImageBase*>(pImage);
}

}

bool loadTarga(FILE* pFile, ImageBase** ppImage)
{
	if (!(pFile && ppImage && !*ppImage)) return false;

	tga_image tgaImage;
	tga_result result = ::tga_read_from_FILE(&tgaImage, pFile);
	if (result != TGA_NOERR) return false;

	if (::tga_is_colormapped(&tgaImage)) {
		::tga_color_unmap(&tgaImage);
	}

	unsigned int width = tgaImage.width;
	unsigned int height = tgaImage.height;
	unsigned int bitsPerPixel = tgaImage.pixel_depth;

	ImageBase* pImageBase = NULL;
	switch (bitsPerPixel) {
	case 8:
			pImageBase = copyFromTGAImage<ImageGray8>(tgaImage);
		break;
	case 16: case 24:
			pImageBase = copyFromTGAImage<ImageRGB8>(tgaImage);
		break;
	case 32:
			pImageBase = copyFromTGAImage<ImageRGBA8>(tgaImage);
		break;
	default:
		break;
	}

	::tga_free_buffers(&tgaImage);

	*ppImage = pImageBase;

	return pImageBase ? true : false;
}


bool loadTarga(const char* pNameFile, ImageBase** ppImage)
{
	if (!(pNameFile && ppImage && !*ppImage)) return false;

	FILE* pFile = ::fopen(pNameFile, "rb");
	if (!pFile) return false;

	bool result = loadTarga(pFile, ppImage);
	::fclose(pFile);

	return result;
}

bool loadTarga(const wchar_t* pNameFile, ImageBase** ppImage)
{
	if (!(pNameFile && ppImage && !*ppImage)) return false;

	FILE* pFile = ::_wfopen(pNameFile, L"rb");
	if (!pFile) return false;

	bool result = loadTarga(pFile, ppImage);
	::fclose(pFile);

	return result;
}

bool saveTarga(FILE* pFile, const ImageBase* pImage)
{
	if (!(pFile && pImage)) return false;

	unsigned int width = pImage->getWidth();
	unsigned int height = pImage->getHeight();

	tga_result result = TGA_NOERR;
	// 8bit gray, RGB, RGBA しか保存できないので変換
	ImageBase* pImageBase8bit = NULL;
	if (ImageUtil::isGray(pImage)) {
		pImageBase8bit = ImageUtil::createAndConvertImage<ImageGray8>(pImage);
		uint8_t* pBuffer = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(pImageBase8bit->getRawBuffer()));
		result = ::tga_write_mono_rle_to_FILE(pFile, pBuffer, width, height);
	} else if (ImageUtil::isRGB(pImage)) {
		pImageBase8bit = ImageUtil::createAndConvertImage<ImageRGB8>(pImage);
		uint8_t* pBuffer = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(pImageBase8bit->getRawBuffer()));
		result = ::tga_write_bgr_rle_to_FILE(pFile, pBuffer, width, height, 24);
	} else if (ImageUtil::isRGBA(pImage)) {
		pImageBase8bit = ImageUtil::createAndConvertImage<ImageRGBA8>(pImage);
		uint8_t* pBuffer = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(pImageBase8bit->getRawBuffer()));
		result = ::tga_write_bgr_rle_to_FILE(pFile, pBuffer, width, height, 32);
	}
	delete pImageBase8bit;

	return (result == TGA_NOERR) ? true : false;
}

bool saveTarga(const char* pNameFile, const ImageBase* pImage)
{
	if (!(pNameFile && pImage)) return false;

	FILE* pFile = ::fopen(pNameFile, "wb");
	if (!pFile) return false;

	bool result = saveTarga(pFile, pImage);
	::fclose(pFile);

	return result;
}

bool saveTarga(const wchar_t* pNameFile, const ImageBase* pImage)
{
	if (!(pNameFile && pImage)) return false;

	FILE* pFile = ::_wfopen(pNameFile, L"wb");
	if (!pFile) return false;

	bool result = saveTarga(pFile, pImage);
	::fclose(pFile);

	return result;
}
