//---------------------------------------------------------------------------

#ifndef DepthFileH
#define DepthFileH

//---------------------------------------------------------------------------

#include "Image.h"

bool loadDepth(const char* pNameFile, ImageBase** ppImage);
bool saveDepth(const char* pNameFile, const ImageBase* pImage);

#endif
