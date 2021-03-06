#ifndef ConvertSingleElementFunctorH
#define ConvertSingleElementFunctorH

#include "ElementUtil.h"

struct ConvertSingleElementFunctor
{
	// src と dst の型が同じで型変換不要なもの
	void operator()(unsigned char elementSrc, unsigned char& rElementDst) const;
	void operator()(unsigned short elementSrc, unsigned short& rElementDst) const;
	void operator()(int elementSrc, int& rElementDst) const;
	void operator()(unsigned int elementSrc, unsigned int& rElementDst) const;
	void operator()(float elementSrc, float& rElementDst) const;
	void operator()(double elementSrc, double& rElementDst) const;

	// src と dst の型が異なるもの
	void operator()(unsigned char elementSrc, unsigned short& rElementDst) const;
	void operator()(unsigned char elementSrc, int& rElementDst) const;
	void operator()(unsigned char elementSrc, unsigned int& rElementDst) const;
	void operator()(unsigned char elementSrc, float& rElementDst) const;
	void operator()(unsigned char elementSrc, double& rElementDst) const;
	void operator()(unsigned short elementSrc, unsigned char& rElementDst) const;
	void operator()(unsigned short elementSrc, int& rElementDst) const;
	void operator()(unsigned short elementSrc, unsigned int& rElementDst) const;
	void operator()(unsigned short elementSrc, float& rElementDst) const;
	void operator()(unsigned short elementSrc, double& rElementDst) const;
	void operator()(int elementSrc, unsigned char& rElementDst) const;
	void operator()(int elementSrc, unsigned short& rElementDst) const;
	void operator()(int elementSrc, unsigned int& rElementDst) const;
	void operator()(int elementSrc, float& rElementDst) const;
	void operator()(int elementSrc, double& rElementDst) const;
	void operator()(unsigned int elementSrc, unsigned char& rElementDst) const;
	void operator()(unsigned int elementSrc, unsigned short& rElementDst) const;
	void operator()(unsigned int elementSrc, int& rElementDst) const;
	void operator()(unsigned int elementSrc, float& rElementDst) const;
	void operator()(unsigned int elementSrc, double& rElementDst) const;
	void operator()(float elementSrc, unsigned char& rElementDst) const;
	void operator()(float elementSrc, unsigned short& rElementDst) const;
	void operator()(float elementSrc, int& rElementDst) const;
	void operator()(float elementSrc, unsigned int& rElementDst) const;
	void operator()(float elementSrc, double& rElementDst) const;
	void operator()(double elementSrc, unsigned char& rElementDst) const;
	void operator()(double elementSrc, unsigned short& rElementDst) const;
	void operator()(double elementSrc, int& rElementDst) const;
	void operator()(double elementSrc, unsigned int& rElementDst) const;
	void operator()(double elementSrc, float& rElementDst) const;
};

// TSrc と TDst の型が同じで型変換不要なもの
inline void ConvertSingleElementFunctor::operator()(unsigned char elementSrc, unsigned char& rElementDst) const
{
	rElementDst = elementSrc;
}

inline void ConvertSingleElementFunctor::operator()(unsigned short elementSrc, unsigned short& rElementDst) const
{
	rElementDst = elementSrc;
}

inline void ConvertSingleElementFunctor::operator()(int elementSrc, int& rElementDst) const
{
	rElementDst = elementSrc;
}

inline void ConvertSingleElementFunctor::operator()(unsigned int elementSrc, unsigned int& rElementDst) const
{
	rElementDst = elementSrc;
}

inline void ConvertSingleElementFunctor::operator()(float elementSrc, float& rElementDst) const
{
	rElementDst = elementSrc;
}

inline void ConvertSingleElementFunctor::operator()(double elementSrc, double& rElementDst) const
{
	rElementDst = elementSrc;
}

// TSrc と TDst の型が違うため型変換が必要なもの
inline void ConvertSingleElementFunctor::operator()(unsigned char elementSrc, unsigned short& rElementDst) const
{
	// char -> short
	// src * (2^16 - 1) / (2^8 - 1) =
	// src * (2^8 + 1) * (2^8 - 1) / (2^8 - 1) =
	// src * (2^8 + 1) =
	// src * 2^8 + src =
	// src << 8 + src
	rElementDst = ((unsigned short)(elementSrc) << 8) + (unsigned short)(elementSrc);
}

inline void ConvertSingleElementFunctor::operator()(unsigned char elementSrc, int& rElementDst) const
{

	// char -> int
	// src * (2^31 - 1) / (2^8 - 1) =
	// src * (2^23 + 1) * (2^8 - 1) / (2^8 - 1) =
	// src * (2^23 + 1) =
	// src * 2^23 + src=
	// src << 23 + src
	rElementDst = ((unsigned short)(elementSrc) << 23) + (unsigned short)(elementSrc);
}

inline void ConvertSingleElementFunctor::operator()(unsigned char elementSrc, unsigned int& rElementDst) const
{

	// char -> int
	// src * (2^32 - 1) / (2^8 - 1) =
	// src * (2^24 + 1) * (2^8 - 1) / (2^8 - 1) =
	// src * (2^24 + 1) =
	// src * 2^24 + src =
	// src << 24 + src
	rElementDst = ((unsigned short)(elementSrc) << 24) + (unsigned short)(elementSrc);
}

inline void ConvertSingleElementFunctor::operator()(unsigned char elementSrc, float& rElementDst) const
{
	// char -> float
	// float 版の最大値は 1.0f なので TSrc の最大値で割るだけ
	rElementDst = float(elementSrc) / float(ElementUtil::getMaxValue<unsigned char>());
}

inline void ConvertSingleElementFunctor::operator()(unsigned char elementSrc, double& rElementDst) const
{
	// char -> float
	// float 版の最大値は 1.0f なので TSrc の最大値で割るだけ
	rElementDst = double(elementSrc) / double(ElementUtil::getMaxValue<unsigned char>());
}

inline void ConvertSingleElementFunctor::operator()(unsigned short elementSrc, unsigned char& rElementDst) const
{
	// short -> char
	rElementDst = (unsigned char)((elementSrc - (elementSrc >> 8) + 128) >> 8);
}

inline void ConvertSingleElementFunctor::operator()(unsigned short elementSrc, int& rElementDst) const
{
	// short -> int
	rElementDst = (int)(elementSrc * (ElementUtil::getMaxValue<int>() / ElementUtil::getMaxValue<unsigned short>()));
}

inline void ConvertSingleElementFunctor::operator()(unsigned short elementSrc, unsigned int& rElementDst) const
{
	// short -> unsigned int
	rElementDst = (unsigned int)((elementSrc * (ElementUtil::getMaxValue<unsigned int>() / ElementUtil::getMaxValue<unsigned short>())));
}

inline void ConvertSingleElementFunctor::operator()(unsigned short elementSrc, float& rElementDst) const
{
	// short -> float
	// float 版の最大値は 1.0f なので TSrc の最大値で割るだけ
	rElementDst = float(elementSrc) / float(ElementUtil::getMaxValue<unsigned char>());
}

inline void ConvertSingleElementFunctor::operator()(unsigned short elementSrc, double& rElementDst) const
{
	// short -> double
	// double 版の最大値は 1.0f なので TSrc の最大値で割るだけ
	rElementDst = double(elementSrc) / double(ElementUtil::getMaxValue<unsigned char>());
}

inline void ConvertSingleElementFunctor::operator()(int elementSrc, unsigned char& rElementDst) const
{
	// int -> char
	rElementDst = (unsigned char)(elementSrc * ElementUtil::getMaxValue<unsigned char>() / ElementUtil::getMaxValue<int>());
}

inline void ConvertSingleElementFunctor::operator()(int elementSrc, unsigned short& rElementDst) const
{
	// int -> short
	rElementDst = (unsigned char)(elementSrc * ElementUtil::getMaxValue<unsigned short>() / ElementUtil::getMaxValue<int>());
}

inline void ConvertSingleElementFunctor::operator()(int elementSrc, unsigned int& rElementDst) const
{
	// int -> unsigned int
	rElementDst = (unsigned int)((elementSrc << 1) +( elementSrc >> 31));
}

inline void ConvertSingleElementFunctor::operator()(int elementSrc, float& rElementDst) const
{
	// short -> float
	// float 版の最大値は 1.0f なので TSrc の最大値で割るだけ
	rElementDst = float(elementSrc) / float(ElementUtil::getMaxValue<int>());
}

inline void ConvertSingleElementFunctor::operator()(int elementSrc, double& rElementDst) const
{
	// short -> double
	// double 版の最大値は 1.0f なので TSrc の最大値で割るだけ
	rElementDst = double(elementSrc) / double(ElementUtil::getMaxValue<int>());
}

inline void ConvertSingleElementFunctor::operator()(unsigned int elementSrc, unsigned char& rElementDst) const
{
	// int -> char
	rElementDst = (unsigned char)(elementSrc * ElementUtil::getMaxValue<unsigned char>() / ElementUtil::getMaxValue<unsigned int>());
}

inline void ConvertSingleElementFunctor::operator()(unsigned int elementSrc, unsigned short& rElementDst) const
{
	// int -> short
	rElementDst = (unsigned char)(elementSrc * ElementUtil::getMaxValue<unsigned short>() / ElementUtil::getMaxValue<unsigned int>());
}

inline void ConvertSingleElementFunctor::operator()(unsigned int elementSrc, int& rElementDst) const
{
	// int -> unsigned int
	rElementDst = (unsigned int)(elementSrc >> 1);
}

inline void ConvertSingleElementFunctor::operator()(unsigned int elementSrc, float& rElementDst) const
{
	// short -> float
	// float 版の最大値は 1.0f なので TSrc の最大値で割るだけ
	rElementDst = float(elementSrc) / float(ElementUtil::getMaxValue<unsigned int>());
}

inline void ConvertSingleElementFunctor::operator()(unsigned int elementSrc, double& rElementDst) const
{
	// short -> double
	// double 版の最大値は 1.0f なので TSrc の最大値で割るだけ
	rElementDst = double(elementSrc) / double(ElementUtil::getMaxValue<unsigned int>());
}

inline void ConvertSingleElementFunctor::operator()(float elementSrc, unsigned char& rElementDst) const
{
	// float -> char
	// float 版の最大値は 1.0f なので TDst の最大値を掛けるだけ
	rElementDst = (unsigned char)(elementSrc * float(ElementUtil::getMaxValue<unsigned char>()) + 0.5f);
}

inline void ConvertSingleElementFunctor::operator()(float elementSrc, unsigned short& rElementDst) const
{
	// float -> short
	// float 版の最大値は 1.0f なので TDst の最大値を掛けるだけ
	rElementDst = (unsigned short)(elementSrc * float(ElementUtil::getMaxValue<unsigned short>()) + 0.5f);
}

inline void ConvertSingleElementFunctor::operator()(float elementSrc, int& rElementDst) const
{
	// float -> short
	// float 版の最大値は 1.0f なので TDst の最大値を掛けるだけ
	rElementDst = (int)(elementSrc * float(ElementUtil::getMaxValue<int>()) + 0.5f);
}

inline void ConvertSingleElementFunctor::operator()(float elementSrc, unsigned int& rElementDst) const
{
	// float -> short
	// float 版の最大値は 1.0f なので TDst の最大値を掛けるだけ
	rElementDst = (unsigned int)(elementSrc * float(ElementUtil::getMaxValue<unsigned int>()) + 0.5f);
}

inline void ConvertSingleElementFunctor::operator()(float elementSrc, double& rElementDst) const
{
	// float -> short
	// float 版の最大値は 1.0f なので TDst の最大値を掛けるだけ
	rElementDst = (double)(elementSrc * float(ElementUtil::getMaxValue<double>()) + 0.5f);
}

inline void ConvertSingleElementFunctor::operator()(double elementSrc, unsigned char& rElementDst) const
{
	// float -> char
	// float 版の最大値は 1.0f なので TDst の最大値を掛けるだけ
	rElementDst = (unsigned char)(elementSrc * double(ElementUtil::getMaxValue<unsigned char>()) + 0.5f);
}

inline void ConvertSingleElementFunctor::operator()(double elementSrc, unsigned short& rElementDst) const
{
	// float -> short
	// float 版の最大値は 1.0f なので TDst の最大値を掛けるだけ
	rElementDst = (unsigned short)(elementSrc * double(ElementUtil::getMaxValue<unsigned short>()) + 0.5f);
}

inline void ConvertSingleElementFunctor::operator()(double elementSrc, int& rElementDst) const
{
	// float -> short
	// float 版の最大値は 1.0f なので TDst の最大値を掛けるだけ
	rElementDst = (int)(elementSrc * double(ElementUtil::getMaxValue<int>()) + 0.5f);
}

inline void ConvertSingleElementFunctor::operator()(double elementSrc, unsigned int& rElementDst) const
{
	// float -> short
	// float 版の最大値は 1.0f なので TDst の最大値を掛けるだけ
	rElementDst = (unsigned int)(elementSrc * double(ElementUtil::getMaxValue<unsigned int>()) + 0.5f);
}

inline void ConvertSingleElementFunctor::operator()(double elementSrc, float& rElementDst) const
{
	// float -> short
	// float 版の最大値は 1.0f なので TDst の最大値を掛けるだけ
	rElementDst = (float)(elementSrc * double(ElementUtil::getMaxValue<float>()) + 0.5f);
}

#endif
