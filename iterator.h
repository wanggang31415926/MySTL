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
struct farward_iterator_tag : public input_iterator_tag {};

//˫������� ֧��˫�������֧�ֶ�ζ�д����
struct bidirectional_iterator_tag : public farward_iterator_tag {};

//������ʵ����� ֧��˫���ƶ���֧�ֶ�ζ�д����, p+n, p-n ��
struct random_access_iterator_tag : public bidirectional_iterator_tag{};

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

//��ȡ������������
template <typename Iterator>
struct iterator_traits
	: public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value>{};


}

#endif // !MY_STL_ITERATOR_H_
