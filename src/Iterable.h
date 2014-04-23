#ifndef ITERABLE_H
#define ITERABLE_H

template <typename T>
class Iterable
{
protected:
	// This enum must be a protected member of class Iterable itself (as opposed to class iterator) so that
	// derived classes will have access to it. Derived classes use this enum when initializing an iterator in
	// the begin() or end() functions.
	enum ITERATOR_STATUS
	{
		INVALID = 0,
		BEGIN = 1,
		MIDDLE = 2,
		END = 3
	};

	// Forward declaration. This class is used in class iterator.
	class IterableNode_;

public:
	virtual ~Iterable() {}

	class iterator;

	/*
	 * These functions should be purely virtual but C++'s rules surrounding co-variant return types are too
	 * strict. Even though the begin() and end() methods of any Iterable derived class will return an
	 * iterator derived from Iterable::iterator, the return type MUST be a pointer or reference to be a valid
	 * override for the purely virtual base class method (seen below). This is a silly constraint and sadly
	 * prevents the use of purely virtual functions in this case.
	 *
	 * Also, we must return a copy since begin() and end() iterators do not exist until the method is called.
	 * It is not worth keeping a local copy of begin() and end() in every instance of a derived class just
	 * so we can enable these purely virtual functions and return a reference or pointer to it.
	 */
	// virtual const iterator begin() = 0;
	// virtual const iterator end() = 0;

	/*
	 * This class is nested inside of "Iterable" so that when another class (presumably a data structure)
	 * inherits from it, an iterator can be instantiated from the scope of the derived class.
	 *
	 * Ex: List<int>::iterator ite;
	 *
	 * Any methods called on a uninitialized iterator (state == INVALID) are undefined with the exception
	 * of assignment, which doesn't rely on an iterator's previous values, and relational operations (=, !=).
	 */
	class iterator
	{
	public:
		/// Constructor
		/*
		 * Note that an iterator is initialized to INVALID. It must be made valid by having a valid iterator
		 * assigned to it, such as begin() or end() of a derived class from Iterable.
		 */
		iterator() :
			status_(INVALID),
			iterNode_(NULL)
		{ }

		virtual ~iterator() {}

		/// Returns true if the iterators point to the same node address, have the same status, and are not INVALID.
		bool operator==(const iterator& rhs) const
		{
			return ((iterNode_ == rhs.iterNode_) && (status_ == rhs.status_) && (status_ != INVALID)) ||
			       ((iterNode_ == NULL) && (rhs.iterNode_ == NULL) && (status_ != INVALID) && (rhs.status_ != INVALID));
		}

		/// Returns the logical not of operator==
		bool operator!=(const iterator& rhs) const
		{
			return !operator==(rhs);
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

		/// Returns the value of the node that the iterator currently points to.
		/*
		 * Undefined behavior if points to end() of some iterable class.
		 */
		T operator*() const
		{
			return iterNode_->value;
		}

		/// Moves the iterator by "distance" elements. "distance" can be negative to move backwards.
		/*
		 * Complexity depends on how the derived class implements next() and previous() for its nodes.
		 * At best, advance is linear with respect to "distance".
		 */
		iterator& advance(int distance)
		{
			while (distance > 0)
			{
				IterableNode_* temp = iterNode_->next();

				if (temp == NULL)
				{
					status_ = END;
				}
				else
				{
					iterNode_ = temp;
				}

				distance--;
			}

			while (distance < 0)
			{
				if (status_ != END)
				{
					iterNode_ = iterNode_->prev();
				}
				else
				{
					status_ = MIDDLE;
				}

				if (iterNode_->prev() == NULL)
				{
					status_ = BEGIN;
				}

				distance++;
			}

			return *this;
		}

		/// Prefix - Move iterator to the next node, then return the value.
		/*
		 * Undefined behavior if the iterator points to end() of some iterable class.
		 */
		iterator& operator++()
		{
			return advance(1);
		}

		/// Prefix - Move iterator to the previous node, then return the value.
		/*
		 * Undefined behavior if the iterator points to begin() of some iterable class.
		 */
		iterator& operator--()
		{
			return advance(-1);
		}

		/// Postfix - Returns current value and then moves iterator to the next node.
		/*
		 * Undefined behavior if the iterator points to end() of some iterable class.
		 */
		iterator operator++(int)
		{
			iterator temp(*this);

			advance(1); // call prefix

			return temp;
		}

		/// Postfix - Returns current value then moves iterator to the previous node.
		/*
		 * Undefined behavior if the iterator points to begin() of some iterable class.
		 */
		iterator operator--(int) const
		{
			iterator temp(*this);

			advance(-1); // call prefix

			return temp;
		}

		iterator operator+(int incr) const
		{
			iterator temp(*this);
			return temp.advance(incr);
		}

		iterator operator-(int decr) const
		{
			iterator temp(*this);
			return temp.advance(-decr);
		}

		void remove() const
		{
			delete iterNode_;
		}

	protected:
		iterator(ITERATOR_STATUS status, IterableNode_* iterNode = NULL) :
			status_(status),
			iterNode_(iterNode)
		{ }

	private:
		ITERATOR_STATUS status_;
		mutable IterableNode_* iterNode_;
	};

protected:
	class IterableNode_
	{
	public:
		IterableNode_(T val) :
			value(val)
		{ }

		virtual ~IterableNode_()
		{ }

		T value;
		virtual IterableNode_* next() = 0;
		virtual IterableNode_* prev() = 0;
	};
};

#endif
