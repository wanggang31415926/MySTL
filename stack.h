#ifndef MY_STL_STACK_H_
#define MY_STL_STACK_H_

// 这个头文件包含了一个模板类 stack
// stack : 栈

#include "deque.h"

namespace mystl
{

// 模板类 stack
// 参数一代表数据类型，参数二代表底层容器类型，缺省使用 mystl::deque 作为底层容器
template <typename T, typename Container = mystl::deque<T>>
class stack
{
public:
	using container_type	= Container;
	// 使用底层容器的型别
	using value_type		= Container::value_type;
	using size_type			= Container::size_type;
	using reference			= Container::reference;
	using const_reference	= Container::const_reference;

	static_assert(mystl::is_same_v<T, value_type>,
		"the value_type of Container should be same with T");

private:
	container_type c_;		// 用底层容器表现 stack

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

	// 访问元素相关操作
	reference top() { return c_.back(); }
	const_reference top() const { return c_.back(); }

	// 容量相关操作
	bool empty() const noexcept { return c_.empty(); }
	size_type size() const noexcept { return c_.size(); }

	// 修改容器相关操作
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

// 重载比较操作符
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

// 重载 mystl 的 swap
template <typename T, typename Container>
void swap(stack<T, Container>& lhs, stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
	lhs.swap(rhs);
}


} //namepace mystl

#endif // !MY_STL_STACK_H_
