#ifndef MY_STL_ALLOCATOR_H_
#define MY_STL_ALLOCATOR_H_

// 这个头文件包含一个模板类 allocator，用于管理内存的分配、释放，对象的构造、析构

#include "construct.h"
#include "util.h"

namespace mystl
{

//模版类 allocator
//模版函数代表数据类型

template <typename T>
class allocator
{
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
public:
	static T* allocate();
	static T* allocate(size_type n);

	static void deallocate(T* ptr);
	static void deallocate(T* ptr, size_type n);

	static void construct(T* ptr);
	static void construct(T* ptr, const T& value);
	static void construct(T* ptr, T&& value);

	template <typename... Args>
	static void construct(T* ptr, Args&& ...args);

	static void destroy(T* ptr);
	static void destroy(T* first, T* last);


};

template<typename T>
inline T* allocator<T>::allocate()
{
	return static_cast<T*>(::operator new(sizeof(T)));
}

template<typename T>
inline T* allocator<T>::allocate(size_type n)
{
	if (n == 0) return nullptr;
	return static_cast<T*>(::operator new(n * sizeof(T)));
}

template<typename T>
inline void allocator<T>::deallocate(T* ptr)
{
	if (ptr != nullptr)
		::operator delete(ptr);
}

template<typename T>
inline void allocator<T>::deallocate(T* ptr, size_type n)
{
	if (ptr != nullptr)
		::operator delete(ptr);
}

template<typename T>
inline void allocator<T>::construct(T* ptr)
{
	mystl::construct(ptr);
}

template<typename T>
inline void allocator<T>::construct(T* ptr, const T& value)
{
	mystl::construct(ptr, value);
}

template<typename T>
inline void allocator<T>::construct(T* ptr, T&& value)
{
	mystl::construct(ptr, mystl::move(value));
}

template<typename T>
inline void allocator<T>::destroy(T* ptr)
{
	mystl::destroy(ptr);
}

template<typename T>
inline void allocator<T>::destroy(T* first, T* last)
{
	mystl::destroy(first, last);
}

template<typename T>
template<typename ...Args>
inline void allocator<T>::construct(T* ptr, Args && ...args)
{
	mystl::construct(ptr, mystl::forward<Args>(args)...);
}

} // namespace mystl


#endif // !MY_STL_ALLOCATOR_H_

