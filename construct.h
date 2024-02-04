#ifndef MY_STL_CONSTRUCT_H_
#define MY_STL_CONSTRUCT_H_

#include <new>

#include "type_traits.h"
#include "util.h"
#include "iterator.h"

/** 
#ifdef _MSC_VER是一个条件编译指令，用于判断是否使用Microsoft Visual C++编译器。
如果使用该编译器，就会执行#pragma warning指令，否则会忽略这段代码。
总的来说，这段代码的作用是在Microsoft Visual C++编译器下，暂时禁用警告4100，
以避免在编译过程中出现未使用的参数的警告信息
*/

#ifdef  _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)		//未使用的参数
#endif //  _MSC_VER


namespace mystl
{


// construct 构造对象
template <typename Ty>
void construct(Ty* ptr)
{
	::new ((void*)ptr) Ty();
}

template <typename Ty1, typename Ty2>
void construct(Ty1* ptr, Ty2& value)
{
	::new ((void*)ptr) Ty1(value);
}

template <typename Ty, typename...Args>
void construct(Ty* ptr, Args&& ...args)
{
	::new ((void*)ptr) Ty(mystl::forward<Args>(args)...);
}


// destroy 将对象析构

template <typename Ty>
void destroy_one(Ty*, std::true_type) {}


template <typename Ty>
void destroy_one(Ty* ptr, std::false_type) 
{
	if (ptr != nullptr)
	{
		ptr->~Ty();
	}
}

template <typename ForwardIter>
void destroy_cat(ForwardIter, ForwardIter, std::true_type) {}

template <typename ForwardIter>
void destroy_cat(ForwardIter first, ForwardIter last, std::false_type) 
{
	for (; first != last; ++first)
	{
		//destroy(&*first) 的写法是为了确保在不同类型的迭代器上都能正确调用对象的析构函数，
		// 而不仅仅是指针类型的迭代器
		destroy(&*first);
	}
}

template <typename Ty>
void destroy(Ty* ptr)
{
	destroy_one(ptr, std::is_trivially_destructible<Ty>{});
}

template <typename ForwardIter>
void destroy(ForwardIter first, ForwardIter last)
{
	destroy_cat(first, last, std::is_trivially_destructible<
		 typename iterator_traits<ForwardIter>::value_type>{});
}

}// namespace mystl

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSV_VER

#endif // !MY_STL_CONSTRUCT_H_

