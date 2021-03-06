//---------------------------------------------------------------------------

#ifndef JpegFileH
#define JpegFileH

//---------------------------------------------------------------------------

#include "Image.h"

// ppImage は関数内で new した実体のポインタを返すので、delete する必要がある
bool loadJpeg(const char* pNameFile, ImageBase** ppImage);
bool loadJpeg(const wchar_t* pNameFile, ImageBase** ppImage);
bool saveJpeg(const char* pNameFile, const ImageBase* pImage, int quality = 100);
bool saveJpeg(const wchar_t* pNameFile, const ImageBase* pImage, int quality = 100);

#endif
