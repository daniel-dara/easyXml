#ifndef NODE_H
#define NODE_H

#include "namespace.h"
#include "Exception.h"
#include "String.h"
#include "List.h"
#include <vector>
#include <set>
#include <string>

namespace EASYXML_NAMESPACE
{
	class Node
	{
	public:
		Node();
		Node(const char* name, const char* value = "");
		Node(const std::string& name, const std::string& value = "");
		// Node(const Node& rhs);
		// Node& operator=(const Node& rhs);

		std::string val() const;
		// Templated short-hand "getter" function
		template <class T> T val() const;

		Node* findNode(const std::string path, bool throwException = false) const;

		// Members should be public so they can be readable and writable by anyone
		String name;
		String value;

		class Attribute
		{
		public:
			Attribute(const std::string& _name, const std::string& _value) :
				name(_name),
				value(_value)
			{ }

			bool operator<(const Attribute& rhs) const
			{
				return name < rhs.name;
			}

			std::string name;
			std::string value;
		};

		// std::set<Attribute> attributes;
		List<Node*> children;
		// std::set<Node*, bool (*)(const Node*, const Node*)> sortedChildren;

	private:
		// Comparator function used to sort the "children" set.
		static bool node_ptr_compare(const Node* lhs, const Node* rhs)
		{
			return &lhs->name < &rhs->name;
		}

		// Comparator function used to sort the "attributes" set.
		static bool attr_ptr_compare(const Attribute* lhs, const Attribute* rhs)
		{
			return lhs->name < rhs->name;
		}
	};
}

#endif
