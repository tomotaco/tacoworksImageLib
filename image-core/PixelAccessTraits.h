#ifndef PixelAccessTraitsH
#define PixelAccessTraitsH

template <class _Type, class _TypeAccess, class _Container>
struct TraitsAccessNonConst;

template <class _Type, class _TypeAccess, class _Container>
struct TraitsAccessConst
{
	typedef _Type TypeValue;
	typedef const _Type&  TypeReference;
	typedef const _Type*  TypePointer;
	typedef _TypeAccess TypeAccessValue;
	typedef const _TypeAccess&  TypeAccessReference;
	typedef const _TypeAccess*  TypeAccessPointer;
	typedef _Container TypeContainer;
	typedef const _Container& TypeContainerReference;
	typedef const _Container* TypeContainerPointer;
};

template <class _Type, class _TypeAccess, class _Container>
struct TraitsAccessNonConst
{
	typedef _Type TypeValue;
	typedef _Type& TypeReference;
	typedef _Type* TypePointer;
	typedef _TypeAccess TypeAccessValue;
	typedef _TypeAccess& TypeAccessReference;
	typedef _TypeAccess* TypeAccessPointer;
	typedef _Container TypeContainer;
	typedef _Container& TypeContainerReference;
	typedef _Container* TypeContainerPointer;
};

#endif
