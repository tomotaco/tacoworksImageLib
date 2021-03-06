#ifndef IntensityBlendFunctorH
#define IntensityBlendFunctorH

template <typename TypeElement>
struct IntensityBlendFunctor
{
	TypeElement operator()(TypeElement valueAlpha, TypeElement valueBg, TypeElement valueFg) const
	{
		TypeElement valueMax = ElementUtil::getMaxValue<TypeElement();
		double alpha = (double)valueAlpha / (double)valueMax;
		double alphaNeg = 1.0f -  alpha;

		return cropValue<TypeElement>((TypeElement)((double)valueBg * alphaNeg + (double)valueFg * alpha), 0, valueMax);
	}
};

// 8bit 版専用
template<>
inline unsigned char IntensityBlendFunctor<unsigned char>::operator()(unsigned char alpha, unsigned char valueBg, unsigned char valueFg) const
{
	// 0 - 255 のα合成は 255 で割る必要があるが、
	// 1 - 256 の範囲で演算すると、8bit シフトで済む
	unsigned int valueMax = ElementUtil::getMaxValue<unsigned char>();
	unsigned int alphaPlusOne = alpha + 1;
	unsigned int alphaNegPlusOne = valueMax - alpha + 1;
	return unsigned char(cropValue<unsigned int>((unsigned int(valueBg) * alphaNegPlusOne + unsigned int(valueFg) * alphaPlusOne) >> 8, 0, valueMax));
}
// 16bit 版, SInt, UInt 版は未実装のため上の double 経由版が使われる

#endif