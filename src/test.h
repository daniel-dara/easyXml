	class iterator : public Iterable<T>::iterator
	{
	public:
		iterator() :
			Iterable<T>::iterator()
		{ }

		iterator(typename Iterable<T>::ITERATOR_STATUS status, typename Iterable<T>::IterableNode_* iterNode = NULL) :
			Iterable<T>::iterator(status, iterNode)
		{ }
	};