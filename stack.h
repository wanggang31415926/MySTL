#ifndef MY_STL_STACK_H_
#define MY_STL_STACK_H_

// ���ͷ�ļ�������һ��ģ���� stack
// stack : ջ

#include "deque.h"

namespace mystl
{

// ģ���� stack
// ����һ�����������ͣ�����������ײ��������ͣ�ȱʡʹ�� mystl::deque ��Ϊ�ײ�����
template <typename T, typename Container = mystl::deque<T>>
class stack
{
public:
	using container_type	= Container;
	// ʹ�õײ��������ͱ�
	using value_type		= Container::value_type;
	using size_type			= Container::size_type;
	using reference			= Container::reference;
	using const_reference	= Container::const_reference;

	static_assert(mystl::is_same_v<T, value_type>,
		"the value_type of Container should be same with T");

private:
	container_type c_;		// �õײ��������� stack

public:
	stack() = default;

	explicit stack(size_type n) :c_(n) {}

	stack(size_type n, const value_type& value) : c_{ n,value } {}

	template <typename IIter>
	stack(IIter first, IIter last) : c_{ first, last } {}

	stack(std::initializer_list<T> ilist) : c_{ ilist.begin(),ilist.end() } {}

	stack(const Container& c) : c_{ c } {}

	stack(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value)
		:c_{ mystl::move(c) } {}

	stack(const stack& rhs) : c_{ rhs.c_ } {}

	stack(stack&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value)
		:c_{ mystl::move(rhs.c_) } {}

	stack& operator=(const stack& rhs)
	{
		c_ = rhs.c_;
		return *this;
	}

	stack& operator=(stack&& rhs) noexcept(std::is_nothrow_move_assignable<Container>::value)
	{
		c_ = mystl::move(rhs.c_);
		return *this;
	}

	stack& operator=(std::initializer_list<T> ilist)
	{
		c_ = ilist;
		return *this;
	}

	~stack() = default;

	// ����Ԫ����ز���
	reference top() { return c_.back(); }
	const_reference top() const { return c_.back(); }

	// ������ز���
	bool empty() const noexcept { return c_.empty(); }
	size_type size() const noexcept { return c_.size(); }

	// �޸�������ز���
	template <typename ...Args>
	void emplace(Args...args)
	{
		return c_.emplace_back(mystl::forward<Args>(args)...);
	}

	void push(const value_type& value)
	{
		c_.push_back(value);
	}

	void push(value_type&& value)
	{
		c_.push_back(mystl::move(value));
	}

	void pop()
	{
		c_.pop_back();
	}

	void clear()
	{
		while (!empty())
		{
			pop();
		}
	}

	void swap(stack& rhs) noexcept(noexcept(mystl::swap(c_, rhs.c_)))
	{
		mystl::swap(c_, rhs.c_);
	}

public:
	friend bool operator==(const stack& lhs, const stack& rhs) { return lhs.c_ == rhs.c_; }
	friend bool operator< (const stack& lhs, const stack& rhs) { return lhs.c_ < rhs.c_; }
};

// ���رȽϲ�����
template <typename T, typename Container>
bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
	return lhs == rhs;
}

template <typename T, typename Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
	return lhs < rhs;
}

template <typename T, typename Container>
bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
	return !(lhs == rhs);
}

template <typename T, typename Container>
bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
	return rhs < lhs;
}

template <typename T, typename Container>
bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
	return !(rhs < lhs);
}

template <typename T, typename Container>
bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
	return !(lhs < rhs);
}

// ���� mystl �� swap
template <typename T, typename Container>
void swap(stack<T, Container>& lhs, stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
	lhs.swap(rhs);
}


} //namepace mystl

#endif // !MY_STL_STACK_H_
