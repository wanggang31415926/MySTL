#ifndef MY_STL_TYPE_TRAITS_H_
#define MY_STL_TYPE_TRAITS_H_

// ���ͷ�ļ�������ȡ������Ϣ


#include <type_traits>

namespace mystl
{

//helper struct
template<typename T, T v>
struct m_intergral_constant
{
	static constexpr T value = v;
};

//�ػ�
template <bool b>
using m_bool_constant = m_intergral_constant<bool, b>;

using m_true_type = m_bool_constant<true>;
using m_false_type = m_bool_constant<false>;

/*****************************************************************************************/


}
#endif // !MY_STL_TYPE_TRAITS_H_

