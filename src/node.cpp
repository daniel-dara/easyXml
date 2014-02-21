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

	Node* Node::findNode(const std::string path, bool useExceptions = false) const
	{
		// save cost of instantiation since findNode is recursive
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
				return (*iter)->findNode(restOfPath);
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

	// Provides a "default" type for the templated getValue() without using C++11.
	std::string Node::getValue()
	{
		return value;
	}

	// This function will only be called if one of the type didn't match one of the specializations.
	template <typename T>
	T Node::getValue()
	{
		throw EasyXmlException("getValue() request: Unsupported type \"" + std::string(typeid(T).name()) + "\"");
	}

	template<>
	std::string Node::getValue<std::string>()
	{
		return value;
	}

	template<>
	int Node::getValue<int>()
	{
		return atoi(value.c_str());
	}

	template<>
	double Node::getValue<double>()
	{
		return atof(value.c_str());
	}
}
