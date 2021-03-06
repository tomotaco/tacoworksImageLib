#ifndef ElementUtilH
#define ElementUtilH

#include "ElementType.h"

/**
1画素の取りうる値の範囲を定義
*/
namespace ElementUtil
{
	template<typename T> T getMinValue();
	template<typename T> T getMaxValue();
	template<typename T> ElementType getType();

	template<>
	inline unsigned char getMinValue<unsigned char>()
	{
		return 0;
	}

	template<>
	inline unsigned char getMaxValue<unsigned char>()
	{
		return 0xff;
	}

	template<>
	inline ElementType getType<unsigned char>()
	{
		return ELEMENT_8;
	}

	template<>
	inline unsigned short getMinValue<unsigned short>()
	{
		return 0;
	}

	template<>
	inline unsigned short getMaxValue<unsigned short>()
	{
		return 0xffff;
	}

	template<>
	inline ElementType getType<unsigned short>()
	{
		return ELEMENT_16;
	}

	template<>
	inline int getMinValue<int>()
	{
		return 0;
	}

	template<>
	inline int getMaxValue<int>()
	{
		return 0x7fffffff;
	}

	template<>
	inline ElementType getType<int>()
	{
		return ELEMENT_32S;
	}

	template<>
	inline unsigned int getMinValue<unsigned int>()
	{
		return 0;
	}

	template<>
	inline unsigned int getMaxValue<unsigned int>()
	{
		return 0xffffffff;
	}

	template<>
	inline ElementType getType<unsigned int>()
	{
		return ELEMENT_32U;
	}

	template<>
	inline float getMinValue<float>()
	{
		return 0.0f;
	}

	template<>
	inline float getMaxValue<float>()
	{
		return 1.0f;
	}

	template<>
	inline ElementType getType<float>()
	{
		return ELEMENT_FLOAT;
	}

	template<>
	inline double getMinValue<double>()
	{
		return 0.0;
	}

	template<>
	inline double getMaxValue<double>()
	{
		return 1.0;
	}

	template<>
	inline ElementType getType<double>()
	{
		return ELEMENT_DOUBLE;
	}
};

#endif
