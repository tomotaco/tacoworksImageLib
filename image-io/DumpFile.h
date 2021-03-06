//---------------------------------------------------------------------------

#ifndef DumpFileH
#define DumpFileH

//---------------------------------------------------------------------------

#include <fstream>
#include <string>

#include "Endian.h"

#include "Image.h"
#include "ImageFactory.h"
#include "ImageAccessorFactory.h"
#include "ImageUtil.h"

#include "DumpFile.h"

namespace {
/*
	Image Dump File Format Ver0.1
	Byte	
	0-10	"IMAGE-DUMP¥0"
	11		Version(Major) = 0
	12		Version(Minor) = 1
	13		TypeElement
	14		NumElements
	15		Endian (0: Big, 1: Little)
	16		Channel Order
	17-20	width
	21-24	height
	25-28	NumBytesBuffer
	29		"¥0"
*/

std::string strHeader = "IMAGE-DUMP";
char versionMajorCurrent = 0;
char versionMinorCurrent = 1;

/*
Channel Order
	BGRA	00
	RGBA	01
	ABGR	10
	ARGB	11

	bit0	BGR(0) or RGB(1)
	bit1	Alpha is tail(0) or head(1)
*/

}

template<typename TypeChar>
bool loadDump(const TypeChar* pNameFile, ImageBase** ppImage)
{
	if (!pNameFile || !ppImage || *ppImage) return false;

	ImageBase* pImageBase = NULL;
	{
		std::basic_ifstream<TypeChar, std::char_traits<TypeChar> > ifs(pNameFile, std::ios::in | std::ios::binary);
		if (!ifs) return false;

		int sizeHeader = strHeader.length() + 1;
		char* pBufHeader = new char[sizeHeader];

		ifs.read(pBufHeader, sizeHeader);
		if (!ifs) return false;
		bool isHeaderValid = true;
		if (pBufHeader[sizeHeader - 1] != '¥0' ||	// ヘッダ文字列の最後は 0 terminated
			strHeader != pBufHeader) {
			isHeaderValid = false;
		}		
		delete pBufHeader;
		if (!isHeaderValid) return false;

		char versionMajor = ifs.get();
		if (!ifs) return false;
		char versionMinor = ifs.get();
		if (!ifs) return false;
		if (!(versionMajor < versionMajorCurrent ||
			(versionMajor == versionMajorCurrent && versionMinor <= versionMinorCurrent))) return false;

		ElementType typeElement = (ElementType)ifs.get();
		if (!ifs) return false;
		unsigned int numElements = (unsigned int)ifs.get();
		if (!ifs) return false;
		bool isLittleEndianFile = (bool)ifs.get();
		if (!ifs) return false;
		char orderChannel = ifs.get();
		if (!ifs) return false;
		if (orderChannel != 0x0) return false; // BGRA しか対応していない

		bool isLittleEndian = Endian::isLittleEndian();

		char bufValue[4];
		ifs.read(bufValue, 4);
		if (!ifs) return false;
		unsigned long widthBigEndian = *(unsigned long*)(&bufValue);
		unsigned long width = isLittleEndian ? Endian::reverseEndian(widthBigEndian) : widthBigEndian;

		ifs.read(bufValue, 4);
		if (!ifs) return false;
		unsigned long heightBigEndian = *(unsigned long*)(&bufValue);
		unsigned long height = isLittleEndian ? Endian::reverseEndian(heightBigEndian) : heightBigEndian;

		ifs.read(bufValue, 4);
		if (!ifs) return false;
		unsigned long numBytesBufferBigEndian = *(unsigned long*)(&bufValue);
		unsigned long numBytesBuffer = isLittleEndian ? Endian::reverseEndian(numBytesBufferBigEndian) : numBytesBufferBigEndian;

		char valueDummy = ifs.get();
		if (!ifs) return false;

		ChannelType typeChannel = getTypeChannelFromNumElements(numElements);
		pImageBase = ImageFactory::createImage(width, height, typeElement, typeChannel);

		// ImageFactory で生成される画像の numBytesRow も型によって違うので一定ではない
		unsigned int numBytesRow = pImageBase->getNumBytesPixel() * width;
		for (unsigned int y = 0; y < height; y ++) {
			ifs.read(reinterpret_cast<char*>(pImageBase->getRawAddress(0, y)), numBytesRow);
			if (!ifs) return false;
		}
	}

	*ppImage = pImageBase;

	return pImageBase ? true : false;
}

template<typename TypeChar>
bool saveDump(const TypeChar* pNameFile, const ImageBase* pImage)
{
	if (!pNameFile || !pImage) return false;

	unsigned int width = pImage->getWidth();
	unsigned int height = pImage->getHeight();

	bool isLittleEndian = Endian::isLittleEndian();

	{
		std::basic_ofstream<TypeChar, std::char_traits<TypeChar> > ofs(pNameFile, std::ios::out | std::ios::trunc | std::ios::binary);
		if (!ofs) return false;

		int sizeHeader = strHeader.length() + 1; // ¥0 まで書きこむ
		ofs.write(strHeader.c_str(), sizeHeader);
		ofs.put(versionMajorCurrent);
		ofs.put(versionMinorCurrent);
		ofs.put((char)pImage->getTypeElement());
		ofs.put((char)pImage->getNumElementsPerPixel());
		ofs.put((char)isLittleEndian);
		ofs.put((char)0x0);	// Channel Order: BGRA

		unsigned int widthInBigEndian = isLittleEndian ? Endian::reverseEndian(width) : width;
		ofs.write((char*)(&widthInBigEndian), 4);

		unsigned int heightInBigEndian = isLittleEndian ? Endian::reverseEndian(height) : height;
		ofs.write((char*)(&heightInBigEndian), 4);

		// ファイルに書く場合は 1 ラインごとの padding は不要
		unsigned int numBytesRow = pImage->getNumBytesPixel() * width;
		unsigned int numBytesBuffer = numBytesRow * height;
		unsigned int numBytesInBigEndian = isLittleEndian ? Endian::reverseEndian(numBytesBuffer) : numBytesBuffer;
		ofs.write((char*)(&numBytesInBigEndian), 4);
		
		ofs.put((unsigned char)0x0);

		// 1 ラインずつ書いていく
		for (unsigned int y = 0; y < height; y ++) {
			ofs.write(reinterpret_cast<const char*>(pImage->getRawAddress(0, y)), numBytesRow);
		}
	}

	return true;
}

#endif
