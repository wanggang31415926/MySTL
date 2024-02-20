#ifndef MY_STL_MEMORY_H
#define MY_STL_MEMORY_H

// 这个头文件负责更高级的动态内存管理
// 包含一些基本函数、空间配置器、未初始化的储存空间管理，以及一个模板类 auto_ptr

#include <cstddef>
#include <cstdlib>
#include <climits>

#include "algobase.h"
#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"

namespace mystl
{

// 获取对象地址
template <typename T>
constexpr T* address_of(T& value) noexcept
{
	return &value;
}

//获取 / 释放临时缓冲区

template <typename T>
pair<T*, ptrdiff_t> get_buffer_helper(ptrdiff_t len, T*)
{
	if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
	{
		len = INT_MAX / sizeof(T);
	}

	while (len > 0)
	{
		T* tmp = static_cast<T*>(malloc(static_cast<size_t>(len) * sizeof(T)));
		if (tmp)
		{
			return pair<T*, ptrdiff_t>(tmp, len);
		}
		len /= 2;	//申请内存失败减少 len 的大小
	}
	return pair<T*, ptrdiff_t>(nullptr, 0);
}

template <typename T>
pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len)
{
	return get_buffer_helper(len, static_cast<T*>(0));
}

template <typename T>
pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len, T*)
{
	return get_buffer_helper(len, static_cast<T*>(0));
}

template <typename T>
void release_temporary_buffer(T* ptr)
{
	free(ptr);
}


// --------------------------------------------------------------------------------------
// 类模板 : temporary_buffer
// 进行临时缓冲区的申请与释放


template<typename ForwardIterator, typename T>
class temporary_buffer
{
private:
	ptrdiff_t original_len;  // 缓冲区申请的大小
	ptrdiff_t len;           // 缓冲区实际的大小
	T* buffer;				// 指向缓冲区的指针

public:
	//构造 析构
	temporary_buffer(ForwardIterator first, ForwardIterator last)
	{
		try
		{
			len = mystl::distance(first, last);
			allocate_buffer();
			if (len > 0)
			{
				initialize_buffer(*first,
					std::is_trivially_default_constructible<T>());
			}
		}
		catch (...)
		{
			free(buffer);
			buffer = nullptr;
			len = 0;
		}
	}
	~temporary_buffer()
	{
		mystl::destroy(buffer, buffer + len);
		free(buffer);
	}

public:
	ptrdiff_t size() const noexcept { return len; }
	ptrdiff_t requested_size() const noexcept { return original_len; }
	T* begin() noexcept { return buffer; }
	T* end() noexcept { return buffer + len; }

private:
	void allocate_buffer()
	{
		original_len = len;
		if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
		{
			len = INT_MAX / sizeof(T);
		}

		while (len > 0)
		{
			buffer = static_cast<T*>(malloc(len * sizeof(T)));
			if (buffer)
			{
				break;
			}
			len /= 2;	// 申请失败时减少申请空间大小
		}
	}
	void initialize_buffer(const T&, std::true_type) {}
	void initialize_buffer(const T& value, std::false_type)
	{
		mystl::uninitialized_fill_n(buffer, len, value);
	}

private:
	temporary_buffer(const temporary_buffer&) = delete;
	void operator=(const temporary_buffer&) = delete;
};


// --------------------------------------------------------------------------------------
// 模板类: auto_ptr
// 一个具有严格对象所有权的小型智能指针
template <typename T>
class auto_ptr
{
public :
	using elem_type = T;
private:
	T* ptr_;
public:
	// 构造、复制、析构函数
	explicit auto_ptr(T* p = nullptr) : ptr_ { p } {}
	auto_ptr(const auto_ptr& rhs) : ptr_ { rhs.release() } {}
	template <typename U>
	auto_ptr(const auto_ptr<U>& rhs) : ptr_{ rhs.release() } {}

	auto_ptr& operator=(const auto_ptr& rhs)
	{
		if (this != &rhs)
		{
			delete ptr_;
			ptr_ = rhs.relese();
		}
		return *this;
	}

	template <typename U>
	auto_ptr& operator=(auto_ptr<U>& rhs)
	{
		if (this->get() != rhs.get())
		{
			delete ptr_;
			ptr_ = rhs.release();
		}
		return *this;
	}

	~auto_ptr() { delete ptr_; }

public:
	T& operator*() const { return *ptr_; }
	T* operator->() const { return ptr_; }

	T* get() const { return ptr_; }

	//释放指针
	T* release()
	{
		T* temp = ptr_;
		ptr_ = nullptr;
		return temp;
	}

	//重置指针
	void reset(T* p = nullptr)
	{
		if (ptr_ != p)
		{
			delete ptr_;
			ptr_ = p;
		}
	}
};

}

#endif // !MY_STL_MEMORY_H

