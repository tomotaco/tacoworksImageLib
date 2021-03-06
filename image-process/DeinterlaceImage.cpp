#include <functional>
#include <algorithm>

#include "DeinterlaceImage.h"

struct BlendPixelsFunctor : std::binary_function<PixelGray8, PixelGray8, PixelGray8>
{
	PixelGray8 operator()(const PixelGray8& rPixel0, const PixelGray8& rPixel1) const
	{
		unsigned short value = ((unsigned short)rPixel0.getA() + (unsigned short)rPixel1.getA()) >> 1;
		return PixelGray8((unsigned char)value);
	}
};

void DeinterlaceImage::deinterlaceImageKeepOddByBlend(ImageGray8& rImage)
{
	unsigned int width = rImage.getWidth();
	unsigned int height = rImage.getHeight();
	BlendPixelsFunctor functor;
	for (unsigned int y = 1; y < height - 1; y += 2) {
		Rect2i rectSrcUpper(0, y - 1, width, y);
		Rect2i rectSrcLower(0, y + 1, width, y + 2);
		Rect2i rectDst(0, y, width, y + 1);

		std::transform(rImage.range_begin(rectSrcUpper), rImage.range_end(rectSrcUpper),
			rImage.range_begin(rectSrcLower),
			rImage.range_begin(rectDst),
			functor);
	}

	Rect2i rectLastSrc(0, height - 2, width, height - 1);
	Rect2i rectLastDst(0, height - 1, width, height);
	std::copy(rImage.range_begin(rectLastSrc), rImage.range_end(rectLastSrc),
		rImage.range_begin(rectLastDst));
}

void DeinterlaceImage::deinterlaceImageKeepOddByCopy(ImageGray8& rImage)
{
	unsigned int width = rImage.getWidth();
	unsigned int height = rImage.getHeight();
	for (unsigned int y = 1; y < height; y += 2) {
		Rect2i rectSrcUpper(0, y - 1, width, y);
		Rect2i rectDst(0, y, width, y + 1);
		std::copy(rImage.range_begin(rectSrcUpper), rImage.range_end(rectSrcUpper),
			rImage.range_begin(rectDst));
	}
}

/* Athlon64X2 5600+(2.8GHz) で 100回の処理時間を計測
	DeinterlaceImage::deinterlaceImageKeepOddByBlendOpt(): 0.0103466sec
	DeinterlaceImage::deinterlaceImageKeepOddByBlend(): 0.26118sec
	BlendOpt is 25.243times faster.
*/
/* Pentium4 2.4BGHz で 100回の処理時間を計測
	DeinterlaceImage::deinterlaceImageKeepOddByBlendOpt(): 0.0317752sec
	DeinterlaceImage::deinterlaceImageKeepOddByBlend(): 0.901009sec
	BlendOpt is 28.3557times faster.
*/
void DeinterlaceImage::deinterlaceImageKeepOddByBlendOpt(ImageGray8& rImage)
{
	unsigned int width = rImage.getWidth();
	unsigned int height = rImage.getHeight();
	for (unsigned int y = 1; y < height - 1; y += 2) {
		const unsigned long* pSrcUpper = reinterpret_cast<const unsigned long*>(rImage.getAddress(0, y - 1));
		const unsigned long* pSrcLower = reinterpret_cast<const unsigned long*>(rImage.getAddress(0, y + 1));
		unsigned long* pDst = reinterpret_cast<unsigned long*>(rImage.getAddress(0, y));

		for (unsigned x = 0; x < width; x += 4, pSrcUpper ++, pSrcLower ++, pDst ++) {
			// v2 = (v0 & v1) + (((v0 ^ v1) & 0xfefefefe) >> 1); // やねう本2の加重平均のビット演算より
			*pDst = (*pSrcUpper & *pSrcLower) + (((*pSrcUpper ^ *pSrcLower) & 0xfefefefe) >> 1);
		}
	}

	const unsigned long* pSrcUpper = reinterpret_cast<const unsigned long*>(rImage.getAddress(0, height - 2));
	unsigned long* pDst = reinterpret_cast<unsigned long*>(rImage.getAddress(0, height - 1));
	::memcpy(pDst, pSrcUpper, width);
}

/* Athlon64X2 5600+(2.8GHz) で 100回の処理時間を計測
	DeinterlaceImage::deinterlaceImageKeepOddByCopyOpt(): 0.00508952sec
	DeinterlaceImage::deinterlaceImageKeepOddByCopy(): 0.171762sec
	BlendOpt is 33.7483times faster.
*/
/* Pentium4 2.4BGHz で 100回の処理時間を計測
	DeinterlaceImage::deinterlaceImageKeepOddByCopyOpt(): 0.00458075sec
	DeinterlaceImage::deinterlaceImageKeepOddByCopy(): 0.560959sec
	BlendOpt is 122.46times faster.
*/
void DeinterlaceImage::deinterlaceImageKeepOddByCopyOpt(ImageGray8& rImage)
{
	unsigned int width = rImage.getWidth();
	unsigned int height = rImage.getHeight();
	for (unsigned int y = 1; y < height; y += 2) {
		const unsigned long* pSrcUpper = reinterpret_cast<const unsigned long*>(rImage.getAddress(0, y - 1));
		unsigned long* pDst = reinterpret_cast<unsigned long*>(rImage.getAddress(0, y));
		::memcpy(pDst, pSrcUpper, width);
	}
}

// 奇数ラインを残し(偶数はスキップ)つつ横幅を半分に縮める
void DeinterlaceImage::deinterlaceImagKeepOddeByShrink(const ImageGray8& rImageSrc, ImageGray8& rImageDst)
{
	unsigned int widthSrc = rImageSrc.getWidth();
	unsigned int halfWidthSrc = widthSrc / 2;
	unsigned int heightSrc = rImageSrc.getHeight();
	unsigned int halfHeightSrc = heightSrc / 2;

	if (rImageDst.getWidth() != halfWidthSrc || rImageDst.getHeight() != halfHeightSrc) {
		rImageDst.setSize(halfWidthSrc, halfHeightSrc);
	}

	for (unsigned int y = 0; y < halfHeightSrc; y ++) {
		const unsigned long* pSrc = reinterpret_cast<const unsigned long*>(rImageSrc.getAddress(0, y << 1));
		unsigned short* pDst = reinterpret_cast<unsigned short*>(rImageDst.getAddress(0, y));

		unsigned int numIteration = halfWidthSrc / 2;
		for (unsigned int indexIteration = 0; indexIteration < numIteration; indexIteration ++, pSrc ++, pDst ++) {
			// 隣接するピクセルの平均を求める(src 側は 4 ピクセル、dst 側は 2ピクセル同時処理)
			unsigned long valueSrc = *pSrc;
			unsigned long valueSrcUpper = (valueSrc & 0xff00ff00) >> 8;
			unsigned long valueSrcLower = valueSrc & 0x00ff00ff;
//			unsigned long sumUnpacked = ((valueSrcUpper + valueSrcLower) & 0x01fe01fe) >> 1;
//			unsigned long sumPacked = (sumUnpacked & 0x00ff0000) >> 8 | (sumUnpacked & 0x000000ff);

			unsigned long sumUnpacked = valueSrcUpper + valueSrcLower;
			unsigned long sumPacked = ((sumUnpacked & 0x01fe0000) >> 9) | ((sumUnpacked & 0x000001fe) >> 1);

			*pDst = (unsigned short)sumPacked;
		}
	}
}
