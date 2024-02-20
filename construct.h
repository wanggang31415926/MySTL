#ifndef MY_STL_CONSTRUCT_H_
#define MY_STL_CONSTRUCT_H_

#include <new>

#include "type_traits.h"
#include "util.h"
#include "iterator.h"

/** 
#ifdef _MSC_VER��һ����������ָ������ж��Ƿ�ʹ��Microsoft Visual C++��������
���ʹ�øñ��������ͻ�ִ��#pragma warningָ�����������δ��롣
�ܵ���˵����δ������������Microsoft Visual C++�������£���ʱ���þ���4100��
�Ա����ڱ�������г���δʹ�õĲ����ľ�����Ϣ
*/

#ifdef  _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)		//δʹ�õĲ���
#endif //  _MSC_VER


namespace mystl
{


// construct �������
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


// destroy ����������

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
		//destroy(&*first) ��д����Ϊ��ȷ���ڲ�ͬ���͵ĵ������϶�����ȷ���ö��������������
		// ����������ָ�����͵ĵ�����
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

