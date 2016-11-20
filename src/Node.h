#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <set>
#include "namespace.h"
#include "Exception.h"
#include "String.h"
#include "Input.h"
#include "Attribute.h"

namespace EASYXML_NAMESPACE
{
	class Node
	{
	public:
		Node* parseXml(Input& input);

		friend Node* parseXml(Input& input);

		Node();
		Node(const char* name, const char* value = "");
		Node(const std::string& name, const std::string& value = "");
		Node(const Node& rhs);

		Node& operator=(const Node& rhs);

		std::string val() const;
		template <class T> T val() const; // Templated short-hand "getter" function

		// Members should be public so they can be readable and writable by anyone
		String name;
		String value;

		Node* getParent();
		Node* getChild(const char* name);
		Node* getChild(const String& name);
		Node* getFirstChild();
		Node* getLastChild();
		Node* getNextSibling();
		Node* getPrevSibling();
		Attribute* getFirstAttribute();

		const Node* getParent() const;
		const Node* getChild(const char* name) const;
		const Node* getChild(const String& name) const;
		const Node* getFirstChild() const;
		const Node* getLastChild() const;
		const Node* getNextSibling() const;
		const Node* getPrevSibling() const;
		const Attribute* getFirstAttribute() const;

		void addChildAfter(Node* child, Node* after);
		void addChildBefore(Node* child, Node* before);
		void addChild(Node* child);

		unsigned int countChildren() const;
		unsigned int countOffspring() const;

		Node* find(const char* path);
		Node* find(const String& path);
		const Node* find(const char* path) const;
		const Node* find(const String& path) const;

	private:
		Node* parent_;
		Node* firstChild_;
		Node* lastChild_;
		Node* nextSibling_;
		Node* prevSibling_;

		Attribute* firstAttribute_;
		// void* document_;

		unsigned int countOffspring(const Node* node) const;
		const Node* find(const Node* node, const String& path) const;
	};
}

#endif
