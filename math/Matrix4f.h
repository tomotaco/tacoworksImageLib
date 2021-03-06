#ifndef __MATRIX_4F_H_INCLUDED
#define __MATRIX_4F_H_INCLUDED

#include <cassert>
#include <iostream>
#include <cmath>
#include "MathUtil.h"
#include "Vector3f.h"
#include "Vector4f.h"

class	Matrix4f
{
public:
	enum
	{
		numRows = 4,
		numColumns = 4,
		numElements = numRows * numColumns
	};

	Matrix4f();
	Matrix4f(const float mat[numElements]);
	Matrix4f(const Matrix4f& rX);

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

	const Matrix4f& operator=(const Matrix4f& rX);
	const Matrix4f& operator+=(const Matrix4f& rX);
	const Matrix4f& operator-=(const Matrix4f& rX);
	const Matrix4f& operator*=(float val);
	const Matrix4f& operator/=(float val);

	const Matrix4f& operator*=(const Matrix4f& rX);

	static Matrix4f getIdentity();
	static Matrix4f getScale(const Vector3f& rV);
	static Matrix4f getTranslate(const Vector3f& rV);
	static Matrix4f getRotate(float angle, const Vector3f& rV);
	static Matrix4f getFrustum(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4f getOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4f getLookAt(const Vector3f& rVecEye, const Vector3f& rVecCenter, const Vector3f& rVecUp);
	static Matrix4f getPerspective(float fovy, float aspect, float zNear, float zFar);
	static Matrix4f getOrientation(const Vector3f& rVecFrontZ, const Vector3f& rVecUpY);

private:
	float	m_mat[numElements];
};

inline Matrix4f::Matrix4f()
{
	clear();
}

inline Matrix4f::Matrix4f(const Matrix4f& rX)
{
	setArray(rX.m_mat);
}

inline Matrix4f::Matrix4f(const float mat[numElements])
{
	setArray(mat);
}

inline void Matrix4f::clear()
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] = 0.0f;
	}
}

inline float Matrix4f::getElement(int indexRow, int indexColumn) const
{
    return m_mat[indexColumn * numRows + indexRow];
}

inline void Matrix4f::setElement(int indexRow, int indexColumn, float val)
{
	m_mat[indexColumn * numRows + indexRow] = val;
}

inline void Matrix4f::setArray(const float mat[numElements])
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] = mat[i];
	}
}

/*
inline float Matrix4f::getDeterminant() const
{
	// Temporary
	return 0.0f;
}
*/

inline void Matrix4f::transpose()
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

inline void Matrix4f::clearTranslate()
{
	for (int i = 0; i < numRows; i ++) {
		setElement(i, numRows - 1, 0.0f);
	}
	for (int j = 0; j < numColumns; j ++) {
		setElement(numColumns - 1, j, 0.0f);
	}
	setElement(numRows - 1, numColumns - 1, 1.0f);
}

inline void Matrix4f::replaceRows(int indexRow0, int indexRow1)
{
	if (indexRow0 == indexRow1) return;

	for (int i = 0; i < numColumns; i ++) {
		float value0 = getElement(indexRow0, i);
		float value1 = getElement(indexRow1, i);
		setElement(indexRow0, i, value1);
		setElement(indexRow1, i, value0);
	}
}

inline void Matrix4f::scaleRow(int indexRow, float ratio)
{
	for (int i = 0; i < numColumns; i ++) {
		setElement(indexRow, i, getElement(indexRow, i) * ratio);
	}
}

inline void Matrix4f::addScaledRow(int indexRowSrc, float ratioRow, int indexRowDst)
{
	for (int i = 0; i < numColumns; i ++) {
		float valueSrc = getElement(indexRowSrc, i);
		float valueDst = getElement(indexRowDst, i);
		setElement(indexRowDst, i, valueSrc * ratioRow + valueDst);
	}
}

inline const Matrix4f& Matrix4f::operator=(const Matrix4f& rX)
{
	setArray(rX.m_mat);

	return *this;
}

inline const Matrix4f& Matrix4f::operator+=(const Matrix4f& rX)
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] += rX.m_mat[i];
	}

	return *this;
}

inline const Matrix4f& Matrix4f::operator-=(const Matrix4f& rX)
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] -= rX.m_mat[i];
	}

	return *this;
}

inline const Matrix4f& Matrix4f::operator*=(float val)
{
	for (int i = 0; i < numElements; i ++) {
		m_mat[i] *= val;
	}

	return *this;
}

inline const Matrix4f& Matrix4f::operator/=(float val)
{
	assert(!isZero(val));
	if (isZero(val)) return *this;

	for (int i = 0; i < numElements; i ++) {
		m_mat[i] /= val;
	}

	return *this;
}

inline const Matrix4f& Matrix4f::operator*=(const Matrix4f& rX)
{
	Matrix4f mat(m_mat);
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

inline Matrix4f operator+(const Matrix4f& rX, const Matrix4f& rY)
{
	Matrix4f ret(rX);
	ret += rY;

	return ret;
}

inline Matrix4f operator-(const Matrix4f& rX, const Matrix4f& rY)
{
	Matrix4f ret(rX);
	ret -= rY;

	return ret;
}

inline Matrix4f operator*(float val, const Matrix4f& rX)
{
	Matrix4f ret(rX);
	ret *= val;

	return ret;
}

inline Matrix4f operator*(const Matrix4f& rX, float val)
{
	Matrix4f ret(rX);
	ret *= val;

	return ret;
}

inline Matrix4f operator/(const Matrix4f& rX, float val)
{
	assert(!isZero(val));
	if (isZero(val)) return rX;

	Matrix4f ret(rX);
	ret /= val;

	return ret;
}

inline Matrix4f operator*(const Matrix4f& rX, const Matrix4f& rY)
{
	Matrix4f ret(rX);
	ret *= rY;

	return ret;
}

inline Vector4f operator*(const Matrix4f& rM, const Vector4f& rV)
{
	Vector4f retV;
	retV.setX(rM.getElement(0, 0) * rV.getX() + rM.getElement(0, 1) * rV.getY() + rM.getElement(0, 2) * rV.getZ() + rM.getElement(0, 3) * rV.getW());
	retV.setY(rM.getElement(1, 0) * rV.getX() + rM.getElement(1, 1) * rV.getY() + rM.getElement(1, 2) * rV.getZ() + rM.getElement(1, 3) * rV.getW());
	retV.setZ(rM.getElement(2, 0) * rV.getX() + rM.getElement(2, 1) * rV.getY() + rM.getElement(2, 2) * rV.getZ() + rM.getElement(2, 3) * rV.getW());
	retV.setW(rM.getElement(3, 0) * rV.getX() + rM.getElement(3, 1) * rV.getY() + rM.getElement(3, 2) * rV.getZ() + rM.getElement(3, 3) * rV.getW());

	return retV;
}


inline std::ostream& operator<<(std::ostream& rOst, const Matrix4f& rX)
{
	for (int i = 0; i < Matrix4f::numRows; i ++) {
		rOst << "[ ";
		for (int j = 0; j < Matrix4f::numRows; j ++) {
			rOst << rX.getElement(i, j) << " ";
		}
		rOst << "] ";
	}
	rOst << std::endl;

	return rOst;
}

#endif // __MATRIX_4F_H_INCLUDED
