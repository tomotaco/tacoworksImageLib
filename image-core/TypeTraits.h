#ifndef TypeTraitsH
#define TypeTraitsH

// NullType, EmptyType
class NullType {};
struct EmptyType {};

// Select
template <bool flag, typename T, typename U>
struct Select
{
	typedef T Result;
};

template <typename T, typename U>
struct Select<false, T, U>
{
	typedef U Result;
};

/*
// Conversion
// bcc 5.9.3 では通らない
template <class T, class U>
class Conversion
{
	typedef char Small;
	class Big { char dummy[2]; };
	static Small Test(U);
	static Big Test(...);
	static T MakeT();
public:
	static const bool exists = (sizeof(Test(MakeT())) == sizeof(Small));
	static const bool sameType = false;
};

template <class T>
class Conversion<T, T>
{
public:
	static const bool exists = true;
	static const bool sameType = true;
};

#define SUPERSUBCLASS(T, U) ¥
	(Conversion<const U*, const T*>::exists && ¥
	!Conversion<const T*, const void*>::sameType)

#define SUPERSUBCLASS_STRICT(T, U) ¥
	(SUPERSUBCLASS(T, U) && ¥
	!Conversion<const T, const U>::sameType)
*/
#endif

