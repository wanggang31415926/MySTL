#ifndef MY_STL_ALGOBASE_H_
#define MY_STL_ALGOBASE_H_

//ʵ��mystl �Ļ����㷨

#include <cstring>
#include "iterator.h"
#include "util.h"

namespace mystl
{


#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min


/*****************************************************************************************/
// max
// ȡ�����еĽϴ�ֵ���������ʱ��֤���ص�һ������
/*****************************************************************************************/

template <typename T>
const T& max(const T& lhs, const T& rhs)
{
	return lhs > rhs ? lhs : rhs;
}

template <typename T, typename Compare>
const T& max(const T& rhs, const T& lhs, Compare com)
{
	//�º��������� lhs < rhs;
	return com(lhs, rhs) ? rhs : lhs;
}

/*****************************************************************************************/
// min
// ȡ�����еĽϴ�ֵ���������ʱ��֤���ص�һ������
/*****************************************************************************************/

template <typename T>
const T& mim(const T& lhs, const T& rhs)
{
	return lhs < rhs ? lhs : rhs;
}

template <typename T, typename Compare>
const T& mim(const T& rhs, const T& lhs, Compare com)
{
	//�º��������� lhs < rhs;
	return com(lhs, rhs) ? lhs : rhs;
}

/*****************************************************************************************/
// iter_swap
// ��������������ָ����Ե�
/*****************************************************************************************/

template <typename FIter1, typename FIter2>
void iter_swap(FIter1 lhs, FIter2 rhs)
{
	mystl::swap(*lhs, *rhs);
}


/*****************************************************************************************/
// copy
// �� [first, last)�����ڵ�Ԫ�ؿ����� [result, result + (last - first))��
/*****************************************************************************************/
// input_iterator_tag �汾
template <typename InputIter, typename OutputIter>
OutputIter unchecked_copy_cat(InputIter first, InputIter last, OutputIter result,
	mystl::input_iterator_tag)
{
	/*while (first != last)
	{
		*result = *first;
		++first;
		++last;
	}*/

	while (first != last && *first++ = *last++);

	return result;
}

// ramdom_access_iterator_tag �汾
template <typename RandomIter, typename OutputIter>
OutputIter unchecked_copy_cat(RandomIter first, RandomIter last, OutputIter result,
	mystl::random_access_iterator_tag)
{
	for (auto n = last - first; n > 0; --n, ++first, ++result)
	{
		*result = *first;
	}

	return result;
}


template <typename InputIter, typename OutputIter>
OutputIter unchecked_copy(InputIter first, InputIter last, OutputIter result)
{
	return unchecked_copy_cat(first, last, result, iterator_category(first));
}

// Ϊ trivially_copy_assignable �����ṩ�ػ��汾
template <typename T, typename U>
	mystl::enable_if_t<
	mystl::is_same_v<mystl::remove_const_t<T>, U> &&
	std::is_trivially_copy_assignable<U>::value,
	U*>
	unchecked_copy(T* first, T* last, U* result)
{
	const auto n = static_cast<size_t>(last - first);
	if (n != 0)
	{
		std::memmove(result, first, n * sizeof(U));
	}
	return result + n;
}

template <typename InputIter, typename OutputIter>
OutputIter copy(InputIter first, InputIter last, OutputIter result)
{
	return unchecked_copy(first, last, result);
}


/*****************************************************************************************/
// copy_backward
// �� [first, last)�����ڵ�Ԫ�ؿ����� [result - (last - first), result)��
/*****************************************************************************************/
// unchecked_copy_backward_cat �� bidirectional_iterator_tag �汾
template <typename BidirectionalIter1, typename BidirectionalIter2>
BidirectionalIter2
unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
	BidirectionalIter2 result, mystl::bidirectional_iterator_tag)
{
	while (first != last)
	{
		*--result = *--last;
	}
	return result;
}

// unchecked_copy_backward_cat �� random_access_iterator_tag �汾
template <typename RandomIter1, typename BidirectionalIter2>
BidirectionalIter2
unchecked_copy_backward_cat(RandomIter1 first, RandomIter1 last,
	BidirectionalIter2 result, mystl::random_access_iterator_tag)
{
	for (auto n = last - first; n > 0; --n)
	{
		*--result = *--last;
	}
	return result;
}


template <typename BidirectionalIter1, typename BidirectionalIter2>
BidirectionalIter2
unchecked_copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
	BidirectionalIter2 result)
{
	return unchecked_copy_backward_cat(first, last, result,
		iterator_category(first));
}

// Ϊ trivially_copy_assignable �����ṩ�ػ��汾
template <typename T, typename U>
	mystl::enable_if_t<
	mystl::is_same_v<mystl::remove_const_t<T>, U> &&
	std::is_trivially_copy_assignable<U>::value, U*>
	unchecked_copy_backward(T* first, T* last, U* result)
{
	const auto n = static_cast<size_t>(last - first);
	if (n != 0)
	{
		result -= n;
		std::memmove(result, first, n * sizeof(Up));
	}
	return result;
}


template <typename BidirectionalIter1, typename BidirectionalIter2>
BidirectionalIter2
	copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
	return unchecked_copy_backward(first, last, result);
}

/*****************************************************************************************/
// copy_if
// ��[first, last)������һԪ���� unary_pred ��Ԫ�ؿ������� result Ϊ��ʼ��λ����
/*****************************************************************************************/
template <typename InputIter, typename OutputIter, typename UnaryPredicate>
OutputIter copy_if(InputIter first, InputIter last, OutputIter result, UnaryPredicate unary_pred)
{
	/*for (; first != last; ++first)
	{
		if (unary_pred(*first))
			*result++ = *first;
	}*/

	while (first != last)
	{
		if (unary_pred(*first))
		{
			*result++ = *first;
		}
		++first;
	}
	return result;
}


/*****************************************************************************************/
// copy_n
// �� [first, first + n)�����ϵ�Ԫ�ؿ����� [result, result + n)��
// ����һ�� pair �ֱ�ָ�򿽱�������β��
/*****************************************************************************************/
template <typename InputIter, typename Size, typename OutputIter>
mystl::pair<InputIter, OutputIter> 
unchecked_copy_n(InputIter first, Size n, OutputIter result, 
	mystl::input_iterator_tag)
{
	/*for (; n > 0; --n, ++first, ++result)
	{
		*result = *first;
	}*/

	while (n > 0)
	{
		*result++ = *first++;
		--n;
	}
	return mystl::pair<InputIter, OutputIter>(first, result);
}

template <typename RandomIter, typename Size, typename OutputIter>
mystl::pair<RandomIter, OutputIter>
unchecked_copy_n(RandomIter first, Size n, OutputIter result,
	mystl::random_access_iterator_tag)
{
	auto last = first + n;
	return mystl::pair<RandomIter, OutputIter>(last, mystl::copy(first, last, result));
}

template <typename InputIter, typename Size, typename OutputIter>
mystl::pair<InputIter, OutputIter>
copy_n(InputIter first, Size n, OutputIter result)
{
	return unchecked_copy_n(first, n, result, iterator_category(first));
}


/*****************************************************************************************/
// move
// �� [first, last)�����ڵ�Ԫ���ƶ��� [result, result + (last - first))��
/*****************************************************************************************/
// input_iterator_tag �汾
template <class InputIter, class OutputIter>
OutputIter
unchecked_move_cat(InputIter first, InputIter last, OutputIter result,
	mystl::input_iterator_tag)
{
	for (; first != last; ++first, ++result)
	{
		*result = mystl::move(*first);
	}
	return result;
}


}
#endif // !MY_STL_ALGOBASE_H_

