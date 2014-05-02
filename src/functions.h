#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "namespace.h"
#include "Node.h"

namespace EASYXML_NAMESPACE
{
	Node* loadXml(const std::string& filePath);
	void saveXml(const Node* node, const std::string& filePath);
	void saveXml(const Node* node, std::ostream& out, std::string indentation = "");

	void printTree(const Node* node, std::string indentation = "");
	void deleteTree(Node* node);

	std::string getElementName(const std::string& data, size_t startIndex);

	// The following 3 trim functions are public domain: http://stackoverflow.com/a/217605
	// trim from start (in place)
	std::string& ltrim(std::string& s);

	// trim from end (in place)
	std::string& rtrim(std::string& s);
	std::string rTrim(std::string s); // copy

	// trim from both ends (in place)
	std::string& trim(std::string& s);
	std::string Trim(std::string s); // copy

	// In-place replaceAll
	// public domain: http://stackoverflow.com/a/17620909
	void replaceAll(std::string& str, const char* from, const std::string& to);
	void replaceAll(std::string& str, const std::string& from, const std::string& to);
}

#endif
