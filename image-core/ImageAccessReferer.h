#ifndef ImageAccessRefererH
#define ImageAccessRefererH

template<class TImage, class TPixelAccess, class TFuncConvertGet, class TFuncConvertSet, class Traits>
class ImageAccessReferer
{
public:
	typedef typename Traits::TypeValue TypeValue;
	typedef typename Traits::TypeReference TypeReference;
	typedef typename Traits::TypePointer TypePointer;
	typedef typename Traits::TypeAccessValue TypeAccessValue;
	typedef typename Traits::TypeAccessReference TypeAccessReference;
	typedef typename Traits::TypeAccessPointer TypeAccessPointer;

	ImageAccessReferer(TypePointer pPixel, const TFuncConvertGet& rFuncGet, const TFuncConvertSet& rFuncSet)
		: m_pPixel(pPixel), m_rFuncGet(rFuncGet), m_rFuncSet(rFuncSet) {}

	const ImageAccessReferer& operator=(const typename Traits::TypeAccessReference rPixelAccess)
	{
		m_rFuncSet(rPixelAccess, *m_pPixel);
		return *this;
	}
	operator TypeAccessValue() const
	{
		return m_rFuncGet(*m_pPixel);
	}
private:
	const TFuncConvertGet& m_rFuncGet;
	const TFuncConvertSet& m_rFuncSet;
	TypePointer m_pPixel;
};

#endif
