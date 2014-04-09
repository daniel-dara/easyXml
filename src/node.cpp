#include "node.h"
#include <iostream>
#include <typeinfo>
#include <cstdlib>

namespace EASYXML_NAMESPACE
{
	// Default Constructor
	Node::Node() :
		name(),
		value(),
		children(),
		sortedChildren(node_ptr_compare)
	{ }

	// Conversion Constructors
	Node::Node(const char* _name, const char* _value) :
		name(_name),
		value(_value),
		children(),
		sortedChildren(node_ptr_compare)
	{ }

	Node::Node(const std::string& _name, const std::string& _value) :
		name(_name),
		value(_value),
		children(),
		sortedChildren(node_ptr_compare)
	{ }

	// Copy Constructor - technically a deep copy since the "children" set is copied
	// but note that the set is just a bunch of pointers. The actual child nodes are not copied.
	Node::Node(const Node& rhs) :
		name(rhs.name),
		value(rhs.value),
		children(),
		sortedChildren(node_ptr_compare)
	{
		for (std::vector<Node*>::const_iterator ite = rhs.children.begin();
		     ite != rhs.children.end();
		     ite++)
		{
			children.push_back(*ite);
		}

		for (std::set<Node*>::const_iterator ite = rhs.sortedChildren.begin();
		     ite != rhs.sortedChildren.end();
		     ite++)
		{
			sortedChildren.insert(*ite);
		}
	}

	// Assignment - deep copy (beware, this loses all children pointers but does not free them)
	Node& Node::operator=(const Node& rhs)
	{
		if (this != &rhs)
		{
			children.clear();
			
			for (std::vector<Node*>::const_iterator ite = rhs.children.begin();
			     ite != rhs.children.end();
			     ite++)
			{
				children.push_back(*ite);
			}

			sortedChildren.clear();

			for (std::set<Node*>::const_iterator ite = rhs.sortedChildren.begin();
			     ite != rhs.sortedChildren.end();
			     ite++)
			{
				sortedChildren.insert(*ite);
			}

			name = rhs.name;
			value = rhs.value;
		}

		return *this;
	}

	std::string Node::val() const
	{
		return value;
	}

	// This function will only be called if the given type didn't match any of the template
	// specializations below.
	template <typename T>
	T Node::val() const
	{
		throw EasyXmlException("Unsupported type requested in val(): \"" + \
		                       std::string(typeid(T).name()) + "\"");
	}

	template<>
	std::string Node::val<std::string>() const
	{
		return value;
	}

	template<>
	int Node::val<int>() const
	{
		return atoi(value.c_str());
	}

	template<>
	double Node::val<double>() const
	{
		return atof(value.c_str());
	}

	template<>
	float Node::val<float>() const
	{
		return static_cast<float>(atof(value.c_str()));
	}

	template<>
	bool Node::val<bool>() const
	{
		if (value == "true")
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	Node* Node::findNode(const std::string path, bool returnNull) const
	{
		// Save the cost of instantiation since findNode is recursive and its name is always overwritten.
		static Node query;

		std::string restOfPath;
		size_t slashIndex = path.find('/');

		if (slashIndex == std::string::npos)
		{
			query.name = path;
		}
		else
		{
			query.name = path.substr(0, slashIndex);
			restOfPath = path.substr(slashIndex + 1);
		}

		static std::set<Node*>::const_iterator iter;
		iter = sortedChildren.find(&query);

		if (iter != sortedChildren.end())
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

		if (returnNull)
		{
			return NULL;
		}
		else
		{
			throw EasyXmlException("Child element \"" + path + "\" not found.", 103);
		}
	}
}
