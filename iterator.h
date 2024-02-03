#ifndef MY_STL_ITERATOR_H_
#define MY_STL_ITERATOR_H_

#include <cstddef>

#include "type_traits.h"

namespace mystl
{

//���ֵ���������
//��������� ��������ָ�����ݲ��ܱ��޸�,ֻ����ֻ��ִ��һ�ζ�����
struct input_iterator_tag {};

//��������� ֻд����һ��ֻ��ִ��һ��д����
struct output_iterator_tag {};

//��������� ֧�ֶ�д������֧�ֶ�ζ�д����
struct forward_iterator_tag : public input_iterator_tag {};

//˫������� ֧��˫�������֧�ֶ�ζ�д����
struct bidirectional_iterator_tag : public forward_iterator_tag {};

//������ʵ����� ֧��˫���ƶ���֧�ֶ�ζ�д����, p+n, p-n ��
struct random_access_iterator_tag : public bidirectional_iterator_tag{};

// ����output_iterator_tag ���඼�̳� input_iterator_tag
// ����ʹ��input_iterator_tag �����û�ָ��ָ������Ķ��� 



//iterator ģ��

template <typename Category, typename T, typename Distance = ptrdiff_t,
	typename Pointer = T*, typename Reference = T&>
struct iterator
{
	using iterator_category = Category;		//����������
	using value_type		= T;			//��������ָ��������
	using pointer			= Pointer;		//��������ָ��������͵�ָ��
	using reference			= Reference;	//��������ָ�������͵�����
	using difference_type	= Distance;		//����������֮��ľ���
};

//�ж����� T �Ƿ���� iterator_category ��Ա����
//����has_iterator_cat�ж��Ƿ��е��������iterator_category�����ﶨ����struct two��С��2��������char���ʹ�СΪ1��
//Ȼ�����������ص�ģ�����test��ͨ�������ģ����������typename U::iterator_category������two ����char�����valueͨ����ӳ������
//Ҳ����value = sizeof(test<T>(0)) == sizeof(char)��test<T>(0)��ȥƥ��test������
//�����T���ж���iterator_category���;ͷ���two ��value����true�����򷵻�char��value����false��

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

//��has_iterator_cat����ֵ����iterator_traits_helper�ڶ������� ��
// ���Ϊfalse��û��Ҫ������ȡ�ˣ����Ϊtrue��̳�iterator_traits_impl��
//template <class From, class To> struct is_convertible; �����ж�From�Ƿ����ʽת��ΪTo��
//������߾����ж�Iterator::iterator_category��������ʽת��Ϊ����������������������

// û�е��������� 
template <typename Iterator, bool>
struct iterator_traits_helper {};

//std::is_convertible ���ڼ��һ�������Ƿ������ʽת��Ϊ��һ�����͡�
//�е��������� �ж��Ƿ�����ʽת��Ϊinput_iterator_tag��output_iterator_tag 
template <typename Iterator>
struct iterator_traits_helper<Iterator, true>
	: public iterator_traits_impl <Iterator,
	std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
	std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
{
};

//��ȡ������������
//
template <typename Iterator>
struct iterator_traits
	: public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value>{};


//���ԭ��ָ���Ƭ�ػ�
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


//��ȡĳ�ֵ�����

//std::is_convertible ���ڼ��һ�������Ƿ������ʽת��Ϊ��һ�����͡�
//�����������ڱ���ʱ�ж�һ�������Ƿ���Ա���ʽת��Ϊ��һ�����͡�
// �����ڼ������T�ĵ���������Ƿ������ʽת��Ϊ����U��
//���ṩ��һ����̬��Ա����value��������Ϳ��Խ�����ʽת������valueΪtrue������Ϊfalse��
template <typename T, typename U, bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of
	: public m_bool_constant<std::is_convertible<
	typename iterator_traits<T>::iterator_category, U>::value>
{
};

//T ���͵���������ת��Ϊ U ����
template <typename T, typename U>
struct has_iterator_cat_of<T, U, false> : public m_false_type {};

//�ж��Ƿ�Ϊinput_iterator_tag ���ĵ�����
template <typename Iter>
struct is_input_iterator : public has_iterator_cat_of <Iter, input_iterator_tag> {};

//�ж��Ƿ�Ϊoutput_iterator_tag ���ĵ�����
template <typename Iter>
struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

//�ж��Ƿ�Ϊinput_iterator_tag ���ĵ�����
template <typename Iter>
struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

//�ж��Ƿ�Ϊinput_iterator_tag ���ĵ�����
template <typename Iter>
struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

//�ж��Ƿ�Ϊinput_iterator_tag ���ĵ�����
template <typename Iter>
struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

//�ж��Ƿ�Ϊinput_iterator_tag ���ĵ�����
template <typename Iterator>
struct is_iterator :
	public m_bool_constant<is_input_iterator<Iterator>::value ||
	is_output_iterator<Iterator>::value>
{
};


// ��ȡĳ�������������� category
template <typename Iterator>
typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&)
{
	using Category =  typename iterator_traits<Iterator>::iterator_category;
	return Category();
}

// ��ȡĳ���������� ��������֮��ľ��� distance_type 
template <typename Iterator>
typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&)
{
	return static_cast<iterator_traits<Iterator>::difference_type*>(0);
}


// ��ȡĳ������������ָ��������� value_type
template <typename Iterator>
typename iterator_traits<Iterator>::value_type* value_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// һ�º������ڼ��������֮��ľ���

//distance �� input_iterator_tag �汾
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

//distance �� random_access_iterator_tag �汾
template <typename RandomIter>
typename iterator_traits<RandomIter>::difference_type
distance_dispatch(RandomIter first, RandomIter last, random_access_iterator_tag)
{
	return last - first;
}

//���յ�����������ѡ����ദ��
template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
	return distance_dispatch(first, last, iterator_category(first));
}


// ���º��������õ�����ǰ�� n ������
//�����������ʵ������Ϳ�������ָ��һ���ƶ�������ֻ�ܱ�����
// ���������˫��������ſ����������������ֻ����ǰ��

// advance �� input_iterator_tag �İ汾
template <typename InputIterator, typename Distance>
void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag)
{
	while (n--)
	{
		++i;
	}
}

// advance �� bidirectional_iterator_tag �İ汾
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

// advance �� random_access_iterator_tag �İ汾
template <typename RandomIter, typename Distance>
void advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag)
{
	i += n;
}

//���յ�����������ѡ����ദ��
template <class InputIterator, class Distance>
void advance(InputIterator& i, Distance n)
{
	advance_dispatch(i, n, iterator_category(i));
}


/*******************************************************************************************************/

// ģ���� reverse_iterator
// ���������
template <typename Iterator>
class reverse_iterator
{
private:
	Iterator current;		//��¼��Ӧ�����������

public:
	// �����������������Ӧ�ͱ�

	using iterator_category		= typename iterator_traits<Iterator>::iterator_category;
	using value_type			= typename iterator_traits<Iterator>::value_type;
	using difference_type		= typename iterator_traits<Iterator>::difference_type;
	using pointer				= typename iterator_traits<Iterator>::pointer;
	using reference				= typename iterator_traits<Iterator>::reference;

	using iterator_type			= Iterator;
	using self					= reverse_iterator<Iterator>;

public:
	//����
	reverse_iterator() {}
	explicit reverse_iterator(iterator_type iter) : current { iter } {}
	reverse_iterator(const self& other) : current { other.current } {}

public:
	//ȡ����Ӧ�����������
	iterator_type base() const
	{
		return current;
	}

	//����* �� ->
	reference operator* () const
	{
		auto tmp = current;
		return *--tmp;
	}

	pointer operator -> () const
	{
		return &(operator*());
	}

	// ǰ��(++)��Ϊ����(--)
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

	// ����(--)��Ϊǰ��(++)
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

// ���� operator-
template <typename Iterator>
typename reverse_iterator<Iterator>::difference_type
operator-(const reverse_iterator<Iterator>& lhs,
	const reverse_iterator<Iterator>& rhs)
{
	return rhs.base() - lhs.base();
}

// ���رȽϲ�����
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
