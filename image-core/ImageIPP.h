#ifndef ImageIPPH
#define ImageIPPH

#include "ImageTraitsAllocatedByIPP.h"
#include "Image.h"
#include "TypeList.h"

// Intel IPP でメモリ管理される画像
typedef Image<PixelRGBA8, ImageTraitsRowAlignedBy32Byte, ImageTraitsAllocatedByIPP<PixelRGBA8> >			ImageRGBA8IPP;
typedef Image<PixelRGB8, ImageTraitsRowAlignedBy32Byte, ImageTraitsAllocatedByIPP<PixelRGB8> >			ImageRGB8IPP;
typedef Image<PixelGray8, ImageTraitsRowAlignedBy32Byte, ImageTraitsAllocatedByIPP<PixelGray8> >			ImageGray8IPP;
typedef Image<PixelRGBA16, ImageTraitsRowAlignedBy32Byte, ImageTraitsAllocatedByIPP<PixelRGBA16> >		ImageRGBA16IPP;
typedef Image<PixelRGB16, ImageTraitsRowAlignedBy32Byte, ImageTraitsAllocatedByIPP<PixelRGB16> >			ImageRGB16IPP;
typedef Image<PixelGray16, ImageTraitsRowAlignedBy32Byte, ImageTraitsAllocatedByIPP<PixelGray16> >		ImageGray16IPP;
typedef Image<PixelRGBAFloat, ImageTraitsRowAlignedBy32Byte, ImageTraitsAllocatedByIPP<PixelRGBAFloat> >	ImageRGBAFloatIPP;
typedef Image<PixelRGBFloat, ImageTraitsRowAlignedBy32Byte, ImageTraitsAllocatedByIPP<PixelRGBFloat> >	ImageRGBFloatIPP;
typedef Image<PixelGrayFloat, ImageTraitsRowAlignedBy32Byte, ImageTraitsAllocatedByIPP<PixelGrayFloat> >	ImageGrayFloatIPP;

typedef TypeList09(ImageRGBA8IPP, ImageRGB8IPP, ImageGray8IPP,
	ImageRGBA16IPP, ImageRGB16IPP, ImageGray16IPP,
	ImageRGBAFloatIPP, ImageRGBFloatIPP, ImageGrayFloatIPP) TypeListImagesTPP;

#endif
