#ifndef __MATRIX_3F_H_INCLUDED
#define __MATRIX_3F_H_INCLUDED

#include <cassert>
#include <iostream>
#include <cmath>
#include "MathUtil.h"
#include "Vector2f.h"
#include "Vector3f.h"

class	Matrix3f
{
public:
	enum
	{
		numRows = 3,
		numColumns = 3,
		numElements = numRows * numColumns
	};

	Matrix3f();
	Matrix3f(const float mat[numElements]);
	Matrix3f(const Matrix3f& rX);

	void clear();
	float getElement(int indexRow, int indexColumn) const;
	const float* getArray() const { return m_mat; }
	float* getArray() { return m_mat; }
	void setElement(int indexRow, int indexColumn, float val);
	void setArray(const float mat[numElements]);

//	float getDeterminant() const;
	bool invert();	// 逆行列を求める
	void transpose();	// 転置行列を求める
	void clearTranslate();	// 移動成分をクリアする

	// Fundamental row operation
	void replaceRows(int indexRow0, int indexRow1);
	void scaleRow(int indexRow, float ratio);
	void addScaledRow(int indexRowSrc, float ratioRow, int indexRowDst);

	const Matrix3f& operator=(const Matrix3f& rX);
	const Matrix3f& operator+=(const Matrix3f& rX);
	const Matrix3f& operator-=(const Matrix3f& rX);
	const Matrix3f& operator*=(float val);
	const Matrix3f& operator/=(float val);

	const Matrix3f& operator*=(const Matrix3f& rX);

	static Matrix3f getIdentity();
	static Matrix3f getScale(const Vector2f& rV);
	static Matrix3f getTranslate(const Vector2f& rV);
	static Matrix3f getRotate(float angle);
	static Matrix3f getRotate(float valCos, float valSin);

private:
	float	m_mat[numElements];
};

inline Matrix3f::Matrix3f()
{
	clear();
}

inline Matrix3f::Matrix3f(const Matrix3f& rX)
{
	setArray(rX.m_mat);
}

inline Matrix3f::Matrix3f(const float mat[numElements])
{
	setArray(mat);
}

inline void Matrix3f::clear()
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] = 0.0f;
	}
}

inline float Matrix3f::getElement(int indexRow, int indexColumn) const
{
    return m_mat[indexColumn * numRows + indexRow];
}

inline void Matrix3f::setElement(int indexRow, int indexColumn, float val)
{
	m_mat[indexColumn * numRows + indexRow] = val;
}

inline void Matrix3f::setArray(const float mat[numElements])
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] = mat[i];
	}
}

/*
inline float Matrix3f::getDeterminant() const
{
	// Temporary
	return 0.0f;
}
*/

inline void Matrix3f::transpose()
{
	for (int i = 0; i < numRows; i ++) {
		for (int j = 0; j < numColumns; j ++) {
			if (i >= j) continue;
			float valueSrc = getElement(i, j);
			float valueDst = getElement(j, i);
			setElement(i, j, valueDst);
			setElement(j, i, valueSrc);
		}
	}
}

inline void Matrix3f::clearTranslate()
{
	for (int i = 0; i < numRows; i ++) {
		setElement(i, numRows - 1, 0.0f);
	}
	for (int j = 0; j < numColumns; j ++) {
		setElement(numColumns - 1, j, 0.0f);
	}
	setElement(numRows - 1, numColumns - 1, 1.0f);
}

inline void Matrix3f::replaceRows(int indexRow0, int indexRow1)
{
	if (indexRow0 == indexRow1) return;

	for (int i = 0; i < numColumns; i ++) {
		float value0 = getElement(indexRow0, i);
		float value1 = getElement(indexRow1, i);
		setElement(indexRow0, i, value1);
		setElement(indexRow1, i, value0);
	}
}

inline void Matrix3f::scaleRow(int indexRow, float ratio)
{
	for (int i = 0; i < numColumns; i ++) {
		setElement(indexRow, i, getElement(indexRow, i) * ratio);
	}
}

inline void Matrix3f::addScaledRow(int indexRowSrc, float ratioRow, int indexRowDst)
{
	for (int i = 0; i < numColumns; i ++) {
		float valueSrc = getElement(indexRowSrc, i);
		float valueDst = getElement(indexRowDst, i);
		setElement(indexRowDst, i, valueSrc * ratioRow + valueDst);
	}
}

inline const Matrix3f& Matrix3f::operator=(const Matrix3f& rX)
{
	setArray(rX.m_mat);

	return *this;
}

inline const Matrix3f& Matrix3f::operator+=(const Matrix3f& rX)
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] += rX.m_mat[i];
	}

	return *this;
}

inline const Matrix3f& Matrix3f::operator-=(const Matrix3f& rX)
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] -= rX.m_mat[i];
	}

	return *this;
}

inline const Matrix3f& Matrix3f::operator*=(float val)
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] *= val;
	}

	return *this;
}

inline const Matrix3f& Matrix3f::operator/=(float val)
{
	assert(!isZero(val));
	if (isZero(val)) return *this;

	for (int i = 0; i < numElements; i ++) {
		m_mat[i] /= val;
	}

	return *this;
}

inline const Matrix3f& Matrix3f::operator*=(const Matrix3f& rX)
{
	Matrix3f mat(m_mat);
	for (int i = 0; i < numRows; i ++) {
		for (int j = 0; j < numColumns; j ++) {
			float val = 0.0f;
			for (int k = 0; k < numRows; k ++) {
				val += mat.getElement(i, k) * rX.getElement(k, j);
			}
			setElement(i, j, val);
		}
	}

	return *this;
}

inline Matrix3f operator+(const Matrix3f& rX, const Matrix3f& rY)
{
	Matrix3f ret(rX);
	ret += rY;

	return ret;
}

inline Matrix3f operator-(const Matrix3f& rX, const Matrix3f& rY)
{
	Matrix3f ret(rX);
	ret -= rY;

	return ret;
}

inline Matrix3f operator*(float val, const Matrix3f& rX)
{
	Matrix3f ret(rX);
	ret *= val;

	return ret;
}

inline Matrix3f operator*(const Matrix3f& rX, float val)
{
	Matrix3f ret(rX);
	ret *= val;

	return ret;
}

inline Matrix3f operator/(const Matrix3f& rX, float val)
{
	assert(!isZero(val));
	if (isZero(val)) return rX;

	Matrix3f ret(rX);
	ret /= val;

	return ret;
}

inline Matrix3f operator*(const Matrix3f& rX, const Matrix3f& rY)
{
	Matrix3f ret(rX);
	ret *= rY;

	return ret;
}

inline Vector3f operator*(const Matrix3f& rM, const Vector3f& rV)
{
	Vector3f retV;
	retV.setX(rM.getElement(0, 0) * rV.getX() + rM.getElement(0, 1) * rV.getY() + rM.getElement(0, 2) * rV.getZ());
	retV.setY(rM.getElement(1, 0) * rV.getX() + rM.getElement(1, 1) * rV.getY() + rM.getElement(1, 2) * rV.getZ());
	retV.setZ(rM.getElement(2, 0) * rV.getX() + rM.getElement(2, 1) * rV.getY() + rM.getElement(2, 2) * rV.getZ());

	return retV;
}

inline std::ostream& operator<<(std::ostream& rOst, const Matrix3f& rX)
{
	for (int i = 0; i < Matrix3f::numRows; i ++) {
		rOst << "[ ";
		for (int j = 0; j < Matrix3f::numRows; j ++) {
			rOst << rX.getElement(i, j) << " ";
		}
		rOst << "] ";
	}
	rOst << std::endl;

	return rOst;
}

#endif // __MATRIX_3F_H_INCLUDED
