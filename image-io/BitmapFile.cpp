#include <stdio.h>
#include <windows.h>

#include "Image.h"
#include "ImageFactory.h"
#include "ImageUtil.h"
#include "ImageAccessor.h"
#include "ImageAccessorFactory.h"
#include "BitmapFile.h"

bool loadBitmap(HBITMAP handleBitmapFile, ImageBase** ppImage)
{
	if (!(handleBitmapFile && ppImage && !*ppImage)) return false;

	BITMAP bitmapFile;
	::GetObject(handleBitmapFile, sizeof(bitmapFile), &bitmapFile);

	// デスクトップのDCを得る
	HDC handleDCDesktop = GetDC(GetDesktopWindow());

	BITMAPINFO infoBitmapMemory;
	::ZeroMemory(&infoBitmapMemory, sizeof(infoBitmapMemory));
	infoBitmapMemory.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoBitmapMemory.bmiHeader.biWidth	= bitmapFile.bmWidth;
	infoBitmapMemory.bmiHeader.biHeight = -bitmapFile.bmHeight;
	infoBitmapMemory.bmiHeader.biPlanes = 1;
	infoBitmapMemory.bmiHeader.biBitCount = 24;
	infoBitmapMemory.bmiHeader.biCompression = BI_RGB;
  
	// ビットマップのハンドルと、作成したバッファの先頭アドレスを取得
	void* pBufferBitmapMemory = NULL;
	HBITMAP handleBitmapMemory = CreateDIBSection(handleDCDesktop, &infoBitmapMemory, DIB_RGB_COLORS, (void**)&pBufferBitmapMemory, 0,0);
 
	// 作成失敗 (致命的)
	if (!handleBitmapMemory) return false;

	// HBITMAPにHDCを結び付ける
	HDC handleDCFile = ::CreateCompatibleDC(handleDCDesktop);
	HDC handleDCMemory = ::CreateCompatibleDC(handleDCDesktop);
	::ReleaseDC(GetDesktopWindow(), handleDCDesktop); // GetDC(hWnd) で取ってきたものには解放の必要あり

	HGDIOBJ handleObjectOldDCFile = ::SelectObject(handleDCFile, handleBitmapFile);
	HGDIOBJ handleObjectOldAtDCMemory = ::SelectObject(handleDCMemory, handleBitmapMemory);

	::BitBlt(handleDCMemory, 0, 0, bitmapFile.bmWidth, bitmapFile.bmHeight, handleDCFile, 0, 0, SRCCOPY);

	::SelectObject(handleDCFile, handleObjectOldDCFile);
	::SelectObject(handleDCMemory, handleObjectOldAtDCMemory);

	::DeleteDC(handleDCFile);
	::DeleteDC(handleDCMemory);

	ImageBase* pImageBase = ImageFactory::createImage<ImageRGB8>(bitmapFile.bmWidth, bitmapFile.bmHeight);

 	unsigned int numBytesPerLine = bitmapFile.bmWidth * 3;
	unsigned int numBytesResidual = numBytesPerLine % 4;
	unsigned int numBytesToPad = (numBytesResidual > 0) ? 4 - numBytesResidual : 0;
	unsigned int numBytesAligned = numBytesPerLine + numBytesToPad;

	unsigned char* pBuffer = reinterpret_cast<unsigned char*>(pBufferBitmapMemory);
	for (unsigned int y = 0; y < pImageBase->getHeight(); y++) {
		::memcpy(pImageBase->getRawAddress(0, y), pBuffer, pImageBase->getNumBytesRow());
		pBuffer += numBytesAligned;
	}
	*ppImage = pImageBase;

	::DeleteObject(handleBitmapMemory);

	return true;
}

bool loadBitmap(const char* pNameFile, ImageBase** ppImage)
{
	if (!(pNameFile && ppImage && !*ppImage)) return false;

	HBITMAP handleBitmapFile = (HBITMAP)LoadImageA(NULL, const_cast<char*>(pNameFile), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (handleBitmapFile == NULL) return FALSE;

	bool result = loadBitmap(handleBitmapFile, ppImage);
	::DeleteObject(handleBitmapFile);

	return result;
}

bool loadBitmap(const wchar_t* pNameFile, ImageBase** ppImage)
{
	if (!(pNameFile && ppImage && !*ppImage)) return false;

	HBITMAP handleBitmapFile = (HBITMAP)LoadImageW(NULL, const_cast<wchar_t*>(pNameFile), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (handleBitmapFile == NULL) return FALSE;

	bool result = loadBitmap(handleBitmapFile, ppImage);
	::DeleteObject(handleBitmapFile);

	return result;
}

bool saveBitmap(FILE* pFile, const ImageBase* pImage)
{
	if (!(pFile && pImage)) return false;

	::fseek(pFile, 0, SEEK_SET);

	unsigned int width  = pImage->getWidth();
	unsigned int height = pImage->getHeight();

	unsigned int numBytesPerLine = width * 3;
	unsigned int numBytesResidual = numBytesPerLine % 4;
	unsigned int numBytesToPad = (numBytesResidual > 0) ? 4 - numBytesResidual : 0;
	unsigned int numBytesAligned = numBytesPerLine + numBytesToPad;

	BITMAPFILEHEADER head;
	BITMAPINFOHEADER info;

	::memset(&head, 0, sizeof(head));
	::memset(&info, 0, sizeof(info));

	unsigned short bm = char('B') | (char('M')<<8);
	head.bfType = bm;
	head.bfSize = sizeof(head) + sizeof(info) + height * numBytesAligned;
	head.bfOffBits = sizeof(head) + sizeof(info);

	::fwrite(&head, sizeof(head), 1, pFile);

	info.biSize	 = sizeof(info);
	info.biWidth	= width;
	info.biHeight = height;
	info.biPlanes = 1;
	info.biBitCount = 24;
	info.biCompression = BI_RGB;

	::fwrite(&info, sizeof(info), 1, pFile);

	const ImageAccessorIF<PixelRGB8>* pAccessorIF = ImageAccessorFactory::createImageAccessor<PixelRGB8>(pImage);
	for (unsigned int j = 0; j < height; j ++) {
		for (unsigned int i = 0; i < width; i ++) {
			PixelRGB8 color = pAccessorIF->getPixel(i, height - j - 1);
			unsigned char colorR = color.getR();
			unsigned char colorG = color.getG();
			unsigned char colorB = color.getB();

			// 下位3BYTEを、B,G,Rの順で書き込む
			::fwrite(&colorB, 1, 1, pFile);
			::fwrite(&colorG, 1, 1, pFile);
			::fwrite(&colorR, 1, 1, pFile);
		}

		if (numBytesToPad >0) {
			int padval = 0;
			::fwrite(&padval, numBytesToPad, 1, pFile);
		}
	}

	return true;
}


bool saveBitmap(const char* pNameFile, const ImageBase* pImage)
{
	if (!(pNameFile && pImage)) return false;

	FILE* pFile = ::fopen(pNameFile, "wb");
	if (!pFile) return false;

	bool result = saveBitmap(pFile, pImage);
	::fclose(pFile);

	return result;
}

bool saveBitmap(const wchar_t* pNameFile, const ImageBase* pImage)
{
	if (!(pNameFile && pImage)) return false;

	FILE* pFile = ::_wfopen(pNameFile, L"wb");
	if (!pFile) return false;

	bool result = saveBitmap(pFile, pImage);
	::fclose(pFile);

	return result;
}
