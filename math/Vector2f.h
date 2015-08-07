#ifndef __VECTOR_2F_H_INCLUDED
#define __VECTOR_2F_H_INCLUDED

#include <cmath>
#include "MathUtil.h"
#include "VectorBase.h"

template <typename T>
class Vector2 : public VectorBase<T, 2>
{
public:
	Vector2(T x = T(), T y = T()) { set(x, y); }
	Vector2(const VectorBase<T, 2>& rX) : VectorBase<T, 2>(rX) {}

	T getX() const { return VectorBase<T, 2>::getElement(0); }
	T getY() const { return VectorBase<T, 2>::getElement(1); }
	void setX(T x) { VectorBase<T, 2>::setElement(0, x); }
	void setY(T y) { VectorBase<T, 2>::setElement(1, y); }
	void set(T x, T y) { setX(x); setY(y); }

	Vector2<T> getPerpendicular() const { return Vector2<T>(getY(), -getX()); }
	T getAngle() const;

	static T crossProduct(const Vector2<T>& rV1, const Vector2<T>& rV2)
	{
		return rV1.getX() * rV2.getY() - rV1.getY() * rV2.getX();
	}
};

template <typename T>
inline T Vector2<T>::getAngle() const
{
	T radian = std::atan2(getY(), getX());
	return radian * 180.0f / g_PI;
}

typedef Vector2<float> Vector2f;

#endif // __VECTOR_2F_H_INCLUDED
