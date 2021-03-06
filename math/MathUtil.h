#ifndef __MATH_UTIL_H_INCLUDED
#define __MATH_UTIL_H_INCLUDED

#include <cmath>

const float g_PI		=  3.14159265358979323846f;
const float g_2_PI      =  6.28318530717958623200f;
const float g_PI_DIV_2	=  1.57079632679489661923f;
const float g_PI_DIV_4	=  0.785398163397448309616f;
const float g_INV_PI	=  0.318309886183790671538f;
// const float g_HUGE		=  1.0e+38f;
const float g_EPSILON	=  1.0e-6f;

inline bool isZero(float val) { return (std::fabs(val) < g_EPSILON) ? true : false; }
inline bool isEqual(float a, float b) { return isZero(a - b); }


// 0.5 が境界になる round (バイアスを出さない丸めを行う)
//
// float -2  -1   0   1   2   3
//      --+---+---+---+---+---+--
// int  | -2| -1| 0 | 1 | 2 | 3 |
//
inline int	roundInt(float val) { return (val > 0.0f) ? int(val + 0.5f) : int(val - 0.5f); }

// .0 が境界になる round (ピクセルの座標用)
// float -2  -1   0   1   2   3
//      --+---+---+---+---+---+--
// int    | -2| -1| 0 | 1 | 2 |
//
inline int roundIntPixel(float x)
{
	return (x > 0.0f) ? int(x) : int(x - 1.0f);
}

inline float getRadian(float degree) { return degree *  g_PI / 180.0f; }
inline float getDegree(float radian) { return radian * 180.0f / g_PI;}

template <typename T>
inline T	cropValue(T val, T minVal, T maxVal)
{
	if (val < minVal) return minVal;
	if (val > maxVal) return maxVal;

	return val;
}


template <typename T>
inline T min3(T val0, T val1, T val2)
{
	return (val0 < val1) ? ((val0 < val2) ? val0 : val2) : ((val1 < val2) ? val1 : val2);
}

/*!
	@brief 2の累乗に切り上げ
	ある数値に最も近い２の累乗を求める
 */
inline int getSizePowerOfTwo(int size)
{
	size --;
	size |= size >> 1;
	size |= size >> 2;
	size |= size >> 4;
	size |= size >> 8;
	size |= size >> 16;
	size ++;

	return size;
}

#endif // __MATH_UTIL_H_INCLUDED
