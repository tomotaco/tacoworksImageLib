#ifndef ConvertMultipleElementsFunctorH
#define ConvertMultipleElementsFunctorH

#include "ConvertSingleElementFunctor.h"

template<typename TSrc, typename TDst, typename TConvertSingleElementFunctor = ConvertSingleElementFunctor >
struct ConvertMultipleElementsFunctor
{
	void operator()(const Pixel<TSrc, 4>& rPixelSrc, Pixel<TDst, 4>& rPixelDst) const;
	void operator()(const Pixel<TSrc, 3>& rPixelSrc, Pixel<TDst, 3>& rPixelDst) const;
	void operator()(const Pixel<TSrc, 1>& rPixelSrc, Pixel<TDst, 1>& rPixelDst) const;

	TConvertSingleElementFunctor m_convertElement;
};

template<typename TSrc, typename TDst, typename TConvertSingleElementFunctor>
inline void ConvertMultipleElementsFunctor<TSrc, TDst, TConvertSingleElementFunctor>::operator()(const Pixel<TSrc, 4>& rPixelSrc, Pixel<TDst, 4>& rPixelDst) const
{
	TDst valR = 0, valG = 0, valB = 0, valA = 0;
	m_convertElement(rPixelSrc.getR(), valR);
	m_convertElement(rPixelSrc.getG(), valG);
	m_convertElement(rPixelSrc.getB(), valB);
	m_convertElement(rPixelSrc.getA(), valA);

	rPixelDst.setR(valR);
	rPixelDst.setG(valG);
	rPixelDst.setB(valB);
	rPixelDst.setA(valA);
}

template<typename TSrc, typename TDst, typename TConvertSingleElementFunctor>
inline void ConvertMultipleElementsFunctor<TSrc, TDst, TConvertSingleElementFunctor>::operator()(const Pixel<TSrc, 3>& rPixelSrc, Pixel<TDst, 3>& rPixelDst) const
{
	TDst valR = 0, valG = 0, valB = 0;
	m_convertElement(rPixelSrc.getR(), valR);
	m_convertElement(rPixelSrc.getG(), valG);
	m_convertElement(rPixelSrc.getB(), valB);

	rPixelDst.setR(valR);
	rPixelDst.setG(valG);
	rPixelDst.setB(valB);
}

template<typename TSrc, typename TDst, typename TConvertSingleElementFunctor>
inline void ConvertMultipleElementsFunctor<TSrc, TDst, TConvertSingleElementFunctor>::operator()(const Pixel<TSrc, 1>& rPixelSrc, Pixel<TDst, 1>& rPixelDst) const
{
	TDst valA = 0;
	m_convertElement(rPixelSrc.getA(), valA);

	rPixelDst.setA(valA);
}

#endif
