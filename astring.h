#ifndef MY_STL_ASTRING_H_
#define MY_STL_ASTRING_H_

// ������ string, wstring, u16string, u32string ����

#include "basic_string.h"

namespace mystl
{

	using string = mystl::basic_string<char>;
	using wstring = mystl::basic_string<wchar_t>;
	using u16string = mystl::basic_string<char16_t>;
	using u32string = mystl::basic_string<char32_t>;

}
#endif // !MY_STL_ASTRING_H_
