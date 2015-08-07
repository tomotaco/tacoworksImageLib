#ifndef ScaleSingleElementFunctorH
#define ScaleSingleElementFunctorH

#include "ElementUtil.h"

template <typename TElementSrc, typename TElementDst>
struct ScaleSingleElementFunctor
{
	ScaleSingleElementFunctor() : m_valueMinSrc(0), m_valueMaxSrc(ElementUtil::getMaxValue<TElementSrc>()) {}
	ScaleSingleElementFunctor(TElementSrc valueMinSrc, TElementSrc valueMaxSrc)
		 : m_valueMinSrc(valueMinSrc), m_valueMaxSrc(valueMaxSrc) {}

	void operator()(TElementSrc elementSrc, TElementDst& rElementDst) const
	{
		TElementDst valueMinDst = 0;
		TElementDst valueMaxDst = ElementUtil::getMaxValue<TElementDst>();

		if (isEqual(m_valueMaxSrc, m_valueMinSrc) || m_valueMaxSrc < m_valueMinSrc) {
			rElementDst = valueMinDst;
			return;
		}

		if (elementSrc < m_valueMinSrc) {
			rElementDst = valueMinDst;
			return;
		} else if (m_valueMaxSrc < elementSrc) {
			rElementDst = valueMaxDst;
			return;
		} else {
			rElementDst = valueMinDst + TElementDst(double(elementSrc - m_valueMinSrc) * double(valueMaxDst - valueMinDst) / double(m_valueMaxSrc - m_valueMinSrc));
			return;
		}
	}

	TElementSrc m_valueMinSrc;
	TElementSrc m_valueMaxSrc;
};

#endif
