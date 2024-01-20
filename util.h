#ifndef MY_STL_UTIL_H_
#define MY_STL_UTIL_H_

// 这个文件包含一些通用工具，包括 move, forward, swap 等函数，以及 pair 等 

#include <cstddef>

#include "type_traits.h"

namespace mystl
{
// 实现is_same
// 类型不一样值为false
template <typename, typename>
struct is_same :public false_type {};

template <typename T>
struct is_same<T, T> :public true_type {};

template <typename T, typename U>
constexpr bool is_same_v = is_same<T, U>::value;

//基类, 存类型
template <typename Type>
struct Type_is
{
    using type = Type;
};


template <typename T>
struct remove_reference : public Type_is<T> {};

template <typename T>
struct remove_reference<T&> : public Type_is<T> {};

template <typename T>
struct remove_reference<T&&> : public Type_is<T> {};

template<typename T>
using remove_reference_t = remove_reference<T>::type;


template <typename T>
struct remove_const : public Type_is<T> {};

template <typename T>
struct remove_const<const T> : public Type_is<T> {};

template <typename T>
struct remove_const<const T*> : public Type_is<T*> {};

template<typename T>
using remove_const_t = remove_const<T>::type;


//判断是不是左值引用
template <typename>
constexpr bool is_lvalue_reference_v = false;

template <typename Tp>
constexpr bool is_lvalue_reference_v<Tp&> = true;

template <typename Tp>
struct is_lvalue_reference : m_bool_constant<is_lvalue_reference_v<Tp>> {};

//判断是不是右值引用
template <typename>
constexpr bool is_rvalue_reference_v = false;

template <typename Tp>
constexpr bool is_rvalue_reference_v<Tp&&> = true;

template <typename Tp>
struct is_rvalue_reference : m_bool_constant<is_rvalue_reference_v<Tp>> {};

// 判断是不是引用
template <typename>
constexpr bool is_reference_v = false;

template <typename Tp>
constexpr bool is_reference_v<Tp&&> = true;

template <typename Tp>
constexpr bool is_reference_v<Tp&> = true;

template <typename Tp>
struct is_reference : m_bool_constant<is_reference_v<Tp>> {};

//判段是不是指针
template <typename>
constexpr bool is_pointer_v = false;

template<typename Tp>
constexpr bool is_pointer_v<Tp*> = true;

template<typename Tp>
constexpr bool is_pointer_v<Tp* const> = true;

template<typename Tp>
constexpr bool is_pointer_v<Tp* volatile> = true;

template<typename Tp>
constexpr bool is_pointer_v<Tp* const volatile> = true;

template <typename Tp>
struct is_pointer : m_bool_constant<is_pointer_v<TP>> {};

// 仅当布尔常量为 true 时，才定义成员 type。
template <bool, typename Tp = void>
struct enable_if {};

template <typename Tp>
struct enable_if<true, Tp>
{
    using type = Tp;
};

template<bool Cond, typename Tp = void>
using enable_if_t = typename enable_if<Cond, Tp>::type;


//类型退化
template <typename T>
class decay
{
    using no_ref_type = remove_reference_t<T>;
public:
    using type = remove_const_t<no_ref_type>;
};

template <typename T>
using decay_t = decay<T>::type;

//move
template <typename T>
remove_reference_t<T>&& move(T&& __t) noexcept
{
    return static_cast<remove_reference_t<T>&&>(__t);
}

// forward

template <typename T>
T&& forward(remove_reference_t<T>& arg) noexcept
{
    return static_cast<remove_reference_t<T>&&>(arg);
}

template <typename T>
T&& forward(remove_reference_t<T>&& arg) noexcept
{
    //static_assert(!is_lvalue_reference_v<T>, "bad forward");
    static_assert(!is_lvalue_reference<T>::value, "bad forward");
    return static_cast<remove_reference_t<T>&&>(arg);
}

template <typename Tp>
void swap(Tp& lhs, Tp& rhs)
{
    auto tmp{ move(lhs) };
    lsh = mystl::move(rhs);
    rhs = mystl::move(tmp);
}

//first1 和 last1 是第一个范围的起始和结束迭代器。
//first2 是第二个范围的起始迭代器。
//swap_ranges 函数会将第一个范围[first1, last1) 中的元素与第二个范围[first2, first2 + (last1 - first1)) 中的元素进行交换。
//返回值：返回一个迭代器，指向第二个范围中最后一个被交换的元素之后的位置。

template <typename ForwardIter1, typename ForwardIter2>
ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
{
    while (first1 != last1)
    {
        mystl::swap(*first1, *first2);
        ++first1;
        (void) ++first2;
    }
    /*for (; first1 != last1; ++first1, (void) ++first2)
        mystl::swap(*first1, *first2);*/
    return first2;
}

template <typename Tp, size_t N>
void swap(Tp(&a)[N], Tp(&b)[N])
{
    mystl::swap_range(a, a + N, b);
}


/***************************************************************************/

//pair

template <typename T1, typename T2>
struct pair
{
    using first_type    = T1;
    using second_type   = T2;

    first_type first_;
    second_type second_;

    //默认可构造
    //enable_if 仅当布尔常量为 true 时，才定义成员 type。
    //is_default_constructible用于检查给定类型是否具有默认构造函数。
    //如果一个类型具有默认构造函数，则可以使用无参数的构造函数来创建该类型的对象。
    //提供一个名为 value 的静态成员常量
    //如果给定类型具有默认构造函数，则 value 为 true，否则为 false。
    template<typename U1 = T1, typename U2 = T2,
        std::enable_if<
        std::is_default_constructible<U1>::value &&
        std::is_default_constructible<U2>::value, void>::type>
    constexpr pair() : first_(), second_()  {}


    //此类型的隐式可构造对象
    //is_copy_constructible 用于检查给定类型是否具有拷贝构造函数。
    //提供一个名为 value 的静态成员常量，
    // 如果给定类型具有拷贝构造函数，则 value 为 true，否则为 false。
    
    //is_convertible 用于检查一个类型是否可以隐式转换为另一个类型。
    //提供一个名为 value 的静态成员常量
    //如果源类型可以隐式转换为目标类型，则 value 为 true，否则为 false。

    template<typename U1 = T1, typename U2 = T2,
        std::enable_if <
        std::is_copy_constructible<U1>::value &&
        std::is_copy_constructible<U2>::value &&
        std::is_convertible<const U1&, T1>::value &&
        std::is_convertible<const U2&, T2>::value, int>::type = 0>
    constexpr pair(const T1& first, const T2& second)
        : first_( first ), second_( second ) {}


    //显式可构造
    template <class U1 = T1, class U2 = T2,
        std::enable_if<
        std::is_copy_constructible<U1>::value &&
        std::is_copy_constructible<U2>::value &&
        (!std::is_convertible<const U1&, T1>::value ||
         !std::is_convertible<const U2&, T2>::value), int>::type = 0>
    explicit constexpr pair(const T1& first, const T2& second)
        : first_{ first }, second_{ second } {}

    pair(const pair& rhs) = default;
    pair(pair&& rhs) = default;

    template <typename U1, typename U2, 
        std::enable_if<
        std::is_constructible<T1, U1>::value &&
        std::is_constructible<T2, U2>::value &&
        std::is_convertible<U1&&, T1>::value &&
        std::is_convertible<U2&&, T2>::value, int>::type = 0>
    constexpr pair (U1&& first, U2&& second)
        : first_(mystl::forward<U1>(first)),
        second_(mystl::forward<U2>(second)) {}

    template <typename U1, typename U2,
        std::enable_if <
        std::is_constructible<T1, U1>::value&&
        std::is_constructible<T2, U2>::value &&
        (!std::is_convertible<U1, T1>::value ||
         !std::is_convertible<U2, T2>::value), int>::type = 0>
    explicit constexpr pair(U1&& first, U2&& second)
        : first_(mystl::forward<U1>(first)), 
        second_(mystl::forward<U2>(second)) {}


    template <typename U1, typename U2,
        std::enable_if<
        std::is_constructible<T1, const U1&>::value &&
        std::is_constructible<T2, const U2&>::value &&
        std::is_convertible<const U1&, T1>::value &&
        std::is_convertible<const U2&, T2>::value, int>::type = 0>
    constexpr pair(const pair<U1, U2>& other)
        : first_(other.first_), second_(other.second_) {}

    template <typename U1, typename U2,
        std::enable_if<
        std::is_constructible<T1, const U1&>::value &&
        std::is_constructible<T2, const U2&>::value &&
        (!std::is_convertible<const U1&, T1>::value ||
         !std::is_convertible<const U2&, T2>::value), int>::type = 0>
    explicit constexpr pair(const pair<U1, U2>& other)
        : first_(other.first_), second_(other.second_) {}


    template <typename U1, typename U2,
        std::enable_if<
        std::is_constructible<T1, U1>::value &&
        std::is_constructible<T2, U2>::value &&
        std::is_convertible<U1, T1>::value &&
        std::is_convertible<U2, T2>::value, int>::type = 0>
    constexpr pair(pair<U1, U2>&& other)
        : first_(mystl::forward<U1>(other.first_)),
        second_(mystl::forward<U2>(other.second_)) {}


    template <class U1, class U2,
        std::enable_if<
        std::is_constructible<T1, U1>::value &&
        std::is_constructible<T2, U2>::value &&
        (!std::is_convertible<U1, T1>::value ||
         !std::is_convertible<U2, T2>::value), int>::type = 0>
    explicit constexpr pair(pair<U1, U2>&& other)
        : first_(mystl::forward<U1>(other.first_)),
        second_(mystl::forward<U2>(other.second_)) {}

    pair& operator = (const pair& rhs)
    {
        if (this != &rhs)
        {
            first_ = rhs.first_;
            second = rhs.second_;
        }
        return *this;
    }

    pair& operator = (pair&& rhs)
    {
        if (this != &rhs)
        {
            first_ = mystl::move(rhs.first_);
            second_ = mystl::move(rhs.second_);
        }
        return *this;
    }

    template <typename U1, typename U2>
    pair<U1, U2>& operator = (const pair<U1, U2>& rhs)
    {
        if (this != &rhs)
        {
            first_ = rhs.first_;
            second = rhs.second_;
        }
        return *this;
    }

    template <typename U1, typename U2>
    pair<U1, U2>& operator = (pair<U1, U2>&& rhs)
    {
        if (this != &rhs)
        {
            first_ = mystl::forward<U1>(rhs.first_);
            second_ = mystl::forward<U2>(rhs.second_);
        }
        return *this;
    }

    ~pair() = default;

    void swap(pair& other)
    {
        if (this != &other)
        {
            mystl::swap(first_, other.first_);
            mystl::swap(second_, other.second_);
        }
    }

};


// 重载运算符
template <typename T1, typename T2>
bool operator == (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return lhs.first_ = rhs.first_ && lhs.second_ = rhs.second_;
}

template <typename T1, typename T2>
bool operator < (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
}

template <typename T1, typename T2>
bool operator != (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return !(lhs == rhs);
}

template <typename T1, typename T2>
bool operator > (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return rhs < lhs;
}

template <typename T1, typename T2>
bool operator <= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return !(rhs < lhs);
}

template <typename T1, typename T2>
bool operator >= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return !(lhs < rhs);
}


// 重载swap
template <typename T1, typename T2>
void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs)
{
    lhs.swap(rhs);
}


//make_pair
template <typename T1, typename T2>
pair<T1, T2> make_pair(T1&& first, T2&& second)
{
    return pair<T1, T2>(mystl::forward<T1>(first), mystl::forward<T2>(second));
}

}
#endif	//!MY_STL_UTIL_H_
