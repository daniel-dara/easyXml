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
		friend Node* loadXml(const std::string& filePath);
		friend void printTree(const Node* node, std::string tabs);
		friend void deleteTree(Node* node);

		Node();
		Node(const char* _name, const char* _val = NULL);
		Node(const std::string& _name, const std::string& _val = "");
		Node(const Node& rhs);
		Node& operator=(const Node& rhs);

		Node* findNode(const std::string path, bool returnNull = false) const;

		const std::string& getName() const;
		void setName(const std::string& newName);

		const std::string& val() const;
		template <class T> T val() const;
		template <class T> void setVal(T val);

		std::set<Node*, bool (*)(const Node*, const Node*)> children;

	private:
		// Comparator function used to sort the "children" set.
		static bool node_ptr_compare(const Node* lhs, const Node* rhs)
		{
			return lhs->getName() < rhs->getName();
		}

		std::string name;
		std::string value;
	};
}

#endif
