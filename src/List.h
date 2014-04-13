#ifndef LIST_H
#define LIST_H

#include "Iterable.h"

template <typename T>
class List : public Iterable<T>
{
protected:
	class Node_ : public Iterable<T>::IterableNode_
	{
	public:
		Node_(T val, Node_* _left = NULL, Node_* _right = NULL) :
			Iterable<T>::IterableNode_(val),
			left(_left),
			right(_right)
		{ }

		virtual Node_* next()
		{
			return right;
		}

		virtual Node_* prev()
		{
			return left;
		}

		Node_* left;
		Node_* right;
	};

	Node_* head_;
	Node_* tail_;
	unsigned int size_;

public:
	List() : head_(NULL), tail_(NULL), size_(0)
	{ }

	~List()
	{
		Node_* cur = head_;
		Node_* next;

		while (cur != NULL)
		{
			next = cur->right;
			delete cur;
			cur = next;
		}
	}

	class iterator : public Iterable<T>::iterator
	{
	public:
		friend class List;

		iterator() :
			Iterable<T>::iterator()
		{ }

	private:
		iterator(typename Iterable<T>::ITERATOR_STATUS status, typename Iterable<T>::IterableNode_* iterNode = NULL) :
			Iterable<T>::iterator(status, iterNode)
		{ }
	};

	virtual const typename List<T>::iterator begin() const
	{
		return iterator(Iterable<T>::BEGIN, head_);
	}

	virtual const typename List<T>::iterator end() const
	{
		return iterator(Iterable<T>::END, tail_);
	}

	unsigned int size() const
	{
		return size_;
	}

	void push_back(T value)
	{
		if (head_ == NULL)
		{
			head_ = tail_ = new Node_(value);
		}
		else
		{
			tail_->right = new Node_(value);
			tail_->right->left = tail_;
			tail_ = tail_->right;
		}

		size_++;
	}

	void erase(const typename Iterable<T>::iterator& ite)
	{
		if (ite == begin())
		{
			head_ = head_->right;
		}

		if (ite == end() - 1)
		{
			tail_ = tail_->left;
		}

		size_--;

		ite.remove();
	}
};

#endif
