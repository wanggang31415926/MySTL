#ifndef MY_STL_BASIC_STRING_H_
#define MY_STL_BASIC_STRING_H_

#include <iostream>

#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "exceptdef.h"
namespace mystl
{

// char_traits

template <typename CharType>
struct char_traits
{
	using char_type = CharType;

	static size_t length(const chat_type* str)
	{
		size_t len = 0;
		while (*str != char_type(0))
		{
			++len;
			++str;
		}
		return len;
	}

	static int compare(const char_type* str1, const char_type* str2, size_t n)
	{
		for (; n != 0; --n, ++str1, ++str2)
		{
			if (*str1 < *str2)
			{
				return -1;
			}
			if (*str1 > *str2)
			{
				return 1;
			}
		}
		return 0;
	}

	static char_type* copy(char_type* dst, const char_type* src, size_t n)
	{
		MYSTL_DEBUG(src + n <= dst || dst + n <= src);
		char_type* s = dst;
		for (; n != 0; --n, ++dst, ++src)
		{
			*dst = *src;
		}
		return s;
	}

	static char_type* move(char_type* dst, const char_type* src, size_t n)
	{
		char_type* s = dst;
		if (dst < src)
		{

		}
	}



};

}
#endif //!MY_STL_BASIC_STRING_H_