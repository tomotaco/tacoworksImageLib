#ifndef OSXFileH
#define OSXFileH

//---------------------------------------------------------------------------

#include "Image.h"

bool loadOSX(const char* pNameFile, ImageBase** ppImage);
bool saveOSX(const char* pNameFile, const ImageBase* pImage);

#endif
