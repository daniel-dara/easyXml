#ifndef PARSER
#define PARSER

#include <fstream>
#include <stack>
#include "node.h"
#include "functions.h"
#include "namespace.h"

namespace EASYXML_NAMESPACE
{
	class Parser
	{
	public:
		static Node* loadXml(const std::string filePath);
		static void deleteTree(Node* node);

	private:
		Parser(); // don't allow instantiation of class
		static std::string getElementName(const std::string& data, int startIndex);

	};
}

#endif
