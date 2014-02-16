#include "node.h"

namespace EASYXML_NAMESPACE
{
	Node::Node()
	{}

	Node::Node(const char* name) : name(name)
	{}

	Node::Node(const std::string name) : name(name)
	{}

	Node::Node(const Node& node)
	{
		std::cout << "Node: copy constructor for " + name << std::endl;
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
			throw EasyXmlException("Child element \"" + path + "\" not found.", 103);
		}
		else
		{
			return NULL;
		}
	}
}
