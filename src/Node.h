#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <set>
#include "namespace.h"
#include "Exception.h"
#include "String.h"

namespace EASYXML_NAMESPACE
{
	class Node
	{
	public:
		Node();
		Node(const char* name, const char* value = "");
		Node(const std::string& name, const std::string& value = "");
		Node(const Node& rhs);

		Node& operator=(const Node& rhs);

		// Nested class for representing XML Attributes
		class Attribute
		{
		public:
			Attribute(const String& name, const String& value) :
				name(name),
				value(value)
			{ }

			String name;
			String value;
		};

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

		const Node* getParent() const;
		const Node* getChild(const char* name) const;
		const Node* getChild(const String& name) const;
		const Node* getFirstChild() const;
		const Node* getLastChild() const;
		const Node* getNextSibling() const;
		const Node* getPrevSibling() const;

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

		// void* firstAttribute_;
		// void* document_;

		unsigned int countOffspring(const Node* node) const;
		const Node* find(const Node* node, const String& path) const;
	};
}

#endif
