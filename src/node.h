#ifndef NODE_H
#define NODE_H

#include "namespace.h"
#include "exception.h"
#include <set>
#include <string>

namespace EASYXML_NAMESPACE
{
	class Node
	{
	public:
		// friend Node* loadXml(const std::string& filePath);

		Node();
		Node(const char* name, const char* value = NULL);
		Node(const std::string& name, const std::string& value = "");
		Node(const Node& rhs);
		Node& operator=(const Node& rhs);

		// Templated short-hand "getter" function
		template <class T> T val() const;

		Node* findNode(const std::string path, bool throwException = false) const;

		// Members should be public so they can be readable and writable by anyone
		std::string name;
		std::string value;

		std::set<Node*, bool (*)(const Node*, const Node*)> children;

	private:
		// Comparator function used to sort the "children" set.
		static bool node_ptr_compare(const Node* lhs, const Node* rhs)
		{
			return lhs->name < rhs->name;
		}
	};
}

#endif
