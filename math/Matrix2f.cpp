#include "Matrix2f.h"

bool Matrix2f::invert()
{
	float det = getDeterminant();
	if (isZero(det)) return false;

	float a = getElement(0, 0);
	float b = getElement(0, 1);
	float c = getElement(1, 0);
	float d = getElement(1, 1);

	Matrix2f mat = Matrix2f::getIdentity();
	mat.setElement(0, 0, d);
	mat.setElement(0, 1, - b);
	mat.setElement(1, 0, - c);
	mat.setElement(1, 1, a);

	mat /= det;

	*this = mat;

	return true;
}

Matrix2f Matrix2f::getIdentity()
{
	Matrix2f ret;
	for (int i = 0; i < numRows; i ++) {
		for (int j = 0; j < numColumns; j ++) {
			ret.setElement(i, j, (i == j) ? 1.0f : 0.0f);
		}
	}

	return ret;
}

Matrix2f Matrix2f::getScale(const Vector2f& rV)
{
	Matrix2f ret = getIdentity();
	ret.setElement(0, 0, rV.getX());
	ret.setElement(1, 1, rV.getY());

	return ret;
}

Matrix2f Matrix2f::getTranslate(const Vector2f& rV)
{
	Matrix2f ret = getIdentity();
	ret.setElement(0, 2, rV.getX());
	ret.setElement(1, 2, rV.getY());

	return ret;
}

Matrix2f Matrix2f::getRotate(float angle)
{
	float rad = angle * g_PI / 180.0f;
	float c = std::cos(rad);
	float s = std::sin(rad);

	return getRotate(c, s);
}

Matrix2f Matrix2f::getRotate(float valCos, float valSin)
{
	Matrix2f ret = getIdentity();

	ret.setElement(0, 0, valCos);
	ret.setElement(0, 1, - valSin);
	ret.setElement(1, 0, valSin);
	ret.setElement(1, 1, valCos);
	return ret;
}

