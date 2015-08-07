//---------------------------------------------------------------------------

#ifndef TargaFileH
#define TargaFileH

//---------------------------------------------------------------------------

#include "Image.h"

bool loadTarga(const char* pNameFile, ImageBase** ppImage);
bool loadTarga(const wchar_t* pNameFile, ImageBase** ppImage);
bool saveTarga(const char* pNameFile, const ImageBase* pImage);
bool saveTarga(const wchar_t* pNameFile, const ImageBase* pImage);

#endif
