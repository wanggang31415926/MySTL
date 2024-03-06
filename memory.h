#ifndef MY_STL_MEMORY_H
#define MY_STL_MEMORY_H

// ���ͷ�ļ�������߼��Ķ�̬�ڴ����
// ����һЩ�����������ռ���������δ��ʼ���Ĵ���ռ�����Լ�һ��ģ���� auto_ptr

#include <cstddef>
#include <cstdlib>
#include <climits>

#include "algobase.h"
#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"

namespace mystl
{

// ��ȡ�����ַ
template <typename T>
constexpr T* address_of(T& value) noexcept
{
	return &value;
}

//��ȡ / �ͷ���ʱ������

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
		len /= 2;	//�����ڴ�ʧ�ܼ��� len �Ĵ�С
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
// ��ģ�� : temporary_buffer
// ������ʱ���������������ͷ�


template<typename ForwardIterator, typename T>
class temporary_buffer
{
private:
	ptrdiff_t original_len;  // ����������Ĵ�С
	ptrdiff_t len;           // ������ʵ�ʵĴ�С
	T* buffer;				// ָ�򻺳�����ָ��

public:
	//���� ����
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
			len /= 2;	// ����ʧ��ʱ��������ռ��С
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
// ģ����: auto_ptr
// һ�������ϸ��������Ȩ��С������ָ��
template <typename T>
class auto_ptr
{
public :
	using elem_type = T;
private:
	T* ptr_;
public:
	// ���졢���ơ���������
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

	//�ͷ�ָ��
	T* release()
	{
		T* temp = ptr_;
		ptr_ = nullptr;
		return temp;
	}

	//����ָ��
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

