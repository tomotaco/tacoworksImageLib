
#include <stdio.h>
#include <setjmp.h>

#include "Image.h"
#include "ImageFactory.h"
#include "ImageUtil.h"
#include "ImageAccessorFactory.h"

//#pragma pack(push, 1)

extern "C" {
// ImageAccessorFactory が windows.h 経由で Basetsd.h を include するので、
// INT32 の宣言が衝突しないように jpeglib.h (jmorecfg.h) のインクルードを後に持っていく
#include "jpeglib.h"

// エラーハンドラ
struct JpegErrorManageStruct {
  jpeg_error_mgr m_fieldPublic;
  jmp_buf m_bufferSetJmp;
};

void exitByLongJump(j_common_ptr pInfo)
{
  JpegErrorManageStruct* pErrorManager = reinterpret_cast<JpegErrorManageStruct*>(pInfo->err);

  ::longjmp(pErrorManager->m_bufferSetJmp, 1);
}
}

//#pragma pack(pop)

#include "JpegFile.h"

bool loadJpeg(FILE* pFile, ImageBase** ppImage)
{
	if (!(pFile && ppImage && !*ppImage)) return false;

	jpeg_decompress_struct jpegStruct;

	JpegErrorManageStruct structErrorManagement;
	jpegStruct.err = ::jpeg_std_error(&structErrorManagement.m_fieldPublic);
	structErrorManagement.m_fieldPublic.error_exit = exitByLongJump;
	if (::setjmp(structErrorManagement.m_bufferSetJmp)) {
		jpeg_destroy_decompress(&jpegStruct);
		return false;
	}

	::jpeg_create_decompress(&jpegStruct);

	::jpeg_stdio_src(&jpegStruct, pFile);
	::jpeg_read_header(&jpegStruct, TRUE);
	::jpeg_start_decompress(&jpegStruct);
	int row_stride = jpegStruct.output_width * jpegStruct.output_components;

	JSAMPARRAY ppBuffer = (*jpegStruct.mem->alloc_sarray)
							(reinterpret_cast<j_common_ptr>(&jpegStruct),
							JPOOL_IMAGE, row_stride, 1);

	if (jpegStruct.out_color_components == 3) {
		ImageRGB8* pImageRGB8 = ImageFactory::createImage<ImageRGB8>(jpegStruct.output_width, jpegStruct.output_height);
		unsigned int y = 0;
		while (jpegStruct.output_scanline < jpegStruct.output_height) {
			::jpeg_read_scanlines(&jpegStruct, ppBuffer, 1);
			JSAMPROW pBuf = *ppBuffer;
			for (unsigned int x = 0; x < jpegStruct.output_width; x ++) {
				PixelRGB8 pixel;
				pixel.setR(*pBuf ++);
				pixel.setG(*pBuf ++);
				pixel.setB(*pBuf ++);
				pImageRGB8->setPixel(x, y, pixel);
			}
			y ++;
		}
		*ppImage = pImageRGB8;
	} else {
		ImageGray8* pImageGray8 = ImageFactory::createImage<ImageGray8>(jpegStruct.output_width, jpegStruct.output_height);
		unsigned int y = 0;
		while (jpegStruct.output_scanline < jpegStruct.output_height) {
			::jpeg_read_scanlines(&jpegStruct, ppBuffer, 1);
			JSAMPROW pBuf = *ppBuffer;
			for (unsigned int x = 0; x < jpegStruct.output_width; x ++) {
				PixelGray8 pixel;
				pixel.setA(*pBuf ++);
				pImageGray8->setPixel(x, y, pixel);
			}
			y ++;
		}
		*ppImage = pImageGray8;
	}

	::jpeg_finish_decompress(&jpegStruct);
	::jpeg_destroy_decompress(&jpegStruct);

	return true;
}

bool loadJpeg(const char* pNameFile, ImageBase** ppImage)
{
	if (!(pNameFile && ppImage &&! *ppImage)) return false;

	FILE* pFile = fopen(pNameFile, "rb");
	if (!pFile)  return false;

	bool result = loadJpeg(pFile, ppImage);
	::fclose(pFile);

	return result;
}

bool loadJpeg(const wchar_t* pNameFile, ImageBase** ppImage)
{
	if (!(pNameFile && ppImage && !*ppImage)) return false;

	FILE* pFile = ::_wfopen(pNameFile, L"rb");
	if (!pFile)  return false;

	bool result = loadJpeg(pFile, ppImage);
	::fclose(pFile);

	return result;
}

bool saveJpeg(FILE* pFile, const ImageBase* pImage, int quality)
{
	if (!(pFile && pImage)) return false;

	jpeg_compress_struct jpegStruct;
	jpeg_create_compress(&jpegStruct);

	JSAMPROW pRow = 0;

	JpegErrorManageStruct structErrorManagement;
	jpegStruct.err = jpeg_std_error(&structErrorManagement.m_fieldPublic);
	structErrorManagement.m_fieldPublic.error_exit = exitByLongJump;
	if (::setjmp(structErrorManagement.m_bufferSetJmp)) {
		if (pRow) delete[] pRow;
		::jpeg_destroy_compress(&jpegStruct);

		return false;
	}

	jpeg_stdio_dest(&jpegStruct, pFile);

	jpegStruct.image_width = pImage->getWidth();
	jpegStruct.image_height = pImage->getHeight();

	unsigned int numElements = pImage->getNumElementsPerPixel();
	jpegStruct.input_components = (numElements == 1) ? 1 : 3;
	jpegStruct.in_color_space = (numElements == 1) ? JCS_GRAYSCALE : JCS_RGB;

	::jpeg_set_defaults(&jpegStruct);
	::jpeg_set_quality(&jpegStruct, quality, true);

	::jpeg_start_compress(&jpegStruct, true);

	if (numElements == 1) {
		const ImageAccessorIF<PixelGray8>* pAccessorIF = ImageAccessorFactory::createImageAccessor<PixelGray8>(pImage);
		unsigned int y = 0;
		pRow = new JSAMPLE[pAccessorIF->getWidth() * numElements];
		while (jpegStruct.next_scanline < jpegStruct.image_height) {
			JSAMPROW pBuf = pRow;
			for (unsigned int x = 0; x < pAccessorIF->getWidth(); x ++) {
				const PixelGray8& rPixel = pAccessorIF->getPixel(x, y);
				*pBuf ++ = rPixel.getA();
			}
			::jpeg_write_scanlines(&jpegStruct, &pRow, 1);
			y ++;
		}
		if (pRow) delete[] pRow;
	} else {
		const ImageAccessorIF<PixelRGB8>* pAccessorIF = ImageAccessorFactory::createImageAccessor<PixelRGB8>(pImage);
		unsigned int y = 0;
		pRow = new JSAMPLE[pAccessorIF->getWidth() * numElements];
		while (jpegStruct.next_scanline < jpegStruct.image_height) {
			JSAMPROW pBuf = pRow;
			for (unsigned int x = 0; x < pAccessorIF->getWidth(); x ++) {
				const PixelRGB8& rPixel = pAccessorIF->getPixel(x, y);
				*pBuf ++ = rPixel.getR();
				*pBuf ++ = rPixel.getG();
				*pBuf ++ = rPixel.getB();
			}
			::jpeg_write_scanlines(&jpegStruct, &pRow, 1);
			y ++;
		}
		if (pRow) delete[] pRow;
	}

	::jpeg_finish_compress(&jpegStruct);

	::jpeg_destroy_compress(&jpegStruct);

	return true;
}

bool saveJpeg(const char* pNameFile, const ImageBase* pImage, int quality)
{
	if (!(pNameFile &&pImage)) return false;

	FILE* pFile = ::fopen(pNameFile, "wb");
	if (!pFile) return false;

	bool result = saveJpeg(pFile, pImage, quality);
	::fclose(pFile);

	return result;
}

bool saveJpeg(const wchar_t* pNameFile, const ImageBase* pImage, int quality)
{
	if (!(pNameFile &&pImage)) return false;

	FILE* pFile = ::_wfopen(pNameFile, L"wb");
	if (!pFile) return false;

	bool result = saveJpeg(pFile, pImage, quality);
	::fclose(pFile);

	return result;
}
