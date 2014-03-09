#ifndef NODE
#define NODE

#include <iostream>
#include <set>
#include <typeinfo>
#include <cstdlib>
#include "exception.h"
#include "namespace.h"

namespace EASYXML_NAMESPACE
{
	class Node
	{
	public:
		friend Node* loadXml(const std::string filePath);
		friend void printTree(const Node* node, std::string tabs);
		friend void deleteTree(Node* node);
		friend struct node_ptr_compare;

		Node();
		Node(const char* name);
		Node(const std::string name);
		Node(const Node& node);

		Node* findNode(const std::string path, bool returnNull = false) const;

		std::string getName() const;
		void setName(const std::string& newName);

		std::string val() const;
		template <class T> T val() const;
		template <class T> void setVal(T val);

	private:
		std::string name;
		std::string value;

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
