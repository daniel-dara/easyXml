#ifndef NODE
#define NODE

#include <iostream>
#include <set>
#include "exception.h"
#include "namespace.h"

namespace EASYXML_NAMESPACE
{
	class Node
	{
	public:
		// friend class Parser;
		friend void printTree(const Node* node, std::string tabs);
		friend void deleteTree(Node* node);

		std::string name;
		std::string value;

		Node();

		Node(const char* name);

		Node(const std::string name);

		Node(const Node& node);

		Node* findElement(const std::string path, bool useExceptions) const;

	private:
		struct node_ptr_compare
		{
			bool operator()(const Node* lhs, const Node* rhs) const
			{
				return lhs->name < rhs->name;
			}
		};

	public:
		std::set<Node*, node_ptr_compare> children;
	};
}

#endif
