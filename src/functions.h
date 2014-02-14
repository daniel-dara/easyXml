#ifndef FUNCTIONS
#define FUNCTIONS

#include <iostream>
#include <algorithm>
#include <fstream>
#include <stack>
#include "node.h"
#include "namespace.h"

namespace EASYXML_NAMESPACE
{
	Node* loadXml(const std::string filePath);
	void printTree(const Node* node, std::string indentation = "");
	void deleteTree(Node* node);

	std::string getElementName(const std::string& data, int startIndex);

	// The following 3 trim functions are public domain: http://stackoverflow.com/a/217605
	// trim from start
	std::string& ltrim(std::string &s);

	// trim from end
	std::string& rtrim(std::string &s);

	// trim from both ends
	std::string& trim(std::string &s);

	// public domain: http://stackoverflow.com/a/17620909
	void replaceAll(std::string& str, const std::string& from, const std::string& to);
}

#endif
