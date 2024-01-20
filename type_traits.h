#ifndef MY_STL_TYPE_TRAITS_H_
#define MY_STL_TYPE_TRAITS_H_

// 这个头文件用于提取类型信息


#include <type_traits>

namespace mystl
{

//helper struct
template<typename Tp, Tp val>
struct m_intergral_constant
{
	static constexpr Tp value = val;
	using value_type = Tp;
	using type = m_intergral_constant<Tp, val>;

	constexpr operator value_type() const noexcept { return value; }
	constexpr operator value_type()() const noexcept { return value; }

};

//template<typename Tp, Tp v>
//constexpr Tp m_integral_constant<Tp, v>::value;

//特化
template <bool v>
using m_bool_constant = m_intergral_constant<bool, v>;

using m_true_type = m_bool_constant<true>;
using m_false_type = m_bool_constant<false>;



/*****************************************************************************************/

template<typename T1, typename T2>
struct pair;

template <typename T> 
struct is_pair : mystl::m_false_type {};

template <typename T1, typename T2>
struct is_pair<mystl::pair<T1, T2>> : mystl::m_true_type {};
}	//namespace mystl

#endif // !MY_STL_TYPE_TRAITS_H_

