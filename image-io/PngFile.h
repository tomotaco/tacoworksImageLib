//---------------------------------------------------------------------------

#ifndef PngFileH
#define PngFileH

//---------------------------------------------------------------------------

#include "Image.h"

bool loadPng(const char* pNameFile, ImageBase** ppImage);
bool loadPng(const wchar_t* pNameFile, ImageBase** ppImage);
bool savePng(const char* pNameFile, const ImageBase* pImage, bool isInterlaced = true);
bool savePng(const wchar_t* pNameFile, const ImageBase* pImage, bool isInterlaced = true);

#endif
