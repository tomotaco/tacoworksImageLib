#ifndef PixelTrailtsElementMapH
#define PixelTrailtsElementMapH

template<typename T, unsigned int N>
struct PixelTraitsElementMapBGRA
{
	static T getElementRFromElements(const T* pElements);
	static T getElementGFromElements(const T* pElements);
	static T getElementBFromElements(const T* pElements);
	static T getElementAFromElements(const T* pElements);

	static void setElementsFromR(const T val, T* pElements);
	static void setElementsFromG(const T val, T* pElements);
	static void setElementsFromB(const T val, T* pElements);
	static void setElementsFromA(const T val, T* pElements);
};

template<typename T>
struct PixelTraitsElementMapBGRA<T, 1>
{
	static T getElementRFromElements(const T*) { assert(0); return T(); }
	static T getElementGFromElements(const T*) { assert(0); return T(); }
	static T getElementBFromElements(const T*) { assert(0); return T(); }
	static T getElementAFromElements(const T* pElements) { return pElements[0]; }

	static void setElementsFromR(const T, T*) { assert(0); }
	static void setElementsFromG(const T, T*) { assert(0); }
	static void setElementsFromB(const T, T*) { assert(0); }
	static void setElementsFromA(const T val, T* pElements) { pElements[0] = val; }
};


template<typename T>
struct PixelTraitsElementMapBGRA<T, 3>
{
	static T getElementRFromElements(const T* pElements) { return pElements[2]; }
	static T getElementGFromElements(const T* pElements) { return pElements[1]; }
	static T getElementBFromElements(const T* pElements) { return pElements[0]; }
	static T getElementAFromElements(const T*) { assert(0); return T(); }

	static void setElementsFromR(const T val, T* pElements) { pElements[2] = val; }
	static void setElementsFromG(const T val, T* pElements) { pElements[1] = val; }
	static void setElementsFromB(const T val, T* pElements) { pElements[0] = val; }
	static void setElementsFromA(const T, T*) { assert(0); }
};

template<typename T>
struct PixelTraitsElementMapBGRA<T, 4>
{
	static T getElementRFromElements(const T* pElements) { return pElements[2]; }
	static T getElementGFromElements(const T* pElements) { return pElements[1]; }
	static T getElementBFromElements(const T* pElements) { return pElements[0]; }
	static T getElementAFromElements(const T* pElements) { return pElements[3]; }

	static void setElementsFromR(const T val, T* pElements) { pElements[2] = val; }
	static void setElementsFromG(const T val, T* pElements) { pElements[1] = val; }
	static void setElementsFromB(const T val, T* pElements) { pElements[0] = val; }
	static void setElementsFromA(const T val, T* pElements) { pElements[3] = val; }
};

template<typename T, unsigned int N>
struct PixelTraitsElementMapRGBA
{
	static T getElementRFromElements(const T* pElements);
	static T getElementGFromElements(const T* pElements);
	static T getElementBFromElements(const T* pElements);
	static T getElementAFromElements(const T* pElements);

	static void setElementsFromR(const T val, T* pElements);
	static void setElementsFromG(const T val, T* pElements);
	static void setElementsFromB(const T val, T* pElements);
	static void setElementsFromA(const T val, T* pElements);
};

template<typename T>
struct PixelTraitsElementMapRGBA<T, 1>
{
	static T getElementRFromElements(const T*) { assert(0); return T(); }
	static T getElementGFromElements(const T*) { assert(0); return T(); }
	static T getElementBFromElements(const T*) { assert(0); return T(); }
	static T getElementAFromElements(const T* pElements) { return pElements[0]; }

	static void setElementsFromR(const T, T*) { assert(0); }
	static void setElementsFromG(const T, T*) { assert(0); }
	static void setElementsFromB(const T, T*) { assert(0); }
	static void setElementsFromA(const T val, T* pElements) { pElements[0] = val; }
};


template<typename T>
struct PixelTraitsElementMapRGBA<T, 3>
{
	static T getElementRFromElements(const T* pElements) { return pElements[0]; }
	static T getElementGFromElements(const T* pElements) { return pElements[1]; }
	static T getElementBFromElements(const T* pElements) { return pElements[2]; }
	static T getElementAFromElements(const T*) { assert(0); return T(); }

	static void setElementsFromR(const T val, T* pElements) { pElements[0] = val; }
	static void setElementsFromG(const T val, T* pElements) { pElements[1] = val; }
	static void setElementsFromB(const T val, T* pElements) { pElements[2] = val; }
	static void setElementsFromA(const T, T*) { assert(0); }
};

template<typename T>
struct PixelTraitsElementMapRGBA<T, 4>
{
	static T getElementRFromElements(const T* pElements) { return pElements[0]; }
	static T getElementGFromElements(const T* pElements) { return pElements[1]; }
	static T getElementBFromElements(const T* pElements) { return pElements[2]; }
	static T getElementAFromElements(const T* pElements) { return pElements[3]; }

	static void setElementsFromR(const T val, T* pElements) { pElements[0] = val; }
	static void setElementsFromG(const T val, T* pElements) { pElements[1] = val; }
	static void setElementsFromB(const T val, T* pElements) { pElements[2] = val; }
	static void setElementsFromA(const T val, T* pElements) { pElements[3] = val; }
};

// typedef template cannot be used on C++03

#ifdef _WIN32
#define PixelTraitsElementMapDefault PixelTraitsElementMapBGRA
#endif

#ifdef __APPLE__
// Confirmed on iOS5.1 Simulator
#define PixelTraitsElementMapDefault PixelTraitsElementMapRGBA
#endif

#endif
