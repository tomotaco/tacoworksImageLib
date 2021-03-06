#ifndef StaticSingleDispatcherH
#define StaticSingleDispatcherH

#include "TypeList.h"

/**
与えられたベースクラスのポインタの実体を
派生クラスの型リストから探し、見つかったら Executor を呼び出す。
見つからなかったらベースクラスの NULL ポインタを引数に Executor を呼び出す。
*/

template<class TypeBase, class TypeListConcrete, class Executor>
struct StaticSingleDispatcher
{
	// const 版
	static void dispatch(const TypeBase* pBase, Executor& rExecutor)
	{
		typedef typename TypeListConcrete::Head TypeConcreteHead;
		typedef typename TypeListConcrete::Tail TypeListConcreteNext;
		const TypeConcreteHead* pConcreteHead = dynamic_cast<const TypeConcreteHead*>(pBase);
		if (pConcreteHead) {
			rExecutor(pConcreteHead);
			return;
		}
		return StaticSingleDispatcher<TypeBase, TypeListConcreteNext, Executor>::dispatch(pBase, rExecutor);
	}

	// Non-const 版
	static void dispatch(TypeBase* pBase, Executor& rExecutor)
	{
		typedef typename TypeListConcrete::Head TypeConcreteHead;
		typedef typename TypeListConcrete::Tail TypeListConcreteNext;
		TypeConcreteHead* pConcreteHead = dynamic_cast<TypeConcreteHead*>(pBase);
		if (pConcreteHead) {
			rExecutor(pConcreteHead);
			return;
		}
		return StaticSingleDispatcher<TypeBase, TypeListConcreteNext, Executor>::dispatch(pBase, rExecutor);
	}
};

template<class TypeBase, class Executor>
struct StaticSingleDispatcher<TypeBase, NullType, Executor>
{
	// const 版
	static void dispatch(const TypeBase* pBase, Executor& rExecutor)
	{
		rExecutor(static_cast<const TypeBase*>(NULL));
	}

	// Non-const 版
	static void dispatch(TypeBase* pBase, Executor& rExecutor)
	{
		rExecutor(static_cast<TypeBase*>(NULL));
	}

};

#endif
