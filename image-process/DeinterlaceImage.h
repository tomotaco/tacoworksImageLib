#include "Image.h"

namespace DeinterlaceImage
{
	// 奇数ラインを残しつつ間は補間
	void deinterlaceImageKeepOddByBlend(ImageGray8& rImage);
	void deinterlaceImageKeepOddByCopy(ImageGray8& rImage);

	void deinterlaceImageKeepOddByBlendOpt(ImageGray8& rImage);
	void deinterlaceImageKeepOddByCopyOpt(ImageGray8& rImage);

	// 奇数ラインを残し(偶数はスキップ)つつ横幅を半分に縮める
	void deinterlaceImagKeepOddeByShrink(const ImageGray8& rImageSrc, ImageGray8& rImageDst);
};
