#ifndef ImageTraitsAllocatedByIPPH
#define ImageTraitsAllocatedByIPPH

#include<ippi.h>

// C++ Template と IPP の ippiMalloc() との対応付け
// #define を使うと短く書けるかも?
// IPP がサポートする他の TypePixel に対応するなら追加せなあかん。
// 要検討:
// - ippiMalloc() の返す stepBytes を無視している
//   (row 単位で 32bit に aligned されている、とマニュアルに明記されいるため)
template<typename TypePixel>
inline unsigned char* allocImageByIPP(unsigned int width, unsigned int height);

template<>
inline unsigned char* allocImageByIPP<Pixel<unsigned char, 1> >(unsigned int width, unsigned int height)
{
	int numBytesRow = 0;
	return reinterpret_cast<unsigned char*>(::ippiMalloc_8u_C1((int)width, (int)height, &numBytesRow));
}

template<>
inline unsigned char* allocImageByIPP<Pixel<unsigned char, 3> >(unsigned int width, unsigned int height)
{
	int numBytesRow = 0;
	return reinterpret_cast<unsigned char*>(::ippiMalloc_8u_C3((int)width, (int)height, &numBytesRow));
}

template<>
inline unsigned char* allocImageByIPP<Pixel<unsigned char, 4> >(unsigned int width, unsigned int height)
{
	int numBytesRow = 0;
	return reinterpret_cast<unsigned char*>(::ippiMalloc_8u_C4((int)width, (int)height, &numBytesRow));
}

template<>
inline unsigned char* allocImageByIPP<Pixel<unsigned short, 1> >(unsigned int width, unsigned int height)
{
	int numBytesRow = 0;
	return reinterpret_cast<unsigned char*>(::ippiMalloc_16u_C1((int)width, (int)height, &numBytesRow));
}

template<>
inline unsigned char* allocImageByIPP<Pixel<unsigned short, 3> >(unsigned int width, unsigned int height)
{
	int numBytesRow = 0;
	return reinterpret_cast<unsigned char*>(::ippiMalloc_16u_C3((int)width, (int)height, &numBytesRow));
}

template<>
inline unsigned char* allocImageByIPP<Pixel<unsigned short, 4> >(unsigned int width, unsigned int height)
{
	int numBytesRow = 0;
	return reinterpret_cast<unsigned char*>(::ippiMalloc_16u_C4((int)width, (int)height, &numBytesRow));
}

template<>
inline unsigned char* allocImageByIPP<Pixel<float, 1> >(unsigned int width, unsigned int height)
{
	int numBytesRow = 0;
	return reinterpret_cast<unsigned char*>(::ippiMalloc_32f_C1((int)width, (int)height, &numBytesRow));
}

template<>
inline unsigned char* allocImageByIPP<Pixel<float, 3> >(unsigned int width, unsigned int height)
{
	int numBytesRow = 0;
	return reinterpret_cast<unsigned char*>(::ippiMalloc_32f_C3((int)width, (int)height, &numBytesRow));
}

template<>
inline unsigned char* allocImageByIPP<Pixel<float, 4> >(unsigned int width, unsigned int height)
{
	int numBytesRow = 0;
	return reinterpret_cast<unsigned char*>(::ippiMalloc_32f_C4((int)width, (int)height, &numBytesRow));
}

// 画像領域の確保のポリシー(IPP で確保)
template<typename TypePixel>
struct ImageTraitsAllocatedByIPP
{
public:
	static unsigned char* allocImage(unsigned int width, unsigned int height)
	{
		return allocImageByIPP<TypePixel>(width, height);
	}
	static void freeImage(unsigned char* pImage)
	{
		::ippiFree(pImage);
	}
};

#endif

