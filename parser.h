#ifndef PARSER
#define PARSER

#include <fstream>
#include <stack>
#include "node.h"

class Parser
{
public:
	static Node* loadXml(const std::string filePath);

private:
	Parser();

	// public domain: http://stackoverflow.com/a/17620909
	static void replaceAll(std::string& str, const std::string& from, const std::string& to);

	static std::string getElementName(const std::string& data, int startIndex);
};

#endif