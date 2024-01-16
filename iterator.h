#ifndef MY_STL_ITERATOR_H_
#define MY_STL_ITERATOR_H_

#include <cstddef>

#include "type_traits.h"

namespace mystl
{

//五种迭代器类型
//输入迭代器 迭代器所指的内容不能被修改,只读且只能执行一次读操作
struct input_iterator_tag {};

//输出迭代器 只写并且一次只能执行一次写操作
struct output_iterator_tag {};

//正向迭代器 支持读写操作且支持多次读写操作
struct farward_iterator_tag : public input_iterator_tag {};

//双向迭代器 支持双向操作且支持多次读写操作
struct bidirectional_iterator_tag : public farward_iterator_tag {};

//随机访问迭代器 支持双向移动且支持多次读写操作, p+n, p-n 等
struct random_access_iterator_tag : public bidirectional_iterator_tag{};

//iterator 模版

template <typename Category, typename T, typename Distance = ptrdiff_t,
	typename Pointer = T*, typename Reference = T&>
struct iterator
{
	using iterator_category = Category;		//迭代器类型
	using value_type		= T;			//迭代器所指对象类型
	using pointer			= Pointer;		//迭代器所指对象的类型的指针
	using reference			= Reference;	//迭代器所指对象类型的引用
	using difference_type	= Distance;		//两个迭代器之间的距离
};

//判断类型 T 是否具有 iterator_category 成员类型
//其中has_iterator_cat判断是否有迭代器类别iterator_category，这里定义了struct two大小是2用来区别char类型大小为1。
//然后有两个重载的模板参数test，通过传入的模板类型有无typename U::iterator_category来返回two 还是char，最后value通过反映出来。
//也就是value = sizeof(test<T>(0)) == sizeof(char)中test<T>(0)会去匹配test函数，
//如果有T中有定义iterator_category类型就返回two 则value就是true，否则返回char则value就是false。

template <typename T>
struct has_iterator_cat
{
private:
	struct two{ char a, char b};
	template <typename U>
	static two test(...);
	
	template <typename U>
	static char test(typename U::iterator_category* = nullptr);

public :
	static const bool value = sizeof test<T>(0) == sizeof(char);
};

template <typename Iterator, bool>
struct iterator_traits_impl {};


template <typename Iterator>
struct iterator_traits_impl <Iterator, true>
{
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type		 value_type;
	typedef typename Iterator::pointer			 pointer;
	typedef typename Iterator::reference		 reference;
	typedef typename Iterator::difference_type	 difference_type;
};

template <typename Iterator, bool>
struct iterator_traits_helper {};

template <typename Iterator>
struct iterator_traits_impl<Iterator, true>
	: public iterator_traits_impl<Iterator,
	std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
	std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value
{
};

//萃取迭代器的特性
template <typename Iterator>
struct iterator_traits
	: public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value>{};


}

#endif // !MY_STL_ITERATOR_H_
