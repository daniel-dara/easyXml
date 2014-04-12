#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "Iterable.h"

template <typename T>
class BinaryTree : public Iterable<T>
{
protected:
	class Node_;

public:
	BinaryTree() : root_(NULL)
	{ }

	class iterator : public Iterable<T>::iterator
	{
	public:
		friend class BinaryTree;

		iterator() :
			Iterable<T>::iterator()
		{ }

	private:
		iterator(typename Iterable<T>::ITERATOR_STATUS status, typename Iterable<T>::IterableNode_* iterNode = NULL) :
			Iterable<T>::iterator(status, iterNode)
		{ }
	};

	virtual const typename BinaryTree<T>::iterator begin()
	{
		return iterator(Iterable<T>::BEGIN, Node_::leftMost(root_));
	}

	virtual const typename BinaryTree<T>::iterator end()
	{
		return iterator(Iterable<T>::END);
	}

	// virtual typename BinaryTree<T>::iterator begin()
	// virtual const typename Iterable<T>::iterator begin()
	// {
	// 	return typename Iterable<T>::iterator(Iterable<T>::BEGIN, Node_::leftMost(root_));
	// }

	// virtual const typename BinaryTree<T>::iterator end()
	// {
	// 	return typename Iterable<T>::iterator(Iterable<T>::END);
	// }

	bool insert(T val)
	{
		Node_* node = root_;
		Node_* parent = NULL;
		Node_** next_node = &root_;

		while (*next_node != NULL)
		{
			parent = *next_node;

			if (val <= node->value)
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

protected:
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

			// if (parent == NULL)
			// {
			// 	return NULL; // no successor, end of tree
			// }

// std::cout << "parent: " << parent->value << "\n";
			node = this;

			while (node->parent != NULL)
			{
				// std::cout << "node: " << node << " parent: " << parent << "\n";
				// check if left child (right turn)
				if (node == node->parent->left) 
				{
					// std::cout << "I am left child\n";
					return node->parent;
				}

				node = node->parent;
			}

			return NULL;
		}

		virtual Node_* prev()
		{
			return NULL; // not implemented yet
		}

		Node_* parent;
		Node_* left;
		Node_* right;
	};

	Node_* root_;
};

#endif
