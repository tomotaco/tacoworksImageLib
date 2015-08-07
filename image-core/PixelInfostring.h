#ifndef __PIXEL_INFO_STRING_H_INCLUDED
#define __PIXEL_INFO_STRING_H_INCLUDED

#include <iomanip>
#include <sstream>
#include "Image.h"

template<typename TElement>
struct DecimalElementInfoStrBuilder
{
	std::string operator()(TElement elem) const
	{
		std::ostringstream osstr;
		osstr << elem;
		return osstr.str();
	}
};

template<>
struct DecimalElementInfoStrBuilder<unsigned char>
{
	std::string operator()(unsigned char elem) const
	{
		std::ostringstream osstr;
		osstr << int(elem);
		return osstr.str();
	}
};

template<typename TElement>
struct HexadecimalElementInfoStrBuilder
{
	std::string operator()(unsigned char) const
	{
		// not supported
		assert(0);
		return std::string();
	}
};

template<>
struct HexadecimalElementInfoStrBuilder<unsigned char>
{
	std::string operator()(unsigned char elem) const
	{
		std::ostringstream osstr;
		osstr << std::hex << std::setw(2) << std::setfill('0') << int(elem);
		return osstr.str();
	}
};

template<>
struct HexadecimalElementInfoStrBuilder<unsigned short>
{
	std::string operator()(unsigned short elem) const
	{
		std::ostringstream osstr;
		osstr << std::hex << std::setw(4) << std::setfill('0') << elem;
		return osstr.str();
	}
};

template<>
struct HexadecimalElementInfoStrBuilder<int>
{
	std::string operator()(int elem) const
	{
		std::ostringstream osstr;
		osstr << std::hex << std::setw(8) << std::setfill('0') << elem;
		return osstr.str();
	}
};

template<>
struct HexadecimalElementInfoStrBuilder<unsigned int>
{
	std::string operator()(unsigned int elem) const
	{
		std::ostringstream osstr;
		osstr << std::hex << std::setw(8) << std::setfill('0') << elem;
		return osstr.str();
	}
};


template<class TPixel, template<typename TypeElement> class TypeElementInfoStrBuilder >
struct ChannelInfoStrBuilder
{
	std::string operator()(const TPixel& rPixel) const
	{
		std::string strRet;
		TypeElementInfoStrBuilder<typename TPixel::TypeElement> builderStrInfoElement;
		for (unsigned int indexElement = 0; indexElement < TPixel::numElements; indexElement ++) {
			strRet += builderStrInfoElement(rPixel.getElement(indexElement));
			if (indexElement < TPixel::numElements - 1) {
				strRet +=  ", ";
			}
		}
		return strRet;
	}
};

template<template<typename TypeElement> class TypeElementInfoStrBuilder>
struct PixelInfoString
{
public:
	PixelInfoString(const Point2i& rPosition) : m_position(rPosition) {}

	template<class TImage>
	void operator()(const TImage* pImage)
	{
		ChannelInfoStrBuilder<typename TImage::TypePixel, TypeElementInfoStrBuilder> builderStrInfoChannel;
		m_strInfo = builderStrInfoChannel(pImage->getPixel(m_position));
	}

	void operator()(const ImageBase* pImageBase)
	{
    	assert(0);
	}

	const char* getInfoString()
	{
		return m_strInfo.c_str();
	}

private:

	Point2i m_position;
	std::string m_strInfo;
};


#endif
