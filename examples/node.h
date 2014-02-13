#ifndef NODE
#define NODE

#include <iostream>
#include <set>
#include "exception.h"

class Node
{
public:
	std::string name;
	std::string value;
	const bool isRoot;

	Node();

	Node(const char* name, bool is_root);

	Node(const std::string name, bool is_root);

	Node(const Node& node);

	~Node();

	Node* findElement(const std::string path, bool useExceptions) const;

	void printTree(const Node* node, std::string tabs = "");

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

#endif
