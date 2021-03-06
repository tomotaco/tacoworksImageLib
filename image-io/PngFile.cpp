#include <wtypes.h>
#include <stdio.h>

#include "png.h"
#include "PixelType.h"
#include "Image.h"
#include "ImageFactory.h"
#include "ImageUtil.h"

#include "PngFile.h"

bool loadPng(FILE* pFile, ImageBase** ppImage)
{
	if (!(pFile && ppImage && !*ppImage)) return false;

	const size_t numSizeSignature = 8;
	const int numBytesSignature = sizeof(unsigned char) * numSizeSignature;
	unsigned char signature[numBytesSignature];

	// シグネチャを読み込む
	size_t numSizeReadSignature = fread(signature, sizeof(unsigned char), numSizeSignature, pFile);
	if (numSizeReadSignature != numSizeSignature) return false;

	// シグネチャの内容が正しいかどうかをチェック
	if (png_sig_cmp(signature, 0, numBytesSignature) != 0) return false;

	png_struct* pPngStruct = ::png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pPngStruct) return false;

	png_info* pPngInfo = ::png_create_info_struct(pPngStruct);
	if (!pPngInfo) {
		::png_destroy_read_struct(&pPngStruct, NULL, NULL);
		return false;
	}

	ImageBase* pImageBase = NULL;
	png_byte** ppRowTable = 0;

	// libpng 内でエラーが発生した場合、ここに longjmp してくる
	if (setjmp(pPngStruct->jmpbuf)) {
		if (ppRowTable) delete[] ppRowTable;
		if (pImageBase) delete pImageBase;
		::png_destroy_read_struct(&pPngStruct, &pPngInfo, NULL);
		return false;
	}

	::png_init_io(pPngStruct, pFile);
	::png_set_sig_bytes(pPngStruct, numBytesSignature);
	::png_read_info(pPngStruct, pPngInfo);

	// 画像サイズ・ピクセル深度・カラータイプを取得
	png_uint_32 width = 0;
	png_uint_32 height = 0;
	int bit_depth = 0;
	int color_type = 0;
	::png_get_IHDR(pPngStruct, pPngInfo, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

	bool hasTRNS = ::png_get_valid(pPngStruct, pPngInfo, PNG_INFO_tRNS) ? true : false;

	// 読み込み時に 8bit に変換するよう設定
	if (color_type == PNG_COLOR_TYPE_PALETTE ||
		(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) ||
		hasTRNS) {
		::png_set_expand(pPngStruct);
	}

	// Gray + Alpha、あるいは Gray + tRNS の時は RGBA に変換
	if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA ||
		(color_type == PNG_COLOR_TYPE_GRAY && hasTRNS)) {
		::png_set_gray_to_rgb(pPngStruct);
	}

	// RGB の並びを BGR に変更
	if (color_type != PNG_COLOR_TYPE_GRAY) {
		::png_set_bgr(pPngStruct);
	}

	// 24bit なら後ろ α=255 を追加して 32bit に変換
	if (hasTRNS &&
		(color_type == PNG_COLOR_TYPE_PALETTE || color_type == PNG_COLOR_TYPE_RGB)) {
		::png_set_filler(pPngStruct, 255, PNG_FILLER_AFTER);
	}

	// PNG ファイルのガンマ値を取得。なければ何もしない
	const double gammaLUT = 1.0f;
	const double gammaCRT = 2.2f;
	double display_exponent = gammaLUT * gammaCRT;
	double gammaFile = 0.0f;
	if (::png_get_gAMA(pPngStruct, pPngInfo, &gammaFile)) {
		::png_set_gamma(pPngStruct, display_exponent, gammaFile);
	}

	// 変換の設定を pPngInfo に反映
	::png_read_update_info(pPngStruct, pPngInfo);


	// 生成する画像の種類の判別
	ElementType typeElement = ELEMENT_UNKNOWN;
	ChannelType typeChannel = CHANNEL_UNKNOWN;

	if (bit_depth == 16) {
		::png_set_swap(pPngStruct);
		typeElement = ELEMENT_16;
	} else {
		typeElement = ELEMENT_8;
	}

	if (color_type == PNG_COLOR_TYPE_GRAY) {
		typeChannel = hasTRNS ? CHANNEL_RGBA : CHANNEL_GRAY;
	} else if (color_type == PNG_COLOR_TYPE_PALETTE || color_type == PNG_COLOR_TYPE_RGB) {
		typeChannel = hasTRNS ? CHANNEL_RGBA : CHANNEL_RGB;
	} else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		typeChannel = CHANNEL_RGBA;
	}
	pImageBase = ImageFactory::createImage(width, height, typeElement, typeChannel);

	int sizeRowBytes = ::png_get_rowbytes(pPngStruct, pPngInfo);
	int sizeRowBytesImageBase = pImageBase->getNumBytesRow();

	// 各ラインの先頭アドレスをテーブルに入れておく
	ppRowTable = new png_byte*[height];
	for (unsigned int y = 0; y < height; y ++) {
		ppRowTable[y] = reinterpret_cast<png_byte*>(pImageBase->getRawAddress(0, y));
	}

	// PNG 画像読み込み
	::png_read_image(pPngStruct, ppRowTable);

	::png_read_end(pPngStruct, NULL);

	if (ppRowTable) delete[] ppRowTable;
	::png_destroy_read_struct(&pPngStruct, &pPngInfo, NULL);

	*ppImage = pImageBase;

	return true;
}

bool loadPng(const char* pNameFile, ImageBase** ppImage)
{
	if (!(pNameFile && ppImage && !*ppImage)) return false;

	FILE* pFile = ::fopen(pNameFile, "rb");
	if (!pFile) return false;

	bool result = loadPng(pFile, ppImage);
	::fclose(pFile);

	return result;
}

bool loadPng(const wchar_t* pNameFile, ImageBase** ppImage)
{
	if (!(pNameFile && ppImage && !*ppImage)) return false;

	FILE* pFile = ::_wfopen(pNameFile, L"rb");
	if (!pFile) return false;

	bool result = loadPng(pFile, ppImage);
	::fclose(pFile);

	return result;
}

bool savePng(FILE* pFile, const ImageBase* pImage, bool isInterlaced)
{
	if (!(pFile && pImage)) return false;
	if (ImageUtil::isFloat(pImage)) return false;

	png_struct* pPngStruct = ::png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pPngStruct) return false;

	png_info* pPngInfo = ::png_create_info_struct(pPngStruct);
	if (!pPngInfo) {
		::png_destroy_write_struct(&pPngStruct, NULL);
		return false;
	}

	png_byte** ppRowTable = 0;
	if (setjmp(pPngStruct->jmpbuf)) {
		if (ppRowTable) delete[] ppRowTable;
		::png_destroy_write_struct(&pPngStruct, &pPngInfo);
		return false;
	}

	::png_init_io(pPngStruct, pFile);
	::png_set_compression_level(pPngStruct, Z_BEST_COMPRESSION);


	int bit_depth = ImageUtil::is16(pImage) ? 16 : 8;
	int color_type = 0;
	if (ImageUtil::isGray(pImage)) {
		color_type = PNG_COLOR_TYPE_GRAY;
	} else if (ImageUtil::isRGB(pImage)) {
		color_type = PNG_COLOR_TYPE_RGB;
	} else if (ImageUtil::isRGBA(pImage)) {
		color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	}
	::png_set_IHDR(pPngStruct, pPngInfo,
			pImage->getWidth(), pImage->getHeight(),
			bit_depth, color_type,
			isInterlaced ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	double CRT_exponent = 2.2;
	double LUT_exponent = 1.0;
	double default_display_exponent = LUT_exponent * CRT_exponent;
	double default_gamma = 1.0 / default_display_exponent;
	::png_set_gAMA(pPngStruct, pPngInfo, default_gamma);

	::png_write_info(pPngStruct, pPngInfo);
	::png_set_bgr(pPngStruct);
	if (bit_depth == 16) {
		::png_set_swap(pPngStruct);
	}

	if (isInterlaced) {
		// インターレース形式のときは全画面を一度に書く
		// 各ラインの先頭アドレスをテーブルに入れておく
		ppRowTable = new png_byte*[pImage->getHeight()];
		for (unsigned int y = 0; y < pImage->getHeight(); y ++) {
			ppRowTable[y] = const_cast<png_byte*>(reinterpret_cast<const png_byte*>(pImage->getRawAddress(0, y)));
		}
		// 1 画面分全部が wpng_info.image_data にあるものとする
		::png_write_image(pPngStruct, ppRowTable);
	} else {
		// プログレッシブ形式のときは 1 ラインずつ書く
		for (unsigned int y = 0; y < pImage->getHeight(); y ++) {
			::png_write_row(pPngStruct,
						const_cast<png_byte*>(reinterpret_cast<const png_byte*>(pImage->getRawAddress(0, y))));
		}
	}
	::png_write_end(pPngStruct, NULL);

	if (ppRowTable) delete[] ppRowTable;

	::png_destroy_write_struct(&pPngStruct, &pPngInfo);

	return true;
}

bool savePng(const char* pNameFile, const ImageBase* pImage, bool isInterlaced)
{
	if (!(pNameFile && pImage)) return false;

	FILE* pFile = ::fopen(pNameFile, "wb");
	if (!pFile) return false;

	bool result = savePng(pFile, pImage, isInterlaced);
	::fclose(pFile);

	return result;
}

bool savePng(const wchar_t* pNameFile, const ImageBase* pImage, bool isInterlaced)
{
	if (!(pNameFile && pImage)) return false;

	FILE* pFile = ::_wfopen(pNameFile, L"wb");
	if (!pFile) return false;

	bool result = savePng(pFile, pImage, isInterlaced);
	::fclose(pFile);

	return result;
}