#ifndef ImageTraitsAllocatedByDIBH
#define ImageTraitsAllocatedByDIBH

#include <windows.h>
#include "Pixel.h"

// 画像領域の確保のポリシー(CreateDIBSection() で確保)
template<typename TypePixel>
class ImageTraitsAllocatedByDIB
{
public:
	unsigned char* allocImage(unsigned int width, unsigned int height);
	void freeImage(unsigned char* pImage);
	HBITMAP getHandle() const { return m_handleBitmap; }
private:
	HBITMAP m_handleBitmap;
};

// 実際には 24bit しか実装しない
template<>
inline unsigned char* ImageTraitsAllocatedByDIB<PixelRGB8>::allocImage(unsigned int width, unsigned int height)
{
	// デスクトップのDCを得る
	HDC handleDCDesktop = GetDC(GetDesktopWindow());

	BITMAPINFO infoBitmapMemory;
	::ZeroMemory(&infoBitmapMemory, sizeof(infoBitmapMemory));
	infoBitmapMemory.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoBitmapMemory.bmiHeader.biWidth	= width;
	infoBitmapMemory.bmiHeader.biHeight = -int(height);
	infoBitmapMemory.bmiHeader.biPlanes = 1;
	infoBitmapMemory.bmiHeader.biBitCount = 24;
	infoBitmapMemory.bmiHeader.biCompression = BI_RGB;
  
	// ビットマップのハンドルと、作成したバッファの先頭アドレスを取得
	void* pBufferBitmapMemory = NULL;
	HBITMAP handleBitmapMemory = ::CreateDIBSection(handleDCDesktop, &infoBitmapMemory, DIB_RGB_COLORS, (void**)&pBufferBitmapMemory, 0,0);
	if (!handleBitmapMemory) return NULL;

	// handleBitmap も保持しておく必要あり
	m_handleBitmap = handleBitmapMemory;

	return reinterpret_cast<unsigned char*>(pBufferBitmapMemory);
}

template<>
inline void ImageTraitsAllocatedByDIB<PixelRGB8>::freeImage(unsigned char* pImage)
{
	// 削除するときは handle を渡さなあかん
	::DeleteObject(m_handleBitmap);
	m_handleBitmap = NULL;
}

#endif

