#ifndef ImageDIBH
#define ImageDIBH

#include "ImageTraitsAllocatedByDIB.h"
#include "Image.h"
#include "TypeList.h"

typedef Image<PixelRGB8, ImageTraitsRowAlignedBy32Bit, ImageTraitsAllocatedByDIB<PixelRGB8> > ImageRGB8DIB;
typedef TypeList01(ImageRGB8DIB) TypeListImagesDIB;

#endif
