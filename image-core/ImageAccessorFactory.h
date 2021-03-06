#ifndef ImageAccessorFactoryH
#define ImageAccessorFactoryH

#if defined(_WIN32)
#define USE_IMAGE_DIB // Windows DIB によりメモリ確保された画像を扱う場合
#if !defined(__BORLANDC__)
//#define USE_IMAGE_IPP // Intel IPP によりメモリ管理された画像を扱う場合
#endif
#endif

#include "Pixel.h"
#include "Image.h"
#include "ImageExternal.h"

#include "TypeList.h"
#include "StaticSingleDispatcher.h"

#ifdef USE_IMAGE_DIB
#include "ImageDIB.h"
#endif // USE_IMAGE_DIB

#ifdef USE_IMAGE_IPP
#include "ImageIPP.h"
#endif // USE_IMAGE_IPP

#include "ImageAccessor.h"

namespace {


// ImageAccessor の生成
template<class TPixel>
struct ImageAccessorCreator
{
	ImageAccessorCreator()
		: m_pAccessorIF(NULL) {}

	template<class TypeImage>
	void operator()(TypeImage* pImage)
	{
		if (!pImage) {
			m_pAccessorIF = NULL;
			return;
        }
		m_pAccessorIF = static_cast<ImageAccessorIF<TPixel>*>(new ImageAccessor<TypeImage, TPixel>(pImage));
	}

	void operator()(ImageBase* pImage)
	{
		m_pAccessorIF = NULL;
	}

	ImageAccessorIF<TPixel>* m_pAccessorIF;
};

// ImageAccessorReadOnly の生成
template<class TPixel>
struct ImageAccessorReadOnlyCreator
{
	ImageAccessorReadOnlyCreator()
		: m_pAccessorIF(NULL) {}

	template<class TypeImage>
	void operator()(const TypeImage* pImage)
	{
		if (!pImage) {
			m_pAccessorIF = NULL;
			return;
        }
		m_pAccessorIF = static_cast<const ImageAccessorIF<TPixel>*>(new ImageAccessorReadOnly<TypeImage, TPixel>(pImage));
	}

	void operator()(const ImageBase* pImage)
	{
		m_pAccessorIF = NULL;
	}

	const ImageAccessorIF<TPixel>* m_pAccessorIF;
};

};

namespace ImageAccessorFactory
{
	template<class TPixel>
	ImageAccessorIF<TPixel>* createImageAccessor(ImageBase* pImage)
	{
		if (!pImage) return NULL;

		typedef ImageAccessorCreator<TPixel> Creator;
		Creator creator;

		StaticSingleDispatcher<ImageBase, TypeListImages, Creator>::dispatch(pImage, creator);
		if (creator.m_pAccessorIF) return creator.m_pAccessorIF;

#ifdef USE_IMAGE_DIB
		StaticSingleDispatcher<ImageBase, TypeListImagesDIB, Creator>::dispatch(pImage, creator);
		if (creator.m_pAccessorIF) return creator.m_pAccessorIF;

#endif // USE_IMAGE_DIB

#ifdef USE_IMAGE_IPP
		StaticSingleDispatcher<ImageBase, TypeListImagesIPP, Creator>::dispatch(pImage, creator);
		if (creator.m_pAccessorIF) return creator.m_pAccessorIF;

#endif // USE_IMAGE_IPP

		// ImageExternal は ReadOnly なので ImageAccessorReadOnly のみ
		return NULL;
	}

	template<class TPixel>
	const ImageAccessorIF<TPixel>* createImageAccessor(const ImageBase* pImage)
	{
		if (!pImage) return NULL;

		typedef ImageAccessorReadOnlyCreator<TPixel> Creator;
		Creator creator;

		StaticSingleDispatcher<ImageBase, TypeListImages, Creator>::dispatch(pImage, creator);
		if (creator.m_pAccessorIF) return creator.m_pAccessorIF;

		StaticSingleDispatcher<ImageBase, TypeListImagesExternal, Creator>::dispatch(pImage, creator);
		if (creator.m_pAccessorIF) return creator.m_pAccessorIF;

#ifdef USE_IMAGE_DIB
		StaticSingleDispatcher<ImageBase, TypeListImagesDIB, Creator>::dispatch(pImage, creator);
		if (creator.m_pAccessorIF) return creator.m_pAccessorIF;
#endif // USE_IMAGE_DIB

#ifdef USE_IMAGE_IPP
		StaticSingleDispatcher<ImageBase, TypeListImagesIPP, Creator>::dispatch(pImage, creator);
		if (creator.m_pAccessorIF) return creator.m_pAccessorIF;
#endif // USE_IMAGE_IPP

/*
#ifdef USE_IMAGE_OSX
		StaticSingleDispatcher<ImageBase, TypeListImagesOSX, Creator>::dispatch(pImage, creator);
		if (creator.m_pAccessorIF) return creator.m_pAccessorIF;
#endif // USE_IMAGE_OSX
*/
		return NULL;
	}
};



#endif
