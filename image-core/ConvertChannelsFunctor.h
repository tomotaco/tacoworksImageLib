#ifndef ConvertChannelsFunctorH
#define ConvertChannelsFunctorH

#include "Pixel.h"

template<typename T>
struct ConvertChannelsFunctor
{
	void operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 4>& rPixelDst) const;
	void operator()(const Pixel<T, 3>& rPixelSrc, Pixel<T, 3>& rPixelDst) const;
	void operator()(const Pixel<T, 1>& rPixelSrc, Pixel<T, 1>& rPixelDst) const;
	void operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 3>& rPixelDst) const;
	void operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 1>& rPixelDst) const;
	void operator()(const Pixel<T, 3>& rPixelSrc, Pixel<T, 4>& rPixelDst) const;
	void operator()(const Pixel<T, 3>& rPixelSrc, Pixel<T, 1>& rPixelDst) const;
	void operator()(const Pixel<T, 1>& rPixelSrc, Pixel<T, 4>& rPixelDst) const;
	void operator()(const Pixel<T, 1>& rPixelSrc, Pixel<T, 3>& rPixelDst) const;
};

template<typename T>
struct ExtractRChannelFunctor
{
	void operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 1>& rPixelDst) const;
	void operator()(const Pixel<T, 3>& rPixelSrc, Pixel<T, 1>& rPixelDst) const;
};

template<typename T>
struct ExtractGChannelFunctor
{
	void operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 1>& rPixelDst) const;
	void operator()(const Pixel<T, 3>& rPixelSrc, Pixel<T, 1>& rPixelDst) const;
};

template<typename T>
struct ExtractBChannelFunctor
{
	void operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 1>& rPixelDst) const;
	void operator()(const Pixel<T, 3>& rPixelSrc, Pixel<T, 1>& rPixelDst) const;
};

template<typename T>
struct ExtractAChannelFunctor
{
	void operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 1>& rPixelDst) const;
};

template<typename T>
inline void ConvertChannelsFunctor<T>::operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 4>& rPixelDst) const
{
	rPixelDst.setR(rPixelSrc.getR());
	rPixelDst.setG(rPixelSrc.getG());
	rPixelDst.setB(rPixelSrc.getB());
	rPixelDst.setA(rPixelSrc.getA());
}

template<typename T>
inline void ConvertChannelsFunctor<T>::operator()(const Pixel<T, 3>& rPixelSrc, Pixel<T, 3>& rPixelDst) const
{
	rPixelDst.setR(rPixelSrc.getR());
	rPixelDst.setG(rPixelSrc.getG());
	rPixelDst.setB(rPixelSrc.getB());
}

template<typename T>
inline void ConvertChannelsFunctor<T>::operator()(const Pixel<T, 1>& rPixelSrc, Pixel<T, 1>& rPixelDst) const
{
	rPixelDst.setA(rPixelSrc.getA());
}

template<typename T>
inline void ConvertChannelsFunctor<T>::operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 3>& rPixelDst) const
{
	rPixelDst.setR(rPixelSrc.getR());
	rPixelDst.setG(rPixelSrc.getG());
	rPixelDst.setB(rPixelSrc.getB());
}

template<typename T>
inline void ConvertChannelsFunctor<T>::operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 1>& rPixelDst) const
{
	rPixelDst.setA((rPixelSrc.getR() + rPixelSrc.getG() + rPixelSrc.getB()) / 3);
}

template<typename T>
inline void ConvertChannelsFunctor<T>::operator()(const Pixel<T, 3>& rPixelSrc, Pixel<T, 4>& rPixelDst) const
{
	rPixelDst.setR(rPixelSrc.getR());
	rPixelDst.setG(rPixelSrc.getG());
	rPixelDst.setB(rPixelSrc.getB());
	rPixelDst.setA(ElementUtil::getMaxValue<T>());
}

template<typename T>
inline void ConvertChannelsFunctor<T>::operator()(const Pixel<T, 3>& rPixelSrc, Pixel<T, 1>& rPixelDst) const
{
	rPixelDst.setA((rPixelSrc.getR() + rPixelSrc.getG() + rPixelSrc.getB()) / 3);
}

template<typename T>
inline void ConvertChannelsFunctor<T>::operator()(const Pixel<T, 1>& rPixelSrc, Pixel<T, 4>& rPixelDst) const
{
	rPixelDst.setR(rPixelSrc.getA());
	rPixelDst.setG(rPixelSrc.getA());
	rPixelDst.setB(rPixelSrc.getA());
	rPixelDst.setA(ElementUtil::getMaxValue<T>());
}

template<typename T>
inline void ConvertChannelsFunctor<T>::operator()(const Pixel<T, 1>& rPixelSrc, Pixel<T, 3>& rPixelDst) const
{
	rPixelDst.setR(rPixelSrc.getA());
	rPixelDst.setG(rPixelSrc.getA());
	rPixelDst.setB(rPixelSrc.getA());
}

template<typename T>
inline void ExtractRChannelFunctor<T>::operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 1>& rPixelDst) const
{
	rPixelDst.setA(rPixelSrc.getR());
}

template<typename T>
inline void ExtractRChannelFunctor<T>::operator()(const Pixel<T, 3>& rPixelSrc, Pixel<T, 1>& rPixelDst) const
{
	rPixelDst.setA(rPixelSrc.getR());
}

template<typename T>
inline void ExtractGChannelFunctor<T>::operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 1>& rPixelDst) const
{
	rPixelDst.setA(rPixelSrc.getG());
}

template<typename T>
inline void ExtractGChannelFunctor<T>::operator()(const Pixel<T, 3>& rPixelSrc, Pixel<T, 1>& rPixelDst) const
{
	rPixelDst.setA(rPixelSrc.getG());
}

template<typename T>
inline void ExtractBChannelFunctor<T>::operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 1>& rPixelDst) const
{
	rPixelDst.setA(rPixelSrc.getB());
}

template<typename T>
inline void ExtractBChannelFunctor<T>::operator()(const Pixel<T, 3>& rPixelSrc, Pixel<T, 1>& rPixelDst) const
{
	rPixelDst.setA(rPixelSrc.getB());
}

template<typename T>
inline void ExtractAChannelFunctor<T>::operator()(const Pixel<T, 4>& rPixelSrc, Pixel<T, 1>& rPixelDst) const
{
	rPixelDst.setA(rPixelSrc.getA());
}

#endif
