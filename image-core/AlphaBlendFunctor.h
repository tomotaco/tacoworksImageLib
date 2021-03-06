#ifndef AlphaBlendFunctorH
#define AlphaBlendFunctorH

template <typename TypeElement>
struct AlphaBlendFunctor : public std::binary_function<typename TypeElement, typename TypeElement, typename TypeElement>
{
	TypeElement operator()(TypeElement valueAlpha0, TypeElement valueAlpha1) const
	{
		TypeElement valueMax = ElementUtil::getMaxValue<TypeElement>();
		return cropValue<TypeElement>(((double)valueAlpha0 + (double)valueAlpha1 - (double)((int)valueAlpha0 * (int)valueAlpha1) / (double)valueMax), 0, valueMax);
	}
};

// 8bit 版専用
template <>
unsigned char AlphaBlendFunctor<unsigned char>::operator()(unsigned char valueAlpha0, unsigned char valueAlpha1) const
{
	unsigned char valueMax = ElementUtil::getMaxValue<unsigned char>();
	int alpha0 = (int)valueAlpha0;
	int alpha1 = (int)valueAlpha1;
	int alpha0PlusOne = alpha0 + 1;
	int alpha1PlusOne = alpha1 + 1;
	return cropValue<unsigned char>(alpha0 + alpha1 - ((alpha0PlusOne * alpha1PlusOne) >> 8), 0, valueMax);
}

// 16bit 版専用
template <>
unsigned short AlphaBlendFunctor<unsigned short>::operator()(unsigned short valueAlpha0, unsigned short valueAlpha1) const
{
	unsigned short valueMax = ElementUtil::getMaxValue<unsigned short>();
	int alpha0 = (int)valueAlpha0;
	int alpha1 = (int)valueAlpha1;
	int alpha0PlusOne = alpha0 + 1;
	int alpha1PlusOne = alpha1 + 1;
	return cropValue<unsigned short>(alpha0 + alpha1 - ((alpha0PlusOne * alpha1PlusOne) >> 8), 0, valueMax);
}

// SInt, UInt の場合 int で計算すると桁あふれが発生する恐れがあるので…とりあえず専用版は用意しない

#endif