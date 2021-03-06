#include <limits>
#include "Matrix4f.h"


bool Matrix4f::invert()
{
//	if (isZero(getDeterminant())) return false;

	Matrix4f srcMat = *this;
	Matrix4f dstMat = Matrix4f::getIdentity();

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

Matrix4f Matrix4f::getIdentity()
{
	Matrix4f ret;
	for (int i = 0; i < numRows; i ++) {
		for (int j = 0; j < numColumns; j ++) {
			ret.setElement(i, j, (i == j) ? 1.0f : 0.0f);
		}
	}

	return ret;
}

Matrix4f Matrix4f::getScale(const Vector3f& rV)
{
	Matrix4f ret = getIdentity();
	ret.setElement(0, 0, rV.getX());
	ret.setElement(1, 1, rV.getY());
	ret.setElement(2, 2, rV.getZ());

	return ret;
}

Matrix4f Matrix4f::getTranslate(const Vector3f& rV)
{
	Matrix4f ret = getIdentity();
	ret.setElement(0, 3, rV.getX());
	ret.setElement(1, 3, rV.getY());
	ret.setElement(2, 3, rV.getZ());

	return ret;
}

Matrix4f Matrix4f::getRotate(float angle, const Vector3f& rV)
{
	Matrix4f ret = getIdentity();

	float length = rV.getLength();
	assert(!isZero(length));
	if (isZero(length)) return ret;

	Vector3f vecNormalized(rV);
	vecNormalized.normalize();

	float rad = angle * g_PI / 180.0f;
	float c = std::cos(rad);
	float s = std::sin(rad);
	float x = vecNormalized.getX();
	float y = vecNormalized.getY();
	float z = vecNormalized.getZ();
	ret.setElement(0, 0, x * x * (1.0f - c) + c);
	ret.setElement(0, 1, x * y * (1.0f - c) - z * s);
	ret.setElement(0, 2, x * z * (1.0f - c) + y * s);
	ret.setElement(1, 0, y * x * (1.0f - c) + z * s);
	ret.setElement(1, 1, y * y * (1.0f - c) + c);
	ret.setElement(1, 2, y * z * (1.0f - c) - x * s);
	ret.setElement(2, 0, x * z * (1.0f - c) - y * s);
	ret.setElement(2, 1, y * z * (1.0f - c) + x * s);
	ret.setElement(2, 2, z * z * (1.0f - c) + c);
	return ret;
}

Matrix4f Matrix4f::getFrustum(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4f ret = getIdentity();

	ret.setElement(0, 0, 2.0f * zNear / (right - left));
	ret.setElement(0, 2, (right + left) / (right - left));
	ret.setElement(1, 1, - 2.0f * zNear / (top - bottom));
	ret.setElement(1, 2, (top + bottom) / (top - bottom));
	ret.setElement(2, 2, - (zFar + zNear) / (zFar - zNear));
	ret.setElement(2, 3, - 2.0f * zFar * zNear / (zFar - zNear));
	ret.setElement(3, 2, - 1.0f);

	return ret;
}

Matrix4f Matrix4f::getOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4f ret = getIdentity();

	ret.setElement(0, 0, 2.0f / (right - left));
	ret.setElement(0, 3, - (right + left) / (right - left));
	ret.setElement(1, 1, 2.0f / (top - bottom));
	ret.setElement(1, 3, - (top + bottom) / (top - bottom));
	ret.setElement(2, 2, - 2.0f / (zFar - zNear));
	ret.setElement(2, 3, - (zFar + zNear) / (zFar - zNear));

	return ret;
}

Matrix4f Matrix4f::getLookAt(const Vector3f& rVecEye, const Vector3f& rVecCenter, const Vector3f& rVecUp)
{
	Vector3f vecF = rVecCenter - rVecEye;
	vecF.normalize();
	Vector3f vecUp = rVecUp;
	vecUp.normalize();

	Vector3f vecS = Vector3f::crossProduct(vecF, vecUp);
	Vector3f vecU = Vector3f::crossProduct(vecS, vecF);

	Matrix4f ret = getIdentity();
	ret.setElement(0, 0, vecS.getX());
	ret.setElement(0, 1, vecS.getY());
	ret.setElement(0, 2, vecS.getZ());
	ret.setElement(1, 0, vecU.getX());
	ret.setElement(1, 1, vecU.getY());
	ret.setElement(1, 2, vecU.getZ());
	ret.setElement(2, 0, - vecF.getX());
	ret.setElement(2, 1, - vecF.getY());
	ret.setElement(2, 2, - vecF.getZ());

	ret *= getTranslate(- rVecEye);

	return ret;
}

Matrix4f Matrix4f::getPerspective(float fovy, float aspect, float zNear, float zFar)
{
	float f = 1.0f / std::tan(getRadian(fovy) * 0.5f);

	Matrix4f ret = getIdentity();
	ret.setElement(0, 0, f / aspect);
	ret.setElement(1, 1, f);
	ret.setElement(2, 2, (zFar + zNear) / (zNear - zFar));
	ret.setElement(2, 3, 2.0f * zFar * zNear / (zNear - zFar));
	ret.setElement(3, 2, -1.0f);

	return ret;
}

Matrix4f Matrix4f::getOrientation(const Vector3f& rVecFront, const Vector3f& rVecUp)
{
	Vector3f vecFront(rVecFront), vecUp(rVecUp);
	vecFront.normalize();
	vecUp.normalize();

	Vector3f vecSide = Vector3f::crossProduct(vecUp, vecFront);
	Vector3f vecUpActual = Vector3f::crossProduct(vecFront, vecSide);

	Matrix4f ret = getIdentity();
	ret.setElement(0, 0, vecSide.getX());	// +X 軸方向が左
	ret.setElement(1, 0, vecSide.getY());
	ret.setElement(2, 0, vecSide.getZ());
	ret.setElement(0, 1, vecUpActual.getX());	// +Y 軸方向が上
	ret.setElement(1, 1, vecUpActual.getY());
	ret.setElement(2, 1, vecUpActual.getZ());
	ret.setElement(0, 2, vecFront.getX());		// +Z 軸方向が前
	ret.setElement(1, 2, vecFront.getY());
	ret.setElement(2, 2, vecFront.getZ());

	return ret;
}


