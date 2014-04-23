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
		// attributes(),
		// children()
		// sortedChildren(node_ptr_compare)
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
		// attributes(),
		// children()
		// sortedChildren(node_ptr_compare)
	{ }

	Node::Node(const std::string& _name, const std::string& _value) :
		name(_name),
		value(_value),
		parent_(NULL),
		firstChild_(NULL),
		lastChild_(NULL),
		nextSibling_(NULL),
		prevSibling_(NULL)
		// attributes(),
		// children()
		// sortedChildren(node_ptr_compare)
	{ }

	// // Copy Constructor - technically a deep copy since the "children" set is copied
	// // but note that the set is just a bunch of pointers. The actual child nodes are not copied.
	// Node::Node(const Node& rhs) :
	// 	name(rhs.name),
	// 	value(rhs.value),
	// 	attributes(),
	// 	children(),
	// 	sortedChildren(node_ptr_compare)
	// {
	// 	for (std::set<Attribute>::const_iterator ite = rhs.attributes.begin(); ite != rhs.attributes.end(); ite++)
	// 	{
	// 		attributes.insert(*ite);
	// 	}

	// 	for (std::vector<Node*>::const_iterator ite = rhs.children.begin(); ite != rhs.children.end(); ite++)
	// 	{
	// 		children.push_back(*ite);
	// 	}

	// 	for (std::set<Node*>::const_iterator ite = rhs.sortedChildren.begin(); ite != rhs.sortedChildren.end(); ite++)
	// 	{
	// 		sortedChildren.insert(*ite);
	// 	}
	// }

	// // Assignment - deep copy (beware, this loses all children pointers but does not free them)
	// Node& Node::operator=(const Node& rhs)
	// {
	// 	if (this != &rhs)
	// 	{
	// 		attributes.clear();
	// 		for (std::set<Attribute>::const_iterator ite = rhs.attributes.begin(); ite != rhs.attributes.end(); ite++)
	// 		{
	// 			attributes.insert(*ite);
	// 		}

	// 		children.clear();
	// 		for (std::vector<Node*>::const_iterator ite = rhs.children.begin(); ite != rhs.children.end(); ite++)
	// 		{
	// 			children.push_back(*ite);
	// 		}

	// 		sortedChildren.clear();
	// 		for (std::set<Node*>::const_iterator ite = rhs.sortedChildren.begin(); ite != rhs.sortedChildren.end(); ite++)
	// 		{
	// 			sortedChildren.insert(*ite);
	// 		}

	// 		name = rhs.name;
	// 		value = rhs.value;
	// 	}

	// 	return *this;
	// }

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

	Node* Node::findNode(const std::string path, bool returnNull) const
	{
		return NULL;
		
		// // Save the cost of instantiation since findNode is recursive and its name is always overwritten.
		// static Node query;

		// std::string restOfPath;
		// size_t slashIndex = path.find('/');

		// if (slashIndex == std::string::npos)
		// {
		// 	query.name = path;
		// }
		// else
		// {
		// 	query.name = path.substr(0, slashIndex);
		// 	restOfPath = path.substr(slashIndex + 1);
		// }

		// static std::set<Node*>::const_iterator iter;
		// iter = sortedChildren.find(&query);

		// if (iter != sortedChildren.end())
		// {
		// 	if (restOfPath.length() == 0)
		// 	{
		// 		return (*iter);
		// 	}
		// 	else
		// 	{
		// 		return (*iter)->findNode(restOfPath);
		// 	}
		// }

		// if (returnNull)
		// {
		// 	return NULL;
		// }
		// else
		// {
		// 	throw EasyXmlException("Child element \"" + path + "\" not found.", 103);
		// }
	}

	const Node* Node::getParent() const
	{
		return parent_;
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

		child->nextSibling_ = before;
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
			child->prevSibling_ = lastChild_;
			lastChild_ = child;
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

	unsigned int Node::countOffspring(const Node* node) const
	{
		unsigned int count = 0;

		for (const xml::Node* child = node->getFirstChild(); child != NULL; child = child->getNextSibling())
		{
			count += countOffspring(child);
		}	

		return count + 1;
	}
}
