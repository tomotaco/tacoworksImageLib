#ifndef __VECTOR_3F_H_INCLUDED
#define __VECTOR_3F_H_INCLUDED

#include <cmath>
#include "MathUtil.h"
#include "VectorBase.h"
#include "Vector2f.h"

template <typename T>
class Vector3 : public VectorBase<T, 3>
{
public:
	Vector3(T x = T(), T y = T(), T z = T()) { set(x, y, z); }
	Vector3(const VectorBase<T, 3>& rX) : VectorBase<T, 3>(rX) {}

	T getX() const { return VectorBase<T, 3>::getElement(0); }
	T getY() const { return VectorBase<T, 3>::getElement(1); }
	T getZ() const { return VectorBase<T, 3>::getElement(2); }
	Vector2<T> getXY() const { return Vector2<T>(getX(), getY()); }
	Vector2<T> getYZ() const { return Vector2<T>(getY(), getZ()); }
	Vector2<T> getZX() const { return Vector2<T>(getZ(), getX()); }
	Vector2<T> getYX() const { return Vector2<T>(getY(), getX()); }
	Vector2<T> getZY() const { return Vector2<T>(getZ(), getY()); }
	Vector2<T> getXZ() const { return Vector2<T>(getX(), getZ()); }

	void setX(T x) { VectorBase<T, 3>::setElement(0, x); }
	void setY(T y) { VectorBase<T, 3>::setElement(1, y); }
	void setZ(T z) { VectorBase<T, 3>::setElement(2, z); }
	void set(T x, T y, T z) { setX(x); setY(y); setZ(z); }
	void setXY(const Vector2<T>& rV) { setX(rV.getX()); setY(rV.getY()); setZ(0.0f); }
	void setYZ(const Vector2<T>& rV) { setY(rV.getX()); setZ(rV.getY()); setX(0.0f); }
	void setZX(const Vector2<T>& rV) { setZ(rV.getX()); setX(rV.getY()); setY(0.0f); }
	void setYX(const Vector2<T>& rV) { setY(rV.getX()); setX(rV.getY()); setZ(0.0f); }
	void setZY(const Vector2<T>& rV) { setZ(rV.getX()); setY(rV.getY()); setX(0.0f); }
	void setXZ(const Vector2<T>& rV) { setX(rV.getX()); setZ(rV.getY()); setY(0.0f); }

	static Vector3<T> crossProduct(const Vector3<T>& rV1, const Vector3<T>& rV2);
};

template <typename T>
inline Vector3<T> Vector3<T>::crossProduct(const Vector3<T>& rV1, const Vector3<T>& rV2)
{
	return Vector3<T>(rV1.getY() * rV2.getZ() - rV1.getZ() * rV2.getY(),
					rV1.getZ() * rV2.getX() - rV1.getX() * rV2.getZ(),
					rV1.getX() * rV2.getY() - rV1.getY() * rV2.getX());
}

typedef Vector3<float> Vector3f;

#endif // __VECTOR_3F_H_INCLUDED
