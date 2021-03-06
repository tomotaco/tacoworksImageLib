#include <limits>
#include "Matrix3f.h"


bool Matrix3f::invert()
{
//	if (isZero(getDeterminant())) return false;

	Matrix3f srcMat = *this;
	Matrix3f dstMat = Matrix3f::getIdentity();

	// to be implemented
	for (int j = 0; j < numColumns; j ++) {
		// 0 でない行を探し、そこを pivot とする
        unsigned int indexPivot = std::numeric_limits<unsigned int>::max();
		for (int i = j; i < numRows; i ++) {
			if (!isZero(srcMat.getElement(i, j))) {
				indexPivot = i;
				break;
			}
		}
        if (indexPivot == std::numeric_limits<unsigned int>::max()) return false;
		srcMat.replaceRows(j, indexPivot);
		dstMat.replaceRows(j, indexPivot);

		// pivot を 1 にする
		float invertValueAtPivot = 1.0f / srcMat.getElement(j, j);
		srcMat.scaleRow(j, invertValueAtPivot);
		dstMat.scaleRow(j, invertValueAtPivot);

		// pivot 以外の行を 0 にする
		for (int i = 0; i < numRows; i ++) {
			if (i == j) continue;
			float value = -srcMat.getElement(i, j);
			srcMat.addScaledRow(indexPivot, value, i);
			dstMat.addScaledRow(indexPivot, value, i);
		}
	}

	*this = dstMat;

	return true;
}

Matrix3f Matrix3f::getIdentity()
{
	Matrix3f ret;
	for (int i = 0; i < numRows; i ++) {
		for (int j = 0; j < numColumns; j ++) {
			ret.setElement(i, j, (i == j) ? 1.0f : 0.0f);
		}
	}

	return ret;
}

Matrix3f Matrix3f::getScale(const Vector2f& rV)
{
	Matrix3f ret = getIdentity();
	ret.setElement(0, 0, rV.getX());
	ret.setElement(1, 1, rV.getY());

	return ret;
}

Matrix3f Matrix3f::getTranslate(const Vector2f& rV)
{
	Matrix3f ret = getIdentity();
	ret.setElement(0, 2, rV.getX());
	ret.setElement(1, 2, rV.getY());

	return ret;
}

Matrix3f Matrix3f::getRotate(float angle)
{
	float rad = angle * g_PI / 180.0f;
	float c = std::cos(rad);
	float s = std::sin(rad);

	return getRotate(c, s);
}

Matrix3f Matrix3f::getRotate(float valCos, float valSin)
{
	Matrix3f ret = getIdentity();

	ret.setElement(0, 0, valCos);
	ret.setElement(0, 1, - valSin);
	ret.setElement(1, 0, valSin);
	ret.setElement(1, 1, valCos);
	return ret;
}

