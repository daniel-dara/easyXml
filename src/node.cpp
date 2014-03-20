#include "node.h"
#include <iostream>
#include <typeinfo>
#include <cstdlib>

namespace EASYXML_NAMESPACE
{
	// Default Constructor
	Node::Node() : children(node_ptr_compare), name(), value()
	{ }

	// Conversion Constructors
	Node::Node(const char* _name, const char* _val) : children(node_ptr_compare), name(_name), value(_val)
	{ }

	Node::Node(const std::string& _name, const std::string& _val) : children(node_ptr_compare), \
	           name(_name), value(_val)
	{ }

	// Copy Constructor - technically a deep copy since the "children" set is copied
	// but note that the set is just a bunch of pointers. The actual child nodes are not copied.
	Node::Node(const Node& rhs) : children(node_ptr_compare), name(rhs.name), value(rhs.value)
	{
		for (std::set<Node*>::const_iterator ite = rhs.children.begin(); ite != rhs.children.end(); ite++)
		{
			children.insert(*ite);
		}
	}

	// Assignment - deep copy (beware, this loses all children pointers but does not free them)
	Node& Node::operator=(const Node& rhs)
	{
		if (this != &rhs)
		{
			children.clear();

			for (std::set<Node*>::const_iterator ite = rhs.children.begin(); ite != rhs.children.end(); ite++)
			{
				children.insert(*ite);
			}

			name = rhs.name;
			value = rhs.value;
		}

		return *this;
	}

	Node* Node::findNode(const std::string path, bool returnNull) const
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

		static std::set<Node*>::iterator iter;
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

		if (returnNull)
		{
			return NULL;
		}
		else
		{
			throw EasyXmlException("Child element \"" + path + "\" not found.", 103);
		}
	}

	const std::string& Node::getName() const
	{
		return name;
	}

	void Node::setName(const std::string& newName)
	{
		name = newName;
	}

	// Provides a "default" type for the templated getValue() without using C++11.
	const std::string& Node::val() const
	{
		return value;
	}

	// This function will only be called if one of the type didn't match one of the specializations.
	template <typename T>
	T Node::val() const
	{
		throw EasyXmlException("getValue() request: Unsupported type \"" + \
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

	template <typename T>
	void Node::setVal(T val)
	{
		value = std::string(val);
	}
}
