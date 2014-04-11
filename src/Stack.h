#ifndef STACK_H
#define STACK_H

#include "namespace.h"

template <typename T>
class Stack2
{
public:
	Stack2() : capacity_(1), top_(-1), array_(new T[capacity_])
	{ }

	Stack2(int capacity) : capacity_(capacity), top_(-1), array_(new T[capacity_])
	{ }

	~Stack2()
	{
		delete[] array_;
	}

	void push(T val)
	{
		if (top_ < capacity_)
		{
			array_[++top_] = val;
		}
	}

	T top()
	{
		if (top_ == -1)
		{
			return NULL;
		}
		else
		{
			return array_[top_];
		}
	}

	T pop()
	{
		if (top_ > -1)
		{
			return array_[top_--];
		}
		else
		{
			return NULL;
		}
	}

private:
	int capacity_;
	int top_;
	T* array_;
};

template <typename T>
class Stack
{
public:
	Stack() : size_(0), top_(NULL)
	{ }

	~Stack()
	{
		while (top_ != NULL)
		{
			pop();
		}
	}

	void push(T val)
	{
		if (top_ == NULL)
		{
			top_ = new Node_(val, NULL);
		}
		else
		{
			struct Node_* node = new Node_(val, top_);
			top_ = node;
		}
		size_++;
	}

	T top()
	{
		if (top_ == NULL)
		{
			return NULL;
		}
		else
		{
			return top_->val;
		}
	}

	T pop()
	{
		T val = top_->val;
		struct Node_* temp = top_;
		top_ = top_->next;
		delete temp;
		size_--;
		return val;
	}

	uint getSize() const
	{
		return size_;
	}

private:
	Stack(const Stack& rhs);
	Stack& operator=(const Stack& rhs);

	struct Node_
	{
		Node_(T Val, struct Node_* Next) : val(Val), next(Next)
		{ }

		T val;
		struct Node_* next;
	};

	uint size_;
	struct Node_* top_;
};

#endif
