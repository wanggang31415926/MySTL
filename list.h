#ifndef MY_SLT_LIST_H_
#define MY_SLT_LIST_H_

// ���ͷ�ļ�������һ��ģ���� list
// list : ˫������

// notes:
//
// �쳣��֤��
// mystl::list<T> ��������쳣��֤�����ֺ������쳣��֤���������µȺ�����ǿ�쳣��ȫ��֤��
//   * emplace_front
//   * emplace_back
//   * emplace
//   * push_front
//   * push_back
//   * insert

#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl
{

template <typename T> struct list_node_base;
template <typename T> struct list_node;

template <typename T>
struct node_traits
{
	using base_ptr = list_node_base<T>*;
	using node_ptr = list_node<T>*;
};

// list �Ľڵ�ṹ
template <typename T>
struct list_node_base
{
	using base_ptr = node_traits<T>::base_ptr;
	using node_ptr = node_traits<T>::node_ptr;

	base_ptr prev;
	base_ptr next;

	list_node_base() = default;

	node_ptr as_node()
	{
		return static_cast<node_ptr>(self());
	}

	void unlink()
	{
		prev = next = self();
	}

	base_ptr self()
	{
		return static_cast<base_ptr>(&*this);
	}
};

template <typename T>
struct list_node : public list_node_base<T>
{
	using base_ptr = node_traits<T>::base_ptr;
	using node_ptr = node_traits<T>::node_ptr;

	T value;

	list_node() = default;
	list_node(const T& value)
		: value(value) {}

	list_node(T&& value)
		: value(mystl::move(value)) {}

	base_ptr as_base()
	{
		return static_cast<base_ptr>(&*this);
	}

	node_ptr self()
	{
		return static_cast<node_ptr>(&*this);
	}
};

// list �ĵ��������
template <typename T>
struct list_iterator : public mystl::iterator<mystl::bidirectional_iterator_tag, T>
{
	using value_type			= T;
	using pointer				= T*;
	using reference				= T&;
	using base_ptr				= node_traits<T>::base_ptr;
	using node_ptr				= node_traits<T>::node_ptr;
	using self					= list_iterator<T>;

	base_ptr node_;		//ָ��ǰ�ڵ�

	list_iterator() = default;
	list_iterator(base_ptr node)
		: node_(node) {}
	list_iterator(node_ptr node)
		: node_(node->as_base()) {}
	list_iterator(const list_iterator& rhs)
		: node_{ rhs.node_ } {}

	// ���ز�����
	reference operator*() const { return node_->as_node()->value; }
	pointer operator->() const { return &(operator*()); }

	self& operator++()
	{
		MYSTL_DEBUG(node_ != nullptr);
		node_ = node_->next;
		return *this;
	}

	self operator++(int)
	{
		self tmp{ *this };
		++*this;
		return tmp;
	}

	self operator--()
	{
		MYSTL_DEBUG(node_ != nullptr);
		node_ = node_->prev;
		return *this;
	}

	self operator--(int)
	{
		self tmp{ *this };
		--*this;
		return tmp;
	}

	// ���رȽϲ�����
	bool operator==(const self& rhs) const { return node_ == rhs.node_; }
	bool operator!=(const self& rhs) const { return node_ != rhs.node_; }
};

template <typename T>
struct list_const_iterator : public iterator<bidirectional_iterator_tag, T>
{
	using value_type			= T;
	using pointer				= const T*;
	using reference				= const T&;
	using base_ptr				= node_traits<T>::base_ptr;
	using node_ptr				= node_traits<T>::node_ptr;
	using self					= list_const_iterator<T>;

	base_ptr node_;

	list_const_iterator() = default;
	list_const_iterator(base_ptr node)
		:node_(node) {}
	list_const_iterator(node_ptr node)
		:node_(node->as_base()) {}
	list_const_iterator(const list_iterator<T>& rhs)
		:node_(rhs.node_) {}
	list_const_iterator(const list_const_iterator& rhs)
		:node_{ rhs.node_ } {}

	reference operator*() const { return node_->as_node()->value; }
	pointer operator->() const { return &(operator*()); }

	self& operator++()
	{
		MYSTL_DEBUG(node_ != nullptr);
		node_ = node_->next;
		return *this;
	}
	self operator++(int)
	{
		self tmp{ *this };
		++*this;
		return tmp;
	}
	self& operator--()
	{
		MYSTL_DEBUG(node_ != nullptr);
		node_ = node_->prev;
		return *this;
	}
	self operator--(int)
	{
		self tmp{ *this };
		--*this;
		return tmp;
	}

	// ���رȽϲ�����
	bool operator==(const self& rhs) const { return node_ == rhs.node_; }
	bool operator!=(const self& rhs) const { return node_ != rhs.node_; }
};

// ģ����: list
// ģ����� T ������������
template <typename T>
class list
{
public:
	// list ��Ƕ���ͱ���
	using allocator_type			= mystl::allocator<T>;
	using data_allocator			= mystl::allocator<T>;
	using base_allocator			= mystl::allocator<list_node_base<T>>;
	using node_allocator			= mystl::allocator<list_node<T>>;

	using value_type				= allocator_type::value_type;
	using pointer					= allocator_type::pointer;
	using const_pointer				= allocator_type::const_pointer;
	using reference					= allocator_type::reference;
	using const_reference			= allocator_type::const_reference;
	using size_type					= allocator_type::size_type;
	using difference_type			= allocator_type::difference_type;

	using iterator					= list_iterator<T>;
	using const_iterator			= list_const_iterator<T>;
	using reverse_iterator			= mystl::reverse_iterator<iterator>;
	using const_reverse_iterator	= mystl::reverse_iterator<const_iterator>;

	using base_ptr					= node_traits<T>::base_ptr;
	using node_ptr					= node_traits<T>::node_ptr;

	allocator_type get_allocator() { return node_allocator(); }

private:
	base_ptr  node_;  // ָ��ĩβ�ڵ�
	size_type size_;  // ��С

public:
	list()
	{ 
		fill_init(0, value_type()); 
	}

	explicit list(size_type n)
	{ 
		fill_init(n, value_type()); 
	}

	list(size_type n, const T& value)
	{
		fill_init(n, value);
	}

	template <typename Iter, mystl::enable_if_t<
		mystl::is_input_iterator<Iter>::value, int> = 0>
	list(Iter first, Iter last)
	{
		copy_init(first, last);
	}

	list(std::initializer_list<T> ilist)
	{
		copy_init(ilist.begin(), ilist.end());
	}

	list(const list& rhs)
	{
		copy_init(rhs.cbegin(), rhs.cend());
	}

	list(list&& rhs) noexcept
		:node_{ rhs.node_ }, size_{ rhs.size_ }
	{
		rhs.node_ = nullptr;
		rhs.size_ = 0;
	}

	list& operator=(const list& rhs)
	{
		if (this != &rhs)
		{
			assign(rhs.begin(), rhs.end());
		}
		return *this;
	}

	list& operator=(list&& rhs) noexcept
	{
		clear();
		splice(end(), rhs);
		return *this;
	}

	list& operator=(std::initializer_list<T> ilist)
	{
		list temp(ilist.begin(), ilist.end());
		swap(temp);
		return *this;
	}

	~list()
	{
		if (node_)
		{
			clear();
			base_allocator::deallocate(node_);
			node_ = nullptr;
			size_ = 0;
		}
	}

public:
	// ��������ز���
	iterator begin() noexcept
	{
		return node_->next;
	}
	const_iterator begin() const noexcept
	{
		return node_->next;
	}

	iterator end() noexcept
	{
		return node_;
	}
	const_iterator end() const noexcept
	{
		return node_;
	}

	reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(end());
	}

	reverse_iterator rend() noexcept
	{
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend() const noexcept
	{
		return reverse_iterator(begin());
	}

	const_iterator cbegin() const noexcept
	{
		return begin();
	}
	const_iterator cend() const noexcept
	{
		return end();
	}

	const_reverse_iterator crbegin() const noexcept
	{
		return rbegin();
	}
	const_reverse_iterator crend() const noexcept
	{
		return cend();
	}

	// ������ز���
	bool empty() const noexcept { return node_->next == node_; }
	size_type size() const noexcept { return size_; }
	size_type max_size() const noexcept { return static_cast<size_type>(-1); }

	// ����Ԫ����ز���
	reference front()
	{
		MYSTL_DEBUG(!empty());
		return *begin();
	}

	const_reference front() const
	{
		MYSTL_DEBUG(!empty());
		return *begin();
	}

	reference back()
	{
		MYSTL_DEBUG(!empty());
		return *(--end());
	}

	const_reference back()  const
	{
		MYSTL_DEBUG(!empty());
		return *(--end());
	}

	// ����������ز���

	// assign
	void assign(size_type n, const value_type& value)
	{
		fill_assign(n, value);
	}

	template <typename Iter, mystl::enable_if_t<
		mystl::is_input_iterator<Iter>::value, int> = 0>
	void assign(Iter first, Iter last)
	{
		copy_assign(first, last);
	}

	void assign(std::initializer_list<T> ilist)
	{
		copy_assign(ilist.begin(), ilist.end());
	}

	// emplace_front / emplace_back / emplace
	template <typename ...Args>
	void emplace_front(Args&&...args)
	{
		THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
		auto link_node = create_node(mystl::forward<Args>(args)...);
		link_nodes_at_front(link_node->as_base(), link_node->as_base());
		++size_;
	}

	template <typename ...Args>
	void emplace_back(Args&& ...args)
	{
		THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
		auto link_node = create_node(mystl::forward<Args>(args)...);
		link_nodes_at_back(link_node->as_base(), link_node->as_base());
		++size_;
	}

	template <typename ...Args>
	iterator emplace(const_iterator pos, Args&& ...args)
	{
		THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
		auto link_node = create_node(mystl::forward<Args>(args)...);
		link_nodes(pos.node_, link_node->as_base(), link_node->as_base());
		++size_;
		return iterator(link_node);
	}

	// insert
	iterator insert(const_iterator pos, const value_type& value)
	{
		THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
		auto link_node = create_node(value);
		++size_;
		return link_iter_node(pos, link_node->as_base());
	}

	iterator insert(const_iterator pos, value_type&& value)
	{
		THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
		auto link_node = create_node(mystl::move(value));
		++size_;
		return link_iter_node(pos, link_node->as_base());
	}

	iterator insert(const_iterator pos, size_type n, const value_type& value)
	{
		THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "list<T>'s size too big");
		return fill_insert(pos, n, value);
	}

	template <typename Iter, mystl::enable_if_t<
		mystl::is_input_iterator<Iter>::value, int> = 0>
	iterator insert(const_iterator pos, Iter first, Iter last)
	{
		size_type n = mystl::distance(first, last);
		THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "list<T>'s size too big");
		return copy_insert(pos, n, first);
	}

	// push_front / push_back
	void push_front(const value_type& value)
	{
		THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
		auto link_node = create_node(value);
		link_nodes_at_front(link_node->as_base(), link_node->as_base());
		++size_;
	}

	void push_front(value_type&& value)
	{
		emplace_front(mystl::move(value));
	}

	void push_back(const value_type& value)
	{
		THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
		auto link_node = create_node(value);
		link_nodes_at_back(link_node->as_base(), link_node->as_base());
		++size_;
	}

	void push_back(value_type&& value)
	{
		emplace_back(mystl::move(value));
	}

	// pop_front / pop_back

	void pop_front()
	{
		MYSTL_DEBUG(!empty());
		auto n = node_->next;
		unlink_nodes(n, n);
		destroy_node(n->as_node());
		--size_;
	}

	void pop_back()
	{
		MYSTL_DEBUG(!empty());
		auto n = node_->prev;
		unlink_nodes(n, n);
		destroy_node(n->as_node());
		--size_;
	}

	// erase / clear

	// ɾ�� pos ����Ԫ��
	iterator erase(const_iterator pos)
	{
		MYSTL_DEBUG(pos != cend());
		auto n = pos.node_;
		auto next = n->next;
		unlink_nodes(n, n);
		destroy_node(n->as_node());
		--size_;
		return iterator(next);
	}
	iterator erase(const_iterator first, const_iterator last)
	{
		if (first != last)
		{
			unlink_nodes(first.node_, last.node_->prev);
			while (first != last)
			{
				auto cur = first.node_;
				++first;
				destroy_node(cur->as_node());
				--size_;
			}
		}
		return iterator(last.node_);
	}

	void clear()
	{
		if (size_ != 0)
		{
			auto cur = node_->next;
			for (base_ptr next = cur->next; cur != node_; cur = next, next = cur->next)
			{
				destroy_node(cur->as_node());
			}
			node_->unlink();
			size_ = 0;
		}
	}

	// resize

	void resize(size_type new_size) { resize(new_size, value_type()); }
	void resize(size_type new_size, const value_type& value)
	{
		auto i = begin();
		size_type len = 0;
		while (i != end() && len < new_size)
		{
			++i;
			++len;
		}
		if (len == new_size)
		{
			erase(i, node_);
		}
		else
		{
			insert(node_, new_size - len, value);
		}
	}

	void swap(list& rhs) noexcept
	{
		mystl::swap(node_, rhs.node_);
		mystl::swap(size_, rhs.size_);
	}

	// list ��ز���

	// �� list x �Ӻ��� pos ֮ǰ
	void splice(const_iterator pos, list& other)
	{
		MYSTL_DEBUG(this != &other);
		if (!other.empty())
		{
			THROW_LENGTH_ERROR_IF(size_ > max_size() - other.size_, "list<T>'s size too big");
			auto first = other.node_->next;
			auto last = other.node_->prev;
			other.unlink_nodes(first, last);
			link_nodes(pos.node_, first, last);

			size_ += other.size_;
			other.size_ = 0;
		}
	}

	// �� it ��ָ�Ľڵ�Ӻ��� pos ֮ǰ
	void splice(const_iterator pos, list& other, const_iterator it)
	{
		if (pos.node_ != it.node_ && pos.node_ != it.node_->next)
		{
			THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");

			auto first = it.node_;

			other.unlink_nodes(first, first);
			link_nodes(pos.node_, first, first);

			++size_;
			--other.size_;
		}
	}

	// �� list x �� [first, last) �ڵĽڵ�Ӻ��� pos ֮ǰ
	void splice(const_iterator pos, list& other, const_iterator first, const_iterator last)
	{
		if (first != last && this != &other)
		{
			size_type n = mystl::distance(first, last);
			THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "list<T>'s size too big");
			auto f = first.node_;
			auto l = last.node_->prev;

			other.unlink_nodes(f, l);
			link_nodes(pos.node_, f, l);

			size_ += n;
			other.size_ -= n;
		}
	}

	void remove(const value_type& value)
	{
		remove_if( [&](const value_type& v) {
			return v == value;
		});
	}

	// ����һԪ���� pred Ϊ true ������Ԫ���Ƴ�
	template <typename UnaryPredicate>
	void remove_if(UnaryPredicate pred)
	{
		auto first = begin();
		auto last = end();

		for (auto next = first; first != last; first = next)
		{
			++next;
			if (pred(*first))
			{
				erase(first);
			}
		}
	}

	void unique()
	{
		unique(mystl::equal_to<T>());
	}

	template <typename BinaryPredicate>
	void unique(BinaryPredicate pred)
	{
		auto i = begin();
		auto e = end();
		auto j = i;
		++j;
		while (j != e)
		{
			if (pred(*i, *j))
			{
				erase(j);
			}
			else
			{
				i = j;
			}
			j = i;
			++j;
		}
	}

	void merge(list& other)
	{
		merge(other, mystl::less<T>());
	}

	template <typename Compare>
	void merge(list& other, Compare comp)
	{
		if (this != &other)
		{
			THROW_LENGTH_ERROR_IF(size_ > max_size() - other.size_, "list<T>'s size too big");
			auto first1 = begin();
			auto last1 = end();
			auto first2 = other.begin();
			auto last2 = other.end();

			while (first1 != last1 && first2 != last2)
			{
				if (comp(*first2, *first1))
				{
					// ʹ comp Ϊ true ��һ������
					auto next = first2;
					++next;
					for (; next != last2 && comp(*next, *first1); ++next)
						;
					auto first = first2.node_;
					auto last = next.node_->prev;
					first2 = next;

					other.unlink_nodes(first, last);
					link_nodes(first1.node_, first, last);
					++first1;
				}
				else
				{
					++first1;
				}
			}
			// ����ʣ�ಿ��
			if (first2 != last2)
			{
				auto first = first2.node_;
				auto last = last2.node_->prev;
				other.unlink_nodes(first, last);
				link_nodes(last1.node_, first, last);
			}
			size_ += other.size_;
			other.size_ = 0;
		}
	}

	void sort()
	{
		list_sort(begin(), end(), size(), mystl::less<T>());
	}
	template <typename Compared>
	void sort(Compared comp)
	{
		list_sort(begin(), end(), size(), comp);
	}

	// �� list ��ת
	void reverse()
	{
		if (size_ <= 1)
		{
			return;
		}

		auto i = begin();
		auto e = end();
		while (i.node_ != e.node_)
		{
			mystl::swap(i.node_->prev, i.node_->next);
			i.node_ = i.node_->prev;
		}
		mystl::swap(e.node_->prev, e.node_->next);
	}
private:
	// helper functions

	// create / destroy node
	template <typename ...Args>
	node_ptr create_node(Args&& ...args)
	{
		node_ptr p = node_allocator::allocate(1);
		try
		{
			data_allocator::construct(mystl::address_of(p->value), mystl::forward<Args>(args)...);
			p->prev = nullptr;
			p->next = nullptr;
		}
		catch (...)
		{
			node_allocator::deallocate(p);
			throw;
		}
		return p;
	}

	// ���ٽ��
	void destroy_node(node_ptr p)
	{
		data_allocator::destroy(mystl::address_of(p->value));
		node_allocator::deallocate(p);
	}

	// initialize
	// �� n ��Ԫ�س�ʼ������
	void fill_init(size_type n, const value_type& value)
	{
		node_ = base_allocator::allocate(1);
		node_->unlink();
		size_ = n;

		try
		{
			for (; n > 0; --n)
			{
				auto node = create_node(value);
				link_nodes_at_back(node->as_base(), node->as_base());
			}
		}
		catch (...)
		{
			clear();
			base_allocator::deallocate(node_);
			node_ = nullptr;
			throw;
		}
	}

	// �� [first, last) ��ʼ������
	template <typename Iter>
	void copy_init(Iter first, Iter last)
	{
		node_ = base_allocator::allocate(1);
		node_->unlink();
		size_type n = mystl::distance(first, last);
		size_ = n;

		try
		{
			for (; n > 0; --n, ++first)
			{
				auto node = create_node(*first);
				link_nodes_at_back(node->as_base(), node->as_base());
			}
		}
		catch (...)
		{
			clear();
			base_allocator::deallocate(node_);
			node_ = nullptr;
			throw;
		}
	}

	// �� pos ������һ���ڵ�
	// link / unlink
	iterator link_iter_node(const_iterator pos, base_ptr link_node)
	{
		if (pos == node_->next)
		{
			link_nodes_at_front(link_node, link_node);
		}
		else if (pos == node_)
		{
			link_nodes_at_back(link_node, link_node);
		}
		else
		{
			link_nodes(pos.node_, link_node, link_node);
		}
		return iterator(link_node);
	}

	// �� pos ������ [first, last] �Ľ��
	void link_nodes(base_ptr pos, base_ptr first, base_ptr last)
	{
		pos->prev->next = first;
		first->prev = pos->prev;
		pos->prev = last;
		last->next = pos;
	}

	// ��ͷ������ [first, last] ���
	void link_nodes_at_front(base_ptr first, base_ptr last)
	{
		first->prev = node_;
		last->next = node_->next;
		last->next->prev = last;
		node_->next = first;
	}

	// ��β������ [first, last] ���
	void link_nodes_at_back(base_ptr first, base_ptr last)
	{
		last->next = node_;
		first->prev = node_->prev; 
		first->prev->next = first;
		node_->prev = last;
	}

	// ������ [first, last] ���Ͽ�����
	void unlink_nodes(base_ptr first, base_ptr last)
	{
		first->prev->next = last->next;
		last->next->prev = first->prev;
	}

	// assign
	
	// �� n ��Ԫ��Ϊ������ֵ
	void fill_assign(size_type n, const value_type& value)
	{
		auto i = begin();
		auto e = end();
		for (; n > 0 && i != e; --n, ++i)
		{
			*i = value;
		}
		if (n > 0)
		{
			insert(e, n, value);
		}
		else
		{
			erase(i, e);
		}
	}

	// ����[f2, l2)Ϊ������ֵ
	template <typename Iter>
	void copy_assign(Iter first, Iter last)
	{
		auto first1 = begin();
		auto last1 = end();
		for (; first1 != last1 && first != last; ++first1, ++first)
		{
			*first1 = *first;
		}
		if (first == last)
		{
			erase(first1, first1);
		}
		else
		{
			insert(last1, first, last);
		}
	}

	// insert
	// �� pos ������ n ��Ԫ��
	iterator fill_insert(const_iterator pos, size_type n, const value_type& value)
	{
		iterator r(pos.node_);
		if (n != 0)
		{
			const auto add_size = n;
			auto node = create_node(value);
			node->prev = nullptr;
			r = iterator(node);
			iterator end = r;
			try
			{
				// ǰ���Ѿ�������һ���ڵ㣬���� n - 1 ��
				for (--n; n > 0; --n, ++end)
				{
					auto next = create_node(value);
					end.node_->next = next->as_base();  // link node
					next->prev = end.node_;
				}
				size_ += add_size;
			}
			catch (...)
			{
				auto enode = end.node_;
				while (true)
				{
					auto prev = enode->prev;
					destroy_node(enode->as_node());
					if (prev == nullptr)
						break;
					enode = prev;
				}
				throw;
			}
			link_nodes(pos.node_, r.node_, end.node_);
		}
		return r;
	}

	// �� pos ������ [first, last) ��Ԫ��
	template <typename Iter>
	iterator copy_insert(const_iterator pos, size_type n, Iter first)
	{
		iterator r(pos.node_);
		if (n != 0)
		{
			const auto add_size = n;
			auto node = create_node(*first);
			node->prev = nullptr;
			r = iterator(node);
			iterator end = r;
			try
			{
				for (--n, ++first; n > 0; --n, ++first, ++end)
				{
					auto next = create_node(*first);
					end.node_->next = next->as_base();  // link node
					next->prev = end.node_;
				}
				size_ += add_size;
			}
			catch (...)
			{
				auto enode = end.node_;
				while (true)
				{
					auto prev = enode->prev;
					destroy_node(enode->as_node());
					if (prev == nullptr)
						break;
					enode = prev;
				}
				throw;
			}
			link_nodes(pos.node_, r.node_, end.node_);
		}
		return r;
	}

	// sort
	// �� list ���й鲢���򣬷���һ��������ָ��������СԪ�ص�λ��
	template <typename Compared>
	iterator list_sort(iterator first1, iterator last2, size_type n, Compared comp)
	{
		if (n < 2)
			return first1;

		if (n == 2)
		{
			if (comp(*--last2, *first1))
			{
				auto ln = last2.node_;
				unlink_nodes(ln, ln);
				link_nodes(first1.node_, ln, ln);
				return last2;
			}
			return first1;
		}

		auto n2 = n / 2;
		auto l1 = first1;
		mystl::advance(l1, n2);
		auto result = first1 = list_sort(first1, l1, n2, comp);  // ǰ��ε���Сλ��
		auto f2 = l1 = list_sort(l1, last2, n - n2, comp);  // ���ε���Сλ��

		// �ѽ�С��һ�������Ƶ�ǰ��
		if (comp(*f2, *first1))
		{
			auto m = f2;
			++m;
			for (; m != last2 && comp(*m, *first1); ++m)
				;
			auto f = f2.node_;
			auto l = m.node_->prev;
			result = f2;
			l1 = f2 = m;
			unlink_nodes(f, l);
			m = first1;
			++m;
			link_nodes(first1.node_, f, l);
			first1 = m;
		}
		else
		{
			++first1;
		}

		// �ϲ�������������
		while (first1 != l1 && f2 != last2)
		{
			if (comp(*f2, *first1))
			{
				auto m = f2;
				++m;
				for (; m != last2 && comp(*m, *first1); ++m)
					;
				auto f = f2.node_;
				auto l = m.node_->prev;
				if (l1 == f2)
					l1 = m;
				f2 = m;
				unlink_nodes(f, l);
				m = first1;
				++m;
				link_nodes(first1.node_, f, l);
				first1 = m;
			}
			else
			{
				++first1;
			}
		}
		return result;
	}
};


// ���رȽϲ�����
template <typename T>
bool operator==(const list<T>& lhs, const list<T>& rhs)
{
	auto f1 = lhs.cbegin();
	auto f2 = rhs.cbegin();
	auto l1 = lhs.cend();
	auto l2 = rhs.cend();
	for (; f1 != l1 && f2 != l2 && *f1 == *f2; ++f1, ++f2)
		;
	return f1 == l1 && f2 == l2;
}

template <typename T>
bool operator<(const list<T>& lhs, const list<T>& rhs)
{
	return mystl::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <typename T>
bool operator!=(const list<T>& lhs, const list<T>& rhs)
{
	return !(lhs == rhs);
}

template <typename T>
bool operator>(const list<T>& lhs, const list<T>& rhs)
{
	return rhs < lhs;
}

template <typename T>
bool operator<=(const list<T>& lhs, const list<T>& rhs)
{
	return !(rhs < lhs);
}

template <typename T>
bool operator>=(const list<T>& lhs, const list<T>& rhs)
{
	return !(lhs < rhs);
}

// ���� mystl �� swap
template <typename T>
void swap(list<T>& lhs, list<T>& rhs) noexcept
{
	lhs.swap(rhs);
}
} // namespace mystl

#endif // !MY_SLT_LIST_H_
