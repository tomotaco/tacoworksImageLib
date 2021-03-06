#ifndef PixelH
#define PixelH

#include <assert.h>
#include "ElementUtil.h"
#include "PixelTraitsElementMap.h"

#pragma pack(push, 1)

template<typename T, unsigned int N,
	class TypeTraitsElementMap = PixelTraitsElementMapDefault<T, N> >
class Pixel : public TypeTraitsElementMap
{
public:
	typedef T TypeElement;
	enum {
		numElements = N
	};

	Pixel() { clear(); }
	Pixel(T valA);
	Pixel(T valR, T valG, T valB);
	Pixel(T valR, T valG, T valB, T valA);
	Pixel(const Pixel<T, N, TypeTraitsElementMap>& rX);

	const Pixel<T, N, TypeTraitsElementMap>& operator=(const Pixel<T, N, TypeTraitsElementMap>& rX);

	T getB() const;
	T getG() const;
	T getR() const;
	T getA() const;
	T getElement(unsigned int indexElement) const;

	T getMinValueOfElements() const;
	T getMaxValueOfElements() const;

	void clear();

	void setB(T val);
	void setG(T val);
	void setR(T val);
	void setA(T val);
	void setElement(unsigned int indexElement, T val);

	static ElementType getTypeElement() { return ElementUtil::getType<T>(); }
	static unsigned int getNumElements() { return N; }
	static unsigned int getNumBytesPerElement() { return sizeof(T); }
	static unsigned int getNumBytes() { return N * sizeof(T); }
	static T getMinValueOfType() { return ElementUtil::getMinValue<T>(); }
	static T getMaxValueOfType() { return ElementUtil::getMaxValue<T>(); }

private:
	T m_element[N];
};

#pragma pack(pop)

// RGBA と m_elements[] との対応はエンディアン等によって変わる

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline Pixel<T, N, TypeTraitsElementMap>::Pixel(T valA)
{
	TypeTraitsElementMap::setElementsFromA(valA, m_element);
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline Pixel<T, N, TypeTraitsElementMap>::Pixel(T valR, T valG, T valB)
{
    TypeTraitsElementMap::setElementsFromR(valR, m_element);
	TypeTraitsElementMap::setElementsFromG(valG, m_element);
	TypeTraitsElementMap::setElementsFromB(valB, m_element);
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline Pixel<T, N, TypeTraitsElementMap>::Pixel(T valR, T valG, T valB, T valA)
{
	TypeTraitsElementMap::setElementsFromR(valR, m_element);
	TypeTraitsElementMap::setElementsFromG(valG, m_element);
	TypeTraitsElementMap::setElementsFromB(valB, m_element);
	TypeTraitsElementMap::setElementsFromA(valA, m_element);
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline Pixel<T, N, TypeTraitsElementMap>::Pixel(const Pixel<T, N, TypeTraitsElementMap>& rX)
{
	for (unsigned int i = 0; i < N; i ++) m_element[i] = rX.m_element[i];
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline const Pixel<T, N, TypeTraitsElementMap>& Pixel<T, N, TypeTraitsElementMap>::operator=(const Pixel<T, N, TypeTraitsElementMap>& rX)
{
	for (unsigned int i = 0; i < N; i ++) m_element[i] = rX.m_element[i];
	return *this;
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline T Pixel<T, N, TypeTraitsElementMap>::getB() const
{
	return TypeTraitsElementMap::getElementBFromElements(m_element);
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline T Pixel<T, N, TypeTraitsElementMap>::getG() const
{
	return TypeTraitsElementMap::getElementGFromElements(m_element);
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline T Pixel<T, N, TypeTraitsElementMap>::getR() const
{
	return TypeTraitsElementMap::getElementRFromElements(m_element);
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline T Pixel<T, N, TypeTraitsElementMap>::getA() const
{
	return TypeTraitsElementMap::getElementAFromElements(m_element);
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline T Pixel<T, N, TypeTraitsElementMap>::getElement(unsigned int indexElement) const
{
	assert(indexElement < numElements);
	if (!(indexElement < numElements)) return T();

	return m_element[indexElement];
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline T Pixel<T, N, TypeTraitsElementMap>::getMinValueOfElements() const
{
	T valueMin = m_element[0];
	for (unsigned int index = 1; index < numElements; index ++) {
		if (m_element[index] < valueMin) valueMin = m_element[index];
	}

	return valueMin;
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline T Pixel<T, N, TypeTraitsElementMap>::getMaxValueOfElements() const
{
	T valueMax = m_element[0];
	for (unsigned int index = 1; index < numElements; index ++) {
		if (m_element[index] < valueMax) valueMax = m_element[index];
	}

	return valueMax;
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline void Pixel<T, N, TypeTraitsElementMap>::clear()
{
	for (unsigned int i = 0; i < N; i ++) {
		m_element[i] = ElementUtil::getMinValue<T>();
	}
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline void Pixel<T, N, TypeTraitsElementMap>::setB(T val)
{
	TypeTraitsElementMap::setElementsFromB(val, m_element);
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline void Pixel<T, N, TypeTraitsElementMap>::setG(T val)
{
	TypeTraitsElementMap::setElementsFromG(val, m_element);
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline void Pixel<T, N, TypeTraitsElementMap>::setR(T val)
{
	TypeTraitsElementMap::setElementsFromR(val, m_element);
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline void Pixel<T, N, TypeTraitsElementMap>::setA(T val)
{
	TypeTraitsElementMap::setElementsFromA(val, m_element);
}

template<typename T, unsigned int N, typename TypeTraitsElementMap>
inline void Pixel<T, N, TypeTraitsElementMap>::setElement(unsigned int indexElement, T val)
{
	assert(indexElement < numElements);
	if (!(indexElement < numElements)) return;

	m_element[indexElement] = val;
}

typedef Pixel<unsigned char, 4> PixelRGBA8;
typedef Pixel<unsigned char, 3> PixelRGB8;
typedef Pixel<unsigned char, 1> PixelGray8;
typedef Pixel<unsigned short, 4> PixelRGBA16;
typedef Pixel<unsigned short, 3> PixelRGB16;
typedef Pixel<unsigned short, 1> PixelGray16;

typedef Pixel<int, 4> PixelRGBASInt;
typedef Pixel<int, 3> PixelRGBSInt;
typedef Pixel<int, 1> PixelGraySInt;
typedef Pixel<unsigned int, 4> PixelRGBAUInt;
typedef Pixel<unsigned int, 3> PixelRGBUInt;
typedef Pixel<unsigned int, 1> PixelGrayUInt;
typedef Pixel<float, 4> PixelRGBAFloat;
typedef Pixel<float, 3> PixelRGBFloat;
typedef Pixel<float, 1> PixelGrayFloat;
typedef Pixel<double, 4> PixelRGBADouble;
typedef Pixel<double, 3> PixelRGBDouble;
typedef Pixel<double, 1> PixelGrayDouble;

#endif
