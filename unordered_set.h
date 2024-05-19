#ifndef MYSTL_UNORDERED_SET_H_
#define MYSTL_UNORDERED_SET_H_

// ���ͷ�ļ���������ģ���� unordered_set �� unordered_multiset
// �������÷��� set �� multiset ���ƣ���ͬ����ʹ�� hashtable ��Ϊ�ײ�ʵ�ֻ��ƣ������е�Ԫ�ز����Զ�����

// notes:
//
// �쳣��֤��
// mystl::unordered_set<Key> / mystl::unordered_multiset<Key> ��������쳣��֤�������µȺ�����ǿ�쳣��ȫ��֤��
//   * emplace
//   * emplace_hint
//   * insert

#include "hashtable.h"

namespace mystl 
{

// ģ���� unordered_set����ֵ�������ظ�
// ����һ�����ֵ���ͣ������������ϣ������ȱʡʹ�� mystl::hash��
// �����������ֵ�ȽϷ�ʽ��ȱʡʹ�� mystl::equal_to
template <typename Key, typename Hash = mystl::hash<Key>, typename KeyEqual = mystl::equal_to<Key>>
class unordered_set
{
private:
	using base_type = hashtable<Key, Hash, KeyEqual>;
	base_type ht_;

public:
	using allocator_type			= base_type::allocator_type;
	using key_type					= base_type::key_type;
	using value_type				= base_type::value_type;
	using hasher					= base_type::hasher;
	using key_equal					= base_type::key_equal;

	using size_type					= base_type::size_type;
	using difference_type			= base_type::difference_type;
	using pointer					= base_type::pointer;
	using const_pointer				= base_type::const_pointer;
	using reference					= base_type::reference;
	using const_reference			= base_type::const_reference;

	using iterator					= base_type::const_iterator;
	using const_iterator			= base_type::const_iterator;
	using local_iterator			= base_type::const_local_iterator;
	using const_local_iterator		= base_type::const_local_iterator;

	allocator_type get_allocator() const { return ht_.get_allocator(); }

public:

	unordered_set() : ht_(100, Hash(), KeyEqual()) {}

	explicit unordered_set(size_type bucket_count, 
		const Hash& hash = Hash(), 
		const KeyEqual& equal = KeyEqual())
		:ht_(bucket_count, hash, equal) {}

	template <typename InputIterator>
	unordered_set(InputIterator first, InputIterator last, 
		const size_type bucket_count = 100,
		const Hash& hash = Hash(), const KeyEqual& equal = KeyEqual())
		: ht_(mystl::max(bucket_count, static_cast<size_type>(mystl::distance(first, last))), hash, equal)
	{
		for (; first != last; ++first)
			ht_.insert_unique_noresize(*first);
	}

	unordered_set(std::initializer_list<value_type> ilist, 
		const size_type bucket_count = 100, 
		const Hash& hash = Hash(),
		const KeyEqual& equal = KeyEqual())
		:ht_(mystl::max(bucket_count, static_cast<size_type>(ilist.size())), hash, equal)
	{
		for (auto first = ilist.begin(), last = ilist.end(); first != last; ++first)
			ht_.insert_unique_noresize(*first);
	}

	unordered_set(const unordered_set& rhs) : ht_{ rhs.ht_ } {}

	unordered_set(unordered_set&& rhs) : ht_{ mystl::move(rhs.ht_) } {}

	unordered_set& operator=(const unordered_set& rhs)
	{
		ht_ = rhs.ht_;
		return *this;
	}
	unordered_set& operator=(unordered_set&& rhs)
	{
		ht_ = mystl::move(rhs.ht_);
		return *this;
	}

	unordered_set& operator=(std::initializer_list<value_type> ilist)
	{
		ht_.clear();
		ht_.reserve(ilist.size());
		for (auto first = ilist.begin(), last = ilist.end(); first != last; ++first)
			ht_.insert_unique_noresize(*first);
		return *this;
	}

	~unordered_set() = default;

	// ���������

	iterator begin() noexcept 
	{
		return ht_.begin();
	}

	const_iterator begin() const noexcept
	{
		return ht_.begin();
	}

	iterator end() noexcept
	{
		return ht_.end();
	}

	const_iterator end() const noexcept
	{
		return ht_.end();
	}

	const_iterator cbegin() const noexcept
	{
		return ht_.cbegin();
	}
	const_iterator cend()   const noexcept
	{
		return ht_.cend();
	}

	// �������

	bool empty() const noexcept { return ht_.empty(); }
	size_type size() const noexcept { return ht_.size(); }
	size_type max_size() const noexcept { return ht_.max_size(); }

	// �޸���������

	// empalce / empalce_hint
	template <typename ...Args>
	pair<iterator, bool> emplace(Args&& ... args)
	{
		return ht_.emplace_unique(mystl::forward<Args>(args)...);
	}
	template <typename ...Args>
	iterator emplace_hint(const_iterator hint, Args&& ...args)
	{
		return ht_.emplace_unique_use_hint(hint, mystl::forward<Args>(args)...);
	}

	// insert

	pair<iterator, bool> insert(const value_type& value)
	{
		return ht_.insert_unique(value);
	}
	pair<iterator, bool> insert(value_type&& value)
	{
		return ht_.emplace_unique(mystl::move(value));
	}

	iterator insert(const_iterator hint, const value_type& value)
	{
		return ht_.insert_unique_use_hint(hint, value);
	}
	iterator insert(const_iterator hint, value_type&& value)
	{
		return ht_.emplace_unique_use_hint(hint, mystl::move(value));
	}

	template <typename InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		ht_.insert_unique(first, last);
	}

	// erase / clear

	void erase(iterator it)
	{
		ht_.erase(it);
	}
	void erase(iterator first, iterator last)
	{
		ht_.erase(first, last);
	}

	size_type erase(const key_type& key)
	{
		return ht_.erase_unique(key);
	}

	void clear()
	{
		ht_.clear();
	}

	void swap(unordered_set& other) noexcept
	{
		ht_.swap(other.ht_);
	}

	// �������

	size_type count(const key_type& key) const
	{
		return ht_.count(key);
	}

	iterator find(const key_type& key)
	{
		return ht_.find(key);
	}
	const_iterator find(const key_type& key) const
	{
		return ht_.find(key);
	}

	pair<iterator, iterator> equal_range(const key_type& key)
	{
		return ht_.equal_range_unique(key);
	}
	pair<const_iterator, const_iterator> equal_range(const key_type& key) const
	{
		return ht_.equal_range_unique(key);
	}

	// bucket interface

	local_iterator begin(size_type n) noexcept
	{
		return ht_.begin(n);
	}
	const_local_iterator begin(size_type n) const noexcept
	{
		return ht_.begin(n);
	}
	const_local_iterator cbegin(size_type n) const noexcept
	{
		return ht_.cbegin(n);
	}

	local_iterator end(size_type n) noexcept
	{
		return ht_.end(n);
	}
	const_local_iterator end(size_type n) const noexcept
	{
		return ht_.end(n);
	}
	const_local_iterator cend(size_type n) const noexcept
	{
		return ht_.cend(n);
	}

	size_type bucket_count() const noexcept
	{
		return ht_.bucket_count();
	}
	size_type max_bucket_count() const noexcept
	{
		return ht_.max_bucket_count();
	}

	size_type bucket_size(size_type n) const noexcept
	{
		return ht_.bucket_size(n);
	}
	size_type bucket(const key_type& key) const
	{
		return ht_.bucket(key);
	}

	// hash policy
	float load_factor() const noexcept { return ht_.load_factor(); }

	float max_load_factor() const noexcept { return ht_.max_load_factor(); }
	void max_load_factor(float ml) { ht_.max_load_factor(ml); }

	void rehash(size_type count) { ht_.rehash(count); }
	void reserve(size_type count) { ht_.reserve(count); }

	hasher hash_fcn() const { return ht_.hash_fcn(); }
	key_equal key_eq() const { return ht_.key_eq(); }
public:
	friend bool operator==(const unordered_set& lhs, const unordered_set& rhs)
	{
		return lhs.ht_.equal_range_unique(rhs.ht_);
	}
	friend bool operator!=(const unordered_set& lhs, const unordered_set& rhs)
	{
		return !lhs.ht_.equal_range_unique(rhs.ht_);
	}
};


// ���رȽϲ�����
template <typename Key, typename Hash, typename KeyEqual, typename Alloc>
bool operator==(const unordered_set<Key, Hash, KeyEqual>& lhs,
	const unordered_set<Key, Hash, KeyEqual>& rhs)
{
	return lhs == rhs;
}

template <typename Key, typename Hash, typename KeyEqual, typename Alloc>
bool operator!=(const unordered_set<Key, Hash, KeyEqual>& lhs,
	const unordered_set<Key, Hash, KeyEqual>& rhs)
{
	return lhs != rhs;
}

// ���� mystl �� swap
template <typename Key, typename Hash, typename KeyEqual, typename Alloc>
void swap(unordered_set<Key, Hash, KeyEqual>& lhs,
	unordered_set<Key, Hash, KeyEqual>& rhs)
{
	lhs.swap(rhs);
}


/*****************************************************************************************/

// ģ���� unordered_multiset����ֵ�����ظ�
// ����һ�����ֵ���ͣ������������ϣ������ȱʡʹ�� mystl::hash��
// �����������ֵ�ȽϷ�ʽ��ȱʡʹ�� mystl::equal_to
template <typename Key, typename Hash = mystl::hash<Key>, typename KeyEqual = mystl::equal_to<Key>>
class unordered_multiset
{
private :
	using base_type = hashtable<Key, Hash, KeyEqual>;
	base_type ht_;

public:
	using allocator_type			= base_type::allocator_type;
	using key_type					= base_type::key_type;
	using value_type				= base_type::value_type;
	using hasher					= base_type::hasher;
	using key_equal					= base_type::key_equal;

	using size_type					= base_type::size_type;
	using difference_type			= base_type::difference_type;
	using pointer					= base_type::pointer;
	using const_pointer				= base_type::const_pointer;
	using reference					= base_type::reference;
	using const_reference			= base_type::const_reference;

	using iterator					= base_type::const_iterator;
	using const_iterator			= base_type::const_iterator;
	using local_iterator			= base_type::const_local_iterator;
	using const_local_iterator		= base_type::const_local_iterator;

	allocator_type get_allocator() const { return ht_.get_allocator(); }

public:
	unordered_multiset(): ht_(100, Hash(), KeyEqual()) {}

	explicit unordered_multiset(size_type bucket_count,
		const Hash& hash = Hash(),
		const KeyEqual& equal = KeyEqual())
		:ht_{ bucket_count, hash, equal }
	{

	}

	template <typename InputIterator>
	unordered_multiset(InputIterator first, InputIterator last,
		const size_type bucket_count = 100,
		const Hash& hash = Hash(),
		const KeyEqual& equal = KeyEqual())
		: ht_{ mystl::max(bucket_count, static_cast<size_type>(mystl::distance(first, last))), hash, equal }
	{
		for (; first != last; ++first)
			ht_.insert_multi_noresize(*first);
	}

	unordered_multiset(std::initializer_list<value_type> ilist,
		const size_type bucket_count = 100,
		const Hash& hash = Hash(),
		const KeyEqual& equal = KeyEqual())
		:ht_{ mystl::max(bucket_count, static_cast<size_type>(ilist.size())), hash, equal }
	{
		for (auto first = ilist.begin(), last = ilist.end(); first != last; ++first)
			ht_.insert_multi_noresize(*first);
	}

	unordered_multiset(const unordered_multiset& rhs)
		:ht_{ rhs.ht_ }
	{
	}
	unordered_multiset(unordered_multiset&& rhs) noexcept
		: ht_{ mystl::move(rhs.ht_) }
	{
	}

	unordered_multiset& operator=(const unordered_multiset& rhs)
	{
		ht_ = rhs.ht_;
		return *this;
	}
	unordered_multiset& operator=(unordered_multiset&& rhs)
	{
		ht_ = mystl::move(rhs.ht_);
		return *this;
	}

	unordered_multiset& operator=(std::initializer_list<value_type> ilist)
	{
		ht_.clear();
		ht_.reserve(ilist.size());
		for (auto first = ilist.begin(), last = ilist.end(); first != last; ++first)
			ht_.insert_multi_noresize(*first);
		return *this;
	}

	~unordered_multiset() = default;

	// ���������

	iterator begin() noexcept
	{
		return ht_.begin();
	}

	const_iterator begin() const noexcept
	{
		return ht_.begin();
	}

	iterator end() noexcept
	{
		return ht_.end();
	}

	const_iterator end() const noexcept
	{
		return ht_.end();
	}

	const_iterator cbegin() const noexcept
	{
		return ht_.cbegin();
	}
	const_iterator cend()   const noexcept
	{
		return ht_.cend();
	}

	// �������

	bool empty() const noexcept { return ht_.empty(); }
	size_type size() const noexcept { return ht_.size(); }
	size_type max_size() const noexcept { return ht_.max_size(); }

	// �޸��������

  // emplace / emplace_hint

	template <typename ...Args>
	iterator emplace(Args&& ...args)
	{
		return ht_.emplace_multi(mystl::forward<Args>(args)...);
	}

	template <typename ...Args>
	iterator emplace_hint(const_iterator hint, Args&& ...args)
	{
		return ht_.emplace_multi_use_hint(hint, mystl::forward<Args>(args)...);
	}

	// insert

	iterator insert(const value_type& value)
	{
		return ht_.insert_multi(value);
	}
	iterator insert(value_type&& value)
	{
		return ht_.emplace_multi(mystl::move(value));
	}

	iterator insert(const_iterator hint, const value_type& value)
	{
		return ht_.insert_multi_use_hint(hint, value);
	}
	iterator insert(const_iterator hint, value_type&& value)
	{
		return ht_.emplace_multi_use_hint(hint, mystl::move(value));
	}

	template <typename InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		ht_.insert_multi(first, last);
	}

	// erase / clear

	void erase(iterator it)
	{
		ht_.erase(it);
	}
	void erase(iterator first, iterator last)
	{
		ht_.erase(first, last);
	}

	size_type erase(const key_type& key)
	{
		return ht_.erase_multi(key);
	}

	void clear()
	{
		ht_.clear();
	}

	void swap(unordered_multiset& other) noexcept
	{
		ht_.swap(other.ht_);
	}

	// �������

	size_type count(const key_type& key) const
	{
		return ht_.count(key);
	}

	iterator find(const key_type& key)
	{
		return ht_.find(key);
	}
	const_iterator find(const key_type& key)  const
	{
		return ht_.find(key);
	}

	pair<iterator, iterator> equal_range(const key_type& key)
	{
		return ht_.equal_range_multi(key);
	}
	pair<const_iterator, const_iterator> equal_range(const key_type& key) const
	{
		return ht_.equal_range_multi(key);
	}

	// bucket interface

	local_iterator begin(size_type n) noexcept
	{
		return ht_.begin(n);
	}
	const_local_iterator begin(size_type n) const noexcept
	{
		return ht_.begin(n);
	}
	const_local_iterator cbegin(size_type n) const noexcept
	{
		return ht_.cbegin(n);
	}

	local_iterator end(size_type n) noexcept
	{
		return ht_.end(n);
	}
	const_local_iterator end(size_type n) const noexcept
	{
		return ht_.end(n);
	}
	const_local_iterator cend(size_type n) const noexcept
	{
		return ht_.cend(n);
	}

	size_type bucket_count() const noexcept
	{
		return ht_.bucket_count();
	}
	size_type max_bucket_count() const noexcept
	{
		return ht_.max_bucket_count();
	}

	size_type bucket_size(size_type n) const noexcept
	{
		return ht_.bucket_size(n);
	}
	size_type bucket(const key_type& key) const
	{
		return ht_.bucket(key);
	}

	// hash policy

	float load_factor() const noexcept { return ht_.load_factor(); }

	float max_load_factor() const noexcept { return ht_.max_load_factor(); }
	void max_load_factor(float ml) { ht_.max_load_factor(ml); }

	void rehash(size_type count) { ht_.rehash(count); }
	void reserve(size_type count) { ht_.reserve(count); }

	hasher hash_fcn() const { return ht_.hash_fcn(); }
	key_equal key_eq() const { return ht_.key_eq(); }

public:
	friend bool operator==(const unordered_multiset& lhs, const unordered_multiset& rhs)
	{
		return lhs.ht_.equal_range_multi(rhs.ht_);
	}
	friend bool operator!=(const unordered_multiset& lhs, const unordered_multiset& rhs)
	{
		return !lhs.ht_.equal_range_multi(rhs.ht_);
	}
};

// ���رȽϲ�����
template <typename Key, typename Hash, typename KeyEqual, typename Alloc>
bool operator==(const unordered_multiset<Key, Hash, KeyEqual>& lhs,
	const unordered_multiset<Key, Hash, KeyEqual>& rhs)
{
	return lhs == rhs;
}

template <typename Key, typename Hash, typename KeyEqual, typename Alloc>
bool operator!=(const unordered_multiset<Key, Hash, KeyEqual>& lhs,
	const unordered_multiset<Key, Hash, KeyEqual>& rhs)
{
	return lhs != rhs;
}

// ���� mystl �� swap
template <typename Key, typename Hash, typename KeyEqual, typename Alloc>
void swap(unordered_multiset<Key, Hash, KeyEqual>& lhs,
	unordered_multiset<Key, Hash, KeyEqual>& rhs)
{
	lhs.swap(rhs);
}

} //namespace mystl
#endif // !MYSTL_UNORDERED_SET_H_

