#include <iostream>
#include <typeinfo>
#include <cstdlib>
#include "Node.h"

namespace EASYXML_NAMESPACE
{
	// Default Constructor
	Node::Node() :
		name(),
		value(),
		parent_(NULL),
		firstChild_(NULL),
		lastChild_(NULL),
		nextSibling_(NULL),
		prevSibling_(NULL)
	{ }

	// Conversion Constructors
	Node::Node(const char* _name, const char* _value) :
		name(_name),
		value(_value),
		parent_(NULL),
		firstChild_(NULL),
		lastChild_(NULL),
		nextSibling_(NULL),
		prevSibling_(NULL)
	{ }

	Node::Node(const std::string& _name, const std::string& _value) :
		name(_name),
		value(_value),
		parent_(NULL),
		firstChild_(NULL),
		lastChild_(NULL),
		nextSibling_(NULL),
		prevSibling_(NULL)
	{ }

	/// Copy Constructor
	/**
	 * Copies name and value but initializes all relatives to NULL.
	 */
	Node::Node(const Node& rhs) :
		name(rhs.name),
		value(rhs.value),
		parent_(NULL),
		firstChild_(NULL),
		lastChild_(NULL),
		nextSibling_(NULL),
		prevSibling_(NULL)
	{ }

	/// Assignment
	/**
	 * Copies name and value but sets all relatives to NULL.
	 */
	Node& Node::operator=(const Node& rhs)
	{
		if (this != &rhs)
		{
			name         = rhs.name;
			value        = rhs.value;
			parent_      = NULL;
			firstChild_  = NULL;
			lastChild_   = NULL;
			nextSibling_ = NULL;
			prevSibling_ = NULL;
		}

		return *this;
	}

	std::string Node::val() const
	{
		return value.c_str();
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
		return value.c_str();
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

	Node* Node::getParent()
	{
		return const_cast<Node*>(static_cast<const Node*>(this)->getParent());
	}

	Node* Node::getChild(const char* name)
	{
		return const_cast<Node*>(static_cast<const Node*>(this)->getChild(name));
	}

	Node* Node::getChild(const String& name)
	{
		return const_cast<Node*>(static_cast<const Node*>(this)->getChild(name));
	}

	Node* Node::getFirstChild()
	{
		return const_cast<Node*>(static_cast<const Node*>(this)->getFirstChild());
	}

	Node* Node::getLastChild()
	{
		return const_cast<Node*>(static_cast<const Node*>(this)->getLastChild());
	}

	Node* Node::getNextSibling()
	{
		return const_cast<Node*>(static_cast<const Node*>(this)->getNextSibling());
	}

	Node* Node::getPrevSibling()
	{
		return const_cast<Node*>(static_cast<const Node*>(this)->getPrevSibling());
	}

	const Node* Node::getParent() const
	{
		return parent_;
	}

	const Node* Node::getChild(const char* name) const
	{
		// Loop over all children.
		for (Node* child = firstChild_; child != lastChild_; child = child->nextSibling_)
		{
			// Return the child if a its name matches the argument.
			if (child->name == name)
			{
				return child;
			}
		}

		// Return null if the child was not found.
		return NULL;
	}

	const Node* Node::getChild(const String& name) const
	{
		return getChild(name.c_str());
	}

	const Node* Node::getFirstChild() const
	{
		return firstChild_;
	}

	const Node* Node::getLastChild() const
	{
		return lastChild_;
	}

	const Node* Node::getNextSibling() const
	{
		return nextSibling_;
	}

	const Node* Node::getPrevSibling() const
	{
		return prevSibling_;
	}

	void Node::addChildAfter(Node* child, Node* after)
	{
		child->parent_ = this;

		child->nextSibling_ = after->nextSibling_;

		if (child->nextSibling_ != NULL)
		{
			child->nextSibling_->prevSibling_ = child;
		}

		child->prevSibling_ = after;
		after->nextSibling_ = child;
	}

	void Node::addChildBefore(Node* child, Node* before)
	{
		child->parent_ = this;

		child->prevSibling_ = before->prevSibling_;

		if (child->prevSibling_ != NULL)
		{
			child->prevSibling_->nextSibling_ = child;
		}

		child->nextSibling_  = before;
		before->prevSibling_ = child;
	}

	void Node::addChild(Node* child)
	{
		child->parent_ = this;

		if (firstChild_ == NULL)
		{
			lastChild_ = firstChild_ = child;
		}
		else
		{
			lastChild_->nextSibling_ = child;
			child->prevSibling_      = lastChild_;
			lastChild_               = child;
		}
	}

	unsigned int Node::countChildren() const
	{
		unsigned int count = 0;

		for (const Node* child = getFirstChild(); child != NULL; child = child->getNextSibling())
		{
			count++;
		}

		return count;
	}

	unsigned int Node::countOffspring() const
	{
		return countOffspring(this);
	}

	Node* Node::find(const char* path)
	{
		return const_cast<Node*>(static_cast<const Node*>(this)->find(path));
	}

	Node* Node::find(const String& path)
	{
		return const_cast<Node*>(static_cast<const Node*>(this)->find(path));
	}

	const Node* Node::find(const char* path) const
	{
		return find(String(path));
	}

	const Node* Node::find(const String& path) const
	{
		return find(this, path);
	}

	unsigned int Node::countOffspring(const Node* node) const
	{
		unsigned int count = 0;

		for (const xml::Node* child = node->getFirstChild(); child != NULL; child = child->getNextSibling())
		{
			count += countOffspring(child);
		}	

		return count + 1;
	}

	const Node* Node::find(const Node* node, const String& path) const
	{
		if (node == NULL)
		{
			return NULL;
		}

		unsigned int index = path.find("/");

		node = node->getChild(path.substr(0, index));

		if (index == String::npos)
		{
			return node;
		}
		else
		{
			return find(node, path.substr(index + 1));
		}
	}
}
