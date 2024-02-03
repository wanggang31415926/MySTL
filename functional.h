#ifndef MY_STL_FUNCTIONAL_H_
#define MY_STL_FUNCTIONAL_H_

// ���ͷ�ļ������� mystl �ĺ����������ϣ����

#include <cstddef>


namespace mystl
{


// ����һԪ�����Ĳ������ͺͷ���ֵ����
template <typename Arg, typename Result>
struct unarg_function
{
	using argument_type = Arg;
	using result_type	= Result;
};

// �����Ԫ�����Ĳ������ͺͷ���ֵ����
template <typename Arg1, typename Arg2, typename Result>
struct binary_function
{
	using first_argument_type	= Arg1;
	using second_argument_type	= Arg2;
	using result_type			= Result;
};


// ��������, �º��� : �ӷ�
template <typename T>
struct plus : public binary_function<T, T, T>
{
	T operator()(const T& x, const T& y) const { return x + y; }
};

// ����
template <typename T>
struct minus : public binary_function<T, T, T>
{
	T operator()(const T& x, const T& y) const { return x - y; }
};

//�˷�
template <typename T>
struct multiplies : public binary_function<T, T, T>
{
	T operator()(const T& x, const T& y) const { return x * y; }
};

// ����
template <typename T>
struct divides : public binary_function<T, T, T> 
{
	T operator()(const T& x, const T& y) const { return x / y; }
};

// ȡģ
template <typename T>
struct modulus : public binary_function<T, T, T>
{
	T operator()(const T& x, const T& y) const { return x % y; }
};

//��
template <typename T>
struct negate : public unarg_function<T, T>
{
	T operator()(const T& x) const { return -x; }
};


// �ӷ���֤ͬԪ��
// ����ѧ�У��ӷ�������һ�������Ԫ�أ���Ϊ֤ͬԪ�أ�identity element����
// ����������ֵ x������֤ͬԪ�غ�Ľ����Ȼ�� x���� x + identity_element = x��
// ͬ��, �˷���ͬԪ��Ϊ1
template <typename T>
T identity_element(plus<T>) { return T(0); }

template <typename T>
T identity_element(multiplies<T>) { return T(1); }

// ����
template <typename T>
struct equal_to : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x == y; }
};

// ������
template <typename T>
struct not_equal_to : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x != y; }
};

//����
template <typename T>
struct greater : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x > y; }
};

//С��
template <typename T>
struct less : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x < y; }
};

//���ڵ���
template <typename T>
struct greater_equal : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x >= y; }
};

//С�ڵ���
template <typename T>
struct less_equal : public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x <= y; }
};

//�߼���
template <typename T>
struct logical_and :public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x && y; }
};

//�߼���
template <typename T>
struct logical_or :public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x || y; }
};

//�߼���
template <typename T>
struct logical_not :public unarg_function<T, bool>
{
	bool operator()(const T& x) const { return !x; }
};


// ֤ͬ����������ı�Ԫ�أ����ر���
template <typename T>
struct identity :public unarg_function<T, bool>
{
	const T& operator()(const T& x) const { return x; }
};

// ѡ����������һ�� pair�����ص�һ��Ԫ��
template <typename Pair>
struct selectfirst :public unarg_function<Pair, typename Pair::first_type>
{
	const typename Pair::first_type& operator()(const Pair& x) const
	{
		return x.first;
	}
};

// ѡ����������һ�� pair�����صڶ���Ԫ��
template <typename Pair>
struct selectsecond :public unarg_function<Pair, typename Pair::second_type>
{
	const typename Pair::second_type& operator()(const Pair& x) const
	{
		return x.second;
	}
};

// Ͷ�亯�������ص�һ����
template <typename Arg1, typename Arg2>
struct projectfirst :public binary_function<Arg1, Arg2, Arg1>
{
	Arg1 operator()(const Arg1& x, const Arg2&) const { return x; }
};

// Ͷ�亯�������صڶ�����
template <typename Arg1, typename Arg2>
struct projectsecond :public binary_function<Arg1, Arg2, Arg1>
{
	Arg2 operator()(const Arg1&, const Arg2& y) const { return y; }
};


/*****************************************************************************************/
// ��ϣ��������

// ���ڴ󲿷����ͣ�hash function ʲô������
template <typename Key>
struct hash{};

// ���ָ��ƫ�ػ�
template <typename T>
struct hash<T*>
{
	size_t operator()(T* p) const noexcept
	{
		return reinterpret_cast<size_t>(p);
	}
};

//��������, ����ԭֵ
#define	MYSTL_TRIVIAL_HASH_FCN(Type)			\
template<> struct hash<Type>					\
{												\
	size_t operator()(Type val) const noexcept	\
	{											\
		return static_cast<size_t>(val);		\
	}											\
};

MYSTL_TRIVIAL_HASH_FCN(bool)

MYSTL_TRIVIAL_HASH_FCN(char)

MYSTL_TRIVIAL_HASH_FCN(signed char)

MYSTL_TRIVIAL_HASH_FCN(unsigned char)

MYSTL_TRIVIAL_HASH_FCN(wchar_t)

MYSTL_TRIVIAL_HASH_FCN(char16_t)

MYSTL_TRIVIAL_HASH_FCN(char32_t)

MYSTL_TRIVIAL_HASH_FCN(short)

MYSTL_TRIVIAL_HASH_FCN(unsigned short)

MYSTL_TRIVIAL_HASH_FCN(int)

MYSTL_TRIVIAL_HASH_FCN(unsigned int)

MYSTL_TRIVIAL_HASH_FCN(long)

MYSTL_TRIVIAL_HASH_FCN(unsigned long)

MYSTL_TRIVIAL_HASH_FCN(long long)

MYSTL_TRIVIAL_HASH_FCN(unsigned long long)

#undef MYSTL_TRIVIAL_HASH_FCN

// ���ڸ�����, ��λ��ϣ
inline size_t bitwise_hash(const unsigned char* first, size_t count)
{
#if (_MSC_VER && _WIN64) || ((__GNUC__ || __clang__) &&__SIZEOF_POINTER__ == 8)
	const size_t fnv_offset = 14695981039346656037ull;
	const size_t fnv_prime = 1099511628211ull;
#else
	const size_t fnv_offset = 2166136261u;
	const size_t fnv_prime = 16777619u;
#endif
	size_t result = fnv_offset;
	for (size_t i = 0; i < count; ++i)
	{
		result ^= (size_t)first[i];
		result *= fnv_prime;
	}
	return result;
}

template <>
struct hash<float>
{
	size_t operator()(const float& val)
	{
		return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(float));
	}
};

template <>
struct hash<double>
{
	size_t operator()(const double& val)
	{
		return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(double));
	}
};

template <>
struct hash<long double>
{
	size_t operator()(const long double& val)
	{
		return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(long double));
	}
};

}

#endif // !MY_STL_FUNCTIONAL_H_

