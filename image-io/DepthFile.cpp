#include <memory.h>
#include "targa.h"

#include "Image.h"
#include "ImageFactory.h"
#include "ImageAccessorFactory.h"
#include "ImageUtil.h"
#include "DepthFile.h"

// Animation Master Depth File (*.dep) を読み込む
// Monochrome 8bit に変換してしまう

bool loadDepth(const char* pNameFile, ImageBase** ppImage)
{
	if (!pNameFile || !ppImage || *ppImage) return false;

	tga_image tgaImage;
	tga_result result = tga_read(&tgaImage, pNameFile);
	if (result != TGA_NOERR) return false;

	if (tga_is_colormapped(&tgaImage)) {
		tga_color_unmap(&tgaImage);
	}

	unsigned int width = tgaImage.width;
	unsigned int height = tgaImage.height;
	unsigned int bitsPerPixel = tgaImage.pixel_depth;

	if (bitsPerPixel != 32) return false;

	// depth の最大値・最小値を取得
	unsigned long minValueDepth = ULONG_MAX;
	unsigned long maxValueDepth = 0;
	for (unsigned int y = 0; y < tgaImage.height; y ++) {
		for (unsigned int x = 0; x < tgaImage.width; x ++) {
			unsigned char* pPixel = tga_find_pixel(&tgaImage, x, y);
			unsigned char valueR = 0, valueG = 0, valueB = 0, valueA = 0;
			tga_unpack_pixel(pPixel, tgaImage.pixel_depth,
							 &valueB, &valueG, &valueR, &valueA);

			unsigned long valueDepth = ((unsigned int)(valueA) << 24) | ((unsigned int)(valueR) << 16) |
			((unsigned int)(valueG) << 8) | (unsigned int)(valueB);
			if (valueDepth < minValueDepth) minValueDepth = valueDepth;
			if (maxValueDepth < valueDepth) maxValueDepth = valueDepth;
		}
	}
	// 最大値・最小値をもとに正規化
	unsigned char maxValueElement = ElementUtil::getMaxValue<ImageGray8::TypeElement>();
	unsigned char minValueElement = ElementUtil::getMinValue<ImageGray8::TypeElement>();
	ImageGray8 *pImage = ImageFactory::createImage<ImageGray8>(tgaImage.width, tgaImage.height);
	for (unsigned int y = 0; y < tgaImage.height; y ++) {
		for (unsigned int x = 0; x < tgaImage.width; x ++) {
			unsigned char* pPixel = tga_find_pixel(&tgaImage, x, y);
			unsigned char valueR = 0, valueG = 0, valueB = 0, valueA = 0;
			tga_unpack_pixel(pPixel, tgaImage.pixel_depth,
							 &valueB, &valueG, &valueR, &valueA);

			unsigned long valueDepth = ((unsigned int)(valueA) << 24) | ((unsigned int)(valueR) << 16) |
			((unsigned int)(valueG) << 8) | (unsigned int)(valueB);

			unsigned char valueNormalized = cropValue<unsigned char>((unsigned char)(float(maxValueElement) * float(valueDepth - minValueDepth) / float(maxValueDepth - minValueDepth)),
								minValueElement, maxValueElement);
			unsigned char value = maxValueElement - valueNormalized;

			pImage->setPixel(x, y, ImageGray8::TypePixel(value));
		}
	}

	tga_free_buffers(&tgaImage);

	*ppImage = static_cast<ImageBase*>(pImage);

	return pImage ? true : false;
}

bool saveDepth(const char* pNameFile, const ImageBase* pImage)
{
	if (!pNameFile || !pImage) return false;

	// depth は読み込み時に変換してしまっているので、保存しても無意味のため実装せず。
	return false;
}

