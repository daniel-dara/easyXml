#ifndef SET_H
#define SET_H

template <typename T>
class Iterable
{
protected:
	enum ITERATOR_STATUS
	{
		INVALID = 0,
		BEGIN = 1,
		MIDDLE = 2,
		END = 3
	};

	class IterableNode_;

public:
	class iterator;

	// virtual iterator& begin(void* childClass)
	// {
	// 	return iterator(BEGIN, static_cast<Iterable<int>::IterableNode_ *>(childClass));
	// }

	// virtual iterator& end(void* childClass)
	// {
	// 	return iterator(END, static_cast<Iterable<int>::IterableNode_ *>(childClass));
	// }

	class iterator
	{
	public:
		iterator() :
			status_(INVALID),
			iterNode_(NULL)
		{ }

		/// Returns true if the iterators point to the same node, have the same status, and are both valid.
		bool operator==(const iterator& rhs) const
		{
			return (iterNode_ == iterNode_) && (status_ == rhs.status_) && (status_ != INVALID);
		}

		bool operator!=(const iterator& rhs) const
		{
			return !(*this == rhs);
		}

		/// Assignment - Shallow copy of 'rhs' to 'this'.
		iterator& operator=(const iterator& rhs)
		{
			if (this != & rhs)
			{
				status_ = rhs.status_;
				iterNode_ = rhs.iterNode_;
			}

			return *this;
		}

		/// Return the value of the node that iterator points to.
		/*
		 * Undefined behavior if the iterator is invalid or points to begin() or end() of
		 * some iterable class.
		 */
		T operator*()
		{
			return iterNode_->value;
		}

		/// Move iterator to the next node.
		/*
		 * Undefined behavior if the iterator points to end() of some iterable class.
		 */
		virtual iterator operator++(int test)
		{
			iterator temp(*this);

			iterNode_ = iterNode_->next();

			if (iterNode_ == NULL)
			{
				status_ = END;
			}

			return temp;
		}

	public: // TODO: hide this somehow
		iterator(ITERATOR_STATUS status, IterableNode_* iterNode = NULL) :
			status_(status),
			iterNode_(iterNode)
		{ }

	private:
		ITERATOR_STATUS status_;
		IterableNode_* iterNode_;
	};

protected:
	class IterableNode_
	{
	public:
		IterableNode_(T val) :
			value(val)
		{ }

		T value;
		virtual IterableNode_* next() = 0;
	};
};

template <typename T>
class Set : public Iterable<T>
{
private:
	class Node_;

public:
	Set() : root_(NULL)
	{ }

	// class iterator : public Iterable<T>::iterator
	// {
	// public:
	// 	iterator() : Iterable<T>::iterator()
	// 	{ }

	// 	iterator(const typename Iterable<T>::iterator rhs) : Iterable<T>::iterator(rhs)
	// 	{ }
	// };

	virtual typename Set<T>::iterator begin()
	{
		return typename Iterable<T>::iterator(Iterable<T>::BEGIN, Node_::leftMost(root_));
		// return Iterable<T>::begin(this);
	}

	virtual typename Set<T>::iterator end()
	{
		return typename Iterable<T>::iterator(Iterable<T>::END);
		// return Iterable<T>::end(this);
	}

	bool insert(T val)
	{
		Node_* node = root_;
		Node_* parent = NULL;
		Node_** next_node = &root_;

		while (*next_node != NULL)
		{
			parent = *next_node;

			if (val < node->value)
			{
				next_node = &node->left;
			}
			else if (node->value < val)
			{
				next_node = &node->right;
			}
			else
			{
				// value already exists in set
				return false;
			}

			node = *next_node;
		}

		*next_node = new Node_(val);
		(*next_node)->parent = parent;

		return true;
	}

	T find(T val)
	{
		Node_* node = root_;

		while (node != NULL)
		{
			if (val < node->value)
			{
				node = node->left;
			}
			else if (node->value < val)
			{
				node = node->right;
			}
			else
			{
				// value found
				return node->value;
			}

			return NULL;
		}
	}

private:


	class Node_ : public Iterable<T>::IterableNode_
	{
	public:
		Node_(T val, Node_* _left = NULL, Node_* _right = NULL) :
			Iterable<T>::IterableNode_(val),
			left(_left),
			right(_right)
		{ }

		static Node_* leftMost(Node_* node)
		{
			Node_* previous = node;

			while (node != NULL)
			{
				previous = node;
				node = node->left;
			}

			return previous;
		}

		virtual Node_* next()
		{
// std::cout << "current: " << Iterable<T>::IterableNode_::value << "\n";
			Node_* node = leftMost(this->right);

			if (node != NULL)
			{
				// std::cout << "leftMost: " << node->value << "\n";
				return node;
			}

			if (parent == NULL)
			{
				return NULL; // no successor, end of tree
			}

// std::cout << "parent: " << parent->value << "\n";
			node = this;

			// while (node->parent != NULL)
			// {
				
			// }
		uprule:
			// std::cout << "node: " << node << " parent: " << parent << "\n";
			// check if left child (right turn)
			if (node == parent->left) 
			{
				// std::cout << "I am left child\n";
				return parent;
			}

			// std::cout << "parent is: " << parent->value << "\n";
			if (node->parent == NULL)
			{
				return NULL;
			}

			node = node->parent;
			goto uprule;
		}

		Node_* parent;
		Node_* left;
		Node_* right;
	};

	Node_* root_;
};

#endif
