#ifndef __MATRIX_2F_H_INCLUDED
#define __MATRIX_2F_H_INCLUDED

#include <cassert>
#include <iostream>
#include <cmath>
#include "MathUtil.h"
#include "Vector2f.h"

class	Matrix2f
{
public:
	enum
	{
		numRows = 2,
		numColumns = 2,
		numElements = numRows * numColumns
	};

	Matrix2f();
	Matrix2f(const float mat[numElements]);
	Matrix2f(const Matrix2f& rX);

	void clear();
	float getElement(int indexRow, int indexColumn) const;
	const float* getArray() const { return m_mat; }
	float* getArray() { return m_mat; }
	void setElement(int indexRow, int indexColumn, float val);
	void setArray(const float mat[numElements]);

	float getDeterminant() const;
	bool invert();	// 逆行列を求める
	void transpose();	// 転置行列を求める
	void clearTranslate();	// 移動成分をクリアする

	// Fundamental row operation
	void replaceRows(int indexRow0, int indexRow1);
	void scaleRow(int indexRow, float ratio);
	void addScaledRow(int indexRowSrc, float ratioRow, int indexRowDst);

	const Matrix2f& operator=(const Matrix2f& rX);
	const Matrix2f& operator+=(const Matrix2f& rX);
	const Matrix2f& operator-=(const Matrix2f& rX);
	const Matrix2f& operator*=(float val);
	const Matrix2f& operator/=(float val);

	const Matrix2f& operator*=(const Matrix2f& rX);

	static Matrix2f getIdentity();
	static Matrix2f getScale(const Vector2f& rV);
	static Matrix2f getTranslate(const Vector2f& rV);
	static Matrix2f getRotate(float angle);
	static Matrix2f getRotate(float valCos, float valSin);

private:
	float	m_mat[numElements];
};

inline Matrix2f::Matrix2f()
{
	clear();
}

inline Matrix2f::Matrix2f(const Matrix2f& rX)
{
	setArray(rX.m_mat);
}

inline Matrix2f::Matrix2f(const float mat[numElements])
{
	setArray(mat);
}

inline void Matrix2f::clear()
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] = 0.0f;
	}
}

inline float Matrix2f::getElement(int indexRow, int indexColumn) const
{
    return m_mat[indexColumn * numRows + indexRow];
}

inline void Matrix2f::setElement(int indexRow, int indexColumn, float val)
{
	m_mat[indexColumn * numRows + indexRow] = val;
}

inline void Matrix2f::setArray(const float mat[numElements])
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] = mat[i];
	}
}

inline float Matrix2f::getDeterminant() const
{
	return getElement(0, 0) * getElement(1, 1) - getElement(1, 0) * getElement(0, 1);
}

inline void Matrix2f::transpose()
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

inline void Matrix2f::clearTranslate()
{
	for (int i = 0; i < numRows; i ++) {
		setElement(i, numRows - 1, 0.0f);
	}
	for (int j = 0; j < numColumns; j ++) {
		setElement(numColumns - 1, j, 0.0f);
	}
	setElement(numRows - 1, numColumns - 1, 1.0f);
}

inline void Matrix2f::replaceRows(int indexRow0, int indexRow1)
{
	if (indexRow0 == indexRow1) return;

	for (int i = 0; i < numColumns; i ++) {
		float value0 = getElement(indexRow0, i);
		float value1 = getElement(indexRow1, i);
		setElement(indexRow0, i, value1);
		setElement(indexRow1, i, value0);
	}
}

inline void Matrix2f::scaleRow(int indexRow, float ratio)
{
	for (int i = 0; i < numColumns; i ++) {
		setElement(indexRow, i, getElement(indexRow, i) * ratio);
	}
}

inline void Matrix2f::addScaledRow(int indexRowSrc, float ratioRow, int indexRowDst)
{
	for (int i = 0; i < numColumns; i ++) {
		float valueSrc = getElement(indexRowSrc, i);
		float valueDst = getElement(indexRowDst, i);
		setElement(indexRowDst, i, valueSrc * ratioRow + valueDst);
	}
}

inline const Matrix2f& Matrix2f::operator=(const Matrix2f& rX)
{
	setArray(rX.m_mat);

	return *this;
}

inline const Matrix2f& Matrix2f::operator+=(const Matrix2f& rX)
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] += rX.m_mat[i];
	}

	return *this;
}

inline const Matrix2f& Matrix2f::operator-=(const Matrix2f& rX)
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] -= rX.m_mat[i];
	}

	return *this;
}

inline const Matrix2f& Matrix2f::operator*=(float val)
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] *= val;
	}

	return *this;
}

inline const Matrix2f& Matrix2f::operator/=(float val)
{
	assert(!isZero(val));
	if (isZero(val)) return *this;

	for (int i = 0; i < numElements; i ++) {
		m_mat[i] /= val;
	}

	return *this;
}

inline const Matrix2f& Matrix2f::operator*=(const Matrix2f& rX)
{
	Matrix2f mat(m_mat);
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

inline Matrix2f operator+(const Matrix2f& rX, const Matrix2f& rY)
{
	Matrix2f ret(rX);
	ret += rY;

	return ret;
}

inline Matrix2f operator-(const Matrix2f& rX, const Matrix2f& rY)
{
	Matrix2f ret(rX);
	ret -= rY;

	return ret;
}

inline Matrix2f operator*(float val, const Matrix2f& rX)
{
	Matrix2f ret(rX);
	ret *= val;

	return ret;
}

inline Matrix2f operator*(const Matrix2f& rX, float val)
{
	Matrix2f ret(rX);
	ret *= val;

	return ret;
}

inline Matrix2f operator/(const Matrix2f& rX, float val)
{
	assert(!isZero(val));
	if (isZero(val)) return rX;

	Matrix2f ret(rX);
	ret /= val;

	return ret;
}

inline Matrix2f operator*(const Matrix2f& rX, const Matrix2f& rY)
{
	Matrix2f ret(rX);
	ret *= rY;

	return ret;
}

inline Vector2f operator*(const Matrix2f& rM, const Vector2f& rV)
{
	Vector2f retV;
	retV.setX(rM.getElement(0, 0) * rV.getX() + rM.getElement(0, 1) * rV.getY());
	retV.setY(rM.getElement(1, 0) * rV.getX() + rM.getElement(1, 1) * rV.getY());

	return retV;
}

inline std::ostream& operator<<(std::ostream& rOst, const Matrix2f& rX)
{
	for (int i = 0; i < Matrix2f::numRows; i ++) {
		rOst << "[ ";
		for (int j = 0; j < Matrix2f::numRows; j ++) {
			rOst << rX.getElement(i, j) << " ";
		}
		rOst << "] ";
	}
	rOst << std::endl;

	return rOst;
}

#endif // __MATRIX_3F_H_INCLUDED
