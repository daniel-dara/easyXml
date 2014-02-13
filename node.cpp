#include "node.h"

Node::Node() : isRoot(false)
{}

Node::Node(const char* name, bool is_root = false) : name(name), isRoot(is_root)
{}

Node::Node(const std::string name, bool is_root = false) : name(name), isRoot(is_root)
{}

Node::Node(const Node& node) : isRoot(node.isRoot)
{
	std::cout << "copy constructor called" << std::endl;
}

Node::~Node()
{
	std::cout << "~~~destructor for node->name: " + name + " was called." << std::endl; 
}

Node* Node::findElement(const std::string path, bool useExceptions = false) const
{
	// save cost of instantiation since findElement is recursive
	static Node query;
	std::string restOfPath;
	int slashIndex = path.find('/');

	if (slashIndex == -1)
	{
		query.name = path;
	}
	else
	{
		query.name = path.substr(0, slashIndex);
		restOfPath = path.substr(slashIndex + 1);
	}

	static std::set<Node*, node_ptr_compare>::iterator iter;
	iter = children.find(&query);

	if (iter != children.end())
	{
		if (restOfPath.length() == 0)
		{
			return (*iter);
		}
		else
		{
			return (*iter)->findElement(restOfPath);
		}
	}

	if (useExceptions)
	{
		throw easyXmlException("Child element \"" + path + "\" not found.");
	}
	else
	{
		return NULL;
	}
}

void Node::printTree(const Node* node, std::string tabs)
{
	std::cout << tabs + node->name << ": " << node->value << std::endl;

	std::set<Node*, node_ptr_compare>::iterator it;
	tabs += "\t";
	for (it = node->children.begin(); it != node->children.end(); ++it)
	{
		printTree((*it), tabs);
	}
}
