#ifndef MY_STL_ALGOBASE_H_
#define MY_STL_ALGOBASE_H_

//实现mystl 的基本算法

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
// 取二者中的较大值，语义相等时保证返回第一个参数
/*****************************************************************************************/

template <typename T>
const T& max(const T& lhs, const T& rhs)
{
	return lhs > rhs ? lhs : rhs;
}

template <typename T, typename Compare>
const T& max(const T& rhs, const T& lhs, Compare com)
{
	//仿函数里面是 lhs < rhs;
	return com(lhs, rhs) ? rhs : lhs;
}

/*****************************************************************************************/
// min
// 取二者中的较大值，语义相等时保证返回第一个参数
/*****************************************************************************************/

template <typename T>
const T& min(const T& lhs, const T& rhs)
{
	return lhs < rhs ? lhs : rhs;
}

template <typename T, typename Compare>
const T& min(const T& rhs, const T& lhs, Compare com)
{
	//仿函数里面是 lhs < rhs;
	return com(lhs, rhs) ? lhs : rhs;
}

/*****************************************************************************************/
// iter_swap
// 将两个迭代器所指对象对调
/*****************************************************************************************/

template <typename FIter1, typename FIter2>
void iter_swap(FIter1 lhs, FIter2 rhs)
{
	mystl::swap(*lhs, *rhs);
}


/*****************************************************************************************/
// copy
// 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
/*****************************************************************************************/
// input_iterator_tag 版本
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

// ramdom_access_iterator_tag 版本
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

// 为 trivially_copy_assignable 类型提供特化版本
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
// 将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
/*****************************************************************************************/
// unchecked_copy_backward_cat 的 bidirectional_iterator_tag 版本
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

// unchecked_copy_backward_cat 的 random_access_iterator_tag 版本
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

// 为 trivially_copy_assignable 类型提供特化版本
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
// 把[first, last)内满足一元操作 unary_pred 的元素拷贝到以 result 为起始的位置上
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
// 把 [first, first + n)区间上的元素拷贝到 [result, result + n)上
// 返回一个 pair 分别指向拷贝结束的尾部
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
// 把 [first, last)区间内的元素移动到 [result, result + (last - first))内
/*****************************************************************************************/
// input_iterator_tag 版本
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


// ramdom_access_iterator_tag 版本
template <typename RandomIter, typename OutputIter>
OutputIter
unchecked_move_cat(RandomIter first, RandomIter last, OutputIter result,
	mystl::random_access_iterator_tag)
{
	for (auto n = last - first; n > 0; --n, ++first, ++result)
	{
		*result = mystl::move(*first);
	}
	return result;
}

template <typename InputIter, typename OutputIter>
OutputIter
unchecked_move(InputIter first, InputIter last, OutputIter result)
{
	return unchecked_move_cat(first, last, result, iterator_category(first));
}

// 为 trivially_copy_assignable 类型提供特化版本
template <typename T, typename U>
typename mystl::enable_if_t<
	mystl::is_same_v<mystl::remove_const_t<T>, U> &&
	std::is_trivially_move_assignable<U>::value, U*>
	unchecked_move(T* first, T* last, U* result)
{
	const size_t n = static_cast<size_t>(last - first);
	if (n != 0) 
	{
		std::memmove(result, first, n * sizeof(U));
	}
	return result + n;
}

template <typename InputIter, typename OutputIter>
OutputIter move(InputIter first, InputIter last, OutputIter result)
{
	return unchecked_move(first, last, result);
}

/*****************************************************************************************/
// move_backward
// 将 [first, last)区间内的元素移动到 [result - (last - first), result)内
/*****************************************************************************************/
// bidirectional_iterator_tag 版本
template <typename BidirectionalIter1, typename BidirectionalIter2>
BidirectionalIter2
unchecked_move_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
	BidirectionalIter2 result, mystl::bidirectional_iterator_tag)
{
	while (first != last)
	{
		*--result = mystl::move(*--last);
	}
	return result;
}

// random_access_iterator_tag 版本
template <typename RandomIter1, typename RandomIter2>
RandomIter2
unchecked_move_backward_cat(RandomIter1 first, RandomIter1 last,
	RandomIter2 result, mystl::random_access_iterator_tag)
{
	for (auto n = last - first; n > 0; --n) 
	{
		*--result = mystl::move(*--last);
	}
	return result;
}

template <typename BidirectionalIter1, typename BidirectionalIter2>
BidirectionalIter2
unchecked_move_backward(BidirectionalIter1 first, BidirectionalIter1 last,
	BidirectionalIter2 result)
{
	return unchecked_move_backward_cat(first, last, result,
		iterator_category(first));
}

// 为 trivially_copy_assignable 类型提供特化版本
template <typename T, typename U>
typename mystl::enable_if_t<
	mystl::is_same_v<mystl::remove_const_t<T>, U> &&
	std::is_trivially_move_assignable<U>::value, U*>
	unchecked_move_backward(T* first, T* last, U* result)
{
	const size_t n = static_cast<size_t>(last - first);
	if (n != 0)
	{
		result -= n;
		std::memmove(result, first, n * sizeof(U));
	}
	return result;
}

template <typename BidirectionalIter1, typename BidirectionalIter2>
BidirectionalIter2
move_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
	return unchecked_move_backward(first, last, result);
}


/*****************************************************************************************/
// equal
// 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
/*****************************************************************************************/
template <typename InputIter1, typename InputIter2>
bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2)
{
	for (; first1 != last1; ++first1, ++first2)
	{
		if (*first1 != *first2)
			return false;
	}
	return true;
}

// 重载版本使用函数对象 comp 代替比较操作
template <typename InputIter1, typename InputIter2, typename Compared>
bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compared comp)
{
	for (; first1 != last1; ++first1, ++first2)
	{
		if (!comp(*first1, *first2))
			return false;
	}
	return true;
}


/*****************************************************************************************/
// fill_n
// 从 first 位置开始填充 n 个值
/*****************************************************************************************/
template <typename OutputIter, typename Size, typename T>
OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value)
{
	/*for (; n > 0; --n, ++first)
	{
		*first = value;
	}*/
	while (n > 0)
	{
		*first = value;
		--n;
		++first;
	}
	return first;
}

// 为 one-byte 类型提供特化版本
template <typename T, typename Size, typename U>
	mystl::enable_if_t<
	std::is_integral<T>::value && sizeof(T) == 1 &&
	!mystl::is_same_v<T, bool> &&
	std::is_integral<U>::value && sizeof(U) == 1,T*>
	unchecked_fill_n(T* first, Size n, U value)
{
	if (n > 0)
	{
		std::memset(first, (unsigned char)value, (size_t)(n));
	}
	return first + n;
}

template <typename OutputIter, typename Size, typename T>
OutputIter fill_n(OutputIter first, Size n, const T& value)
{
	return unchecked_fill_n(first, n, value);
}

/*****************************************************************************************/
// fill
// 为 [first, last)区间内的所有元素填充新值
/*****************************************************************************************/
template <typename ForwardIter, typename T>
void fill_cat(ForwardIter first, ForwardIter last, const T& value,
	mystl::forward_iterator_tag)
{
	/*for (; first != last; ++first)
	{
		*first = value;
	}*/
	while (first != last)
	{
		*first = value;
		++first;
	}
}

template <typename RandomIter, typename T>
void fill_cat(RandomIter first, RandomIter last, const T& value,
	mystl::random_access_iterator_tag)
{
	fill_n(first, last - first, value);
}

template <typename ForwardIter, typename T>
void fill(ForwardIter first, ForwardIter last, const T& value)
{
	fill_cat(first, last, value, iterator_category(first));
}


/*****************************************************************************************/
// lexicographical_compare
// 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
// (1)如果第一序列的元素较小，返回 true ，否则返回 false
// (2)如果到达 last1 而尚未到达 last2 返回 true
// (3)如果到达 last2 而尚未到达 last1 返回 false
// (4)如果同时到达 last1 和 last2 返回 false
/*****************************************************************************************/
template <typename InputIter1, typename InputIter2>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
	InputIter2 first2, InputIter2 last2)
{
	/*for (; first1 != last1 && first2 != last2; ++first1, ++first2)
	{
		if (*first1 < *first2)
			return true;
		if (*first2 < *first1)
			return false;
	}*/

	while (first1 != last1 && first2 != last2)
	{
		if (*first1 < *first2) 
		{
			return true;
		}

		if (*first2 < *first1)
		{
			return false;
		}
		++first1;
		++first2;
	}
	return first1 == last1 && first2 != last2;
}

// 重载版本使用函数对象 comp 代替比较操作
template <typename InputIter1, typename InputIter2, typename Compred>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
	InputIter2 first2, InputIter2 last2, Compred comp)
{
	/*for (; first1 != last1 && first2 != last2; ++first1, ++first2)
	{
		if (comp(*first1, *first2))
			return true;
		if (comp(*first2, *first1))
			return false;
	}*/

	while (first1 != last1 && first2 != last2)
	{
		if (comp(*first1 < *first2))
		{
			return true;
		}

		if (comp(*first2 < *first1))
		{
			return false;
		}
		++first1;
		++first2;
	}
	return first1 == last1 && first2 != last2;
}

// 针对 const unsigned char* 的特化版本
bool lexicographical_compare(const unsigned char* first1,
	const unsigned char* last1,
	const unsigned char* first2,
	const unsigned char* last2)
{
	const auto len1 = last1 - first1;
	const auto len2 = last2 - first2;
	// 先比较相同长度的部分
	const auto result = std::memcmp(first1, first2, mystl::min(len1, len2));
	// 若相等，长度较长的比较大
	return result != 0 ? result < 0 : len1 < len2;
}


/*****************************************************************************************/
// mismatch
// 平行比较两个序列，找到第一处失配的元素，返回一对迭代器，分别指向两个序列中失配的元素
/*****************************************************************************************/
template <typename InputIter1, typename InputIter2>
mystl::pair<InputIter1, InputIter2>
mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2)
{
	while (first1 != last1 && *first1 == *first2)
	{
		++first1;
		++first2;
	}
	return mystl::pair<InputIter1, InputIter2>(first1, first2);
}

// 重载版本使用函数对象 comp 代替比较操作
template <typename InputIter1, typename InputIter2, typename Compred>
mystl::pair<InputIter1, InputIter2>
mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compred comp)
{
	while (first1 != last1 && comp(*first1, *first2))
	{
		++first1;
		++first2;
	}
	return mystl::pair<InputIter1, InputIter2>(first1, first2);
}



}
#endif // !MY_STL_ALGOBASE_H_

