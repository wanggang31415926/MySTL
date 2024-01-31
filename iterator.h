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
struct forward_iterator_tag : public input_iterator_tag {};

//双向迭代器 支持双向操作且支持多次读写操作
struct bidirectional_iterator_tag : public forward_iterator_tag {};

//随机访问迭代器 支持双向移动且支持多次读写操作, p+n, p-n 等
struct random_access_iterator_tag : public bidirectional_iterator_tag{};

// 除了output_iterator_tag 其余都继承 input_iterator_tag
// 可以使用input_iterator_tag 的引用或指针指向子类的对象 



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
	struct two { char a; char b; };
	template <typename U>
	static two test(...);
	
	template <typename U>
	static char test(typename U::iterator_category* = 0);

public :
	static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

//template <typename T>
//struct has_iterator_cat
//{
//	static const bool value = false;
//};
//
//template <typename T>
//struct has_iterator_cat<typename T::iteraoer_category>
//{
//	static const bool value = true;
//};

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

//而has_iterator_cat返回值传给iterator_traits_helper第二个参数 ，
// 如果为false就没必要继续萃取了，如果为true则继承iterator_traits_impl。
//template <class From, class To> struct is_convertible; 用来判断From是否可隐式转换为To。
//所以这边就是判断Iterator::iterator_category算否可以隐式转换为输入迭代器或输入迭代器。

// 没有迭代器类型 
template <typename Iterator, bool>
struct iterator_traits_helper {};

//std::is_convertible 用于检查一个类型是否可以隐式转换为另一个类型。
//有迭代器类型 判断是否能隐式转换为input_iterator_tag或output_iterator_tag 
template <typename Iterator>
struct iterator_traits_helper<Iterator, true>
	: public iterator_traits_impl <Iterator,
	std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
	std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
{
};

//萃取迭代器的特性
//
template <typename Iterator>
struct iterator_traits
	: public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value>{};


//针对原生指针的片特化
template <typename T>
struct iterator_traits<T*>
{
	using iterator_category			= random_access_iterator_tag;
	using value_type				= T;
	using pointer					= T*;
	using reference					= T&;
	using difference_type			= ptrdiff_t;
};

template <typename T>
struct iterator_traits<const T*>
{
	using iterator_category			= random_access_iterator_tag;
	using value_type				= T;
	using pointer					= const T*;
	using reference					= const T&;
	using difference_type			= ptrdiff_t;
};


//萃取某种迭代器

//std::is_convertible 用于检查一个类型是否可以隐式转换为另一个类型。
//它的作用是在编译时判断一个类型是否可以被隐式转换为另一个类型。
// 它用于检查类型T的迭代器类别是否可以隐式转换为类型U。
//它提供了一个静态成员常量value，如果类型可以进行隐式转换，则value为true，否则为false。
template <typename T, typename U, bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of
	: public m_bool_constant<std::is_convertible<
	typename iterator_traits<T>::iterator_category, U>::value>
{
};

//T 类型迭代器不能转换为 U 类型
template <typename T, typename U>
struct has_iterator_cat_of<T, U, false> : public m_false_type {};

//判断是否为input_iterator_tag 类别的迭代器
template <typename Iter>
struct is_input_iterator : public has_iterator_cat_of <Iter, input_iterator_tag> {};

//判断是否为output_iterator_tag 类别的迭代器
template <typename Iter>
struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

//判断是否为input_iterator_tag 类别的迭代器
template <typename Iter>
struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

//判断是否为input_iterator_tag 类别的迭代器
template <typename Iter>
struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

//判断是否为input_iterator_tag 类别的迭代器
template <typename Iter>
struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

//判断是否为input_iterator_tag 类别的迭代器
template <typename Iterator>
struct is_iterator :
	public m_bool_constant<is_input_iterator<Iterator>::value ||
	is_output_iterator<Iterator>::value>
{
};


// 萃取某个迭代器的类型 category
template <typename Iterator>
typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&)
{
	using Category =  typename iterator_traits<Iterator>::iterator_category;
	return Category();
}

// 萃取某个迭代器的 两迭代器之间的距离 distance_type 
template <typename Iterator>
typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&)
{
	return static_cast<iterator_traits<Iterator>::difference_type*>(0);
}


// 萃取某个迭代器的所指对象的类型 value_type
template <typename Iterator>
typename iterator_traits<Iterator>::value_type* value_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// 一下函数用于计算迭代器之间的距离

//distance 的 input_iterator_tag 版本
template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type 
distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
{
	typename iterator_traits<InputIterator>::difference_type n = 0;
	while (first != last)
	{
		++first;
		++n;
	}
	return n;
}

//distance 的 random_access_iterator_tag 版本
template <typename RandomIter>
typename iterator_traits<RandomIter>::difference_type
distance_dispatch(RandomIter first, RandomIter last, random_access_iterator_tag)
{
	return last - first;
}

//按照迭代器的种类选择分类处理
template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
	return distance_dispatch(first, last, iterator_category(first));
}


// 以下函数用于让迭代器前进 n 个距离
//如果是随机访问迭代器就可以像处理指针一样移动，否则只能遍历，
// 并且如果是双向迭代器才可以往后遍历，否则只能往前。

// advance 的 input_iterator_tag 的版本
template <typename InputIterator, typename Distance>
void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag)
{
	while (n--)
	{
		++i;
	}
}

// advance 的 bidirectional_iterator_tag 的版本
template <typename BidirectionalIterator, typename Distance>
void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
{
	if (n >= 0)
	{
		while (n--) 
		{
			++i; 
		}
	}
	else
	{
		while (n++) 
		{ 
			--i; 
		}
	}	
}

// advance 的 random_access_iterator_tag 的版本
template <typename RandomIter, typename Distance>
void advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag)
{
	i += n;
}

//按照迭代器的种类选择分类处理
template <class InputIterator, class Distance>
void advance(InputIterator& i, Distance n)
{
	advance_dispatch(i, n, iterator_category(i));
}


/*******************************************************************************************************/

// 模版类 reverse_iterator
// 反向迭代器
template <typename Iterator>
class reverse_iterator
{
private:
	Iterator current;		//记录对应的正向迭代器

public:
	// 反向迭代器的五种相应型别

	using iterator_category		= typename iterator_traits<Iterator>::iterator_category;
	using value_type			= typename iterator_traits<Iterator>::value_type;
	using difference_type		= typename iterator_traits<Iterator>::difference_type;
	using pointer				= typename iterator_traits<Iterator>::pointer;
	using reference				= typename iterator_traits<Iterator>::reference;

	using iterator_type			= Iterator;
	using self					= reverse_iterator<Iterator>;

public:
	//构造
	reverse_iterator() {}
	explicit reverse_iterator(iterator_type iter) : current { iter } {}
	reverse_iterator(const self& other) : current { other.current } {}

public:
	//取出对应的正向迭代器
	iterator_type base() const
	{
		return current;
	}

	//重载* 和 ->
	reference operator* () const
	{
		auto tmp = current;
		return *--tmp;
	}

	pointer operator -> () const
	{
		return &(operator*());
	}

	// 前进(++)变为后退(--)
	self& operator++()
	{
		--current;
		return *this;
	}
	self operator++(int)
	{
		self tmp{ *this };
		--current;
		return tmp;
	}

	// 后退(--)变为前进(++)
	self& operator--()
	{
		++current;
		return *this;
	}
	self operator--(int)
	{
		self tmp{ *this };
		++current;
		return tmp;
	}

	self& operator+=(difference_type n)
	{
		current -= n;
		return *this;
	}
	self operator+(difference_type n) const
	{
		return self( current - n );
	}
	self& operator-=(difference_type n)
	{
		current += n;
		return *this;
	}
	self operator-(difference_type n) const
	{
		return self( current + n );
	}

	reference operator[](difference_type n) const
	{
		return *(*this + n);
	}
};

// 重载 operator-
template <typename Iterator>
typename reverse_iterator<Iterator>::difference_type
operator-(const reverse_iterator<Iterator>& lhs,
	const reverse_iterator<Iterator>& rhs)
{
	return rhs.base() - lhs.base();
}

// 重载比较操作符
template <typename Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs,
	const reverse_iterator<Iterator>& rhs)
{
	return lhs.base() == rhs.base();
}

template <typename Iterator>
bool operator<(const reverse_iterator<Iterator>& lhs,
	const reverse_iterator<Iterator>& rhs)
{
	return rhs.base() < lhs.base();
}

template <typename Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs,
	const reverse_iterator<Iterator>& rhs)
{
	return !(lhs == rhs);
}

template <typename Iterator>
bool operator>(const reverse_iterator<Iterator>& lhs,
	const reverse_iterator<Iterator>& rhs)
{
	return rhs < lhs;
}

template <typename Iterator>
bool operator<=(const reverse_iterator<Iterator>& lhs,
	const reverse_iterator<Iterator>& rhs)
{
	return !(rhs < lhs);
}

template <typename Iterator>
bool operator>=(const reverse_iterator<Iterator>& lhs,
	const reverse_iterator<Iterator>& rhs)
{
	return !(lhs < rhs);
}

}


#endif // !MY_STL_ITERATOR_H_
