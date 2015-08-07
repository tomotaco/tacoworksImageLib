#ifndef __VECTOR_BASE_H_INCLUDED
#define __VECTOR_BASE_H_INCLUDED

#include <cassert>
#include <cmath>
#include <iostream>
#include "MathUtil.h"

template<typename T, unsigned int N>
class VectorBase
{
public:
	VectorBase() { clear(); }
	VectorBase(const VectorBase<T, N>& rX);

	void clear();

	T getElement(unsigned int indexElement) const;
	void setElement(unsigned int indexElement, T val);

	const T* getData() const { return m_elements; }
	T* getData() { return m_elements; }

	T getLengthSquared() const;
	T getLength() const { return  std::sqrt(getLengthSquared()); }

	const VectorBase<T, N>& normalize();
	const VectorBase<T, N>& operator=(const VectorBase<T, N>& rX);
	const VectorBase<T, N>& operator+=(const VectorBase<T, N>& rX);
	const VectorBase<T, N>& operator-=(const VectorBase<T, N>& rX);
	const VectorBase<T, N>& operator*=(T val);
	const VectorBase<T, N>& operator/=(T val);
	VectorBase<T, N> operator-() const;

	bool operator==(const VectorBase<T, N>& rX) const;
	bool operator!=(const VectorBase<T, N>& rX) const { return !(*this == rX); }
	bool operator<(const VectorBase<T, N>& rX) const; // for sorting in std::set, std::map.

	static T dotProduct(const VectorBase<T, N>& rV1, const VectorBase<T, N>& rV2);

private:
	T m_elements[N];

};

template<typename T, unsigned int N>
inline VectorBase<T, N>::VectorBase(const VectorBase& rX)
{
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		m_elements[indexElement] = rX.m_elements[indexElement];
	}
}

template<typename T, unsigned int N>
inline void VectorBase<T, N>::clear()
{
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		m_elements[indexElement] = T();
	}
}

template<typename T, unsigned int N>
inline T VectorBase<T, N>::getElement(unsigned int indexElement) const
{
	assert(indexElement < N);
	if (!(indexElement < N)) return T();

	return m_elements[indexElement];
}

template<typename T, unsigned int N>
inline void VectorBase<T, N>::setElement(unsigned int indexElement, T value)
{
	assert(indexElement < N);
	if (!(indexElement < N)) return;
	m_elements[indexElement] = value;
}


template<typename T, unsigned int N>
inline T VectorBase<T, N>::getLengthSquared() const
{
	T sum = T();
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		T elem = m_elements[indexElement];
		sum += elem * elem;
	}
	
	return sum;
}

template<typename T, unsigned int N>
inline const VectorBase<T, N>& VectorBase<T, N>::normalize()
{
	T length = getLength();
	assert(!isZero(length));
	if (isZero(length)) { clear(); return *this; }
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		m_elements[indexElement] /= length;
	}
	return *this;
}

template<typename T, unsigned int N>
const VectorBase<T, N>& VectorBase<T, N>::operator=(const VectorBase<T, N>& rX)
{
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		m_elements[indexElement] = rX.m_elements[indexElement];
	}
	return *this;
}

template<typename T, unsigned int N>
const VectorBase<T, N>& VectorBase<T, N>::operator+=(const VectorBase<T, N>& rX)
{
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		m_elements[indexElement] += rX.m_elements[indexElement];
	}
	return *this;
}

template<typename T, unsigned int N>
const VectorBase<T, N>& VectorBase<T, N>::operator-=(const VectorBase<T, N>& rX)
{
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		m_elements[indexElement] -= rX.m_elements[indexElement];
	}
	return *this;
}

template<typename T, unsigned int N>
const VectorBase<T, N>& VectorBase<T, N>::operator*=(T val)
{
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		m_elements[indexElement] *= val;
	}
	return *this;
}

template<typename T, unsigned int N>
const VectorBase<T, N>& VectorBase<T, N>::operator/=(T val)
{
	assert(!isZero(val));
	if (isZero(val)) { clear(); return *this; }
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		m_elements[indexElement] /= val;
	}
	return *this;
}

template<typename T, unsigned int N>
inline VectorBase<T, N> VectorBase<T, N>::operator-() const
{
	VectorBase<T, N> ret(*this);
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		ret.setElement(indexElement, - m_elements[indexElement]);
	}	
  return ret;
}

template<typename T, unsigned int N>
inline bool VectorBase<T, N>::operator==(const VectorBase<T, N>& rX) const
{
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		if (!isEqual(m_elements[indexElement], rX.m_elements[indexElement])) return false;
	}
	return true;
}

template<typename T, unsigned int N>
inline bool VectorBase<T, N>::operator<(const VectorBase<T, N>& rX) const
{
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		if (!isEqual(m_elements[indexElement], rX.m_elements[indexElement])) return (m_elements[indexElement] < rX.m_elements[indexElement]) ? true : false;
	}
	return false;
}


template<typename T, unsigned int N>
inline T VectorBase<T, N>::dotProduct(const VectorBase<T, N>& rV1, const VectorBase<T, N>& rV2)
{
	T sum;
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		sum += rV1.getElement(indexElement) * rV2.getElement(indexElement);
	}
	return sum;
}

template<typename T, unsigned int N>
inline VectorBase<T, N> operator+(const VectorBase<T, N>& rV1, const VectorBase<T, N>& rV2)
{
	VectorBase<T, N> v = rV1;
	v += rV2;

	return v;
}

template<typename T, unsigned int N>
inline VectorBase<T, N> operator-(const VectorBase<T, N>& rV1, const VectorBase<T, N>& rV2)
{
	VectorBase<T, N> v = rV1;
	v -= rV2;

	return v;
}

template<typename T, unsigned int N>
inline VectorBase<T, N> operator*(const VectorBase<T, N>& rV, float val)
{
	VectorBase<T, N> v = rV;
	v *= val;

	return v;
}

template<typename T, unsigned int N>
inline VectorBase<T, N> operator/(const VectorBase<T, N>& rV, float val)
{
	VectorBase<T, N> v = rV;
	v /= val;

	return v;
}

template<typename T, unsigned int N>
inline VectorBase<T, N> operator*(float val, const VectorBase<T, N>& rV)
{
	VectorBase<T, N> v = rV;
	v *= val;

	return v;
}

template<typename T, unsigned int N>
inline VectorBase<T, N> operator/(float val, const VectorBase<T, N>& rV)
{
	VectorBase<T, N> v = rV;
	v /= val;

	return v;
}

template<typename T, unsigned int N>
inline std::ostream& operator<<(std::ostream& rOst, const VectorBase<T, N>& rX)
{
	rOst << "( ";
	for (unsigned int indexElement = 0; indexElement < N; indexElement ++) {
		rOst << rX.getElement(indexElement) << " ";
	}
	rOst << ") ";

	return rOst;
}



#endif // __VECTOR_BASE_H_INCLUDED
