#ifndef __VECTOR_4F_H_INCLUDED
#define __VECTOR_4F_H_INCLUDED

#include <cmath>
#include "MathUtil.h"
#include "VectorBase.h"
#include "Vector3f.h"

template <typename T>
class Vector4 : public VectorBase<T, 4>
{
public:
	Vector4(T x = T(), T y = T(), T z = T(), T w = T()) { set(x, y, z, w); }
	Vector4(const VectorBase<T, 4>& rX) : VectorBase<T, 4>(rX) {}
	Vector4(const Vector3f& rV, T w = 1.0) { setX(rV.getX()); setY(rV.getY()); setZ(rV.getZ()); setW(w); }

	T getX() const { return VectorBase<T, 4>::getElement(0); }
	T getY() const { return VectorBase<T, 4>::getElement(1); }
	T getZ() const { return VectorBase<T, 4>::getElement(2); }
	T getW() const { return VectorBase<T, 4>::getElement(3); }
	void setX(T x) { VectorBase<T, 4>::setElement(0, x); }
	void setY(T y) { VectorBase<T, 4>::setElement(1, y); }
	void setZ(T z) { VectorBase<T, 4>::setElement(2, z); }
	void setW(T w) { VectorBase<T, 4>::setElement(3, w); }
	void set(T x, T y, T z, T w) { setX(x); setY(y); setZ(z); setW(w); }

	Vector3<T> getXYZNormalized() const;
};

template <typename T>
inline Vector3<T> Vector4<T>::getXYZNormalized() const
{
	T w = getW();
	return Vector3<T>(getX() / w, getY() / w, getZ() / w);
}

typedef Vector4<float> Vector4f;

#endif // __VECTOR_4F_H_INCLUDED
