#ifndef PixelTraitsH
#define PixelTraitsH

template <class _Type, class _Container>
struct TraitsNonConst;

template <class _Type, class _Container>
struct TraitsConst
{
	typedef _Type TypeValue;
	typedef const _Type&  TypeReference;
	typedef const _Type*  TypePointer;
	typedef _Container TypeContainer;
	typedef const _Container& TypeContainerReference;
	typedef const _Container* TypeContainerPointer;
};

template <class _Type, class _Container>
struct TraitsNonConst
{
	typedef _Type TypeValue;
	typedef _Type& TypeReference;
	typedef _Type* TypePointer;
	typedef _Container TypeContainer;
	typedef _Container& TypeContainerReference;
	typedef _Container* TypeContainerPointer;
};


#endif
