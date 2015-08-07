//---------------------------------------------------------------------------

#ifndef BitmapFileH
#define BitmapFileH

//---------------------------------------------------------------------------

#include "Image.h"

bool loadBitmap(const char* pNameFile, ImageBase** ppImage);
bool loadBitmap(const wchar_t* pNameFile, ImageBase** ppImage);
bool saveBitmap(const char* pNameFile, const ImageBase* pImage);
bool saveBitmap(const wchar_t* pNameFile, const ImageBase* pImage);

#endif
